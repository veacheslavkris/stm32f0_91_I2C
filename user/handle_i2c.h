#ifndef HANDLE_I2C_H
#define HANDLE_I2C_H

#include "stm32f091xc.h"
#include "i2c_structs.h"	



void I2C2_InitHandle(void);

I2CStructHandle* I2C2_GetHandle(void);

void I2C2_ClearHandle(void);




























#endif






















