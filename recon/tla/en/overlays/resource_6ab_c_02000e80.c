// Matches: differ 0 (tla-ja, 2026-11-02)
// The Japanese form only: measure with --edition tla-ja. Its listing section
// sits under .ifdef TLA_EDITION_JA; the English form is already adopted in
// games/tla/src/scene/mars_lighthouse_star_room/s0e80.c, behind a
// LANGUAGE != LANGUAGE_JAPANESE guard, so tla-en compares this with a different
// function and its figure means nothing. Adoption goes in that file's Japanese
// branch; run the edition compare there, since this diff leaves pool words out.
// This is the English flag-clear arm without the flag test, with the first arm's
// sprite store order (nextFrame first, unk1b after unk5c, which settles r1 and
// r2), and an ending that puts the leader back at (0x2a8, 186) in place of the
// turn to 0x4000. The Call and Value layer is the English file's. The s32 zero
// is a one-constant local for the halfword store: as a plain *screen = 0 it
// loads from the pool, the extra word opens a mid-function pool, differ 40.
// Check at adoption: the Japanese listing has MsgMarsStar09 as the plain word
// 0x2f9b.
#include "edition.h"
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_ids.h"

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov66d;

s32 Camera_MoveWithinMapBounds();
void FieldEvent_Wait(s32 frames);
struct Actor *Func_020041e4(s32 type, s32 x, s32 y, s32 z);
s32 Func_020041ec();
s32 Func_02004204();
s32 Func_0200420c();
s32 Func_0200423c();
s32 Func_02004244();
s32 Func_02004264();
s32 Func_02004294();
void Func_020042ac(s32 sprite);
void Func_020042dc(void);
void Func_020042e4(void);
s32 Func_020043a4();
s32 Func_020043b4();
s32 Func_020043bc();
s32 Func_020043c4();
void Func_020043d4(void);
s32 Func_02004404();
s32 Func_0200440c();
s32 Func_02004414();
s32 Func_0200444c();
void Func_0200445c(void);
s32 Func_0200446c();
s32 GameFlag_SetBit();
s32 GameFlag_Test(s32 flag);
s32 Actor_ArmCallback();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
struct Actor *Actor_GetById(s32 id);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Task_Add();
s32 Task_RemoveFar();
s32 FieldMessage_Show();
s32 VramCache_Load(s32 block, s32 count, u8 *source);
s32 wait();
extern struct Actor *Data_02004cec;
extern u8 Func_02001424[];
extern u8 MsgMarsStar09[];

void Func_02000e80(void) {
  struct Actor **slot;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *buffer;
  u16 *screen;
  s32 zero;

  screen = *(u16 **)0x3000080;
  Func_020042dc();
  Func_0200444c(0);
  FieldMessage_Show((s32)MsgMarsStar09, 1);
  Func_0200446c(78);
  wait(1);
  Func_0200446c(182);
  slot = &Data_02004cec;
  object = (struct Actor *)Value4((s32 (*)())Func_020041e4, 0x1e9, 0x2a80000,
                                   0x80000, 0x930000);
  *slot = object;
  if (object != 0) {
    sprite = object->animation;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= 2;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Func_020042ac(247);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner,
        0x2a8, 186);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 40);
  Func_0200446c(0);
  Func_0200446c(141);
  Call3((void (*)())Func_02004264, 0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Call2((void (*)())Func_020043bc, gMainRoutineWork.currentOwner, 0x102);
  Call3((void (*)())Func_02004264, 0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  Func_0200440c(0x10000, 0);
  Func_02004404(0x10006, 0);
  Func_02004414(80);
  FieldEvent_Wait(80);
  Call2((void (*)())Task_Add, (s32)Func_02001424, 0x480);
  Call2((void (*)())Func_020043c4, 0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2a80000, -1, 0x1e00000, 1);
  Func_020043d4();
  FieldEvent_Wait(80);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  Func_0200446c(0x186);
  Func_02004404(0x7fff, 0);
  Func_02004414(80);
  wait(60);
  Task_RemoveFar((s32)Func_02001424);
  wait(20);
  Func_02004264(0, 0, 0);
  FieldEvent_Wait(20);
  Call6((void (*)())Func_0200423c, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 65, 65, 9, 21, 38, 24);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 38, 87);
  Call6((void (*)())Func_02004244, 76, 64, 9, 22, 102, 23);
  Func_02004294(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                (s32)screen);
  zero = 0;
  *screen = zero;
  Call2((void (*)())Func_02004404, 0x10000, 0);
  Func_02004414(120);
  wait(160);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x2a80000, -1, 0xba0000, 1);
  Func_020043d4();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    Call4((void (*)())Func_02004204, (s32)object, 0x2a00000, 0, 0xa00000);
    Func_0200420c((s32)*slot);
    Func_0200446c(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    Call4((void (*)())Func_02004204, (s32)object, 0x2980000, 0, 0x9e0000);
    Func_0200420c((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Func_020041ec(object);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 0x2a8, 186);
  FieldEvent_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_0200445c();
  GameFlag_SetBit(0xa30);
  gMainRoutineWork.special = (s32)&SceneId_Ov66d;
  Func_020042e4();
}
