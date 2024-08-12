/*
 * 75xx165.c
 *
 *  Created on: Aug 12, 2024
 *      Author: pablo-jean
 */

/*
 * Include
 */

#include "l74xx165.h"

/*
 * Privates
 */

/* Macros */
#define _LOAD_ENABLED		false
#define _LOAD_DISABLED		true
#define _CS_ENABLED			false
#define _CS_DISABLED		true

#define _SUCCESS			0

/* Functions */

static void _mtx_locK(l74xx165_t *handler){
	if (handler->fxns.fxnMtxLock != NULL){
		handler->fxns.fxnMtxLock();
	}
}

static void _mtx_unlocK(l74xx165_t *handler){
	if (handler->fxns.fxnMtxUnlock != NULL){
		handler->fxns.fxnMtxUnlock();
	}
}

static void _set_cs(l74xx165_t *handler, bool Signal){
	assert(handler->fxns.fxnGpioCS != NULL);
	handler->fxns.fxnGpioCS(Signal);
}

static void _set_load(l74xx165_t *handler, bool Signal){
	assert(handler->fxns.fxnGpioLoad != NULL);
	handler->fxns.fxnGpioLoad(Signal);
}

static uint8_t _spi_rx(l74xx165_t *handler, uint8_t *Buffer, uint8_t len){
	assert(handler->fxns.fxnSpiReceived != NULL);
	return handler->fxns.fxnSpiReceived(Buffer, len);
}

/*
 * Publics
 */

l74xx165_err_e l74xx165_init(l74xx165_t *handler, l74xx165_params_t *params){
	assert(handler != NULL);
	assert(params != NULL);
	assert(params->fxnGpioCS != NULL);
	assert(params->fxnGpioLoad != NULL);
	assert(params->fxnSpiReceived != NULL);
	assert(params->u8NumberOfDevices > 0);

	if (handler->bInitialized == true){
		return L74XX165_OK;
	}

	handler->fxns.fxnGpioCS = params->fxnGpioCS;
	handler->fxns.fxnGpioLoad = params->fxnGpioLoad;
	handler->fxns.fxnSpiReceived = params->fxnSpiReceived;
	handler->fxns.fxnMtxLock = params->fxnMtxLock;
	handler->fxns.fxnMtxUnlock = params->fxnMtxUnlock;
	handler->u8NumberOfDevices = params->u8NumberOfDevices;

	if (params->pu8ExtBuffer != NULL){
		handler->pu8IntBuffer = params->pu8ExtBuffer;
	}
	else{
		handler->pu8IntBuffer = (uint8_t*)malloc(handler->u8NumberOfDevices);
	}
	assert(handler->pu8IntBuffer != NULL);

	_mtx_locK(handler);
	_set_load(handler, _LOAD_DISABLED);
	_set_cs(handler, _CS_DISABLED);
	_mtx_unlocK(handler);

	handler->bInitialized = true;

	return L74XX165_OK;
}

l74xx165_err_e l74xx165_read_byte(l74xx165_t *handler, uint8_t Index, uint8_t *ReadedByte){
	uint8_t u8Ret;

	assert(handler != NULL);
	assert(handler->bInitialized == true);
	assert(ReadedByte != NULL);
	assert(Index < handler->u8NumberOfDevices);

	_mtx_locK(handler);

	_set_load(handler, _LOAD_ENABLED);
	_set_load(handler, _LOAD_DISABLED);
	_set_cs(handler, _CS_ENABLED);
	u8Ret = _spi_rx(handler, handler->pu8IntBuffer, handler->u8NumberOfDevices);
	_set_cs(handler, _CS_DISABLED);

	_mtx_unlocK(handler);

	if (u8Ret != _SUCCESS){
		*ReadedByte = 0;
		return L74XX165_FAIL;
	}
	*ReadedByte = handler->pu8IntBuffer[Index];

	return L74XX165_OK;
}

l74xx165_err_e l74xx165_read_bit(l74xx165_t *handler, uint8_t Index, uint8_t bit, bool *Readedbit){
	uint8_t u8DesiredByte;

	assert(handler != NULL);
	assert(handler->bInitialized == true);
	assert(Readedbit != NULL);
	assert(Index < handler->u8NumberOfDevices);
	assert(bit < 8);

	if (l74xx165_read_byte(handler, Index, &u8DesiredByte) != _SUCCESS){
		*Readedbit = false;
		return L74XX165_FAIL;
	}
	*Readedbit = ((u8DesiredByte >> bit) & 0x1);

	return L74XX165_OK;
}

