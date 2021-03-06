var AbstractRequest = require('./AbstractRequest');
var Promise = require('bluebird');
var Exceptions = require("./Exceptions");
var Requester = require('request');
var NodeUtils = require('util');
var UriBuilder = require('./UriBuilder');
var UriPattern = require('../core/Enums').Uri;
var Form = require('../core/Enums').Form;
var Utils = require('../core/Utils');

var WcsRequest = function(params) {
  AbstractRequest.apply(this, arguments);
};

WcsRequest.prototype = Object.create(AbstractRequest.prototype);
WcsRequest.prototype.constructor = WcsRequest;

WcsRequest.prototype.request = function() {
  var self = this;
  return  new Promise(function(resolve, reject) {
    var args = Object.assign({}, self.params);
    args[UriPattern.SCHEME] = "HTTP";
    var uriWcsAsHttp = UriBuilder.buildUri(args, self.syntax()) + "?service=WCS&version=2.0.1&request=GetCapabilities";
    
    Requester(uriWcsAsHttp, function(err, resp, body) {
      if (err)
        reject(new Exceptions.ConnectionError("Error in wcs request"));
      else {
        if (resp.statusCode === 200) {
          // todo: parse WCS GetCapabilities xml
          resolve();
        }
        else
          reject(new Exceptions.ConnectionError(NodeUtils.format("Error in http request: (%d) - %s", resp.statusCode, resp.statusMessage)));
      }
    });
  });
};

WcsRequest.fields = function() {
  return Utils.makeCommonRequestFields("WCS", 80, null, [UriPattern.HOST, UriPattern.PORT], [
    UriPattern.HOST,
    UriPattern.PORT,
    UriPattern.USER,
    {
      key: UriPattern.PASSWORD,
      type: Form.Field.PASSWORD
    },
    UriPattern.PATHNAME
  ])
};



module.exports = WcsRequest;