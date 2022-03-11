/******************************************************************************
 *
 * Module: Memory Abstraction Interface
 *
 * File Name: MemIf_Cfg.h
 *
 * Description: precompile configuration Header file for MEMIF Module
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef MEMIF_CFG_H
#define MEMIF_CFG_H

#include "MemIf_Types.h"
/*
 * Module Version 1.0.0
 */
#define MEMIF_CFG_SW_MAJOR_VERSION             (1U)
#define MEMIF_CFG_SW_MINOR_VERSION             (0U)
#define MEMIF_CFG_SW_PATCH_VERSION             (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define MEMIF_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define MEMIF_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define MEMIF_CFG_AR_RELEASE_PATCH_VERSION     (3U)
   
#define MEMIF_DEV_ERROR_DETECT                 (STD_OFF)
#define MEMIF_VERSION_INFO_API                 (STD_OFF)
#define MEMIF_NUM_OF_DVICES                    (1)

#endif /* MEMIF_CFG_H */