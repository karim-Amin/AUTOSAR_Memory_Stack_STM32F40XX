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
        .BlockNumber = FEE_BLOCK_BOOT_MANAGER_NUM,
        .BlockSize = FEE_BLOCK_BOOT_MANAGER_SIZE,
        .ImmediateData = FALSE,
        .NumberOfWriteCycles = 0xFFFFFFFF
    },

    {    /* Image1  */
        .DeviceIndex = FEE_INDEX,
        .BlockNumber = FEE_BLOCK_IMAGE1_NUM,
        .BlockSize = FEE_BLOCK_IMAGE1_SIZE,
        .ImmediateData = FALSE,
        .NumberOfWriteCycles = 0xFFFFFFFF
    },
    
     {    /* Image2  */
        .DeviceIndex = FEE_INDEX,
        .BlockNumber = FEE_BLOCK_IMAGE2_NUM,
        .BlockSize = FEE_BLOCK_IMAGE2_SIZE,
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

