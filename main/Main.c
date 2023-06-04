#include "main.h"

char **split_string(const char *packet)  //takes input packet string and tokenizes it
{
    char* data = strdup(packet);
    char* head = data;
    data++;
    
    char **list = malloc(16 * sizeof(char*));               // creating an array of substrings
    if (list)
    for (int i = 0; i < 16; i++)
    list[i] = malloc((strlen(packet) + 1) * sizeof(char));

    char* token = NULL;
    int i = 0;
    while(true)
    {
        if(*data == ',' || *data == '*')   //if 2 or more commas are together then assign "no parameter" string
        {
            list[i++] = "no parameter";
            data++;
            continue;
        }
        // if()
        token = strtok_r(data, ",*", &data);
        strcpy(list[i++], token);
        
        if(i==16)  //all parameters completed so break out of loop
        break;
    }
    free(head);
    return list;    
}

GPS_data* Parse_gps_data(const char* packet)  //assigning values to variables from the packet string
{
    if(validate(packet) != 1)
    return NULL; 
    
    char** list = split_string(packet);   
    
    GPS_data* values = malloc(sizeof(GPS_data));
    values->Header = list[0]; 
    values->Time = list[1];
    values->Latitude = list[2];
    values->Lat_Ind = list[3];
    values->Longitude = list[4];
    values->Long_Ind = list[5];
    values->Geoidal_Unit = list[12];
    
    for(int i = 6; i < 16; i++)
    {
        if(i == 12)  // is a string so no need to assign -1
        continue;
        else if(strcmp(list[i], "no parameter") == 0)
        {
            list[i] = NOT_RECEIVED;    //assigning -1 to variable if no parameter was found
        }
    }
    
    values->Quality_Indicator = atoi(list[6]);
    values->Satellite_Num = atoi(list[7]); 
    values->HDOP = atof(list[8]);
    values->Altitude = atof(list[9]);
    values->Altitude_Unit = list[10];
    values->Geoidal_Separation = atof(list[11]);
    values->Differential_Corrections = atof(list[13]);
    values->StationID = atoi(list[14]);
    values->Checksum = strtoul(list[15], NULL, 16);

    free(list);
    return values;
}

uint8_t GPGGA_checksum_calc(const char *packet)   //calculating checksum of packet
{
	packet++;
	uint8_t chk = 0;

	while ('*' != *packet && '\n' != *packet && '\0' != *packet) {
		chk ^= (uint8_t) *packet;
		packet++;
	}

	return chk;
}

int validate(const char* packet)
{
    if(strlen(packet) < 22)    //minimum packet string length 22
    return 0;
    
    if(strlen(packet) > 82)   //maximum packet string length 82
    return 0;
    
    if(*packet != '$')     //packet starts with '$'
    return 0;
    
    for(int i = 1; i < 6; i++)   //packet header should be in capital letters
    {
        if(packet[i] < 'A' || packet[i] >'Z')
        return 0;
    }
    
    if(packet[6] != ',')  //',' should be present after 
    return 0;
    
    if(packet[strlen(packet) - 3] != '*')
    return 0;
    
    char chksum[] = {packet[strlen(packet) - 2], packet[strlen(packet) - 1]};
    uint8_t Checksum = strtoul(chksum, NULL, 16);
    if(Checksum != GPGGA_checksum_calc(packet))
    return 0;

    int i = 0;
    char commas_counted = 0;
    while(packet[i] != '\0')
    {
        if(packet[i] == ',')
        commas_counted++;
        
        i++;
    }
    if(commas_counted != 14)
    return 0;
    
    return 1;
}

void app_main(void)
{
    //using this packet string in this code
    const char* gps = "$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E";
    GPS_data* object = Parse_gps_data(gps);

    while (true)
    {
        if(object == NULL)                          //checking if the object was created or not
        printf("Invalid Packet String!\n");
        else
        printf("Valid Packet String!\n");
        
        vTaskDelay(pdSECOND);
    }

    free(object);
    object = NULL;
}