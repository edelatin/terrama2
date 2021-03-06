var DataManager = require("../../core/DataManager");
var DataProviderError = require('./../../core/Exceptions').DataProviderError;
var RequestFactory = require("../../core/RequestFactory");
var Utils = require('./../../core/Utils');
var TokenCode = require('./../../core/Enums').TokenCode;
var passport = require('./../../config/Passport');

module.exports = function(app) {
  return {
    post: function(request, response) {
      var dataProviderReceived = request.body;

      var uriObject = dataProviderReceived.uriObject;

      var requester = RequestFactory.build(uriObject);

      var handleError = function(response, err, code) {
        response.status(code);
        response.json({status: code || 400, message: err.message});
      };

      var _makeProvider = function() {
        var projectName = dataProviderReceived.project;

        // check project
        DataManager.getProject({name: projectName}).then(function(project) {
          // getting intent id
          DataManager.getDataProviderIntent({name: dataProviderReceived.data_provider_intent_name || requester.intent()}).then(function(intentResult) {
            // getting provider type id
            DataManager.getDataProviderType({name: uriObject[requester.syntax().SCHEME]}).then(function(typeResult) {
              var dataProviderObject = {
                name: dataProviderReceived.name,
                uri: requester.uri,
                description: dataProviderReceived.description,
                data_provider_intent_id: intentResult.id,
                data_provider_type_id: typeResult.id,
                project_id: project.id,
                active: dataProviderReceived.active || false
              };

              // try to save
              DataManager.addDataProvider(dataProviderObject).then(function(result) {
                // generating token
                var token = Utils.generateToken(app, TokenCode.SAVE, result.name);
                response.json({status: 200, result: result.toObject(), token: token});
              }).catch(function(err) {
                handleError(response, err, 400);
              });
            }).catch(function(err) {
              handleError(response, err, 400);
            });

          }).catch(function(err) {
            handleError(response, err, 400);
          });

        }).catch(function(err) {
          handleError(response, err, 400);
        })
      };

      // check connection
      requester.request().then(function() {
        _makeProvider();
      }).catch(function(err) {
        _makeProvider();
      });

    },

    get: function(request, response) {
      var name = request.query.name;

      if (name) {
        DataManager.getDataProvider({name: name}).then(function(dataProvider) {
          response.json(dataProvider.toObject());
        }).catch(function(err) {
          response.status(400);
          response.json({status: 400, message: err.message});
        })
      } else {
        var output = [];
        DataManager.listDataProviders().forEach(function(element) {
          output.push(element.rawObject());
        });
        response.json(output);
      }
    },

    put: function(request, response) {
      var dataProviderId = request.params.id;
      var uriObject = request.body.uriObject;

      var requester = RequestFactory.build(uriObject);

      var toUpdate = {
        name: request.body.name,
        active: request.body.active,
        description: request.body.description,
        uri: requester.uri
      }

      if (dataProviderId) {
        DataManager.updateDataProvider(parseInt(dataProviderId), toUpdate).then(function() {
          DataManager.getDataProvider({id: dataProviderId}).then(function(dProvider) {
            // generating token
            var token = Utils.generateToken(app, TokenCode.UPDATE, dProvider.name);

            response.json({status: 200, result: dProvider, token: token});
          }).catch(function(err) {
            response.status(400);
            response.json({status: 400, message: err.message})
          });
        }).catch(function(err) {
          response.status(400);
          response.json({status: 400, message: err.message});
        })

      } else {
        response.status(400);
        response.json({status: 400, message: "DataProvider not identified"});
      }
    },

    delete: function(request, response) {
      var id = request.params.id;
      if (id) {
        DataManager.getDataProvider({id: id}).then(function(dProvider) {
          DataManager.removeDataProvider({id: id}).then(function() {
          // generating token
            response.json({status: 200, name: dProvider.name});
          }).catch(function(err) {
            Utils.handleRequestError(response, err, 400);
          })
        }).catch(function(err) {
          Utils.handleRequestError(response, err, 400);
        })
      } else {
        Utils.handleRequestError(response, new DataProviderError("Missing data provider id"), 400);
      }
    }
  };
};
