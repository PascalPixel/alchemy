#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Func_02003310(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003318(struct ObjectRuntime *object);
void Func_02003328(s32 srcX, s32 srcY, s32 dst_x, s32 dst_y, s32 width,
                   s32 height);
void Func_02003380(s32 x, s32 z, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003488(s32 a);
void Func_020034b8(s32 cue);

static __inline__ void Func_02003328_(s32 srcX, s32 srcY, s32 dstX, s32 dstY,
                                      s32 width, s32 height) {
  /* FAKEMATCH: forwarding through the inline sets the width before the height,
   * as the native allocates them (1 in r5, 2 in r6); called directly the two
   * constants swap registers (differ 4). */
  Func_02003328(srcX, srcY, dstX, dstY, width, height);
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a metatile column twice beside it.
void Func_02001a94(void) {
  struct ObjectRuntime *object;
  s32 column;
  s32 target[3];

  if (gPartyState.entrance == 1) {
    object = Object_GetById(8);
    GameFlag_SetBit(0x301);
    column = 11;
  } else if (gPartyState.entrance == 3) {
    object = Object_GetById(9);
    GameFlag_SetBit(0x302);
    column = 26;
  } else {
    object = Object_GetById(10);
    GameFlag_SetBit(0x303);
    column = 53;
  }
  Func_02003398();
  Func_02003488(0);
  target[0] = (object->x & 0xfff00000) + 0x80000;
  target[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  Func_02003488(0);
  Func_02003310(object, target[0], 0, target[2]);
  Func_02003318(object);
  Func_020034b8(188);
  Func_02003328_(18, 4, column, 4, 1, 2);
  WaitFrames(5);
  Func_02003328_(18, 7, column, 4, 1, 2);
  WaitFrames(5);
  Func_02003380(column << 20, 0x580000, 0, 0);
  Func_020033a0();
}
