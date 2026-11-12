#include "local.h"
#include "io_reg.h"

extern const u8 gData_02005528[];
extern const u8 gData_0200552e[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004ee8(void);
void Func_020006dc(const u8 *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_0200063c(struct Actor *object);
void Func_02004cb0(struct Actor *object);

void Func_02000a30(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  /* FAKEMATCH(pin): copy in r5, as the native keeps it; as a plain local,
   * 78 of 281 instructions differ in tla-en. */
  register struct Actor *copy asm("r5");
  s32 scene;
  s32 x;
  s32 z;

  Func_02004d40();
  Event_ResolvePendingActions(0);
  Func_02004ee8();
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room6) {
    Func_020006dc(gData_02005528);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      if (Event_GetFlag(770)) {
        object->gravity = 0x1000;
        object->priorityFlags |= 2;
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f10;
        copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                           object->z + 0x100000, 312);
        Func_0200063c(object);
        Event_PlaySound(213);
        Func_02004cb0(copy);
        Func_02004c98(object, 4);
        System_Wait(1);
        REG_BLDCNT = 0;
      } else {
        Func_0200063c(object);
        Event_PlaySound(188);
      }
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    Func_020006dc(gData_0200552e);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      object->gravity = 0x1000;
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      REG_BLDCNT = 0x3f10;
      copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                         object->z + 0x100000, 312);
      Func_0200063c(object);
      Event_PlaySound(213);
      Func_02004cb0(copy);
      Func_02004c98(object, 4);
      System_Wait(1);
      REG_BLDCNT = 0;
      Func_0200063c(object);
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  }
  x = object->x >> 20;
  z = object->z >> 20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    if (z == 16)
      Event_SetFlag(2544);
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    if (z == 41)
      Event_SetFlag(2545);
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    if (x == 21) {
      if (z == 43)
        Event_SetFlag(2546);
      if (z == 45)
        Event_SetFlag(2547);
      if (z == 47)
        Event_SetFlag(2548);
    }
  } else if (scene == (s32)&SceneId_Ov692Room6) {
    if (x == 47) {
      Event_SetFlag(2549);
      {
        /* FAKEMATCH(temporary): the column and row in locals, so each stack
         * word builds in its own register; as constants, 4 of 281 instructions
         * differ in tla-en. */
        s32 column = 46;
        s32 row = 73;

        MapGrid_CopyCellAttributes(column, 74, 1, 1, column, row);
      }
    }
    if (x == 57)
      Event_SetFlag(2550);
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    if (x == 43)
      Event_SetFlag(2551);
  }
  Func_02004d48();
}

#include "types.h"
#include "actor.h"
#include "frame.h"
#include "field_effect.h"

void Func_02004e70(s32 a, s32 b);
void Func_02004e78(s32 owner, u16 value);
s32 Field_DispatchTypeHandler(s32 type);
s32 Func_02004e80(void);
s32 Func_02004e88(void);

void Func_02000ce4(s16 value) {
  struct MainRoutineWork *party;

  Func_02004e70(151, 1);
  party = &gMainRoutineWork;
  Func_02004e78(party->currentOwner, value);
  Field_DispatchTypeHandler(1);
  Func_02004e80();
  Func_02004e88();
}

extern const s32 gData_0200502c[];

void Func_02004f28(struct Actor *object, s32 sound);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

/// An object's update: every fourth frame it spawns an effect of type 403 with
/// its own script at a random point within 4 pixels of the object along z,
/// moving along z at a random speed; every sixteenth frame it also plays sound
/// 136 at the object.
void Func_02000d1c(struct Actor *object) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 dz;
  s32 vz;
  s32 z;

  if ((frame & 3) != 0)
    return;
  options.type = 403;
  options.priority = 2;
  options.script = gData_0200502c;
  if ((frame & 15) == 0)
    Func_02004f28(object, 136);
  dz = Random16Far() & 15;
  vz = ((Random16Far() & 31) - 16) << 12;
  /* FAKEMATCH(temporary): z read into its own local; as object->z in the
   * call, 4 of 60 instructions differ in tla-en. */
  z = object->z;
  Func_0200015c(object->x, object->y, z + ((dz - 8) << 15), 0, 0, vz, 0x320001,
                &options);
}

void Func_02000da4(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetSpriteColor(object, 10);
  else
    Actor_SetSpriteColor(object, 0);
}

// The cell watch's table lookup over (id, cell, value) rows ended by 0xffff:
// for the row of the cell under the watch, sets the watch's object to the row's
// object and returns its value; otherwise returns the value of the last row
// passed (0 for none). The loop is spelled with gotos in the native's rotated
// layout, which rebuilds 0xffff on every pass, where loop.c would hoist it out
// of a C loop.

struct CellWatch {
  u8 unk00[4];
  s32 x;
  u8 unk08[4];
  s32 z;
  u8 unk10[4];
  struct Actor *object;
};

s32 Func_02000658(u32 layer, s32 x, s32 y);

s32 Func_02000dcc(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 cell = Func_02000658(0, watch->x >> 20, watch->z >> 20) >> 8;
  u32 id;

  goto test;
again:
  result = *table++;
test:
  if (*table != 0xffff) {
    id = *table++;
    if (cell != *table++)
      goto again;
    watch->object = Actor_Get(id);
    result = *table;
  }
  return result;
}

void Func_02000e14(void) { Event_ResolvePendingActions(0); }
