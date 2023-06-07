#pragma once

#include "cilo72/ic/sd2405.h"
#include <stdint.h>

class HourMinute
{
public:
  class Time
  {
  public:
    Time(const cilo72::ic::SD2405::Time & time)
    : hour_(time.hour())
    , minute_(time.minute())
    {

    }

    Time()
    : hour_(0)
    , minute_(0)
    {

    }

    bool operator!=(const Time &rhs) const
    {
      return not operator==(rhs);
    }

    bool operator==(const Time &rhs) const
    {
      return this->hour_ == rhs.hour_ && this->minute_ == rhs.minute_;
    }

    uint8_t hour() const { return hour_; }
    uint8_t minute() const { return minute_; }
    void setHour(uint8_t value) { hour_ = value; }
    void setMinute(uint8_t value) { minute_ = value; }

  private:
    uint8_t hour_;   ///< The hour component.
    uint8_t minute_; ///< The minute component.
  };

  HourMinute(cilo72::ic::SD2405 &rtc)
      : rtc_(rtc)
  {
  }

  void update()
  {
    cilo72::ic::SD2405::Time time = rtc_.time();
    now_.setHour(time.hour());
    now_.setMinute(time.minute());
  }

  operator const Time &()
  {
    return now_;
  }

private:
  Time now_;
  cilo72::ic::SD2405 &rtc_;
};