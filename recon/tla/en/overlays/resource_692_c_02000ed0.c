// Draft, differ 263 of 270: Magma Rock: an object of type 403 sent from another
// object along its facing a step at a time until it reaches a cell of value
// 212 or 222 or meets an object on the same level within 8 pixels. At an
// object whose sprite shape is not 272 it moves there and flag 752 is set
// around that object's reaction (Func_02000ce4); at the end cell it moves
// there and spawns effects for 16 frames (update Func_02000ea4), then cell 222
// starts the room's event (Func_02001350); otherwise the camera returns to
// the leader.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "frame.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02004c98(struct ObjectRuntime *object, s32 mode);
void Func_02004e90(s32 value);
s32 Math_Cosine(s32 angle);

extern const s32 gData_0200502c[];

s32 Func_02000658(s32 layer, s32 x, s32 z);
s32 Func_020005b0(s32 *from, s32 *to);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004cc8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004cd0(struct ObjectRuntime *object);
void Func_02004cb0(struct ObjectRuntime *object);
void Func_02004e10(s32 a, s32 b);
void Func_02004e20(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
u32 Random16Far(void);
void Func_02000d1c(struct ObjectRuntime *object);
void Func_02000ea4(struct ObjectRuntime *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02001350(void);
void Func_02000ce4(u16 id);

void Func_02000ed0(struct ObjectRuntime *source) {
  struct ObjectRuntime *flame = (struct ObjectRuntime *)Import_ActorCreate(
      403, source->x, source->y, source->z);
  struct ObjectRuntime *other;
  struct EffectOptions options;
  s32 position[3];
  s32 step = Math_Cosine(source->angle) << 1;
  s32 cell;
  s32 id;
  s32 i;
  u32 frame;

  flame->motionFlags = 0;
  Func_02004c98(flame, 1);
  ObjectDispatch_SetSingleChildFlags(flame, 0);
  Func_02004d40();
  Func_02004e90(0);
  position[0] = flame->x;
  position[1] = flame->y;
  position[2] = flame->z;
  for (;;) {
    position[0] += step;
    cell = Func_02000658(2, position[0] >> 20,
                         (position[2] - position[1]) >> 20) >>
           8;
    if (cell == 212 || cell == 222)
      break;
    for (id = 8; id != 64; id++) {
      other =
          ((struct ObjectRuntime **)((u8 *)Ram_HeapSlots->eventWork + 20))[id];
      if (other->script == 0)
        continue;
      if (source->x >> 20 == other->x >> 20 &&
          source->y >> 20 == other->y >> 20 &&
          source->z >> 20 == other->z >> 20)
        continue;
      if (Func_020005b0(&other->x, position) > 8)
        continue;
      if (other->y >> 20 != position[1] >> 20)
        continue;
      if (*(s16 *)(*(u8 **)((u8 *)other->animation + 40)) != 272)
        goto hit;
      goto end;
    }
  }
end:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  Func_02004cc8(flame, position[0], position[1], position[2]);
  Func_02004e10(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02004cd0(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  for (i = 0; i != 16; i++) {
    frame = gFrameCount & 1;
    if (frame == 0) {
      options.type = 403;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))Func_02000ea4;
      options.script = gData_0200502c;
      Func_0200015c(position[0], position[1], position[2],
                    ((Random16Far() & 31) - 16) << 13, 0,
                    ((Random16Far() & 31) - 16) << 13, 0x1320000, &options);
    }
    WaitFrames(1);
  }
  WaitFrames(24);
  if (cell == 222) {
    Func_02001350();
    Func_02004d48();
    return;
  }
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02004e20();
  WaitFrames(10);
  Func_02004d48();
  return;
hit:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  Func_02004cc8(flame, position[0], position[1], position[2]);
  Func_02004e10(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(position[0], position[1], position[2], 1);
  Func_02004cd0(flame);
  flame->update = 0;
  Func_02004cb0(flame);
  GameFlag_SetBit(752);
  Func_02000ce4((u16)id);
  GameFlag_ClearBit(752);
  Camera_FollowActor(gAdventure.currentOwner, 1);
  Func_02004e20();
  WaitFrames(10);
  Func_02004d48();
}
