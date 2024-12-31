# finishline_sensor_fw
Firmware for finishline tracker timers.

Boards can be based on Arduino Uno architecture. The firmware has been used on Metro Mini 328 v2 boards. 

Boards take in two interrupt flags: one to mark the start time of a race, the second to mark the finish of a race.

The board is mean to be used as a slave on an I2C bus. Requests to get data from the board will give the time for the race. If the race hasn't started or has started but not finished, the board will return 0. If the race has finished, the board will return the calculated finish time.
