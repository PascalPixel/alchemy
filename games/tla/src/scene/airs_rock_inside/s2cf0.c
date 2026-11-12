#include "types.h"
#include "system.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "bg_scroll.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "call.h"
#include "frame.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};
/// An object's priority flags read as a whole-byte bit-field.
struct ObjectPriorityByte {
  u8 unk00[35];
  u8 priorityFlags : 8;
};
extern char ResourceId_LightningMap;
extern char ResourceId_LightningTiles;
extern const u8 gData_02005fd8[];
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void *Resource_GetTableEntry(s32 id);
void Func_02005d24(void *resource, void *dest);
void Func_02005f94(s32 cue);
struct Actor *Actor_GetById(u32 objectId);
void Func_02002cc0(void);
void Func_02002c40(void);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_02005d6c(struct Actor *object, const u8 *script);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void FieldEvent_Wait(s32 frames);
void Func_02005f0c(s32 a, s32 b);
s32 Func_02005f04(s32 a, s32 b);
s32 Func_02005f14(s32 frames);
s32 Camera_FollowActor(s32 id, s32 a);
void Func_02005ee4(void);
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02002aa8(s32 x, s32 z);
void Func_02005eec(s32 actor, s32 a);
void Func_02005e94(s32 actor, s32 a);
void Func_02002b58(s32 x, s32 z);
void Func_02001084(struct Actor *object, s32 a, s32 b);
void Func_02005d34(void);
void Func_02005d2c(void);
void Func_02005e04(s32 hue, s32 brightness);
void GameFlag_SetBit(s32 flag);

/// The second pillar of light, Func_02003654 with object 11 glowing, the camera
/// at (0x138, 0x2a8), the cells at (0x40, 78) opening and the light reaching
/// object 10 (objects 8 and 9 lit with it); sets flag 0x89a.
void Func_02002cf0(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02005e14();
  Func_02005f2c(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address; as a plain block, 2 of 446 instructions differ in
   * tla-en. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Func_02005f94(246);
  ((u8 *)Actor_GetById(11))[98] = 9;
  ((u8 *)Actor_GetById(11))[99] = 1;
  Actor_GetById(11)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(11)->x,
                                 Actor_GetById(11)->y, Actor_GetById(11)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(11)->sprite)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer; as
         * spark->animation, 28 of 446 instructions differ in tla-en. */
        cell = spark->sprite;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      // Through the bit-field views the flags' load is issued before the
      // priority store, as the native does; as plain bytes it waits for it.
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())Func_02005f0c, 0x10000, 0);
  Call2((void (*)())Func_02005f04, 0x10003, 0);
  Func_02005f14(60);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  Call2((void (*)())Func_02005ed4, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(312), -1, PIXELS(680),
        1);
  Func_02005ee4();
  Actor_GetById(11)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(11), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Func_02005f94(138);
  Call2((void (*)())Func_02005f0c, 0x10003, 0);
  Call2((void (*)())Func_02005f04, 0x7fff, 0);
  Func_02005f14(1);
  wait(1);
  Func_02005f0c(0x7fff, 0);
  Func_02005f04(0x10003, 0);
  Func_02005f14(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Func_02005d9c(64, 78, 18, 105, 3, 3);
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  Func_02005f0c(0x10003, 0);
  Func_02005f04(0x10000, 0);
  Func_02005f14(60);
  wait(30);
  Func_02005f94(148);
  Func_02002aa8(0x1380000, 0x2d00000);
  Call6((void (*)())Func_02005d9c, 64, 78, 18, 105, 3, 5);
  Func_02005ed4(0x18000, 0x3000);
  Func_02005eec(10, 1);
  Func_02005ee4();
  Func_02005f94(146);
  Call2((void (*)())Func_02005e94, 10, 0x100);
  Func_02002b58(Actor_GetById(10)->x, Actor_GetById(10)->z);
  Func_02005e94(10, 0);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(3);
  Actor_SetPartAttribute(Actor_GetById(8), 0);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  Actor_SetPartAttribute(Actor_GetById(10), 0);
  wait(40);
  Func_02001084(Actor_GetById(10), 8, 0);
  Func_02005d34();
  Func_02005d2c();
  Func_02005e04(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x89a);
  Func_02005e1c();
}

