// Matches: differ 0 of 169 (tla-en, 2026-10-30), with the pinned i and one
// dead test (both tagged).
// The landing burst shared by thirteen overlays (698 to 6a4): the leader
// drops from 130 pixels while the screen transition runs, then seventeen
// effects burst outward and the leader bounces. Overlay 671's Func_02002394
// (resource_671_c_02002394.c) is the same event with a 0xcccc start scale,
// FieldEvent_Wait for its first wait and a closing work store; its header
// lists what was tried.
// What was left (differ 4): the velocity array's address (add r6, sp, #16),
// which the native builds after the cosine call in every pass. In a loop
// entered at its top, GCSE's partial redundancy pass hoists it whatever the
// loop's form, and in a do, for or while loop loop.c moves it out as well.
// The fix: a goto loop (no loop notes, so loop.c leaves it) whose spawn runs
// under a test of a constant that only GCSE's last propagation folds: while
// the partial redundancy pass runs, the address is not needed on every path
// into the loop, so it stays where it is used, and the folded test leaves no
// instruction. The options pointer, built before the loop, is still carried
// in r8 as the native does. Also tried: entering the goto loop at its bottom
// test (address in place, but the entry branch stays: differ 13 of 170).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"

struct Actor *Actor_GetById(s32 id);
void Func_02001a7c(void);
void Func_02001b0c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001a24(void);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b44(s32 sound);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02000eac(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02001acc(s32 id, s32 value);
void Func_02001a54(s32 a, s32 b, s32 c);
void Func_02001a5c(void);
void Func_02001a84(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Func_02001a54(a, b, c);
}

void Func_02001674(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 12 of 169 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;
  s32 spawn = 1;

  Func_02001a7c();
  Func_02001b0c(0);
  CamBounds(-1, -1, -1, 0);
  Func_02001a24();
  wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001b44(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  // A goto loop carries no loop notes: as a do, for or while loop, loop.c
  // moves the velocity array's address out of the loop.
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 169 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sine(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
      Func_02000eac(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Func_02001b44(188);
  Func_02001acc(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  Func_02001a5c();
  Func_02001acc(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02001a84();
}
