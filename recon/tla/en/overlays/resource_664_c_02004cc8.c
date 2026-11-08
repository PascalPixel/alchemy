// Matches: differ 0 (tla-en, 2026-11-08)
// One source for all six editions of Func_02004cc8 of overlay 664: the
// Japanese release's older form (still its own TLA_EDITION_JA block of
// .text.x0200ccc8 in recon/tla/raw/overlays/resource_664_overlay.s) and the
// later one games/tla/src/scene/airs_rock_inside/s4cc8.c links under
// #if TLA_PATCH_1. They differ only in the leader's priorityFlags: from
// TLA_PATCH_1 on they are saved after Func_02005f2c and put back after the
// last Actor_SetSingleChildFlags; the Japanese form leaves them as the
// descent set them (1). This body is s4cc8.c's with those two statements and
// their local under #if TLA_PATCH_1. Measured 8 November: differ 0 of 200 in
// tla-en, tla-de, tla-es, tla-fr and tla-it, and differ 0 of 190 in tla-ja,
// whose compiled pool (gMainRoutineWork, 0xffe00000, 0xfff80000,
// gFrameCount, 0xfff00000) is the scaffold's .L_ja_0200ce88 to
// .L_ja_0200ce98, word for word, with no padding before it.
// To adopt: this body replaces s4cc8.c's (dropping its file-wide
// #if TLA_PATCH_1 wrap and the comment above it), the scaffold's
// TLA_EDITION_JA block for Func_02004cc8 (lines 1648 to 1854 today, the
// .L_ja_ pool with it) goes with its .text.x0200ccc8 section and that
// section's line in resource_664.ld, then make compare-editions; the draft is
// retired once all twelve print OK.
#include "version.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

extern u32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Func_02005dec(struct Actor *object);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Func_02005d94(struct Actor *object);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02005d7c(struct Actor *object);

void Func_02004cc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Func_02005e14();
  Func_02005f2c(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(leader, 0);
  leader->speedLimit = 0x18000;
  leader->acceleration = 0x18000;
  lift = Import_ActorCreate(280, leader->x, leader->y - 0x80000, leader->z);
  Func_02005d5c(lift, 2);
  Actor_SetSingleChildFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = 0x16666;
  lift->acceleration = 0x16666;
  Func_02005d8c(leader, leader->x, leader->y + 0x500000, leader->z);
  Func_02005d8c(lift, lift->x, lift->y + 0x480000, lift->z);
  Func_02005f94(137);
  Event_StartScreenTransition();
  while (!Func_02005dec(leader)) {
    leader->angle = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Func_02005f94(288);
  FieldEvent_Wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  height = Map_GetTerrainHeight(0, lift->x, lift->z - 0x100000);
  leader->y = height;
  leader->terrainHeight = height;
  Func_02005d5c(leader, 6);
  wait(6);
  Func_02005f94(152);
  Func_02005d5c(leader, 7);
  leader->speedLimit = 0x20000;
  leader->acceleration = 0x8000;
  leader->velocityY = 0x40000;
  leader->motionFlags = 2;
  leader->priorityFlags = 1;
  Func_02005d8c(leader, leader->x, leader->y, leader->z - 0x100000);
  OverlayObject_WaitUntilIdle(leader);
  Func_02005d94(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02005d5c(lift, 5);
  wait(12);
  Func_02005d7c(lift);
  leader->motionFlags = 3;
  leader->terrainHeight = leader->y;
  Actor_SetSingleChildFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Func_02005d7c(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Func_02005e1c();
}
