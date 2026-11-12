#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "fixed_math.h"
#include "frame.h"
#include "ram_buffer.h"
#include "magma_rock.h"
#include "macros.h"
#include "scene_ids.h"

extern s16 gData_02003de4[];

struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001e08(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;
  u8 *terrainId;

  cell[0] = leader->x + 0x100000;
  cell[1] = leader->y;
  cell[2] = leader->z;
  object = Actor_FindInCell(cell, leader);
  Func_0200074c(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 92 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    Func_020005cc(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  Func_020003b4();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de4);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    Event_SetFlag(0x8d9);
}

extern s16 gData_02003de8[];

struct Actor *Actor_FindInCell(s32 *position, struct Actor *except);
void Func_0200074c(struct Actor *object);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);
void Func_020003b4(void);
void Func_020006cc(s16 *ids);

void Func_02001edc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  struct Actor *object;
  u8 *terrainId;

  position[0] = leader->x - 0x100000;
  position[1] = leader->y;
  position[2] = leader->z;
  object = Actor_FindInCell(position, leader);
  Func_0200074c(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals; as call arguments,
     * 2 of 90 instructions differ in tla-en. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    Func_020005cc(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  Func_020003b4();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                (object->terrainHeight >> 19) + 4);
  Func_020006cc(gData_02003de8);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(0x8db);
}

void Func_02001fb0(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(0x250);
}

void Func_02001fd4(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(0x251);
}

void Func_02001ffc(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(0x252);
}

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[8];
};

extern u32 gData_02003d50[];

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);

void Func_02002024(s32 x, s32 z) {
  struct MagmaRockEffectConfig config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.unk08 = Q16_ONE * 2;
  config.unk0c = Q16_ONE * 2;
  config.id = 403;
  kind = 1; // Holds one constant: the native loads 1 before the data address.
  config.data = gData_02003d50;
  config.kind = kind;
  i = 0;
loop:
  position[0] = x + ((Random16Far() & 63) << 16) - Q16_ONE * 32;
  position[1] = (Random16Far() & 15) << 12;
  position[2] = z + ((Random16Far() & 63) << 16) - Q16_ONE * 32;
  Func_0200015c(position[0], 0, position[2], 0, position[1], 0, 0x3a0001,
                &config);
  i++;
  if (i != 4)
    goto loop;
  Event_SetFlag(0x8dc);
}

void Func_020020d0(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetSpriteColor(object, 7);
  else
    Actor_SetSpriteColor(object, 0);
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_020020d0(struct Actor *object);
void Func_02002024(s32 x, s32 z);
void Func_02003820(s32 x, s32 y, s32 z, s32 kind);
void Func_020038dc(void);
void Func_02003998(void);

/// Func_020014bc's counterpart: on cue 0 it hands the target to Func_020020d0;
/// modes 2 and 3 run Func_02002024 at the target's cell, and mode 3 also clears
/// its position and copies the cells at columns 32 and 96.
void Func_020020f8(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0) {
    struct Actor *object;

    Func_020038dc();
    object = work->target;
    BYTE_AT(object, 99) = 4;
    object->update = (void *)Func_020020d0;
  }
  if (mode == 2) {
    Func_02002024(work->target->x, work->target->z);
    Func_02003820(work->target->x, work->target->y + 0xf0000, work->target->z,
                  1);
  }
  if (mode == 3) {
    struct Actor *target;
    /* FAKEMATCH(temporary): the attribute copies' last argument as a
     * local set first keeps it in r6; as the constant 8, 12 of 104
     * instructions differ in tla-en. */
    s32 last = 8;

    Func_02002024(work->target->x, work->target->z);
    Func_02002024(work->target->x, work->target->z);
    Func_02002024(work->target->x, work->target->z - 0x200000);
    Func_02003820(work->target->x, work->target->y + 0xf0000, work->target->z,
                  16);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
    MapGrid_CopyCellAttributes(0, 32, 5, 8, 42, 17);
    MapGrid_CopyCellAttributes(0, 96, 5, 8, 42, 81);
    MapGrid_CopyCellsTo(0, 32, 42, 17, 5, last);
    MapGrid_CopyCellsTo(0, 96, 42, 81, 5, last);
  }
  if (cue == 0x3e7)
    Func_02003998();
}

struct Trigger;

extern const u8 gData_0200460c[];
extern const u8 gData_02004720[];
extern const u8 gData_020047e0[];
extern const u8 gData_0200487c[];
extern const u8 gData_02004930[];
extern const u8 gData_02004a5c[];

const struct Trigger *Func_020021e8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3)
      return (const struct Trigger *)gData_02004720;
    if (gMainRoutineWork.entrance <= 8)
      return (const struct Trigger *)gData_020047e0;
    if (gMainRoutineWork.entrance <= 12)
      return (const struct Trigger *)gData_0200487c;
    return (const struct Trigger *)gData_02004930;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gData_02004a5c;
  return (const struct Trigger *)gData_0200460c;
}
