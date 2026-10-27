// Under -mthumb-slow-loads (2026-10-26): differ 4 of 177; left: the velocity
// array's address is hoisted out of the loop by GCSE, as overlay 698's copy
// records (resource_698_c_02001674.c).
// Draft, differ 10 of 177: the leader drops into the room from above: the
// screen fades in, the leader lands with seventeen dust effects, a shake and a
// bounce. Overlay 671's Func_02002394 (whose draft holds the notes) is the same
// scene with other scales; overlay 686's Func_02000418 is a copy. Measured
// against its copies: overlay 686's Func_02000418 (differ 10 of 177).
#include "types.h"
#include "lib/call.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "system/field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e5c(void);
void Func_02004fcc(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004dc4(void);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005014(s32 sound);
void FieldEvent_WaitFrames(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02004f34(s32 id, s32 value);
void Object_SetModeById(u32 id, s32 mode);
void Func_02004e24(s32 a, s32 b, s32 c);
void Func_02004e2c(void);
s32 Func_02004e64(void);

s32 Func_0200042c(void) {
  struct ObjectRuntime *actor = Object_GetById(gPartyState.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer, as in
   * overlay 671's draft. */
  register u32 i asm("r10");
  struct EffectOptions options;

  Func_02004e5c();
  Func_02004fcc(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02004dc4();
  WaitFrames(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005014(204);
  actor->motionFlags = 3;
  FieldEvent_WaitFrames(24);
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
    Func_0200015c(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Func_02005014(188);
  Func_02004f34(gPartyState.currentOwner, 0x101);
  Object_SetModeById(gPartyState.currentOwner, 49);
  Call3((void (*)())Func_02004e24, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02004e24, -1, -1, 0xe666);
  Func_02004e2c();
  Func_02004f34(gPartyState.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_WaitFrames(10);
  Object_SetModeById(gPartyState.currentOwner, 1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  return Func_02004e64();
}
