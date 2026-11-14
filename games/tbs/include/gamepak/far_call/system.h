#ifndef ALCHEMY_FAR_CALL_SYSTEM_H
#define ALCHEMY_FAR_CALL_SYSTEM_H

/// @file
/// Tasks, frames, memory, resources and the other services the System
/// provides.

void waitFar(s32 frames);
s32 Task_AddFar(TaskFunc func, s32 order);
s32 Task_RemoveFar(TaskFunc func);
u32 Random16Far(void);
s32 ArcTan2Far(s32 x, s32 y);
s32 FixedSqrtFar(s32 value);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, void *position);
void Irq_SetHandlerFar(u32 irq, s32 vcount, InterruptHandler handler);
void *HeapSlot_AllocFar(s32 kind, s32 size);
void *HeapSlot_AllocEwramFar(s32 kind, s32 size);
void *Heap_AllocFar(s32 size);
void *Heap_AllocEwramFar(s32 size);
/// Unpacks a byte-oriented LZ resource with a RAM copy of its ARM decoder, and
/// returns the unpacked size in bytes.
s32 Decompress_ByteLzFar(const void *source, void *destination);
/// Unpacks a tagged LZ resource (a first byte of 0 or 1) the same way and
/// returns the unpacked size in bytes.
s32 Decompress_TaggedLzFar(const void *source, void *destination);
s32 VramCache_FreeEntryFar(u32 no);
s32 VramCache_ReleaseVramFar(u32 no);
s32 VramCache_LoadFar(u32 no, u32 size, const void *src);
s32 VramCache_FindFreeEntryFar(void);
s32 OamMatrix_AllocFromTransformFar(struct AffineTransform *source);
void Oam_AddSpriteAtDepthFar(void *slot_entry, s32 slot);
void Window_LoadGraphicsFar(void);
void *Resource_GetTableEntryFar(s32 index);
void Stub_NoOpHookFar(s32 unused);
/// Fades BG1 to BG3, the sprites and the backdrop out to black over `vblanks`
/// VBlanks; it only starts the screen fade.
void ScreenFade_ToBlackFar(s32 vblanks);
void ScreenFade_WaitFar(void);
void Serial_InitializeFar(void);
s32 Serial_PollAndTransferFar(void *send, void *receive);
void Serial_RemoveIrqHandlersFar(void);
s32 Sound_LoadReverbCodeFar(s32 code);
s32 Serial_StartSendFar(void *source, s32 transferValue);
s32 Serial_StartReceiveFar(void *destination);
s32 Serial_GetActiveTransfersFar(void);
s32 Task_SetFlagsFar(TaskFunc func, u32 mask);

#endif
