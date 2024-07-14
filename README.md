# Watchy FOS watchface
Watchy FOS is a minimalist watchface for the SQFMI Watchy. FOS stands for file operating system. The watchface is a modified version of the [DOS](https://github.com/sqfmi/Watchy/tree/master/examples/WatchFaces/DOS) watchface.
This is my first watchface.

<img src="https://github.com/tschutschagames/watchy_fos/blob/main/Watchy_FOS.jpg" width="300" />

# Features
* **Time:** Displays the current time.
* **Day:** Day of the week in text form.
* **Date:** Current date.
* **Year:** Current year.
* **Steps:** Display the number of steps taken.
* **Kilometers:** Distance traveled in kilometers.
* **WIFI connection:** Status of the WLAN connection.
* **Temperature:** Current temperature at the location.
* **Weather:** Weather forecast for the set location.
* **Battery:** Battery level of the device.
* **Charging status:** Shows whether the watch is charging.

## Setup
Before everything works you have to set a few things:
### Watchy_FOS.cpp
* Your name
* Comment out kilometers or miles
* Comment out celsius or fahrenheit

### settings.h
* Insert CITY_ID
* Insert your API key for wether OPENWEATHERMAP_APIKEY
* Select in TEMP_UNIT metric or imperial
* Choose your language TEMP_LANG


## Contribution
Contributions are welcome! If you find a bug or would like to propose an enhancement, please reach out on my [Discord](https://discord.com/invite/bpBw3dzrzD).  
