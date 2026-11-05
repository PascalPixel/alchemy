// Draft, differ 40 of 260 (tla-en, 5 November 2026). Plain loops throughout, the figure
// unchanged by them. Creates actor 403 at the source's position and moves it along x by
// twice the cosine of the source's angle until the layer 2 cell under it has attribute 212
// or 222, or an active object 8 to 63 in another cell is within 8 of it (Func_020005a0) on
// its height row. When it stops on the cell or on an object whose animation value is 0x110,
// it settles, and effect 403 is thrown every other frame for 16 frames; on any other object
// it calls Func_02000c08 with that object's index while flag 0x2f0 is set. Left: in the
// spawn block the config's address takes r6 and the mask r8, the native the reverse (.444
// against .107 in local-alloc); GCSE merges the position's address into r7 where the native
// has add r7, sp, #56 again; CSE keeps flag 0x2f0 in r5 across Func_02000c08.
#include "call.h"
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
  for (;;) {
  position[0] += step;
  attributes = MapCell_GetAttributes(2, position[0] >> 20,
                                     (position[2] - position[1]) >> 20) >>
               8;
  if (attributes == 212 || attributes == 222)
    goto stop;
  for (i = 8; i != 64; i++) {
  other = ((struct EventWork *)Ram_HeapSlots->eventWork)->objects[i];
  if (other->script == NULL)
    continue;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    continue;
  if (Func_020005a0(&other->x, position) > 8)
    continue;
  if (other->y >> 20 != position[1] >> 20)
    continue;
  if (*((struct ObjectAnimationState *)other->animation)->value28 == 0x110)
    goto stop;
  goto hit;
  }
  }

stop:
  object->acceleration = 0x20000;
  object->speedLimit = 0x20000;
  object->update = Func_02000d40;
  Func_02003a84(object, position[0], position[1], position[2]);
  Call2((void (*)())Func_02003b5c, 0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02003a8c(object);
  object->update = NULL;
  Func_02003a6c(object);
  for (i = 0; i != 16; i++)
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
      dx = Random16Far() & mask;
      dz = Random16Far() & mask;
      Func_0200015c(position[0], position[1], position[2],
                    (dx - 16) << 13, (dz - 16) << 13, step,
                    0x1320000, &config);
    }
  wait(1);
  }
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
  Call1((void (*)())GameFlag_SetBit, 0x2f0);
  Func_02000c08((u16)i);
  GameFlag_ClearBit(0x2f0);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  wait(10);
  return Func_02003af4();
}
