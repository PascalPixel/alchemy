// Matches: differ 0 of 270 (tla-en, 2026-11-06).
// The two Random16Far calls sit in the effect call's arguments, called
// directly; through a forwarded inline or with the values in locals the frame
// bit and the options address take other registers (differ 30 to 47).
// The three loops are labels because real loops rotate and hoist; step and id
// are reused for the frame bit and the spark count, as the listing's registers
// are. Reusing other variables (source, flame, other) is far worse. The two
// forwarded inlines are scene_call.h's layer.
// Magma Rock: an object of type 403 sent from another
// object along its facing a step at a time until it reaches a cell of value
// 212 or 222 or meets an object on the same level within 8 pixels. At an
// object whose sprite shape is not 272 it moves there and flag 752 is set
// around that object's reaction (Func_02000ce4); at the end cell it moves
// there and spawns effects for 16 frames (update Func_02000ea4), then cell 222
// starts the room's event (Func_02001350); otherwise the camera returns to
// the leader.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "field_effect.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02004c98(struct Actor *object, s32 mode);
void Func_02004e90(s32 value);
s32 Math_Cosine(s32 angle);

extern const s32 gData_0200502c[];

s32 Func_02000658(s32 layer, s32 x, s32 z);
s32 Func_020005b0(s32 *from, s32 *to);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004cc8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004cd0(struct Actor *object);
void Func_02004cb0(struct Actor *object);
void Func_02004e10(s32 a, s32 b);
void Func_02004e20(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
u32 Random16Far(void);
void Func_02000d1c(struct Actor *object);
void Func_02000ea4(struct Actor *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001350(void);
void Func_02000ce4(u16 id);

struct ActorTable {
  u8 unk_0[20];
  struct Actor *actors[64];
};

static __inline__ void Func_02004e10_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004e10(a, b);
}

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

void Func_02000ed0(struct Actor *source) {
  struct Actor *flame =
      Import_ActorCreate_(403, source->x, source->y, source->z);
  struct Actor *other;
  s32 position[3];
  struct EffectOptions options;
  s32 step = Math_Cosine(source->angle) << 1;
  s32 cell;
  s32 id;

  flame->motionFlags = 0;
  Func_02004c98(flame, 1);
  Actor_SetSingleChildFlags(flame, 0);
  Func_02004d40();
  Func_02004e90(0);
  position[0] = flame->x;
  position[1] = flame->y;
  position[2] = flame->z;
next:
  position[0] += step;
  cell =
      Func_02000658(2, position[0] >> 20, (position[2] - position[1]) >> 20) >>
      8;
  if (cell == 212 || cell == 222)
    goto end;
  id = 8;
inner:
  other = ((struct ActorTable *)Ram_HeapSlots->eventWork)->actors[id];
  if (other->script == 0)
    goto skip;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    goto skip;
  if (Func_020005b0(&other->x, position) > 8)
    goto skip;
  if (other->y >> 20 != position[1] >> 20)
    goto skip;
  if (*(s16 *)(*(u8 **)((u8 *)other->animation + 40)) == 272)
    goto end;
  goto hit;
skip:
  id++;
  if (id == 64)
    goto next;
  goto inner;
end:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  Func_02004cc8(flame, position[0], position[1], position[2]);
  Func_02004e10_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02004cd0(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  id = 0;
spark:
  step = gFrameCount;
  step &= 1;
  if (step == 0) {
    options.type = 403;
    options.priority = 1;
    options.update = (void (*)(union FieldObject *))Func_02000ea4;
    options.script = gData_0200502c;
    Func_0200015c(position[0], position[1], position[2],
                  ((Random16Far() & 31) - 16) << 13,
                  ((Random16Far() & 31) - 16) << 13, step, 0x1320000,
                  &options);
  }
  wait(1);
  id++;
  if (id != 16)
    goto spark;
  wait(24);
  if (cell == 222) {
    Func_02001350();
    Func_02004d48();
    return;
  }
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  wait(10);
  Func_02004d48();
  return;
hit:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  Func_02004cc8(flame, position[0], position[1], position[2]);
  Func_02004e10_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02004cd0(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  Flag_Set(752);
  Func_02000ce4((u16)id);
  Flag_Clear(752);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004e20();
  wait(10);
  Func_02004d48();
}
