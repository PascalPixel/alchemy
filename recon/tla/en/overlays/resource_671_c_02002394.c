// 26 October 2026, under -mthumb-slow-loads: differ 4 of 177: the velocity
// array's address, which the native builds after the cosine call in each pass
// and the draft hoists out of the loop (also as a goto loop); alchemy permute
// (5 to 6 minutes) and an empty asm or one-pass block after any one statement
// do not move it.
// Draft, differ 10 of 177 (was 22): written from the listing (☀️'s
// FieldScene_RunSupplementalSequenceOne is its twin). i pinned to r10 gives the
// options pointer r8, as the native has them; a pinned i needs the do-while
// loop (as a for loop it keeps an entry test, differ 16). Left: the velocity
// array's address is built before the cosine call, not after it; and both
// random terms load the velocity word one row later than the native (it loads
// between the shift and the add of x * 3). The inlines keep the -1 and 0x50000
// constants per call, as ☀️'s scene layer does (differ 59 called directly).
// Tried: declaration orders, velocity at function scope, the random terms as
// separate subtractions, permuter 4 minutes (24), velocity through a pointer
// pinned to r6 with the cosine taken into a local first (63).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_0200564c(void);
void WaitTicks(s32 ticks);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005854(s32 sound);
void FieldEvent_WaitTicks(s32 ticks);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02005794(s32 id, s32 value);
void Func_02005694(s32 a, s32 b, s32 c);
void Func_0200569c(void);
void Func_020056bc(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Func_02005694(a, b, c);
}

void Func_02002394(void) {
  struct ObjectRuntime *actor = Object_GetById(gPartyState.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 22 of 177 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;

  Func_020056b4();
  Func_0200580c(0);
  CamBounds(-1, -1, -1, 0);
  Func_0200564c();
  WaitTicks(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005854(204);
  actor->motionFlags = 3;
  FieldEvent_WaitTicks(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
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
    Func_020000b8(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Func_02005854(188);
  Func_02005794(gPartyState.currentOwner, 0x101);
  Object_SetModeById(gPartyState.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  Func_0200569c();
  Func_02005794(gPartyState.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  WaitTicks(10);
  Object_SetModeById(gPartyState.currentOwner, 1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Func_020056bc();
}
