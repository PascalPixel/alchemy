#ifndef ALCHEMY_SYSTEM_STUBS_H
#define ALCHEMY_SYSTEM_STUBS_H

/* Events, frames, memory, resources and the other services the system library
   provides. */

void   sySynchStub(Sint32 frames);
Sint32 sySetEventStub(EVENT_FUNC func, Sint32 order);
Sint32 syCloseEventStub(EVENT_FUNC func);
Uint32 syRandomStub(void);
ANGLE  syAtanStub(FIXED x, FIXED y);
Sint32 sySquartFXStub(Sint32 value);
void   syVectorAddPolarOffsetStub(Sint32 radius, ANGLE angle, POINT position);
void   syIntFunctionStub(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler);
void  *syHeapSlotAllocStub(Sint32 kind, Sint32 size);
void  *syHeapSlotAllocEWRAMStub(Sint32 kind, Sint32 size);
void  *syHeapAllocStub(Sint32 size);
void  *syHeapAllocEWRAMStub(Sint32 size);
/* Unpacks a byte-oriented LZ resource with a RAM copy of its ARM decoder, and
   returns the unpacked size in bytes. */
Sint32 syDecompressByteLZStub(const void *source, void *destination);
/* Unpacks a tagged LZ resource (a first byte of 0 or 1) the same way and
   returns the unpacked size in bytes. */
Sint32 syDecompressTaggedLZStub(const void *source, void *destination);
Sint32 syVRAMCacheFreeEntryStub(Uint32 entry);
Sint32 syVRAMCacheReleaseVRAMStub(Uint32 entry);
Sint32 syVRAMCacheLoadStub(Uint32 entry, Uint32 size, const void *src);
Sint32 syVRAMCacheFindFreeEntryStub(void);
Sint32 syOAMMatrixAllocFromTransformStub(AFFINE_TRANSFORM *source);
void   syOAMAddSprAtDepthStub(void *slotEntry, Sint32 slot);
void   syWindowLoadGraphicsStub(void);
void  *syResourceGetTableEntryStub(Sint32 index);
void   syDummyNoOperationHookStub(Sint32 unused);
/* Fades BG1 to BG3, the sprites and the backdrop out to black over `vblanks`
   VBlanks; it only starts the screen fade. */
void   syScreenFadeToBlackStub(Sint32 vblanks);
void   syScreenFadeWaitStub(void);
void   sySerialInitStub(void);
Sint32 sySerialPollAndTransferStub(void *send, void *receive);
void   sySerialRemoveIRQHandlersStub(void);
Sint32 sySndLoadReverbCodeStub(Sint32 code);
Sint32 sySerialStartSendStub(void *source, Sint32 size);
Sint32 sySerialStartReceiveStub(void *destination);
Sint32 sySerialGetActiveTransfersStub(void);
Sint32 sySetEventFlagsStub(EVENT_FUNC func, Uint32 mask);

#endif
