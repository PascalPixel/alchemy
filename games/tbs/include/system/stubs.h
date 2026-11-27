#ifndef ALCHEMY_SYSTEM_STUBS_H
#define ALCHEMY_SYSTEM_STUBS_H

/* Events, ticks, memory, resources and the other services the system library
   provides. */

void   sySynchStub(Sint32 ticks);
Sint32 sySetEventStub(EVENT_FUNC callback, Sint32 order);
Sint32 syCloseEventStub(EVENT_FUNC callback);
Uint32 syRandomStub(void);
ANGLE  syAtanStub(FIXED x, FIXED y);
Sint32 sySquartFXStub(Sint32 value);
void   syVectorAddPolarOffsetStub(Sint32 radius, ANGLE angle, POINT pos);
void   syIntFunctionStub(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler);
void  *syHeapSlotAllocateStub(Sint32 kind, Sint32 size);
void  *syHeapSlotAllocateEWRAMStub(Sint32 kind, Sint32 size);
void  *syHeapAllocateStub(Sint32 size);
void  *syHeapAllocateEWRAMStub(Sint32 size);
/* Unpacks a byte-oriented LZ resource with a RAM copy of its ARM decoder, and
   returns the unpacked size in bytes. */
Sint32 syDecompressByteLZStub(const void *src, void *dest);
/* Unpacks a tagged LZ resource (a first byte of 0 or 1) the same way and
   returns the unpacked size in bytes. */
Sint32 syDecompressTaggedLZStub(const void *src, void *dest);
Sint32 syVRAMCacheFreeEntryStub(Uint32 entry);
Sint32 syVRAMCacheReleaseVRAMStub(Uint32 entry);
Sint32 syVRAMCacheLoadStub(Uint32 entry, Uint32 size, const void *src);
Sint32 syVRAMCacheFindFreeEntryStub(void);
Sint32 syOAMMatrixAllocateFromTransformStub(AFFINE_TRANSFORM *src);
void   syOAMAddSpriteAtDepthStub(void *slotEntry, Sint32 slot);
void   syWindowLoadGraphicsStub(void);
void  *syResourceGetTableEntryStub(Sint32 index);
void   syDummyNoOperationHookStub(Sint32 unused);
/* Fades BG1 to BG3, the sprites and the backdrop out to black over `frames`
   Frames; it only starts the screen fade. */
void   syScreenFadeToBlackStub(Sint32 frames);
void   syScreenFadeWaitStub(void);
void   sySerialInitStub(void);
Sint32 sySerialPollAndTransferStub(void *send, void *receive);
void   sySerialRemoveIRQHandlersStub(void);
Sint32 sySoundLoadReverbCodeStub(Sint32 code);
Sint32 sySerialStartSendStub(void *src, Sint32 size);
Sint32 sySerialStartReceiveStub(void *dest);
Sint32 sySerialGetActiveTransfersStub(void);
Sint32 sySetEventFlagsStub(EVENT_FUNC callback, Uint32 mask);

#endif
