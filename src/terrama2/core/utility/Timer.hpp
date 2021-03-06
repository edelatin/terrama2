/*
  Copyright (C) 2007 National Institute For Space Research (INPE) - Brazil.

  This file is part of TerraMA2 - a free and open source computational
  platform for analysis, monitoring, and alert of geo-environmental extremes.

  TerraMA2 is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  TerraMA2 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with TerraMA2. See LICENSE. If not, write to
  TerraMA2 Team at <terrama2-team@dpi.inpe.br>.
*/

/*!
  \file terrama2/core/utility/Timer.hpp

  \brief

  \author Jano Simas
  \author Vinicius Campanha
*/

#ifndef __TERRAMA2_CORE_TIMER_HPP__
#define __TERRAMA2_CORE_TIMER_HPP__

// TerraMA2
#include "../Typedef.hpp"
#include "../data-model/Schedule.hpp"

//Qt
#include <QTimer>

//terralib
#include <terralib/datatype/TimeInstantTZ.h>

namespace terrama2
{
  namespace core
  {
    /*!
      \brief Timer class for scheduling processes.

      This class when created starts a timer based on the Schedule that will emit a timeoutSignal.
    */
    class Timer : public QTimer
    {
      Q_OBJECT

    public:
      Timer(const Schedule& dataSchedule, ProcessId processId, std::shared_ptr< te::dt::TimeInstantTZ > lastEmit);

      virtual ~Timer();
      Timer(const Timer& other) = delete;
      Timer(Timer&& other) = delete;
      Timer& operator=(const Timer& other) = delete;
      Timer& operator=(Timer&& other) = delete;

      ProcessId processId() const;

    signals:

        void timeoutSignal(ProcessId processId) const;

    private slots:

        //! Slot called when the timer times out, emits timeoutSignal.
        void timeoutSlot() noexcept;

    private:
        /*!
       * \brief The method checks if the process execution is by frequency or by schedule, calcules the
       *        time to start it and starts a timer with the countdown to the time of execution.
       * \param dataSchedule
       */
      void prepareTimer(const terrama2::core::Schedule& dataSchedule);

      /*!
       * \brief A method to calculate the seconds for frequency stored in the Schedule
       * \param dataSchedule The schedule struct with the information for when a process should be executed
       * \return A double containing the frequency in seconds
       */
      double frequencySeconds(const Schedule& dataSchedule);

      /*!
       * \brief A method to calculate the seconds until the scheduled date and time in stored in the Schedule
       * \param dataSchedule The schedule struct with the information for when a process should be executed
       * \return A double containing the seconds until the scheduled timestamp
       */
      double scheduleSeconds(const Schedule& dataSchedule);

      struct Impl;
      Impl* impl_;
    };
  }
}
#endif //__TERRAMA2_CORE_TIMER_HPP__
