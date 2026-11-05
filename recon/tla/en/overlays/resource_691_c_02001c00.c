// Draft, differ 20 of 218, skeleton gap 4 removed, 4 added (tla-en, 2026-11-01)
//
// The leader is held at mode 10 and action 32 while eight effect bursts go off
// around it, five frames apart; it then falls under gravity 0xcccc with no
// bounce, facing 0xc000, and a burst goes off every other frame until its
// vertical velocity reaches 0 or 60 frames pass. Mode 22 and the closing calls
// follow, with flag 0x260 set throughout.
//
// Left: one thing, the landing bursts' frame address (facts: the burst family;
// resource_69c_c_02001f50.c has the RTL). The native builds position's address
// in the first loop, after the first Random16Far (movs r2, #16; add r2, sp),
// keeps it in r8 and reuses it unbuilt through the second loop. PRE hoists it
// to the loop's entry edge here, as add r3, sp, #16, which moves the zero's
// scratch to r2 and turns the rest into reloads through r8 (mov r2, r8 before
// the first store, r3 for the second where the native has r2). Every other
// difference is a pool offset or branch distance that follows from those
// 4 instructions. sfp is never killed, so gcse treats the address as invariant
// in any plain form: this is the compiler question in K3, Pascal's call, not a
// source form still to find.
//
// Measured: position written directly, or a pointer set at the top of the loop
// or just before its store, 23 (gap 4/5; the address stays in the preheader, as
// its split form); the pointer set once above the loop, 20, kept. A pointer set
// again in the second loop, 111. Inner-block position in each loop, 23; a
// struct position, 23; a for or do first loop hoists 15 and 0xfff80000, 70;
// the second loop as do/break, 102. i = 0 before the pointer, 23. An options
// pointer changes nothing and is gone. Declaration order: no effect found.
// Call3 and Call4 are the camera's and Func_02003ac4's (each constant rebuilt
// per call): called direct, differ 53.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct MagmaRockEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

extern u32 gFrameCount;

struct Actor *Actor_GetById(s32 id);
void Func_02003aec(void);
void Func_02003bcc(s32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02003c54(s32 cue);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetActionById(s32 id, s32 action);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectOptions *options);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void FieldEvent_Wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003ac4(s32 a, s32 b, s32 c);
void Func_02003acc(void);
void Func_02003b74(s32 a, s32 b);
void Func_02003b6c(void);
void Func_02003af4(void);

void Func_02001c00(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  s32 position[3];
  struct MagmaRockEffectOptions *opt;
  s32 *offset;
  s32 i;
  u32 phase;

  Func_02003aec();
  Func_02003bcc(0);
  GameFlag_SetBit(0x260);
  Func_02003c54(133);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 6);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 32);
  opt = &options;
  opt->kind = 1;
  opt->palette = 14;
  opt->scaleX = 0xcccc;
  opt->scaleY = 0xcccc;
  i = 0;
first:
  {
    s32 r = Random16Far();

    /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
     * position's address to the loop's entry; as no asm, 20 of 218 instructions
     * differ in tla-en. */
    __asm__("" : : : "sfp");
    offset = position;
    offset[0] = ((r & 15) << 16) - 0x80000;
  }
  offset[1] = (Random16Far() & 15) << 16;
  Func_0200015c(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                0x13333, 0, 0xb0000, opt);
  wait(5);
  i++;
  if (i != 8)
    goto first;
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  gMainRoutineWork.renderMode = 0;
  leader->motionFlags = 3;
  leader->gravity = 0xcccc;
  leader->bounce = 0;
  FieldEvent_Wait(1);
  leader->angle = 0xc000;
  i = 60;
second:
  phase = gFrameCount & 1;
  if (phase == 0) {
    offset[0] = ((Random16Far() & 15) << 16) - 0x80000;
    offset[1] = (Random16Far() & 15) << 16;
    Func_0200015c(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                  0x13333, phase, 0xb0000, opt);
  }
  wait(1);
  if (leader->velocityY != 0) {
    i--;
    if (i != 0)
      goto second;
  }
  Func_02003c54(188);
  Actor_SetSingleChildFlags(leader, 1);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Func_02003ac4, 0x50000, 0x80000, 0x10000);
  Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
  Func_02003acc();
  Func_02003b74(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  Actor_SetModeById(gMainRoutineWork.currentOwner, 4);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  GameFlag_ClearBit(0x260);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Func_02003af4();
}
