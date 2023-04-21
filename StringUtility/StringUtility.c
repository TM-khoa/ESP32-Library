/*
 * String_Handle.c
 *
 *  Created on: Apr 8, 2023
 *      Author: SpiritBoi
 */

#include "StringUtility.h"
#ifdef CONFIG_USE_STRING_UTILITY

char ArrBuffStr[MAX_STRING_BUFFER][MAX_STRING_LENGTH];

StringStatus_t StringHandle_TokenMessage(char *String)
{
	if(!String) return STRING_NULL;
	char *p;
	uint8_t i=1;
	p = strtok(String," ,{};");
	if(strlen(p) < MAX_STRING_LENGTH){
		strcpy(ArrBuffStr[0],p);
	} else return STRING_TOKEN_LENGTH_TOO_LONG;
	while(p!=NULL){
		p = strtok(NULL," ,{};");
		if(strlen(p) < MAX_STRING_LENGTH) strcpy(ArrBuffStr[i],p);
		else return STRING_TOKEN_LENGTH_TOO_LONG;
		i++;
		if(i==MAX_STRING_BUFFER && p!=NULL) return STRING_BUFFER_OVERFLOW; 
		else if(i==MAX_STRING_BUFFER) break;
	}
	return STRING_OK;
}

StringStatus_t StringHandle_TokenKeyValue(char *String)
{
	if(!String) return STRING_NULL;
	char *p;
    // bypass the first string, which is key string
	p = strtok(String,":=");
	// get value string
	p = strtok(NULL,":=");
	//copy again to the String
	if(strlen(p) < MAX_STRING_LENGTH){
	    strcpy(String,p);
	} else return STRING_TOKEN_LENGTH_TOO_LONG; 
}

StringStatus_t StringHandle_SearchKey(char *String, char* KeySearch){
	char *c;
	for(uint8_t i=0;i<sizeof(MAX_STRING_BUFFER);i++)
	{
		c=strstr(ArrBuffStr[i],KeySearch);
		if(c!=NULL) return STRING_KEY_FOUND;
	}
	return STRING_KEY_NOT_FOUND;
}

void StringHandle_ReturnValueToString(char *s,StringStatus_t retVal){
    switch(retVal){
        case STRING_KEY_FOUND: strcpy(s,"STRING_KEY_FOUND"); break;
        case STRING_KEY_NOT_FOUND: strcpy(s,"STRING_KEY_NOT_FOUND"); break;
        case STRING_OK: strcpy(s,"STRING_OK"); break;
        case STRING_BUFFER_OVERFLOW: strcpy(s,"STRING_BUFFER_OVERFLOW"); break;
        case STRING_TOKEN_LENGTH_TOO_LONG: strcpy(s,"STRING_TOKEN_LENGTH_TOO_LONG"); break;
        case STRING_INVALID_ARG: strcpy(s,"STRING_INVALID_ARG"); break;
        case STRING_NULL: strcpy(s,"STRING_NULL"); break;
    }
}

double StrUtil_ConvertToDouble(char *String)
{
    return strtod(String,NULL);
}

int64_t StrUtil_ConvertToInt64(char *String)
{
    return strtol(String,NULL,10);
}
#endif

