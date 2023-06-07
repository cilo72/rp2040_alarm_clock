/*
  Copyright (c) 2023 Daniel Zwirner
  SPDX-License-Identifier: MIT-0
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "cilo72/hw/blink_forever.h"
#include "cilo72/hw/elapsed_timer_ms.h"
#include "cilo72/hw/i2c_bus.h"
#include "cilo72/hw/uart.h"
#include "cilo72/hw/gpiokey.h"
#include "cilo72/ic/sd2405.h"
#include "cilo72/ic/ssd1306.h"
#include "cilo72/ic/ws2812.h"
#include "cilo72/ic/bh1750fvi.h"
#include "cilo72/ic/df_player_pro.h"
#include "cilo72/core/onchange.h"
#include "cilo72/fonts/font_8x5.h"
#include "statemachine.h"
#include "state.h"
#include "menu.h"
#include "hourminute.h"
#include <time.h>
#include <cstring>
#include <list>

uint8_t constexpr PIN_PIXELS_DIN = 9;

uint8_t constexpr PIN_I2C_SDA  = 2;
uint8_t constexpr PIN_I2C_SCL  = 3;

uint8_t constexpr PIN_UART_RX  = 17;
uint8_t constexpr PIN_UART_TX  = 16;

uint8_t constexpr PIN_KEY_1    = 28;
uint8_t constexpr PIN_KEY_2    = 27;
uint8_t constexpr PIN_KEY_3    = 26;
uint8_t constexpr PIN_KEY_4    = 22;

uint8_t constexpr PIXEL_LEFT   = 2;
uint8_t constexpr PIXEL_MIDDLE = 1;
uint8_t constexpr PIXEL_RIGHT  = 0;
uint8_t constexpr PIXEL_FRONT  = 3;

static constexpr int8_t brightnessMapLength                  = 41;
static constexpr uint32_t brightnessMap[brightnessMapLength] = {0, 2, 3, 4, 6, 8, 11, 16, 23, 32, 45, 64, 90, 128, 181, 255, 255,255,255,255,255,255,255,255,255,255,181, 128, 90, 64, 45, 32, 23, 16, 11, 8, 6, 4, 3, 2};

struct Intensity2brightness
{
  uint32_t intensity;
  uint32_t oled;
  uint32_t pixel;
};

static constexpr uint32_t intensity2brightnessMapLength = 16;
static constexpr Intensity2brightness intensity2brightnessMap[intensity2brightnessMapLength] = {
{  0,   0,  5},
{  2,   2,  5},
{  3,   3,  5},
{  4,   4,  5},
{  6,   6,  5},
{  8,   8,  5},
{ 11,  11,  6},
{ 16,  16,  7},
{ 23,  23,  8},
{ 32,  32,  9},
{ 45,  45, 10},
{ 64,  64, 11},
{ 90,  90, 12},
{128, 128, 13},
{181, 181, 14},
{255, 255, 15}};

class TimeSet
{
public:
  TimeSet(cilo72::ic::SSD1306 &oled, cilo72::hw::GpioKey & keyUp, cilo72::hw::GpioKey & keyDown, cilo72::hw::GpioKey & keyEnter, const cilo72::fonts::Font &font = cilo72::fonts::Font8x5())
      : oled_(oled)
      , keyUp_(keyUp)
      , keyDown_(keyDown)
      , keyEnter_(keyEnter)
      , selected_(0)
      , font_(font)
  {
  }

  void init(const cilo72::ic::SD2405::Time &time)
  {
    time_ = time;
    selected_ = 0;
    draw();
  }
  
  bool run(bool & pressed)
  {
    if(keyEnter_.pressed())
    {
      selected_++;
      draw();
    }

    if(keyUp_.pressed())
    {
      pressed = true;
      switch (selected_)
      {
      case 0:
        time_.setHour(time_.hour() + 10);
        break;

      case 1:
        time_.setHour(time_.hour() + 1);
        break;

      case 2:
        time_.setMinute(time_.minute() + 10);
        break;

      case 3:
        time_.setMinute(time_.minute() + 1);
        break;

      default:
        break;
      }
      draw();
    }

    if(keyDown_.pressed())
    {
      pressed = true;
      switch (selected_)
      {
      case 0:
        time_.setHour(time_.hour() - 10);
        break;

      case 1:
        time_.setHour(time_.hour() - 1);
        break;

      case 2:
        time_.setMinute(time_.minute() - 10);
        break;

      case 3:
        time_.setMinute(time_.minute() - 1);
        break;

      default:
        break;
      }
      draw();
    }

    return selected_ < 4;
  }

  void draw(uint8_t c, bool selected, uint32_t & x, uint32_t & y)
  {

    char s[10];
    sprintf(s, "%01i", c);
    
    if(selected)
    {
      oled_.drawSquare(x-1, y-1, font_.width() * scale + 2, font_.height() * scale, cilo72::ic::SSD1306::Color::White);
      oled_.drawString(x, y, scale, s, cilo72::ic::SSD1306::Color::Black);
    }
    else
    {
      oled_.drawString(x, y, scale, s, cilo72::ic::SSD1306::Color::White);
    }
    x += (font_.width() * scale)+2;
  }

  void draw()
  {
    uint32_t x = 1;
    uint32_t y = 4;

    oled_.clear();

    draw(time_.hour() / 10, selected_ == 0, x, y);
    draw(time_.hour() % 10, selected_ == 1, x, y);

    oled_.drawString(x, y, scale, ":", cilo72::ic::SSD1306::Color::White);
    x += (font_.width() * scale);

    draw(time_.minute() / 10, selected_ == 2, x, y);
    draw(time_.minute() % 10, selected_ == 3, x, y);

    oled_.update();
  }

  const cilo72::ic::SD2405::Time & time() const
  {
    return time_;
  }

private:
  cilo72::ic::SSD1306 &oled_;
  cilo72::ic::SD2405::Time time_;
  cilo72::hw::GpioKey & keyUp_;
  cilo72::hw::GpioKey & keyDown_;
  cilo72::hw::GpioKey & keyEnter_;
  uint32_t selected_;
  const cilo72::fonts::Font & font_;
  static constexpr uint32_t scale = 4;
};

int main()
 {
  stdio_init_all();

  cilo72::hw::GpioKey keyPlus(PIN_KEY_1);
  cilo72::hw::GpioKey keyMinus(PIN_KEY_2);
  cilo72::hw::GpioKey keyAlarm(PIN_KEY_3);
  cilo72::hw::GpioKey keyEnter(PIN_KEY_4);
  cilo72::hw::BlinkForever blink(PICO_DEFAULT_LED_PIN, 1);
  cilo72::hw::I2CBus i2cBus(PIN_I2C_SDA, PIN_I2C_SCL);
  cilo72::ic::SD2405 rtc(i2cBus);
  cilo72::ic::BH1750FVI lux(i2cBus);
  cilo72::ic::WS2812 pixels(PIN_PIXELS_DIN, 4);
  cilo72::ic::SSD1306 oledRight(i2cBus);
  cilo72::ic::SSD1306 oledLeft(i2cBus, false);
  cilo72::hw::Uart uart(PIN_UART_RX, PIN_UART_TX, 115200, 8, 1, UART_PARITY_NONE);
  cilo72::ic::DfPlayerPro dfPlayerPro(uart);
  cilo72::hw::ElapsedTimer_ms elapsedTimer;
  cilo72::hw::ElapsedTimer_ms elapsedTimerAlarmBlink;
  cilo72::hw::ElapsedTimer_ms elapsedTimerAlarmOff;
  uint8_t alarmRedBrightnesIndex;

  HourMinute hm(rtc);

  State stateIdle;
  State stateMenu;
  State stateMenuTime;
  State stateMenuAlarm;
  State stateMenuVolumen;
  State stateShowAlarm;

  TimeSet timeSet(oledRight, keyPlus, keyMinus, keyEnter);

  bool alarmIsPlaying               = false;
  bool alarmOn                      = false;
  bool isAlarm                      = false;
  bool lastIsAlarm                  = false;
  uint8_t intensity2brightnessMapIndex = 0;

  Menu menu(oledLeft);
  menu.add(new MenuItem("Alarm", &stateMenuAlarm));
  menu.add(new MenuItem("Zeit", &stateMenuTime));
  menu.add(new MenuItem("Volumen", &stateMenuVolumen));
  menu.add(new MenuItem("Exit", &stateIdle));

  dfPlayerPro.setPlayMode(cilo72::ic::DfPlayerPro::PlayMode::PLAY_RANDOMLY);
  
  cilo72::core::OnChange<bool> onChangeAlarm(alarmOn, [&](const bool &last, const bool & value)
  {
    pixels.set(PIXEL_FRONT, 0, 0, value ? 255 : 0);
    pixels.update();
  });

  cilo72::core::OnChange<HourMinute::Time> onChangeTime(hm, [&](const HourMinute::Time &last, const HourMinute::Time &time)
  {
    char s[20];
    sprintf(s, "%02i", time.hour());
    oledLeft.clear();
    oledLeft.drawString(40, 1, 8, s);
    oledLeft.update();

    sprintf(s, "%02i", time.minute());
    oledRight.clear();
    oledRight.drawString(1, 1, 8, s);
    oledRight.update();

    HourMinute::Time alarm(rtc.alarm());
    
    isAlarm = alarm == time;

    if(isAlarm == true and lastIsAlarm == false and alarmOn)
    {
      alarmIsPlaying = true;
      alarmRedBrightnesIndex  = brightnessMapLength;
      elapsedTimerAlarmBlink.start();
      elapsedTimerAlarmOff.start();
      dfPlayerPro.play();
    }

    lastIsAlarm = isAlarm;
  }, 
  [&]() { hm.update(); });

  cilo72::core::OnChange<uint8_t> onChangeBrightness(intensity2brightnessMapIndex, [&](const uint8_t &last, const uint8_t &now)
  {
    if(not alarmIsPlaying)
    {
      pixels.setBrightness(intensity2brightnessMap[now].pixel);
      pixels.update();
    }

    oledLeft.contrast(intensity2brightnessMap[now].oled);    
    oledRight.contrast(intensity2brightnessMap[now].oled);    
  });

  cilo72::core::OnChange<double> onChangeLightIntensity(lux, [&](const double &last, const double &now)
  {
    uint32_t v;

    if(now > 255.0)
    {
      v = 255;
    }
    else
    {
      v = now;
    }

    for(intensity2brightnessMapIndex=0; intensity2brightnessMapIndex < intensity2brightnessMapLength; intensity2brightnessMapIndex++)
    {
      if(v <= intensity2brightnessMap[intensity2brightnessMapIndex].intensity)
      {
        break;
      }
    }
  }, 
  [&]() { lux.update(); });  
  
  // -----------------------------------------------------------------------------------------
  // IDLE ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateIdle.setOnEnter([&]() 
  {
    pixels.set(PIXEL_LEFT,   0, 0, 0);
    pixels.set(PIXEL_MIDDLE, 0, 0, 0);
    pixels.set(PIXEL_RIGHT,  0, 0, 0);
    pixels.update();
    onChangeTime.action();
  });

  stateIdle.setOnRun([&](State &state) -> const StateMachineCommand * 
  { 
    bool switchOff = false;
    onChangeTime.evaluate();
    onChangeAlarm.evaluate();
    onChangeLightIntensity.evaluate();
    onChangeBrightness.evaluate();

    if(keyEnter.pressed())
    {
      return state.changeTo(&stateMenu);
    }

    if(keyAlarm.pressed())
    {
      
      alarmOn = not alarmOn;
      if(alarmOn)
      {
        return state.changeTo(&stateShowAlarm);
      }
      else
      {
        switchOff = true;
      }
    }

    if(alarmIsPlaying and (elapsedTimerAlarmOff.elapsed() > 10* 60 * 1000 or switchOff))
    {
        dfPlayerPro.pause();
        alarmIsPlaying = false;
        onChangeBrightness.evaluate(true);
        alarmOn = false;
    }

    if(elapsedTimerAlarmBlink.elapsed() >= 50 and alarmIsPlaying)
    {
      pixels.set(PIXEL_FRONT, brightnessMap[alarmRedBrightnesIndex], 0, 0);
      pixels.update();

      alarmRedBrightnesIndex++;
      if(alarmRedBrightnesIndex >= brightnessMapLength)
      {
        alarmRedBrightnesIndex = 0;
      }

      elapsedTimerAlarmBlink.start();
    }

    return state.nothing(); 
  });

  // -----------------------------------------------------------------------------------------
  // MENU ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateMenu.setOnEnter([&]() 
  {
    pixels.set(PIXEL_LEFT,   255, 255, 255);
    pixels.set(PIXEL_MIDDLE, 255, 255, 255);
    pixels.set(PIXEL_RIGHT,  255, 255, 255);
    pixels.update(); 
    elapsedTimer.start();

    menu.reset();
    menu.draw();
    oledRight.clear();
    oledRight.update();
  });

  stateMenu.setOnRun([&](State &state) -> const StateMachineCommand *
  { 
    if(keyEnter.pressed())
    {
      return state.changeTo(menu.selected()->next());
    }
    else if(keyMinus.pressed())
    {
      elapsedTimer.start();
      menu.up();
      menu.draw();
    }
    else if(keyPlus.pressed())
    {
      elapsedTimer.start();
      menu.down();
      menu.draw();
    }

    if(elapsedTimer.elapsed() > 10000)
    {
      return state.changeTo(&stateIdle);
    }
    else
    {
      return state.nothing();
    }
  });

  // -----------------------------------------------------------------------------------------
  // TIME ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateMenuTime.setOnEnter([&]() 
  {
    pixels.update(); 
    elapsedTimer.start();

    timeSet.init(rtc.time());
  });

  stateMenuTime.setOnRun([&](State &state) -> const StateMachineCommand *
  {
    bool pressed = false;

    if(timeSet.run(pressed) == false)
    {
      rtc.setTime(timeSet.time());
      return state.changeTo(&stateIdle);
    }

    if(pressed)
    {
      elapsedTimer.start();
    }

    if(elapsedTimer.elapsed() > 10000)
    {
       return state.changeTo(&stateIdle);
    }
    else
    {
      return state.nothing();
    }
  });

  // -----------------------------------------------------------------------------------------
  // ALARM ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateMenuAlarm.setOnEnter([&]() 
  {
    pixels.update(); 
    elapsedTimer.start();

    timeSet.init(rtc.alarm());
  });

  stateMenuAlarm.setOnRun([&](State &state) -> const StateMachineCommand *
  {
    bool pressed = false;

    if(timeSet.run(pressed) == false)
    {
      rtc.setAlarm(timeSet.time());
      return state.changeTo(&stateIdle);
    }

    if(pressed)
    {
      elapsedTimer.start();
    }

    if(elapsedTimer.elapsed() > 10000)
    {
       return state.changeTo(&stateIdle);
    }
    else
    {
      return state.nothing();
    }
  });

  // -----------------------------------------------------------------------------------------
  // MENU ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateShowAlarm.setOnEnter([&]() 
  {
    char s[20];
    cilo72::ic::SD2405::Time time = rtc.alarm();
    sprintf(s, "%02i", time.hour());
    oledLeft.clear();
    oledLeft.drawString(40, 1, 8, s);
    oledLeft.update();

    sprintf(s, "%02i", time.minute());
    oledRight.clear();
    oledRight.drawString(1, 1, 8, s);
    oledRight.update();
  });

  stateShowAlarm.setOnRun([&](State &state) -> const StateMachineCommand *
  {
    onChangeAlarm.evaluate();
    if(keyAlarm.isPressed())
    {
       return state.nothing();
    }
    else
    {
      return state.changeTo(&stateIdle);      
    }
  });

  // -----------------------------------------------------------------------------------------
  // Volume ------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  stateMenuVolumen.setOnEnter([&]() 
  {
    oledLeft.clear();
    oledLeft.drawString(40, 1, 8, "-");
    oledLeft.update();
    dfPlayerPro.play();

    oledRight.clear();
    oledRight.drawString(1, 1, 8, "+");
    oledRight.update();
    elapsedTimer.start();
  });

  stateMenuVolumen.setOnRun([&](State &state) -> const StateMachineCommand *
  {
    onChangeAlarm.evaluate();
    if(elapsedTimer.elapsed() > 10000)
    {
       return state.changeTo(&stateIdle);
    }
    else if(keyEnter.pressed())
    {
      return state.changeTo(&stateIdle);
    }
    else if(keyMinus.pressed())
    {
      dfPlayerPro.incVolume(-1);
      elapsedTimer.start();
    }
    else if(keyPlus.pressed())
    {
      dfPlayerPro.incVolume(1);
      elapsedTimer.start();
    }

      return state.nothing();
  });

  stateMenuVolumen.setOnExit([&]() 
  {
    dfPlayerPro.pause();
  });  

  StateMachine sm(&stateIdle);

  pixels.set(0, 0, 0);
  pixels.update();

  while (true)
  {
    sm.run();

  }
}
