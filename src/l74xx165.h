/*
 * 74xx165.h
 *
 *  Created on: Aug 12, 2024
 *      Author: pablo-jean
 */

#ifndef l74XX165_H_
#define l74XX165_H_

/*
 * Includes
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

#include "l74xx165_defs.h"


/*
 * Macros
 */

#define l74XX165_VALUE		0

/*
 * Enumerates
 */

typedef enum{
	L74XX165_OK,
	L74XX165_FAIL
}l74xx165_err_e;

/*
 * Typedefs
 */

typedef void (*l74xx165_gpio_t)(bool sig);
typedef void (*l74xx165_mtx_t)(void);
typedef uint8_t (*l74xx165_spi_t)(uint8_t *bff, uint8_t len);

/*
 * Structs and Unions
 */

typedef struct{
	struct{
		l74xx165_spi_t fxnSpiReceived;
		l74xx165_gpio_t fxnGpioCS;
		l74xx165_gpio_t fxnGpioLoad;
		l74xx165_mtx_t fxnMtxLock;
		l74xx165_mtx_t fxnMtxUnlock;
	}fxns;
	bool bInitialized;
	uint8_t u8NumberOfDevices;
	uint8_t *pu8IntBuffer;
}l74xx165_t;

typedef struct{
	l74xx165_spi_t fxnSpiReceived;
	l74xx165_gpio_t fxnGpioCS;
	l74xx165_gpio_t fxnGpioLoad;
	l74xx165_mtx_t fxnMtxLock;
	l74xx165_mtx_t fxnMtxUnlock;
	uint8_t u8NumberOfDevices;
	uint8_t *pu8ExtBuffer;
}l74xx165_params_t;

/*
 * Publics Function Prototypes
 */

l74xx165_err_e l74xx165_init(l74xx165_t *handler, l74xx165_params_t *params);

l74xx165_err_e l74xx165_read_byte(l74xx165_t *handler, uint8_t Index, uint8_t *ReadedByte);

l74xx165_err_e l74xx165_read_bit(l74xx165_t *handler, uint8_t Index, uint8_t bit, bool *Readedbit);

#endif /* l74XX165_H_ */
