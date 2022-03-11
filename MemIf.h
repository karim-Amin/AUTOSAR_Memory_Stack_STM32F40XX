/******************************************************************************
 *
 * Module: Memory Abstraction Interface
 *
 * File Name: MemIf.h
 *
 * Description:  Header file for MEMIF Module
 *
 * Author: Karim Mohamed Amin
 ******************************************************************************/
#ifndef MEMIF_H
#define MEMIF_H
   
#include "Fee.h"
#include "Std_Types.h"
#include "MemIf_Cfg.h"

#define MEMIF_SW_MAJOR_VERSION             (1U)
#define MEMIF_SW_MINOR_VERSION             (0U)
#define MEMIF_SW_PATCH_VERSION             (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define MEMIF_AR_RELEASE_MAJOR_VERSION     (4U)
#define MEMIF_AR_RELEASE_MINOR_VERSION     (0U)
#define MEMIF_AR_RELEASE_PATCH_VERSION     (3U)

/* Function Prototypes */
INLINE void MemIf_SetMode(MemIf_ModeType  Mode);
INLINE Std_ReturnType MemIf_Read(
                                 uint8 DeviceIndex,
                                 uint16 BlockNumber,
                                 uint16 BlockOffset,
                                 uint8* DataBufferPtr,
                                 uint16 Length
                                   );
INLINE Std_ReturnType MemIf_Write(
                                 uint8 DeviceIndex,
                                 uint16 BlockNumber,
                                 const uint8* DataBufferPtr
                                   );
INLINE void MemIf_Cancel(uint8 DeviceIndex);
INLINE MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex);
INLINE MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex);
INLINE Std_ReturnType MemIf_InvalidateBlock(
                                uint8 DeviceIndex,
                                uint16 BlockNumber
                                    );

#endif /* MEMIF_H */