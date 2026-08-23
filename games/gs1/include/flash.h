#ifndef ALCHEMY_FLASH_H
#define ALCHEMY_FLASH_H

#include "types.h"

#define EraseFlashSector                 Func_08006d50
#define ProgramFlashByte                 Func_08006dec
#define ProgramFlashSectorWithEraseRetry Func_08006e24
#define ProgramFlashSector               Func_08006f84
#define CountRemainingErasedFlashBytes   Func_08006f48
#define RunFlashEraseVerifier            Func_08006f6c
#define StopFlashTimer                   Func_08006a78
#define ReadFlashByte                    Func_08006abc
#define CopyFlashReadRoutineToRam        Func_08006ac0
#define ReadFlashId                      Func_08006878
#define IdentifyFlash                    Func_08006910
#define EraseFlashChip                   Func_08006cdc
#define StartFlashTimer                  Func_08006a00
#define WaitForFlashWrite                Func_08006af8
#define EraseAtmelFlashChip              Func_08007028
#define EraseAtmelFlashSector            Func_08007098
#define EraseAtmelFlashBlock             Func_0800711c
#define ProgramAtmelFlashSector          Func_080071a8
#define ProgramAtmelFlashBlock           Func_08007220
#define FlashTimerIntr                   Func_080069a4
#define SetFlashTimerIntr                Func_080069c8
#define ReadFlashCore                    Func_08006b84
#define ReadFlash                        Func_08006ba8
#define VerifyFlashCore                  Func_08006c24
#define VerifyFlashSector                Func_08006c68

u16 EraseFlashSector(u16 sector);
u16 ProgramFlashByte(u8 *source, u8 *destination);
u16 ProgramFlashSectorWithEraseRetry(u16 sector, u8 *source);
u16 ProgramFlashSector(u16 sector, u8 *source);
void StopFlashTimer(void);
u8 ReadFlashByte(u8 *source);
void CopyFlashReadRoutineToRam(void *destination);
s32 ReadFlashId(void);
s32 IdentifyFlash(void);
u16 EraseFlashChip(void);
s32 WaitForFlashWrite(u8 timing_index, s32 address, u8 expected);
u16 EraseAtmelFlashChip(void);
u16 EraseAtmelFlashSector(u16 sector);
u16 EraseAtmelFlashBlock(u16 block);
u16 ProgramAtmelFlashSector(u32 sector, const u8 *source);
u16 ProgramAtmelFlashBlock(u16 block, u8 *source);
void FlashTimerIntr(void);
s32 SetFlashTimerIntr(u8 timer_index, void (**callback)(void));
void ReadFlashCore(u32 source, u8 *destination, s32 size);
void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size);
u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size);
s32 VerifyFlashSector(u16 sector, s32 source);

#endif
