#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "resource.h"

struct CueEvent {
  u8 unk000[358];
  u16 cue;
};

extern u8 gPartyState[];

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);
s32 Func_080e4244(struct ObjectRuntime *object);

void Func_080e42d4(void) {
  struct CueEvent *event = Ram_HeapSlots->eventWork;
  u8 *party = gPartyState;
  struct ObjectRuntime *object = ObjectTable_Get(*(s32 *)(party + 532));
  s32 reason = 0;

  if (*(s8 *)(party + 707) != 0)
    reason = 1;
  if (Func_080dbcd8(object->x, object->z, object->terrainId) == 0)
    reason = 2;
  if (party[530] == 5 && *(s16 *)((u8 *)object + 102) != 0 &&
      object->action == 0) {
    s32 x;
    s32 z;
    s32 back;

    if (Func_080dbd48(object->x, object->z, 2) == 0)
      reason = 3;
    x = object->x & 0xfff00000;
    z = object->z & 0xfff00000;
    back = -0x80000;
    /* FAKEMATCH: ⚓️ loads the back offset before it builds the forward one */
    asm("" : "+l"(back));
    /* FAKEMATCH: and builds the forward offset only after that load */
    asm volatile("");
    if (Func_080dbd48(x + 0x80000, z + back, 2) == 0)
      reason = 4;
  }
  if (reason != 0 && (Func_080e4244(object) & 2) == 0)
    event->cue = 0x208b;
}

/// The heap slot (a byte offset into Ram_HeapSlots) that holds the RAM copy of
/// the ARM frame decoder while a picture is drawn.
#define HEAP_SLOT_FRAME_DECODER 80

// How a picture's frames are stored.
#define FRAME_FORMAT_TYPE01 1     // one type-01 stream
#define FRAME_FORMAT_COMPRESSED 3 // compressed, then run through the decoder

struct FrameSource {
  u8 unk00[5];
  u8 decodeMode;
  u8 unk06;
  u8 format;
  const void **frames;
  u8 unk0c[10];
  u8 frame;
};

typedef void (*FrameDecoder)(const void *source, void *destination, s32 mode);

extern const u8 BattleFx_DecodeFrameCode[];
extern const u8 BattleFx_DecodeFrameCopyEnd[];
/// The decoder with its tables and work area (constants.ld). The block is
/// larger than the copy, which stops before the work area.
extern u8 BattleFx_DecodeFrameBlockSize[];

/// Draws the current frame of a picture into a cleared buffer of the given
/// size. The ARM frame decoder runs from RAM: when no copy is loaded yet, this
/// loads one for the call and releases it afterwards.
void Func_080e43a4(struct FrameSource *source, void *buffer, s32 size) {
  s32 decoderLoaded = 1;
  FrameDecoder decode = Ram_HeapSlots->unk4c[1];

  if (decode == NULL) {
    Dma_Set(BattleFx_DecodeFrameCode,
            Runtime_AllocateHeapBlock(HEAP_SLOT_FRAME_DECODER,
                                      (s32)BattleFx_DecodeFrameBlockSize),
            0x84000000 | ((u32)(BattleFx_DecodeFrameCopyEnd -
                                BattleFx_DecodeFrameCode) >>
                          2),
            REG_DMA3);
    decode = Ram_HeapSlots->unk4c[1];
    decoderLoaded = 0;
  }

  Iwram_ClearWords(buffer, size);
  if (source->format == FRAME_FORMAT_TYPE01) {
    Resource_DecodeType01(source->frames[source->frame], buffer);
  } else if (source->format == FRAME_FORMAT_COMPRESSED) {
    const void *stream =
        Iwram_Decompress(source->frames[source->frame], buffer);

    if (stream != NULL)
      decode(stream, buffer, 0);
  } else {
    decode(source->frames[source->frame], buffer, source->decodeMode);
  }

  if (!decoderLoaded)
    Runtime_ReleaseHeapBlock(HEAP_SLOT_FRAME_DECODER);
}
