#ifndef ALCHEMY_UNIT_RUNTIME_RUNTIME_H
#define ALCHEMY_UNIT_RUNTIME_RUNTIME_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

struct AffineTransform;

void System_Initialize(void);
void Runtime_IdleForever(void);
s32 System_GetLanguage(void);
void RuntimeDispatch_ReservedNoOpA(void);
void RuntimeDispatch_ReservedNoOpB(void);
void RuntimeDispatch_ReservedNoOpC(void);
void RuntimeDispatch_ReservedNoOpD(void);
void RuntimeDispatch_ReservedNoOpE(void);
s32 RuntimeDispatch_ReturnZero(s32 unused0, const u8 *unused1);
void Resource_LoadWorkHeader(void);
void RuntimeDispatch_NoOpHook(s32 unused);
void *Resource_GetTableEntry(s32 index);
void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length);
void Resource_LoadCode(u32 index, void *destination);
void Runtime_IgnoreInterrupt(void);
void Runtime_InstallIwramAndIrqs(void);
void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Input_UpdateKeyRepeatAndDirection(void);
void System_VBlankHandler(void);
s32 Sound_LoadPresetParameters(s32 preset);
void Sound_ReservedNoOpA(void);
void Sound_ReservedNoOpB(void);
void Input_HandleKeyInterrupt(void);
void Blend_SetDarkenTarget16(s32 frames);
void Blend_SetDarkenTarget0(s32 frames);
void Blend_SetBrightenTarget16(s32 frames);
void Blend_SetBrightenTarget0(s32 frames);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_DecodeType2(const void *source, void *destination);
s32 Resource_DecodeByteLz(const void *source, void *destination);
void SerialRuntime_WaitForTransferA(void);
void SerialRuntime_WaitForTransferB(void);

#endif
