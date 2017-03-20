# SiKMavEncrypt
##Simple project to encrypt Mavlink data over SiK using PJRC Teensy between radio and mavlink interface##
>>Makes use of the CAPE arduino encryption project (https://github.com/gioblu/Cape)
>>You need to download this and include with the SiKMavEncrypt in Arduino IDE to compile.


##To do:## 

>>1: Encapsulate encrypted data inside dummy Mavlink frame to allow use of Mavlink optimised setting in SiK radio firmware (currently you must use rawdata).
>>2: Handle non-encrypted RSSI packets generated by SiK radio firmware.

##Hardware required:##

>>2x 3DR Radio configured in RawData mode (both must be 'Air' type, i.e. have serial rather than USB connectors)
>>2x PJRC Teensy 3.2
>>1x USB to TTL serial adapter


##Setup:##

>>Configure your encryption key and baud rate in SiKMavEncrypt.ino and upload the same settings to both Teensy boards.

>>Pixhawk Telemetry Port <-> Teensy <-> 3DR radio
>>USB Serial Uart <-> Teensy <-> 3DR radio

>>Aircraft side:
>>Pixhawk Telemetry TX  --> Teensy RX1
>>Pixhawk Telemetry RX  --> Teensy TX1
>>3DR Radio TX --> Teensy RX2
>>3DR Radio RX --> Teensy TX2

>>Ground Station Side
>>USB TTL Serial TX  --> Teensy RX1
>>USB TTL Serial RX  --> Teensy TX1
>>3DR Radio TX --> Teensy RX2
>>3DR Radio RX --> Teensy TX2

