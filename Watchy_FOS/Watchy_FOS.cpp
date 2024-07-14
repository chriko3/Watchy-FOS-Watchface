#include "Watchy_FOS.h"

// Charge symbole
const unsigned char charge [] PROGMEM = {
	0x00, 0x10, 0x00, 0x70, 0x00, 0xe0, 0x01, 0xe0, 0x03, 0xe0, 0x07, 0xc0, 0x0f, 0xc0, 0x1f, 0xff, 
	0x3f, 0xfe, 0x7f, 0xfc, 0x7f, 0xf8, 0x03, 0xf0, 0x03, 0xe0, 0x07, 0xc0, 0x07, 0x80, 0x07, 0x00, 
	0x0e, 0x00, 0x0c, 0x00
};

// Down button press for minutes
void WatchyFOS::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

  if (wakeupBit & DOWN_BTN_MASK) {

        time_t now = time(nullptr);
        struct tm* currentTime = localtime(&now);
        int minutes = currentTime->tm_min;

        int vibrations = 0;
        if (minutes >= 1 && minutes <= 15) {
            vibrations = 1;
        } else if (minutes >= 16 && minutes <= 30) {
            vibrations = 2;
        } else if (minutes >= 31 && minutes <= 45) {
            vibrations = 3;
        } else if (minutes >= 46 && minutes < 60) {
            vibrations = 4;
        }

        for (int i = 0; i < vibrations; i++) {
            vibMotor(55, 4);
            delay(100);
        }
  }

  if (wakeupBit & MENU_BTN_MASK) {
      Watchy::handleButtonPress();
      return;
  }
}

void WatchyFOS::drawWatchFace() {
    //00:00 = reset
    if (currentTime.Hour == 0 && currentTime.Minute == 0) {
        sensor.resetStepCounter();
    }

    // Steps in kilometers
    const float stepLength = 0.70; 
    uint32_t stepCount = sensor.getCounter();
    float distanceInmeters = stepCount * stepLength;
    float distanceInkilometers = distanceInmeters / 1000.0;
    float distanceInmiles = distanceInkilometers * 0.621371;

    // Name will display at the bottom
    String yourName= "watchyfos";

    // Day
    String dayOfWeek = dayStr(currentTime.Wday);

    // Battery
    float VBAT = getBatteryVoltage();
    String batterySymbol;

    if (VBAT > 3.90) {
      batterySymbol = "[][][]";   // Full battery symbol
    }
    else if (VBAT > 3.75 && VBAT <= 3.90) {
      batterySymbol = "[][]";  // High battery symbol
    }
    else if (VBAT > 3.60 && VBAT <= 3.75) {
      batterySymbol = "[]"; // Medium battery symbol
    }
    else if (VBAT <= 3.60) {
      batterySymbol = "____";  // Low battery symbol
    }

    // Wifi check
    String wifiStatus;
    if (WIFI_CONFIGURED) {
        wifiStatus = "true";
    } else {
        wifiStatus = "false";
    }

    // Weather data
    weatherData currentWeather = getWeatherData();

    int8_t temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    String weatherState;

    if (WIFI_CONFIGURED) {
      temperature = currentWeather.temperature;
        //https://openweathermap.org/weather-conditions
        if (weatherConditionCode > 801) {
            weatherState = "cloudy";
        } else if (weatherConditionCode == 801) {
            weatherState = "cloudsun";
        } else if (weatherConditionCode == 800) {
            weatherState = "sunny";
        } else if (weatherConditionCode >= 700) {
            weatherState = "atmosphere";
        } else if (weatherConditionCode >= 600) {
            weatherState = "snow";
        } else if (weatherConditionCode >= 500) {
            weatherState = "rain";
        } else if (weatherConditionCode >= 300) {
            weatherState = "drizzle";
        } else if (weatherConditionCode >= 200) { 
            weatherState = "thunder";
        }
    } else {
        weatherState = "???";
        temperature = 0;
    }

    // Display settings
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&Px437_IBM_BIOS5pt7b);

    // Head print
    display.setCursor(0, 8);
    display.println("WATCHY-FOS V3 ");
    display.println("Copyright (c) 2024");

    display.println();
    
    // Time print
    display.print("TIME   .BAT ");
    int displayHour = currentTime.Hour;
    if (displayHour < 10) {
        display.print("0");
    }
    display.print(displayHour);
    display.print(":");
    if (currentTime.Minute < 10) {
        display.print("0");
    }
    display.println(currentTime.Minute);

    // Day print
    display.print("DAY    .TXT ");
    display.println(dayOfWeek);

    // Date print
    display.print("DATE   .EXE ");
    String month = monthShortStr(currentTime.Month);
    display.print(currentTime.Day);
    display.print(".");
    display.println(month);

    // Year print
    display.print("YEAR   .SYS ");
    display.println(tmYearToCalendar(currentTime.Year));

    // Steps print
    display.print("STEPS  .PY  ");
    display.println(stepCount);

    // Kilometer/Miles print
    display.print("KM     .DIS ");
    display.print(distanceInkilometers);
    display.println(" km");

    /*
    display.print("MI     .DIS ");
    display.print(distanceInmiles);
    display.println(" mi");
    */

    // Wifi print
    display.print("WIFI   .SYS ");
    display.println(wifiStatus);

    // Temp print
    display.print("TEMP   .MET ");
    display.print(temperature);
    display.println(" C");

    /*
    display.print("TEMP   .IMP ");
    display.print(temperature);
    display.println(" F");
    */

    // Weather state print
    display.print("WEATHER.INF ");
    display.println(weatherState);
    
    display.println();

    // Battery print
    display.print("POWER  .INI ");
    display.println(batterySymbol);

    #ifdef ARDUINO_ESP32S3_DEV
    if (USB_PLUGGED_IN) {
        display.drawBitmap(183, display.getCursorY() - 23, charge, 16, 18, GxEPD_BLACK);
    }
    #endif

    display.println();

    // Bottom print
    display.println("ESP32S3 Dev Module");
    display.println("8MB with Spiffs");
    display.print("LibVer: ");
    display.println(WATCHY_LIB_VER);
    display.println();
    display.print("C:\\");
    display.println(yourName);
}
