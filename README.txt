A simple program simulating the communication interface between a device and the user. It works in a loop awaiting subsequent commands until EXIT is received.
There are three variables (floating point) to read and modify (initialized to 0) - TEMP, ALTITUDE and PRESSURE.
Available commands:
    SET PARAM X   - Set PARAM (TEMP, PRESSURE, ALTITUDE) to value X
    GET PARAM     - Get current value of PARAM (TEMP, PRESSURE, ALTITUDE)
    HELP          - Display this help message
    EXIT          - Exit the program

Use cmake to install.