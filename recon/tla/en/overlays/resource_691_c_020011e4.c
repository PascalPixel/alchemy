// Near miss, differ 69 of 260 (tla-en, 2026-10-31): the control flow, calls,
// fields and frame (68 bytes: config at sp+16, position at sp+56) are the
// native's; what is left is allocation and scheduling. GCSE merges every
// computation of the position's address into one register (r7) from the
// first copy on; the native computes it again for the first copy (r2), at the
// head of the advance loop and at stop (add r7, sp, #56), as this compiler
// does only with -fno-gcse. In the spawn block local allocation gives the
// config's address r6 and the mask 31 r8; the native has the config in r8
// (copied to r0 for each store) and the mask in r6. CSE keeps flag 0x2f0 in
// r5 across Func_02000c08 where the native builds it for each call (through
// flag_call.h's Flag_Set and Flag_Clear, differ 63). sched2 places the
// motionFlags store's movs r3, #0 and stop's lsls r1, r1, #7 one step off.
// Measured forms: the creation's x, y and z read into locals first (passed
// straight, the loads come after the 403 and go through copies of r8: 150 to
// 134); `if (c) goto A; goto B;` comes out inverted (beq B; b A), so the
// search's end is written with the native's tests turned round (130); one
// local for the step and, later, the frame's parity, as the native's r10
// holds both (69; a separate parity variable is caller-saved in r4 and
// spilled, and the step and the counter swap r9 and r10). As a for loop the
// search lets loop.c hoist 0x110 and the allocation cascades (226).
//
// Creates actor 403 at the source's position and moves it along x by twice
// the cosine of the source's angle until the layer 2 cell under it has
// attribute 212 or 222, or an active object 8 to 63 in another cell is within
// 8 of it (Func_020005a0) on its height row. When it stops on the cell or on
// an object whose animation value is 0x110, it settles, and effect 403 is
// thrown every other frame for 16 frames; on any other object it calls
// Func_02000c08 with that object's index while flag 0x2f0 is set.
#include "types.h"
#include "actor.h"
#include "field/event_work.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

extern u32 gFrameCount;
extern u32 gData_02003d50[];

struct FieldActor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
s32 Math_Cosine(s32 angle);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
u32 Random16Far(void);
void wait(s32 frames);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);
s32 Func_020005a0(s32 *a, s32 *b);
void Func_02000c08(u32 index);
void Func_02000d40(void);
void Func_020011b8(void);
void Func_02003a54(struct Actor *object, s32 a);
void Func_02003a6c(struct Actor *object);
void Func_02003a84(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003a8c(struct Actor *object);
void Func_02003aec(void);
s32 Func_02003af4(void);
void Func_02003b5c(s32 a, s32 b);
void Func_02003b6c(void);
void Func_02003bcc(s32 value);

s32 Func_020011e4(struct Actor *source) {
  s32 position[3];
  struct MagmaRockEffectConfig config;
  struct Actor *object;
  struct Actor *other;
  s32 step;
  s32 i;
  s32 attributes;

  {
    s32 x = source->x;
    s32 y = source->y;
    s32 z = source->z;

    object = (struct Actor *)Import_ActorCreate(403, x, y, z);
  }
  step = Math_Cosine(source->angle) << 1;
  object->motionFlags = 0;
  Func_02003a54(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Func_02003aec();
  Func_02003bcc(0);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
advance:
  position[0] += step;
  attributes = MapCell_GetAttributes(2, position[0] >> 20,
                                     (position[2] - position[1]) >> 20) >>
               8;
  if (attributes == 212 || attributes == 222)
    goto stop;
  i = 8;
search:
  other = ((struct EventWork *)Ram_HeapSlots->eventWork)->objects[i];
  if (other->script == NULL)
    goto next;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    goto next;
  if (Func_020005a0(&other->x, position) > 8)
    goto next;
  if (other->y >> 20 != position[1] >> 20)
    goto next;
  if (*((struct ObjectAnimationState *)other->animation)->value28 == 0x110)
    goto stop;
  goto hit;
next:
  i++;
  if (i == 64)
    goto advance;
  goto search;

stop:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = Func_02000d40;
  Func_02003a84(object, position[0], position[1], position[2]);
  Func_02003b5c(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02003a8c(object);
  object->update = NULL;
  Func_02003a6c(object);
  i = 0;
spawn:
  {
    // The native keeps the frame's parity in the step's variable (r10).
    step = gFrameCount & 1;

    if (step == 0) {
      s32 dx;
      s32 dz;
      s32 mask;

      config.id = 403;
      config.kind = 1;
      config.unk24 = Func_020011b8;
      mask = 31;
      config.data = gData_02003d50;
      dx = Random16Far();
      dz = Random16Far();
      Func_0200015c(position[0], position[1], position[2],
                    ((dx & mask) - 16) << 13, ((dz & mask) - 16) << 13, step,
                    0x1320000, &config);
    }
  }
  wait(1);
  i++;
  if (i != 16)
    goto spawn;
  wait(24);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  wait(10);
  return Func_02003af4();

hit:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = Func_02000d40;
  Func_02003a84(object, position[0], position[1], position[2]);
  Func_02003b5c(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02003a8c(object);
  object->update = NULL;
  Func_02003a6c(object);
  GameFlag_SetBit(0x2f0);
  Func_02000c08((u16)i);
  GameFlag_ClearBit(0x2f0);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  wait(10);
  return Func_02003af4();
}
