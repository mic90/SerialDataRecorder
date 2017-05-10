# SerialDataRecorder
SDR is application for viewing and recording data from serial ports. Currently it only supports simple ascii data format, 
but it's written in a manner that would easily allow to support more formats in the future.

It still in very early development stage, so it's not recommended to use it for recording any sensitive data that You are worried to lost.

![alt text](https://github.com/mic90/SerialDataRecorder/blob/master/screenshots/sdr_screen.png)
## Implemented features
+ Night/Day mode (visible in screenshots)
+ Simultaneous read from couple serial ports
+ Saving/loading charts configuration to/from files
+ Exporting all charts to png images
+ Exporting all data to CSV file (Data count is currently limited to 1000000 samples)
+ Resizing charts vertically and horizontally
+ Pausing data read
+ Configurable charts (chart title, axis names, autoscaling, horizontal range etc.)
## Usage
+ To resize given chart vertically press ctrl + scroll mouse wheel
+ To resize given chart horiznotally press shift + scroll mouse wheel
+ To switch between night/day modes press N
## Building
To build the application You should have fully working installation of Qt 5.8.x, no other libraries are required.
## Dependencies
This project uses QsLog for logging and QCustomPlot for plotting
## License
This project is licensed under the GPL License
