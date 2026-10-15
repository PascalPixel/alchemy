#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "render_input.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

void RenderResource_LoadPair(s32 groupIndex, s32 resourceIndex) {
  void *stagingBuffer = Runtime_AllocateBlock(56, 0x400);
  s32 resourceAddress = gRenderResource_PairSourceTable[groupIndex];

  if (resourceIndex <= 0x5F) {
    Resource_DecodeByteLzInRam((void *)resourceAddress, stagingBuffer);
    VramBlock_LoadCached(resourceIndex, 0x200, stagingBuffer);
    Runtime_ReleaseHeapBlock(56);
  }
}

struct RenderOutput *
RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y) {
  s32 index;
  struct RenderOutput *first;
  struct RenderOutput *second;

  index = Resource_FindFreeEntry();
  if (index > 95)
    return NULL;

  RenderResource_LoadPair(group, index);
  first = RenderOutput_Create(index, 0x80004000, input, x, y);
  first->sentinel = 0xFD;
  second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
  second->sentinel = 0xFD;
  second->oam.index += 8;
  return first;
}

struct BobPairObject {
  struct BobPairObject *next;
  u8 unk04[4];
  u8 baseY;
  u8 unk09[11];
  u8 y;
};

// The frame counter, advanced by the vblank interrupt.
extern volatile u32 gFrameTick;
extern const u8 gUi_PairBobOffsets[];

// Bobs the object and the one after it by the same step of an eight-step
// table, advanced every four frames.
void Func_08045018(struct BobPairObject *object) {
  s32 value;

  value = object->baseY + gUi_PairBobOffsets[(gFrameTick >> 2) & 7];
  object->y = value;
  /* FAKEMATCH: tying the object to an empty asm after the first store
     loads the next object before the second tick is shifted, as the
     native does; plain, the shift comes first (one reordered row,
     differ 2). */
  asm("" : "+r"(object));
  object = object->next;
  value = object->baseY + gUi_PairBobOffsets[(gFrameTick >> 2) & 7];
  object->y = value;
}
