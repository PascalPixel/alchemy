#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

struct FieldActor;

void GameFlag_SetBit(s32 flag);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
void Func_02004eec(s32 id, s32 a, s32 b);
void Func_02004f6c(s32 a);
void Func_02005014(s32 cue);

static __inline__ void Func_02004eec_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004eec(id, a, b);
}

void Func_0200036c(void) {
  ObjectMotion_SetActionVariant(gAdventure.currentOwner, 3);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1ea0000, 286);
  Func_02004eec_(gAdventure.currentOwner, 6, 0x800);
  Func_02005014(310);
  GameFlag_SetBit(290);
  Func_02004f6c(8);
}
