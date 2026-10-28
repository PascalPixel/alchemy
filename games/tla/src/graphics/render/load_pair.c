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
    VramCache_Load(resourceIndex, 0x200, stagingBuffer);
    Runtime_ReleaseHeapBlock(56);
  }
}

struct WindowSprite *
RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y) {
  s32 index;
  struct WindowSprite *first;
  struct WindowSprite *second;

  index = Resource_FindFreeEntry();
  if (index > 95)
    return NULL;

  RenderResource_LoadPair(group, index);
  first = WindowSprite_Create(index, 0x80004000, input, x, y);
  first->sentinel = 0xFD;
  second = WindowSprite_Create(index, 0x80004000, input, x + 32, y);
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

/// Counted up by every VBlank.
extern volatile u32 gVBlankCount;
extern const u8 gWindow_PairBobOffsets[];

/// Bobs the object and the one after it by the same step of an eight-step
/// table, advanced every four VBlanks.
void Func_08045018(struct BobPairObject *object) {
  s32 value;

  value = object->baseY + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  object->y = value;
  /* FAKEMATCH: tying the object to an empty asm loads the next object before
   * the second count is shifted, as native; plainly, 2 instructions differ in
   * tla-en. */
  asm("" : "+r"(object));
  object = object->next;
  value = object->baseY + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  object->y = value;
}