#include "airs_rock_inside.h"

s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02005efc(s32 countdown);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

/// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
/// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void Func_02003170(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 width;
  s32 height;

  if (Func_02000690(1, leader->x >> 20, (leader->z >> 20) - 1) >> 8 != 30)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(188);
  width = 1;
  height = 2;
  Func_02005d9c(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_GetById(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Func_02005f94(123);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 312, 792);
  FieldEvent_Wait(5);
  Func_02005efc(15);
  Func_02005e1c();
}

extern s16 gData_02006050[];
extern s16 gData_02006054[];
extern s16 gData_0200605a[];
extern s16 gData_02006064[];
extern s16 gData_02006068[];
extern s16 gData_02006072[];
extern s16 gData_0200607e[];

void Func_02005df4(s32 x, s32 z, s32 layer, s32 value);
void Func_02005f74(const s16 *list);

static inline void ClearLayer2Shape(s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which builds the
   * shared z constant again for the second call, as the ROM does; as a direct
   * call, the constant is kept in r5 across the first call, 12 of 68
   * instructions differ in tla-en. */
  Func_02005df4(x, z, 2, 0);
}

/// Runs each room's list on entering it; room 7 also clears two cells' shapes
/// and room 9 stops object 10's timer.
void Func_02003254(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov664Room1) {
    Func_02005f74(gData_02006050);
  } else if (scene == (s32)&SceneId_Ov664Room2) {
    Func_02005f74(gData_02006054);
  } else if (scene == (s32)&SceneId_Ov664Room3) {
    Func_02005f74(gData_0200605a);
  } else if (scene == (s32)&SceneId_Ov664Room4) {
    Func_02005f74(gData_02006064);
  } else if (scene == (s32)&SceneId_Ov664Room7) {
    ClearLayer2Shape(0x2600000, 0x1500000);
    Func_02005df4(0x2700000, 0x1500000, 2, 0);
    Func_02005f74(gData_02006068);
  } else if (scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f74(gData_02006072);
  } else if (scene == (s32)&SceneId_Ov664Room9) {
    Actor_GetById(10)->timer = 0;
    Func_02005f74(gData_0200607e);
  }
}

#include "ram_buffer.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

void Func_02005f7c(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);

