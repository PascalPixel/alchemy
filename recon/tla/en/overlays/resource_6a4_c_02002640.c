// Near miss, differ 4 of 169 (tla-en, 2026-10-30): the velocity array's
// address (add r6, sp, #16), which the native builds after the cosine call in
// each pass and gcse's PRE hoists here (PRE/HOIST: edge (0,1)). The native is
// the cse dump's form left in place: PRE leaves the address in the loop only
// when the loop's header is its exit test, as when the entry jumps to a bottom
// test (i pinned, "goto test" at entry and the test after the body: the
// address stays after the cosine call, 13 of 169 with the entry branch and one
// load a row later; unpinned, 25). No plain loop keeps that entry jump past
// gcse: jump1 copies a simple exit test to the top and cse1 folds it (for and
// while loops, unpinned 16, pinned 10 with the entry test kept and the address
// inserted after it). A pointer pinned to r6 only copies the hoisted
// register (61). Overlay 6a4's copy of the function drafted in
// resource_698_c_02001674.c (overlay 698's Func_02001674), whose header says
// what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
void Func_02002a88(void);
void Func_02002b18(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02002a18(void);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002b68(s32 sound);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02001e78(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02002ad8(s32 id, s32 value);
void Func_02002a60(s32 a, s32 b, s32 c);
void Func_02002a68(void);
void Func_02002a90(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Func_02002a60(a, b, c);
}

void Func_02002640(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 22 of 177 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;

  Func_02002a88();
  Func_02002b18(0);
  CamBounds(-1, -1, -1, 0);
  Func_02002a18();
  wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002b68(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_02001e78(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Func_02002b68(188);
  Func_02002ad8(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  Func_02002a68();
  Func_02002ad8(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02002a90();
}
