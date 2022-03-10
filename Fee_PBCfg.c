/******************************************************************************
 *
 * Module: flash memory
 *
 * File Name: Fee_PBCfg.h
 *
 * Description: Post build configuration Source file for Fee module 
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#include "Fee.h"
#include "NvM_Cbk.h"

const Fee_BlockConfigType BlockConfigList[] = {

    {    /* Boot Manager */
        .DeviceIndex = FEE_INDEX,
        .BlockNumber = FEE_BLOCK_NUM_BOOT_MANAGER,
        .BlockSize = FEE_BLOCK_SIZE_BOOT_MANAGER,
        .ImmediateData = FALSE,
        .NumberOfWriteCycles = 0xFFFFFFFF
    },

    {    /* Image1  */
        .DeviceIndex = FEE_INDEX,
        .BlockNumber = FEE_BLOCK_NUM_IMAGE1,
        .BlockSize = FEE_BLOCK_SIZE_IMAGE1,
        .ImmediateData = FALSE,
        .NumberOfWriteCycles = 0xFFFFFFFF
    },
    
     {    /* Image2  */
        .DeviceIndex = FEE_INDEX,
        .BlockNumber = FEE_BLOCK_NUM_IMAGE2,
        .BlockSize = FEE_BLOCK_SIZE_IMAGE2,
        .ImmediateData = FALSE,
        .NumberOfWriteCycles = 0xFFFFFFFF
    },
    
};
const Fee_ConfigType Fee_Config = {
    .General = {
        .NvmJobEndCallbackNotificationCallback = NULL_PTR,
        .NvmJobErrorCallbackNotificationCallback = NULL_PTR,
    },
    .BlockConfig = BlockConfigList,
};

