#ifndef ALCHEMY_SYSTEM_STUBS_H
#define ALCHEMY_SYSTEM_STUBS_H

/* @file */
/* Tasks, frames, memory, resources and the other services the system */
/* library provides. */

void sySynchStub(s32 frames);
s32 syTaskAddStub(TASK_FUNC func, s32 order);
s32 syTaskRemoveStub(TASK_FUNC func);
u32 syRandomStub(void);
ANGLE syTrigAtanStub(FIXED x, FIXED y);
s32 syMathSqrtFXStub(s32 value);
void syVectorAddPolarOffsetStub(s32 radius, ANGLE angle, POINT position);
void syIrqSetHandlerStub(u32 irq, s32 vcount, INTERRUPT_HANDLER handler);
void *syHeapSlotAllocStub(s32 kind, s32 size);
void *syHeapSlotAllocEwramStub(s32 kind, s32 size);
void *syHeapAllocStub(s32 size);
void *syHeapAllocEwramStub(s32 size);
/* Unpacks a byte-oriented LZ resource with a RAM copy of its ARM decoder, and
 */
/* returns the unpacked size in bytes. */
s32 fdDecompressByteLzStub(const void *source, void *destination);
/* Unpacks a tagged LZ resource (a first byte of 0 or 1) the same way and */
/* returns the unpacked size in bytes. */
s32 fdDecompressTaggedLzStub(const void *source, void *destination);
s32 syVramCacheFreeEntryStub(u32 entry);
s32 syVramCacheReleaseVramStub(u32 entry);
s32 syVramCacheLoadStub(u32 entry, u32 size, const void *src);
s32 syVramCacheFindFreeEntryStub(void);
s32 syOamMatrixAllocFromTransformStub(AFFINE_TRANSFORM *source);
void syOamAddSprAtDepthStub(void *slotEntry, s32 slot);
void mnWindowLoadGraphicsStub(void);
void *syResourceGetTableEntryStub(s32 index);
void syDummyNoOpHookStub(s32 unused);
/* Fades BG1 to BG3, the sprites and the backdrop out to black over `vblanks` */
/* VBlanks; it only starts the screen fade. */
void syScrFadeToBlackStub(s32 vblanks);
void syScrFadeWaitStub(void);
void sySerialInitStub(void);
s32 sySerialPollAndTransferStub(void *send, void *receive);
void sySerialRemoveIrqHandlersStub(void);
s32 sySndLoadReverbCodeStub(s32 code);
s32 sySerialStartSendStub(void *source, s32 size);
s32 sySerialStartReceiveStub(void *destination);
s32 sySerialGetActiveTransfersStub(void);
s32 syTaskSetFlagsStub(TASK_FUNC func, u32 mask);

#endif