/// After the pushed object (the event work's object at 0xcbc) comes to rest:
/// if it stopped above the ground it drops onto it with cue 188; then, by
/// room, the place where it stopped sets the flag for that pillar's spot
/// (Ov664Room1 to 4 and 9), in Ov664Room7 marks two cells and shows objects
/// 13 and 14 by where objects 9, 10 and 11 stand, and in Ov664Room8 and 9
/// sets or clears the flags of the spots along its rows and redraws the
/// pillars' cells (Func_0200062c).
void Func_0200332c(void) {
  struct Actor *pushed = *(struct Actor **)(EVENT_WORK + 0xcbc);
  s32 x, z;

  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f7c();
  if (Map_GetTerrainHeight(2, pushed->x, pushed->z) != pushed->y) {
    pushed->terrainId = 2;
    pushed->motionFlags = 3;
    OverlayObject_WaitUntilIdle(pushed);
    Func_02005f94(188);
    OverlayObject_WaitUntilIdle(pushed);
    pushed->motionFlags = 0;
    pushed->terrainId = 0;
  }
  x = pushed->x >> 19;
  z = pushed->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1) {
    if (x == 103 && z == 41)
      GameFlag_SetBit(0xa8e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room2) {
    if (x == 13 && z == 49)
      GameFlag_SetBit(0x898);
    else if (x == 41 && z == 49)
      GameFlag_SetBit(0x897);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3) {
    if (x == 33 && z == 79)
      GameFlag_SetBit(0x89b);
    else if (x == 77 && z == 71)
      GameFlag_SetBit(0x89c);
    else if (x == 59 && z == 65)
      GameFlag_SetBit(0x89d);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    if (x == 81 && z == 37)
      GameFlag_SetBit(0x89e);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
    Call4((void (*)())Func_02005df4, 0x2600000, 0x1500000, 2, -4);
    Call4((void (*)())Func_02005df4, 0x2700000, 0x1500000, 2, -4);
    if (x == 43 && z == 87)
      GameFlag_SetBit(0x8d0);
    if ((Actor_GetById(9)->x >> 19 == 73 && Actor_GetById(9)->z >> 19 == 43) ||
        (Actor_GetById(10)->x >> 19 == 73 && Actor_GetById(10)->z >> 19 == 43))
      ((u8 *)Actor_GetById(13))[99] = 1;
    else
      ((u8 *)Actor_GetById(13))[99] = 0;
    if (pushed->x == Actor_GetById(11)->x &&
        pushed->z == Actor_GetById(11)->z) {
      if (x == 83 && z == 43)
        ((u8 *)Actor_GetById(14))[99] = 1;
      else
        ((u8 *)Actor_GetById(14))[99] = 0;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    if (x == 35 && z == 59)
      GameFlag_SetBit(0x8d1);
    if (x == 45 && z == 105)
      GameFlag_SetBit(0x8d2);
    if (x == 23 && z == 77) {
      GameFlag_SetBit(0x200);
      GameFlag_SetBit(0xa8f);
    }
    if (z == 59 && (x == 67 || x == 71 || x == 73))
      GameFlag_SetBit(513);
    if (z == 19 && (x == 75 || x == 79))
      GameFlag_SetBit(514);
    if (x == 69 && z == 59)
      GameFlag_ClearBit(513);
    if (x == 77 && z == 19)
      GameFlag_ClearBit(514);
    Func_0200062c(2, Actor_GetById(8)->x >> 20, Actor_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Actor_GetById(9)->x >> 20, Actor_GetById(9)->z >> 20, 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
    if (x == 45 && z == 33)
      GameFlag_SetBit(0x8d3);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20, 0);
  }
  Func_02005e1c();
}

#undef EVENT_WORK

/// A pillar of light rises in Air's Rock: BG0 is hidden while its tiles and
/// its map (the palette bits cleared, copied in 16 rows of 8 bytes) are loaded
/// into VRAM; object 10 glows (Func_02002cc0) and sheds a spark (object type
/// 0x2a0, Func_02002c40) every fourth frame for 45 frames; the camera moves to
/// the light, BG0 is shown blended (cue 138), the light flashes, the cells at
/// (68, 84) open, the blend fades over 32 frames, then (cue 148) the light
/// reaches object 8, which flashes and leaves (Func_02001084); the display is
/// put back and flag 0x899 set.
void Func_02003654(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02005e14();
  Func_02005f2c(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address; as a plain block, 2 of 439 instructions differ in
   * tla-en. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Func_02005d24(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Func_02005f94(246);
  ((u8 *)Actor_GetById(10))[98] = 9;
  ((u8 *)Actor_GetById(10))[99] = 1;
  Actor_GetById(10)->update = Func_02002cc0;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(0x2a0, Actor_GetById(10)->x,
                                 Actor_GetById(10)->y, Actor_GetById(10)->z);
      {
        s32 priority = ((s8 *)Actor_GetById(10)->sprite)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer; as
         * spark->animation, 26 of 439 instructions differ in tla-en. */
        cell = spark->sprite;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      // Through the bit-field views the flags' load is issued before the
      // priority store, as the native does; as plain bytes it waits for it.
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02002c40;
    }
    FieldEvent_Wait(1);
  }
  Call2((void (*)())Func_02005f0c, 0x10000, 0);
  Call2((void (*)())Func_02005f04, 0x10003, 0);
  Func_02005f14(60);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  Call2((void (*)())Func_02005ed4, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(216), -1, PIXELS(224),
        1);
  Func_02005ee4();
  Actor_GetById(10)->update = 0;
  Actor_SetPartAttribute(Actor_GetById(10), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff98;
  gBgScroll[0].y = 42;
  wait(30);
  Func_02005f94(138);
  Call2((void (*)())Func_02005f0c, 0x10003, 0);
  Call2((void (*)())Func_02005f04, 0x7fff, 0);
  Func_02005f14(1);
  wait(1);
  Func_02005f0c(0x7fff, 0);
  Func_02005f04(0x10003, 0);
  Func_02005f14(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  Func_02005d9c(68, 84, 12, 77, 3, 3);
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
  i = 0;
fade:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  wait(1);
  i++;
  if (i != 32)
    goto fade;
  Func_02005f0c(0x10003, 0);
  Func_02005f04(0x10000, 0);
  Func_02005f14(60);
  wait(30);
  Func_02005f94(148);
  Func_02002aa8(0xd80000, 0x12d0000);
  Call6((void (*)())Func_02005d9c, 68, 84, 12, 77, 3, 6);
  Func_02005ed4(0x18000, 0x3000);
  Func_02005eec(8, 1);
  Func_02005ee4();
  Func_02005f94(146);
  Call2((void (*)())Func_02005e94, 8, 0x100);
  Func_02002b58(Actor_GetById(8)->x, Actor_GetById(8)->z + 0x200000);
  Func_02005e94(8, 0);
  Actor_SetPartAttribute(Actor_GetById(8), 7);
  wait(3);
  Actor_SetPartAttribute(Actor_GetById(8), 0);
  wait(40);
  Func_02001084(Actor_GetById(8), 8, 0);
  Func_02005d34();
  Func_02005d2c();
  Func_02005e04(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_SetBit(0x899);
  Func_02005e1c();
}

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02005e84(s32 owner, s32 a, s32 b);

void Func_02003ac4(void) {
  Func_02005e14();
  Func_02005f2c(0);
  OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  Func_02005e84(gMainRoutineWork.currentOwner, 6, 0);
  Func_02005efc(3);
  Func_02005e1c();
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit, s32 acceleration);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);

/// While the leader is no further than z 653, moves the leader to (376, 668)
/// with bit 0 of its byte 90 cleared for the move.
void Func_02003b08(void) {
  u8 *flags;
  u8 bit;

  if (((s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->z)[1] > 653)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90) &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 376, 668);
  flags = &BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90);
  /* FAKEMATCH(temporary): the bit held in a u8 local, built in the register it
   * ORs into, as the ROM does; as |= 1, the byte loads there instead, 4 of
   * 47 instructions differ in tla-en (also as an s32 local). */
  bit = 1;
  *flags = bit | *flags;
  Func_02005e1c();
}

void Func_02005d7c(struct Actor *object);

void Func_02003b7c(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(288, x << 16, 0, y << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSingleChildFlags(marker, 0);
  sprite = marker->sprite;
  sprite->blendMode = 2;
  Func_02005e14();
  Func_02005f2c(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02005f94(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  i++;
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  wait(1);
  i++;
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  Call6((void (*)())Func_02005d9c, 65, 1, cellX + 64, cellY, 1, 2);
  Func_0200062c(0, cellX, cellY, 0xff00);
  Func_02005d7c(marker);
  Func_02005e1c();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

void Func_02002790(void);

/// Reads what the leader stands on (Func_02000690, kind 2): on cells 1 and 7
/// it only redraws the marker when flag 0x200 is set; elsewhere it clears
/// (Func_02002790), redraws the marker in the row the event work's counter
/// gives, sets flag 0x200 and the counter's flag (312 on).
void Func_02003cac(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cell = Func_02000690(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_02002790();
    Func_02003b7c(0x108, row * 8 + 10);
    if (!Value1((s32 (*)())GameFlag_Test, 0x200))
      GameFlag_SetBit(0x200);
    GameFlag_SetBit(*(s16 *)work + 312);
  } else if (GameFlag_Test(0x200)) {
    Func_02003b7c(0x108, row * 8 + 10);
  }
}

/// As Func_02002980 with a 1 by 2 block: saves the cells 64 columns across
/// from the controlled object at (73, 72) and stamps the cells kept at
/// (70, 74) in their place.
void Func_02003d40(void) {
  struct Actor *object = Actor_GetById(Func_02005f84());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006c0c) {
    gData_02006c0c = key;
    gData_020073dc = 1;
    gData_020073e0 = 2;
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
    CopyCells(gData_020073e4, gData_020073e8, 1, 2, 73, 72);
    CopyCells(70, 74, gData_020073dc, gData_020073e0, gData_020073e4,
              gData_020073e8);
  }
}

extern const u8 gData_02006084[];

void Func_02003dd4(void) {
  Func_02005dac(26, 17, 1, 1, 8, 19);
  Func_02005dac(26, 17, 1, 1, 24, 19);
  Call6((void (*)())Func_02005d9c, 90, 17, 72, 19, 1, 2);
  Func_02005d9c(90, 17, 88, 19, 1, 2);
}

void Func_02003e28(void) {
  Func_02005dac(25, 17, 1, 1, 8, 19);
  Func_02005dac(25, 17, 1, 1, 24, 19);
  Call6((void (*)())Func_02005d9c, 89, 17, 72, 19, 1, 2);
  Func_02005d9c(89, 17, 88, 19, 1, 2);
}

void Func_02003e7c(struct Actor *source) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Import_ActorCreate, 0x2a0, source->x, source->y, source->z);

  WORD_AT(object, 28) = 0x6666;
  WORD_AT(object, 24) = 0x6666;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02005d5c(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02005d6c(object, gData_02006084);
}

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 countdown;
  u8 unk63;
  s16 angle;
  u16 speed;
  struct Actor *anchor;
};

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);

s32 Func_02003ed4(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Call3((void (*)())Vector_AddPolarOffsetFar, orbiter->radius, orbiter->angle,
        (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == 0x20000)
    return 1;
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Vector_AddPolarOffsetFar(orbiter->radius, orbiter->angle, &orbiter->x);
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}

struct Orbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
// Called by its linked name, as Gaia Rock's Func_0200142c does.
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

// Moves around in a circle.
s32 Func_02003f94(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Math_Cosine(object->angle), object->radius);
  z = IwramMulQ16(Math_Sine(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sine(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

s32 Func_02004038(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  from = object->y;
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}

s32 Func_02004084(struct Actor *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}

#include "system/field_effect.h"

void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020040b0(struct Actor *object) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    struct EffectOptions options;
    s32 position[3];

    options.palette = 7;
    options.type = 0x2a1;
    options.priority = 1;
    position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
    position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
    position[2] = object->z;
    Func_020002f4(
        position[0], position[1], position[2] + 0x40000, 0, 0x8000, phase,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
}

struct SparkBytes {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

union SparkView {
  struct Actor actor;
  struct SparkBytes bytes;
};

extern const u8 gData_020060e0[], gData_02006094[], gData_020060b0[],
    gData_02006104[], gData_020060c0[], gData_02006128[], gData_02006158[];
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Func_02005f3c(void);

void Func_02004138(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  s32 never;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  Func_02005f94(78);
  FieldEvent_Wait(30);
  Func_02005ed4(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16, ((s16 *)&a->y)[1] << 16,
                             ((s16 *)&a->z)[1] << 16, 1);
  Func_02005f94(141);
  Call3(Func_02005dcc, 0x20000, 0x20000, 0x10000);
  Func_02005f0c(0x10000, 0);
  Func_02005f04(0x10003, 1);
  Func_02005f14(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_020060e0);
  Func_02005f94(194);
  FieldEvent_Wait(45);
  Func_02005e94(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
first:
  {
    struct Actor *spark = Import_ActorCreate(0x2a0, a->x, a->y, a->z);
    Func_02005f94(246);
    spark->x += ((Random16Far() & 31) - 16) << 16;
    spark->y += ((Random16Far() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_02006094);
    REG_BLDALPHA = 0x1004;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
    REG_BLDALPHA = 0x1010;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
  }
  i++;
  if (i != 16)
    goto first;
  REG_BLDALPHA = 0x1008;
  wait(30);
  /* FAKEMATCH(dead): the never-true jump into the loop; as no test at all, 200
   * of 402 instructions differ in tla-en. */
  never = idB << 1;
  i = 0;
  if (never & 1) {
    i = 1;
    goto tail;
  }
second:
  {
    struct Actor *spark = (struct Actor *)Value4((s32 (*)())Import_ActorCreate,
                                                 0x2a0, b->x, b->y, b->z);
    Func_02005f94(195);
    angle = &spark->timer;
    *angle = (Random16Far() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->watchedObject = b;
    ((union SparkView *)spark)->bytes.unk62 = (16 - i) * 8 + 15;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Call3(Vector_AddPolarOffsetFar, spark->unk4c, *angle, (s32)&spark->x);
    Func_02005d6c(spark, gData_020060b0);
    if (i == 3) {
      Call2(Func_02005e94, idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): the empty asm before the call; as no asm, 2 of 402
       * instructions differ in tla-en. */
      asm("");
      Actor_SetMovementTypeById(idB, gData_02006104);
    }
  tail:
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Func_02005f94(220);
  wait(16);
  i = 2;
third:
  {
    struct Actor *spark =
        (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 0x2a0, b->x,
                               b->y + i * 65536 - 0x80000, b->z);
    spark->timer = Random16Far();
    spark->unk66 = 0x800;
    ((union SparkView *)spark)->bytes.unk62 = i;
    ((union SparkView *)spark)->bytes.unk63 = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->watchedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetPartAttribute(spark, 2);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_020060c0);
  }
  i += 2;
  if (i != 32)
    goto third;
  Func_02005f94(220);
  wait(50);
  Func_02005f04(0x7fff, 0);
  Func_02005f14(8);
  wait(16);
  Func_02005dcc(0, 0, 0);
  Func_02005e94(idA, 0);
  Func_02005e94(idB, 0);
  Func_02005f94(574);
  Func_02005f04(0x10000, 0);
  Func_02005f14(80);
  Actor_SetMovementTypeById(idA, gData_02006128);
  Actor_SetMovementTypeById(idB, gData_02006158);
  b->update = Func_020040b0;
  wait(120);
  b->update = 0;
  Func_02005f94(390);
  Func_02005f3c();
  wait(1);
}

#include "text/msg_ids.h"

void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004534(void);
#include "event_work.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Func_02005ea4();
s32 Func_02005e74();
s32 Func_02005eac();
s32 Func_02005e6c();
s32 Func_02005dd4();
s32 Party_LearnAbility();
void Actor_TurnToFaceEachOther(s32 actorId, s32 other, s32 frames);
void Actor_WaitForMoveById(s32 actorId);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);

static inline void Call0(void (*f)()) {
  /* FAKEMATCH(wrapper): the call layer without arguments, through which the two
   * -1s before the call are negated in this order; as a direct call, 2 of 202
   * instructions differ in tla-en. */
  f();
}

/// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
/// the scene with actor 6 follows, as Func_020030cc of Magma Rock does with
/// the Blaze tablet.
void Func_02004534(void) {
  struct Actor *object;

  Func_02005e14();
  Func_02005f2c(0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Value1(GameFlag_Test, 0x8d4) != 0) {
    Func_02005e1c();
  } else {
    Func_02005ea4((s32)MsgCheckedTheTablet);
    EVENT_WORK->messageId++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 6, 0xb333, 0x5999);
    Func_02005e74(6, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Call3(Actor_ResetAndSetPositionInMode2, gMainRoutineWork.currentOwner, 256,
          136);
    Actor_ResetAndSetPositionInMode2(6, 272, 136);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_WaitForMoveById(6);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_ArmCallback(6, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Call2(Func_02005eac, 6, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 256, 152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_ArmCallback(6, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetModeById(6, 3);
    FieldEvent_Wait(30);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Call3(Actor_SetPositionAndReset, 6, 264, 137);
    Actor_ArmCallback(6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    Call3(Func_02005dcc, 0x50000, 0x70000, 0x10000);
    Call3(Func_02005dcc, -1, -1, 0xe666);
    /* FAKEMATCH(wrapper): the call through Call0's pointer, which negates the
     * two -1s before it in this order; as a direct call, 2 of 202 instructions
     * differ in tla-en. */
    Call0(Func_02005dd4);
    Call2(Func_02004138, 8, 6);
    Party_LearnAbility(6, 0x90);
    GameFlag_SetBit(0x8d4);
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 6, 0);
    FieldEvent_Wait(10);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
    Actor_SetModeById(6, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_ResetAndSetPosition(6, FIELD_AT(object, s16, 10),
                                FIELD_AT(object, s16, 18));
    Actor_WaitForMoveById(6);
    Func_02005e6c(6, 0, 0);
    Actor_SetActionCallback(Actor_GetById(8), 9);
    Func_02005e1c();
  }
}

void Func_02004740(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 107);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_020047b0(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004818(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004880(void) {}

void Func_020007b4(void);

void Func_02004884(void) {
  Func_02005e14();
  Func_02005f2c(0);
  if (Actor_GetById(8)->x >> 19 == 65)
    GameFlag_SetBit(771);
  else
    GameFlag_ClearBit(771);
  Func_02005e1c();
}

void Func_020048bc(void) { Func_020007b4(); }

#include "graphics/vram_tab.h"

extern u16 gData_0200621c[];
void Func_020006cc(struct Actor *object);

void Func_020048c4(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = ((u8 *)first)[0x63] | ((u8 *)second)[0x63];
  if (busy != 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  firstSprite = first->sprite;
  secondSprite = second->sprite;
  second->motionFlags = busy;
  second->y += -0x1c0000;
  wait(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->watchedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->watchedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedObject, entry->size);
  Dma_Set(first->watchedObject, second->watchedObject, 0x840000c0,
          (volatile u32 *)0x040000d4);
  first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
  second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = 3;
  first->scaleY = 0x10000;
  second->y = second->terrainHeight;
  GameFlag_SetBit(first->timer);
  GameFlag_ClearBit(second->timer);
  Func_02005d5c(first, ((struct FieldSprite *)first->sprite)->frame + 1);
  Func_02005d5c(second, ((struct FieldSprite *)second->sprite)->frame - 1);
  Func_020006cc(first);
  Func_020006cc(second);
  FieldEvent_Wait(10);
  Func_02005e1c();
}

/// When the event work's warp number (0x154) is set, plays cue 186 and moves
/// the leader to that warp's place (warps 50 to 55), inside an event.
void Func_02004ab8(void) {
  s16 *warp;
  s32 x, z;

  x = 0;
  z = 0;
  warp = (s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);
  if (*warp == 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  switch (*warp) {
  case 50:
    // XXX: the leader is looked up twice and neither result is used; likely
    // left from a position read, because the other warps give theirs as
    // constants.
    Actor_GetById(gMainRoutineWork.currentOwner);
    Actor_GetById(gMainRoutineWork.currentOwner);
    x = 148;
    z = 632;
    break;
  case 51:
    x = 264;
    z = 652;
    break;
  case 52:
    x = 504;
    z = 508;
    break;
  case 53:
    x = 552;
    z = 508;
    break;
  case 54:
    x = 616;
    z = 188;
    break;
  case 55:
    x = 648;
    z = 188;
    break;
  }
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, x, z);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_02005e1c();
}

extern s32 gData_02006c10[];
extern s32 gData_02006d6c[];
extern s32 gData_02006e2c[];
extern s32 gData_02006eec[];
extern s32 gData_02006fa0[];
extern s32 gData_0200706c[];
extern s32 gData_020070fc[];
extern s32 gData_02007228[];
extern s32 gData_02007318[];
extern s32 gData_020073a8[];

void Func_02004bb4(void) {
  const u8 *message;

  Func_02005e14();
  Func_02005f2c(0);
  message = MsgCheckedThePsynergy;
  FieldMessage_Show(message, 1);
  Func_02005f94(126);
  Func_02005f34(999, 0);
  FieldEvent_Wait(10);
  GameFlag_ClearBit(322);
  FieldMessage_Show(message + 1, 1);
  Func_02005e1c();
}

/// The placement table for the current room.
s32 *Func_02004c00(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov664Room1)
    return gData_02006c10;
  if (scene == (s32)&SceneId_Ov664Room2)
    return gData_02006d6c;
  if (scene == (s32)&SceneId_Ov664Room3)
    return gData_02006e2c;
  if (scene == (s32)&SceneId_Ov664Room4)
    return gData_02006eec;
  if (scene == (s32)&SceneId_Ov664Room5)
    return gData_02006fa0;
  if (scene == (s32)&SceneId_Ov664Room6)
    return gData_0200706c;
  if (scene == (s32)&SceneId_Ov664Room7)
    return gData_020070fc;
  if (scene == (s32)&SceneId_Ov664Room8)
    return gData_02007228;
  if (scene == (s32)&SceneId_Ov664Room9)
    return gData_02007318;
  if (scene == (s32)&SceneId_Ov664Room10)
    return gData_020073a8;
  return gData_02006d6c;
}

#include "version.h"

void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
s32 Func_02005dec(struct Actor *object);
void Func_02005d94(struct Actor *object);

void Func_02004cc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Func_02005e14();
  Func_02005f2c(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(leader, 0);
  leader->speedLimit = 0x18000;
  leader->acceleration = 0x18000;
  lift = Import_ActorCreate(280, leader->x, leader->y - 0x80000, leader->z);
  Func_02005d5c(lift, 2);
  Actor_SetSingleChildFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = 0x16666;
  lift->acceleration = 0x16666;
  Func_02005d8c(leader, leader->x, leader->y + 0x500000, leader->z);
  Func_02005d8c(lift, lift->x, lift->y + 0x480000, lift->z);
  Func_02005f94(137);
  Event_StartScreenTransition();
  while (!Func_02005dec(leader)) {
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Func_02005f94(288);
  FieldEvent_Wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  height = Map_GetTerrainHeight(0, lift->x, lift->z - 0x100000);
  leader->y = height;
  leader->terrainHeight = height;
  Func_02005d5c(leader, 6);
  wait(6);
  Func_02005f94(152);
  Func_02005d5c(leader, 7);
  leader->speedLimit = 0x20000;
  leader->acceleration = 0x8000;
  leader->velocityY = 0x40000;
  leader->motionFlags = 2;
  leader->priorityFlags = 1;
  Func_02005d8c(leader, leader->x, leader->y, leader->z - 0x100000);
  OverlayObject_WaitUntilIdle(leader);
  Func_02005d94(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02005d5c(lift, 5);
  wait(12);
  Func_02005d7c(lift);
  leader->motionFlags = 3;
  leader->terrainHeight = leader->y;
  Actor_SetSingleChildFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Func_02005d7c(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Func_02005e1c();
}

void Func_02000550(void);

void Func_02004eb0(void) {
  struct Actor *object;
  s32 i = 0;
  struct FieldSprite *sprite;

  do {
    object = Import_ActorCreate(672, 0, 0, 0);
    sprite = object->sprite;
    sprite->priority = 1;
    ((u8 *)object)[0x63] = i << 4;
    {
      s32 zero = 0;
      object->motionFlags = zero;
      object->priorityFlags = zero;
    }
    object->acceleration = object->speedLimit = (i << 12) + 0x8000;
    object->update = Func_02000550;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 2);
    if (i & 1) {
      object->timer = 212;
      object->unk66 = 0;
    } else {
      object->timer = 116;
      object->unk66 = 0;
    }
    Animation_SetStateFlags(object, 1);
    i++;
  } while (i != 8);
}

extern s32 gData_020073d8;

void Func_02004f3c(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 level;

  actor->priorityFlags |= 32;
  if (actor->y + 0x200000 >= 0x1380000)
    level = 2;
  else if (actor->y + 0x200000 >= 0xd80000)
    level = 1;
  else
    level = 0;
  if (gData_020073d8 == level)
    return;
  switch (level) {
  case 2: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 64, 6, 4, a4, a5);
  }
    Func_02005dac(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 68, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 72, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  }
  gData_020073d8 = level;
}
