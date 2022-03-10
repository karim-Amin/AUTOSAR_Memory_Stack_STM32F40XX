/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: Fee_Cfg.h
 *
 * Description: precompile configuration Header file for Fee module
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef FEE_CFG_H
#define FEE_CFG_H

#include "MemIf_Types.h"
#include "Fee.h"

#define FEE_DEV_ERROR_DETECT			 STD_ON
#define FEE_VERSION_INFO_API			 STD_ON
#define FEE_POLLING_MODE		         STD_ON

#define FEE_INDEX				(0xFE)
/* This size in Bytes */
#define FEE_VIRTUAL_PAGE_SIZE                    8
#define FEE_MAX_NUM_SETS                         1

#define FEE_BLOCK_NUM_BOOT_MANAGER               1
#define FEE_BLOCK_NUM_IMAGE1                     4097
#define FEE_BLOCK_NUM_IMAGE2                     8194

#define FEE_BLOCK_SIZE_BOOT_MANAGER              (32768U)
#define FEE_BLOCK_SIZE_IMAGE1                    (32768U)
#define FEE_BLOCK_SIZE_IMAGE2                    (32768U)

#define FEE_NUM_OF_BLOCKS		         3

/* Divide the flash Memory into Banks */
/* The configured blocks will be aligned from the start address of the first bank */
#define FEE_BANK1_OFFSET			   0x4000
#define FEE_BANK2_OFFSET			   0x20000
#define FEE_BANK1_LENGTH			   0x1C000
#define FEE_BANK2_LENGTH			   0x60000

#endif /* FEE_CFG_H */