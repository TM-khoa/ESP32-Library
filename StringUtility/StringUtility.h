/*
 * StringHandle.h
 *
 *  Created on: Apr 8, 2023
 *      Author: SpiritBoi
 */

#ifndef STRINGUTILITY_H_
#define STRINGUTILITY_H_
#include "../LibraryConfig.h"
#ifdef CONFIG_USE_STRING_UTILITY
#include "string.h"

#define MAX_STRING_BUFFER 5
#define MAX_STRING_LENGTH 15

typedef enum{
	STRING_OK,
	STRING_INVALID_ARG,
	STRING_NULL,
	STRING_BUFFER_OVERFLOW,
	STRING_TOKEN_LENGTH_TOO_LONG,
	STRING_NULL,
	STRING_KEY_FOUND,
	STRING_KEY_NOT_FOUND,
}StringStatus_t;



#endif /*CONFIG_USE_STRING_HANDLE*/
#endif /*STRINGHANDLE_H_*/
