
# Code Explained

This code parses GPGGA packets from NMEA 0183 protocol. 




## Input & Output
The code takes input of a GPGGA packet in string literals and ouputs a dynamically allocated object of user defined type "Gps-data" which contains all the parameters assigned to variables. The GPS_data data type is as follows:
```bash
  typedef struct
{
    char *Header, *Time, *Longitude, *Long_Ind, *Latitude, *Lat_Ind, *Altitude_Unit, *Geoidal_Unit;
    int Satellite_Num, Quality_Indicator, StationID;
    uint8_t Checksum;
    float HDOP, Altitude, Geoidal_Separation, Differential_Corrections;
} GPS_data;
```
The types of the variables are predefined and can be changed according the user's need.

## Splitting the packet
The `split_string()` function takes the packet string as an input and returns a dynamically allocated pointer to an array of substrings which contains the parameters mentioned in the packet.
## Packet Validation & Integrity
The packet's Checksum is calculated using `GPGGA_checksum_calc()` which takes Xor of every ascii character in the packet string and returns a `uint_8` number. The `validate()` function checks the packet string for certain properties and returns 1 if the packet is valid.
## Attributes Assignment
The `Parse_gps_data()` creates an object of `GPS_data` in heap and assigns the parameters in the packet string to the attributes of the object and returns the address of the object. If a parameter is not found the packet, the corresponding attribute(if it is an int or a float) is assigned `NOT_RECEIVED` which is a value of -1 or if it is a `char*` then it is assigned `"no parameter"`. If the packet string is invalid then `NULL` is returned.
## Example
An example packet string is `"$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E"` is used in the code. It is first validated then assigned to the `GPS_data` object.
