#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "resource.h"

struct CueEvent {
  u8 unk000[358];
  u16 cue;
};

extern u8 gMainRoutineWork[];

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);
s32 Func_080e4244(struct Actor *object);

void Func_080e42d4(void) {
  struct CueEvent *event = Ram_HeapSlots->eventWork;
  u8 *party = gMainRoutineWork;
  struct Actor *object = ObjectTable_Get(*(s32 *)(party + 532));
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
/// the ARM picture decoder while a picture is drawn.
#define HEAP_SLOT_PICTURE_DECODER 80

// How a picture is stored.
#define PICTURE_FORMAT_TYPE01 1     ///< one type-01 stream
#define PICTURE_FORMAT_COMPRESSED 3 ///< compressed, then run through the decoder

struct PictureSource {
  u8 unk00[5];
  u8 decodeMode;
  u8 unk06;
  u8 format;
  const void **pictures;
  u8 unk0c[10];
  u8 picture;
};

typedef void (*PictureDecoder)(const void *source, void *destination, s32 mode);

extern const u8 BattleEffect_DecodePictureCode[];
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/// The decoder with its tables and work area (constants.ld). The block is
/// larger than the copy, which stops before the work area.
extern u8 BattleEffect_DecodePictureBlockSize[];

/// Draws the current picture into a cleared buffer of the given
/// size. The ARM picture decoder runs from RAM: when no copy is loaded yet, this
/// loads one for the call and releases it afterwards.
void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size) {
  s32 decoderLoaded = 1;
  PictureDecoder decode = Ram_HeapSlots->unk4c[1];

  if (decode == NULL) {
    Dma_Set(BattleEffect_DecodePictureCode,
            HeapSlot_Alloc(HEAP_SLOT_PICTURE_DECODER,
                           (s32)BattleEffect_DecodePictureBlockSize),
            DMA_COPY_WORDS((u32)(BattleEffect_DecodePictureCopyEnd -
                                 BattleEffect_DecodePictureCode) >>
                           2),
            REG_DMA3);
    decode = Ram_HeapSlots->unk4c[1];
    decoderLoaded = 0;
  }

  Iwram_ClearWords(buffer, size);
  if (source->format == PICTURE_FORMAT_TYPE01) {
    Resource_DecodeType01(source->pictures[source->picture], buffer);
  } else if (source->format == PICTURE_FORMAT_COMPRESSED) {
    const void *stream =
        Iwram_Decompress(source->pictures[source->picture], buffer);

    if (stream != NULL)
      decode(stream, buffer, 0);
  } else {
    decode(source->pictures[source->picture], buffer, source->decodeMode);
  }

  if (!decoderLoaded)
    HeapSlot_Release(HEAP_SLOT_PICTURE_DECODER);
}
