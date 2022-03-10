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
#include "fls.h"
#include "NvM_Cbk.h"

#define FEE_DEV_ERROR_DETECT			 STD_ON
#define FEE_VERSION_INFO_API			 STD_ON
#define FEE_POLLING_MODE		         STD_ON

#define FEE_INDEX				(0xFE)
/* This size in Bytes */
#define FEE_VIRTUAL_PAGE_SIZE                    8
#define FEE_MAX_NUM_SETS                         1

#define FEE_BLOCK_BOOT_MANAGER_NUM               1
#define FEE_BLOCK_IMAGE1_NUM                     4097
#define FEE_BLOCK_IMAGE2_NUM                     8194


#define FEE_BLOCK_BOOT_MANAGER_SIZE              (32768U)
#define FEE_BLOCK_IMAGE1_SIZE                    (32768U)
#define FEE_BLOCK_IMAGE2_SIZE                    (32768U)

#define FEE_NUM_OF_BLOCKS		         3

/* Divide the flash Memory into Banks */
/* The configured blocks will be aligned from the start address of the first bank */
#define FEE_BANK1_OFFSET			   0x4000
#define FEE_BANK2_OFFSET			   0x20000
#define FEE_BANK1_LENGTH			   0x1C000
#define FEE_BANK2_LENGTH			   0x60000

#define NUM_OF_BANKS			           2
#endif /* FEE_CFG_H */