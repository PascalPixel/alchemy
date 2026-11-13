/// @file
/// The Mars Lighthouse rooms of this overlay below the aerie: floors of ice
/// the leader slides across (MarsLighthouseRoomF_SlideOnIce), the platforms
/// that carry the party (Ride_UpdateCarried), cracked floors it drops through
/// (CrackedTile_Crack) and pushable statues (PushTarget_Push).

#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "field/event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct SceneWork {
  u8 unk000[0x166];
  u16 unk166;
  u8 unk168[0x44];
  s32 unk1ac;
  u8 unk1b0[0xb88];
  u8 unkd38;
};

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 Ride_SetEntry(s32 index, s32 value);
s32 Func_020027dc(s32 index);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);
void Actor_StartScript(struct Actor *object, const void *script);
void FieldCamera_WaitForArrival(void);
s32 Field_FindNearbyTalkTarget(s32 id);
void Field_TryJumpForward(void);
void ObjectTable_Snapshot(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
void Leader_PushObjectAhead(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_SetBlendMode(struct Actor *object, u32 value);
s32 Trig_Cos(s32 angle);

void Func_020001f4(s32 unused, s32 id);
void Actor_SetupScriptedPart(struct Actor *object, s32 part);
void Func_020002ec(void);
void Func_020003b4(void);
void Func_0200055c(s32 id);
void Func_02001448(void);

static inline struct Actor *Func_02005498_(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}
#include "version.h"
#include "system/iwram_call.h"
#include "system/input.h"
#include "macros.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/frame.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "call.h"
#include "field/field_coordinate.h"
#include "field/event_work.h"
#include "battle/battle_unit.h"
#include "module/field/event_commands.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "module/system/system.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "objcmd.h"

struct EffectOptions;
struct LightOam;

struct WarpWork {
  s16 unk00;
  s16 positionFlag;
  s16 unk04;
  s16 sourceX;
  s16 sourceZ;
  s16 count;
  s16 current;
  s16 busy;
  u8 unk10[0x10];
  struct WarpEntry entries[1];
};

extern s16 gData_02005f18[];

void Func_02000cb8(s32 id);

void Leader_SetSlideStopped(s32 value);

void Func_020025b0(void);

void Func_02002604(void);

void Func_020027cc(s32 index, s16 value);

void CrackedTile_DropIn(void);

s32 Func_02004938(s32 id);

void LightSprites_InitB(void);

void LightSprites_Add(s32 id, s32 value);

void Func_020053bc(s32 value);

void Func_02001468(void);

void Func_02001490(void);

void Whirlwind_Update(struct Actor *self);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void Ride_UpdateCarried(struct Actor *object);

void Ride_UpdateLanding(struct Actor *object);

void Func_02001fb4();
void Func_020021a0();

void Func_020021e8();

void Func_02002230();
void Beam_DrawTargets(void);
void Func_020000bc(void);

void Func_02000168(void);

void Func_02000170(void);

void Func_0200021c(void);

void Func_02000418(void);

void Func_020004f8(void);

void Func_02000554(void);

void Func_02000754(void);

void Func_02000760(void);

void Func_0200076c(void);

void Func_02000790(void);

void Func_020007a8(void);

void Func_020007c0(void);

void Func_020007d8(void);

void Func_020007e4(void);

void Func_020008bc(void);

void Func_02000944(void);

void Func_02000ad8(void);

void Func_02000b40(void);

s32 MarsLighthouseRoomF_SlideOnIce(void);

void Leader_SlideOnIce(void);

extern u32 gData_02006980[];

void Party_TakeFallDamage(void);

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

void Leader_CenterInCell(void);

s32 Import_FieldCheckSpheresOverlap(s32 *a, s32 radiusA, struct Position *b,
                                    s32 radiusB);

extern volatile s16 gMarsLighthouseRoomF_SlideStopped;

s32 Actor_HasArrived(struct Actor *object);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *object);
void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Actor_StopAtPushTarget(struct Actor *object);

extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct Actor *gData_020074d0[];
extern struct LightOam Data_0200f510[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Func_02000d80(void);

void Actor_Release(void *handle);

void Actor_DisableById(s32 id);
void FieldEffect_StartBlankOverlay(s32 a0);
void Tightrope_Initialize(void);
void Tightrope_Place(s32 a0, s32 a1, s32 a2);
void Actor_DepartInRing(s32 id);
void Actor_ArriveInRing(s32 id);
void TileMap_ConvertBuffer(s32 scene, s32 a1);
extern u16 gData_02007088[];
extern u16 gData_0200708c[];
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();
extern s32 Data_02007064;
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMarsLighthouseRoomF_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room4Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room1Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room3Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room2Placements[];

s32 Func_0200004c(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF4)
    return gMarsLighthouseRoomF_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF2)
    return gMarsLighthouseRoomF_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF1)
    return gMarsLighthouseRoomF_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF3)
    return gMarsLighthouseRoomF_Room3Placements;
  return gMarsLighthouseRoomF_Placements;
}

void Func_020000bc(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[3384] = 1;
}

void Func_020000d0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u32 facing = object->facing;

  if (facing == 0 || facing == 0x8000) {
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    if (facing != 0)
      x--;
    else
      x++;
    object = Actor_Get(10);
    if (object->x >> 20 != x || object->z >> 20 != z) {
      object = Actor_Get(11);
      if (object->x >> 20 != x || object->z >> 20 != z) {
        object = Actor_Get(12);
        if (object->x >> 20 != x || object->z >> 20 != z) {
          Field_TryJumpForward();
          return;
        }
      }
    }
  }
  if (Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner) != -1)
    Leader_PushObjectAhead();
}

void Func_02000168(void) { Field_TryJumpForward(); }

#if TLA_PATCH_1

void Func_02000170(void) {
  s32 position[3];
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = (object->y + 0x40000) & 0xfff80000;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  object = Actor_FindAtCell(position, 1);
  if (object != NULL) {
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    FieldVector_AddPolarOffset(0x100000, angle, position);
    if (Actor_FindAtCell(position, 1) != NULL)
      return;
  }
  Leader_PushObjectAhead();
}

#else

void Func_02000170(void) { Leader_PushObjectAhead(); }

#endif

extern s16 gData_02005b5c[];

void Func_020001f4(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 50) {
    Event_SetFlag(0xa38);
    object->motionFlags = 3;
  }
}

void Func_0200021c(void) {
  struct Actor *object = Actor_Get(16);

  Leader_PushObjectAhead();
  if (object->x >> 20 == 50) {
    Event_SetFlag(0xa38);
    object->motionFlags = 3;
  }
}

void Func_02000248(s32 unused, s32 id) {
  if (id == 10)
    Actor_Get(10)->motionFlags = 0;
}

void Func_0200025c(s32 unused, s32 id) {
  if (id == 16) {
    Func_020001f4(unused, 16);
  } else if (id == 10) {
    Actor_Get(10)->motionFlags = 3;
#if TLA_PATCH_1 /* From patch 1. */
    Func_020002ec();
#endif
  }
}

void Func_02000280(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 kind;

  kind = gData_02005b5c[id - 18];
  Actor_SetPositionFixed(kind, S16_AT(object, 100) << 16,
                         (S16_AT(object, 102) << 16) - PIXELS(8));
}

void Func_020002b8(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  const s16 *table;
  s32 offset;
  s32 x;
  s32 kind;

  table = gData_02005b5c;
  x = object->x;
  offset = (id - 18) * 2;
  kind = *(s16 *)((u8 *)table + offset);
  if (x >> 20 == object->timer >> 4)
    Actor_SetPosition(kind, 0, 0);
}

#if TLA_PATCH_1 /* From patch 1. */
void Func_020002ec(void) {
  struct Actor *object;
  s32 id;
  s32 x;
  s32 z;

  MapGrid_CopyCellAttributes(53, 72, 1, 1, 38, 72);
  MapGrid_CopyCellAttributes(53, 72, 1, 1, 43, 72);
  MapGrid_CopyCellAttributes(53, 8, 1, 1, 47, 8);
  for (id = 10; id <= 12; id++) {
    object = Actor_Get(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 38 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 38, 72);
  }
  for (id = 10; id <= 12; id++) {
    object = Actor_Get(id);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 43 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 43, 72);
  }
  object = Actor_Get(10);
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 47 && z == 8)
    MapGrid_CopyCellAttributes(54, 8, 1, 1, 47, 8);
}
#endif

void Func_020003b4(void) {
  if (Actor_Get(15)->z >> 20 == 14) {
    Ride_SetEntry(0, 65);
    Ride_SetEntry(2, 19);
  } else {
    Ride_SetEntry(0, 17);
    Ride_SetEntry(2, 67);
  }
  if (Actor_Get(16)->x >> 20 == 6) {
    Ride_SetEntry(1, 65);
    Ride_SetEntry(3, 3);
  } else {
    Ride_SetEntry(1, 49);
    Ride_SetEntry(3, 67);
  }
}

void Func_02000418(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    PushTarget_Push(box);
    Func_020003b4();
  }
}

#define DEFINE_FUNC_02000444(name, n1, n2, n3)                                 \
  void name(s32 state, s32 unused) {                                           \
    if (state == 1) {                                                          \
      MapGrid_CopyCells(29, 56, 3, 4, n1, 50);                                 \
      MapGrid_CopyCellsLayered(93, 56, 3, 4, n2, 50);                          \
      Event_SetFlag(n3);                                                       \
    }                                                                          \
  }

DEFINE_FUNC_02000444(Func_02000444, 10, 74, 0xa44)

DEFINE_FUNC_02000444(Func_02000480, 18, 82, 0xa45)

DEFINE_FUNC_02000444(Func_020004bc, 14, 78, 0xa46)

void Func_020004f8(void) {
  Event_PlaySound(SE_584);
  Ride_SetEntry(0, 3);
  Event_SetFlag(FLAG_SCENE_TEMP_20);
}

void Func_02000514(void) {
  struct Actor *object = Actor_Get(9);

  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(9, S16_AT(object, 100), S16_AT(object, 102));
  Event_SetFlag(0xa42);
  Event_PlaySound(SE_WHOOSH);
}

void Func_02000554(void) { Func_02001448(); }

extern u32 gData_02005b68[];
extern u32 gData_02005bac[];

void Func_0200055c(s32 id) {
  struct Actor *origin = Actor_Get(id);
  s32 i;

  for (i = 11; i >= 0; i--) {
    s32 a = Random16();
    s32 x = origin->x + (a << 4) - (Random16() << 4);
    s32 b = Random16();
    s32 c = Random16();
    struct Actor *object =
        FieldActor_Create(0x149, x, (b << 3) + origin->y, (c << 4) + origin->z);

    if (object != NULL) {
      s32 angle = (Random16() >> 2) + 0xe000;
      s32 speed = Random16() << 3;
      s32 scale = (Random16() >> 1) + 0x4000;

      {
        u32 pick = Random16();

        if (pick <= 0x7fff)
          Actor_StartScript(object, gData_02005b68);
        else
          Actor_StartScript(object, gData_02005bac);
      }
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = (angle << 2) + 0x20000;
      object->velocityZ = Iwram_MulQ16(speed, Trig_Cos(angle));
      {
        s32 vx = Iwram_MulQ16(speed, Math_Sin(angle));

        object->acceleration = 0;
        object->scaleX = scale;
        object->scaleY = scale;
        object->gravity = 0x547a;
        object->velocityX = vx;
        object->bounce = 0x8000;
      }
      Actor_SetSpriteColorMode(object, 1);
    }
  }
}

void Func_02000670(void) {
  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk166 = 0;
  Event_ClearFlag(FLAG_MENU_OPEN);
  Event_ClearFlag(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 712, 1);
  FieldCamera_WaitForArrival();
  System_Wait(20);
  FieldCamera_SetSpeed(5);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 872, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void Func_020006f4(s32 burst) {
  if (!Flag_IsSet(0xa46)) {
    MapGrid_CopyCells(29, 60, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 60, 3, 4, 78, 50);
    if (burst && !Flag_IsSet(0xa43))
      Func_0200055c(14);
    Flag_Set(0xa43);
  }
}

void Func_02000754(void) { Import_EventSetExitRequest(2); }

void Func_02000760(void) { Import_EventSetExitRequest(3); }

void Func_0200076c(void) { Import_EventSetExitRequest(4); }

void Func_02000778(void) { Import_EventSetExitRequest(5); }

void Func_02000784(void) { Import_EventSetExitRequest(6); }

void Func_02000790(void) { Import_EventSetExitRequest(7); }

void Func_0200079c(void) { Import_EventSetExitRequest(8); }

void Func_020007a8(void) { Import_EventSetExitRequest(9); }

void Func_020007b4(void) { Import_EventSetExitRequest(10); }

void Func_020007c0(void) { Import_EventSetExitRequest(11); }

void Func_020007cc(void) { Import_EventSetExitRequest(12); }

void Func_020007d8(void) { Import_EventSetExitRequest(13); }

void Func_020007e4(void) { Import_EventSetExitRequest(14); }

void Func_020007f0(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 13) {
    object->motionFlags = 3;
    if (object->terrainHeight !=
        Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z)) {
      s32 frames;

      System_Wait(2);
      for (frames = 0; frames < 30 && object->y > object->terrainHeight;
           frames++)
        System_Wait(1);
      Event_PlaySound(SE_IMPACT);
    }
    MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
    MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
    MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
    Event_PlaySound(SE_MAP_TRANSITION);
    Actor_SetPosition(id, 0, 0);
    Flag_Set(FLAG_AREA_TEMP_58);
  }
}

void Func_020008a0(s32 unused, s32 id) {
  if (Actor_Get(id)->x >> 20 == 7)
    Flag_Set(0xa37);
}

void Func_020008bc(void) {
  MapGrid_CopyCells(17, 24, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 24, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 20, 1, 2, 54, 20);
}

void Func_02000900(void) {
  MapGrid_CopyCells(17, 25, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 25, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 22, 1, 2, 54, 20);
}

void Func_02000944(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 frame;

  Leader_StopMoving();
  Actor_SetSpriteFlags(object, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  frame = 0;
step:
  if (frame == 5)
    Event_PlaySound(SE_DUST_PUFF);
  object->motionFlags = 0;
  object->scaleX += (-0x400);
  object->scaleY += -0x300;
  object->y += -0x9999;
  System_Wait(1);
  if (++frame <= 39)
    goto step;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(16);
}

void Func_020009ec(void) {
  Ride_SetEntry(2, 65);
  Ride_SetEntry(1, 65);
  Ride_SetEntry(0, 65);
  Flag_Clear(FLAG_AREA_TEMP_50);
  if (Actor_Get(10)->z >> 20 != 15 && Actor_Get(9)->z >> 20 != 15) {
    Ride_SetEntry(1, 33);
    return;
  }
  if (Actor_Get(9)->z >> 20 != 13) {
    Ride_SetEntry(0, 17);
    return;
  }
  if (Actor_Get(8)->x >> 20 != 37) {
    Ride_SetEntry(2, 49);
    return;
  }
  Flag_Set(FLAG_AREA_TEMP_50);
  if (!Flag_IsSet(FLAG_AREA_TEMP_51)) {
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(616, CAMERA_KEEP, 24, 1);
    FieldEvent_Wait(60);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(9);
  }
}

void Func_02000aac(void) {
  Ride_SetEntry(3, 3);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 560, 1);
}

void Func_02000ad8(void) {
  struct SceneWork *work = Ram_HeapSlots->eventWork;

  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  work->unk166 = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  FieldEvent_Wait(60);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 744, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Event_End();
}

void Func_02000b40(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    PushTarget_Push(box);
    Func_020009ec();
    if (Func_020027dc(0) != 64 && Flag_IsSet(FLAG_SCENE_TEMP_21)) {
      struct Actor *object = Actor_Get(12);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(12, 6);
      Flag_Clear(FLAG_SCENE_TEMP_21);
      Func_02000cb8(12);
    }
    if (Func_020027dc(2) != 64 && Flag_IsSet(FLAG_SCENE_TEMP_20)) {
      struct Actor *object = Actor_Get(11);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(11, 6);
      Flag_Clear(FLAG_SCENE_TEMP_20);
      Func_02000cb8(11);
    }
  } else {
    s32 angle;
    const s16 *table;
    u32 *keys;

    Actor_Get(gMainRoutineWork.currentOwner);
    keys = &gInput.held;
    table = gData_02005f18;
    angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 15) << 1));
    if (angle != -1) {
      struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
      struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
      s32 position[3];
      s32 x;
      s32 z;
      u8 *cells;

      SET_VECTOR(position, (leader->x & 0xfff00000) + 0x80000, leader->y,
                 (leader->z & 0xfff00000) + 0x80000);
      FieldVector_AddPolarOffset(0x100000, angle, position);
      x = position[0] >> 20;
      z = position[2] >> 20;
      cells = Ram_MapCellBuffer;
      if (map != NULL)
        cells = map->layers[2].cells;
      cells += (x + (z << 7)) << 2;
      if (cells[2] != 255)
        Field_TryJumpForward();
    }
  }
}

extern struct Trigger gMarsLighthouseRoomF_Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room4Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room1Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room3Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room2Triggers[];
void Leader_SlideOnIceFromEdge(void);

void Func_02000c80(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 row = (object->z >> 20) << 7;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + row;

  cell += offset << 2;
  cell[2] = 255;
  Actor_Get(id)->priorityFlags = 0;
}

void Func_02000cb8(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  s32 row = (object->z >> 20) << 7;
  s32 x = object->x;
  u8 *cell = map->layers[2].cells;
  s32 offset = (x >> 20) + row;

  cell += offset << 2;
  cell[2] = 0;
  {
    u8 *flags = &Actor_Get(id)->priorityFlags;
    s32 value = 2;

    value |= *flags;
    *flags = value;
  }
}

#define DEFINE_FUNC_02000CF4(name, n1, n2)                                     \
  void name(s32 unused, s32 id) {                                              \
    Func_02000c80(id);                                                         \
    Flag_Set(n1);                                                              \
    if (Func_020027dc(n2) != 64) {                                             \
      struct Actor *object = Actor_Get(id);                                    \
                                                                               \
      WORD_AT(object, 108) = 0;                                                \
      Actor_SetSpriteColorMode(object, 0);                                     \
      Actor_SetAnimation(id, 6);                                               \
      Flag_Clear(n1);                                                          \
      Func_02000cb8(id);                                                       \
    }                                                                          \
  }

DEFINE_FUNC_02000CF4(Func_02000cf4, 0x220, 2)

DEFINE_FUNC_02000CF4(Func_02000d38, 0x221, 0)

void Func_02000d80(void) {
  Flag_Set(0xa40);
  MapGrid_CopyCellAttributes(24, 38, 3, 9, 10, 38);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 74, 36);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 10, 100);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 74, 38);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 10, 102);
}

void Func_02000df8(void) {
  Leader_SlideOnIceFromEdge();
  MarsLighthouseRoomF_ContinueSlide();
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF4)
    return gMarsLighthouseRoomF_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF2)
    return gMarsLighthouseRoomF_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF1)
    return gMarsLighthouseRoomF_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseRoomF3)
    return gMarsLighthouseRoomF_Room3Triggers;
  return gMarsLighthouseRoomF_Triggers;
}

void Func_02000e68(void) {
  if (Func_02002cfc())
    Leader_SetSlideStopped(1);
}

void JupiterLighthouse_LoadPalettesZero(s32 mode);
extern u16 gData_02005bf0[];
extern u16 gData_02005bf6[];
extern u16 gData_02005bfc[];
extern u16 gData_02005c0e[];
extern u16 gData_02005c14[];
extern u16 gData_02005c26[];
extern u16 gData_02005c2c[];
extern u16 gData_02005c32[];
extern u16 gData_02005c38[];
extern u16 gData_02005c46[];
extern u16 gData_02005c4c[];
extern u16 gData_02005c5a[];
extern u16 gData_02005c60[];
extern u16 gData_02005c6e[];

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  s32 entrance = work->entrance;
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 value;
  u8 *flags;
  s32 bits;
  s32 other;

  /* FAKEMATCH(temporary): the first flag update through other. */

  ((struct SceneWork *)heap->eventWork)->unk1ac = 516;
  value = 516;
  Flag_Set(FLAG_MARS_LIGHTHOUSE_LIT);
  Func_020025b0();
  if (scene == (s32)&SceneId_MarsLighthouseRoomF1) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    if (entrance - 1 <= 1U) {
      Beam_Start(gData_02005bf0, gData_02005bf6, Func_02000514, 88);
      if (Flag_IsSet(0xa42))
        Actor_SetPosition(10, 0, 0);
      else
        Actor_SetPosition(9, 0, 0);
      if (Flag_IsSet(0xa43))
        Func_020006f4(0);
      if (Flag_IsSet(0xa44))
        Func_02000444(1, 0);
      if (Flag_IsSet(0xa45))
        Func_02000480(1, 0);
      if (Flag_IsSet(0xa46))
        Func_020004bc(1, 0);
    } else {
      Beam_Start(gData_02005bfc, gData_02005c0e, 0, 88);
      Actor_DisableById(15);
      Actor_DisableById(16);
      Func_02004938(15);
      Func_02004938(16);
      Func_020003b4();
    }
  } else if (scene == (s32)&SceneId_MarsLighthouseRoomF2) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Beam_Start(gData_02005c14, gData_02005c26, Func_02001468, 88);
    Func_020027cc(3, 1);
    if (entrance == 2)
      Func_02002604();
    if (entrance == 9) {
      Flag_Set(FLAG_AREA_TEMP_51);
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(work->currentOwner, 0, 0);
      Actor_SetPosition(17, 0, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Actor_SetPosition(18, 0, 0);
      Actor_SetPosition(17, 264, 520);
      Event_PlaySound(SE_WHOOSH);
      Event_Wait(60);
      Flag_Set(FLAG_KEEP_PARTY_POSITION);
      return ((s32 (*)())Import_EventSetExitRequest)(3);
    }
    if (Flag_IsSet(FLAG_AREA_TEMP_50)) {
      Actor_SetPosition(8, 600, 144);
      Actor_SetPosition(9, 544, 208);
      Actor_SetPosition(10, 544, 240);
      Flag_Set(FLAG_SCENE_TEMP_20);
      Flag_Set(FLAG_SCENE_TEMP_21);
    }
    Actor_DisableById(8);
    Actor_DisableById(9);
    Actor_DisableById(10);
    Func_02004938(8);
    Func_02004938(9);
    Func_02004938(10);
    flags = &Actor_GetById(11)->priorityFlags;
    bits = 2;
    other = *flags;
    other |= bits;
    *flags = other;
    flags = &Actor_GetById(12)->priorityFlags;
    bits |= *flags;
    *flags = bits;
    if (Flag_IsSet(FLAG_SCENE_TEMP_20))
      Func_02000c80(11);
    if (Flag_IsSet(FLAG_SCENE_TEMP_21))
      Func_02000c80(12);
    Func_020009ec();
    if (Flag_IsSet(FLAG_AREA_TEMP_50))
      Actor_SetPosition(18, 0, 0);
    else
      Actor_SetPosition(17, 0, 0);
    if (Flag_IsSet(0xa40))
      Func_02000d80();
    Task_Add(Func_02000e68, 0x480);
  } else if (scene == (s32)&SceneId_MarsLighthouseRoomF3) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    JupiterLighthouse_LoadPalettesZero(1);
    LightSprites_InitB();
    Func_020053bc(2);
    LightSprites_Add(11, 6);
    if (Flag_IsSet(FLAG_AREA_TEMP_58)) {
      MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
      MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
      MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
      Actor_SetPosition(11, 0, 0);
    }
    if (Flag_IsSet(0xa37))
      Actor_SetPosition(12, 120, 296);
    if (entrance - 14 <= 1U) {
      Beam_Start(gData_02005c2c, gData_02005c32, Func_02001490, 88);
      Func_020027cc(0, 1);
    } else {
      Beam_Start(gData_02005c38, gData_02005c46, 0, 88);
    }
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 7 ||
          entrance == 8 || entrance == 10 || entrance == 11 || entrance == 13)
        Actor_DepartInRing(gMainRoutineWork.currentOwner);
      if (entrance == 3 || entrance == 5 || entrance == 9 || entrance == 12 ||
          entrance == 14)
        Actor_ArriveInRing(gMainRoutineWork.currentOwner);
      if (entrance == 16 && !Flag_IsSet(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
  } else if (scene == (s32)&SceneId_MarsLighthouseRoomF4) {
    FieldEffect_StartBlankOverlay(0);
    ((struct SceneWork *)heap->eventWork)->unk1ac = value;
    Tightrope_Initialize();
    Tightrope_Place(0, 8, 9);
    BYTE_AT(Actor_GetById(8), 98) = 1;
    BYTE_AT(Actor_GetById(9), 98) = 1;
    if (entrance - 5 <= 1U || entrance == 8) {
      Beam_Start(gData_02005c4c, gData_02005c5a, 0, 88);
#if TLA_PATCH_1
      Func_020002ec();
#endif
    } else if (entrance == 11 || entrance == 13 || entrance == 14 ||
               entrance == 1 || entrance == 15) {
      Beam_Start(gData_02005c60, gData_02005c6e, 0, 88);
    }
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == 15)
        Actor_SetPosition(23, 664, 712);
      if (entrance == 11)
        Actor_SetPosition(17, 536, 552);
    }
    Actor_GetById(16)->motionFlags = 0;
    if (Flag_IsSet(0xa38)) {
      Actor_SetPosition(16, 808, 312);
      Actor_GetById(16)->y = 0;
      Actor_GetById(16)->terrainHeight = 0;
    }
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
    Actor_GetById(10)->priorityFlags |= 192;
    Actor_GetById(11)->priorityFlags |= 128;
    Actor_GetById(12)->priorityFlags |= 128;
  }
  return 0;
}

extern s32 gData_02005c84[];
extern s32 gData_02005ce0[];
extern s32 gData_02005d3c[];

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  s32 flag = 0xa30;

  party = &gMainRoutineWork;
  if (Event_GetFlag(flag))
    TileMap_ConvertBuffer(
        party->map + ((s32)&SceneId_MarsLighthouseRoomM3 - (s32)&SceneId_Prox2),
        0x5000000);
  return 0;
}

void Func_02001448(void) { Actor_Attach(11, 12, 13, 0x4000, gData_02005c84); }

void Func_02001468(void) {
  Actor_Attach(19, 21, 22, 0x4000, gData_02005ce0);
  Ride_SetEntry(3, 131);
}

void Func_02001490(void) { Actor_Attach(14, 15, 16, 0x4000, gData_02005d3c); }

#include "shared/scene_set_arrival_flags.c"

extern s32 gData_02007040;

void Func_020014dc(s32 value) { gData_02007040 = value; }

extern s32 gData_0200703c;
extern u16 gData_02007068[];

void PaletteCycle_Step(void) {
  u16 colors[16];
  s32 phase = (gData_0200703c / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02007040 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    /* FAKEMATCH(temporary): the slot and the table address held in locals.
     */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02007068;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, DMA_COPY_WORDS(2), REG_DMA3);
  gData_0200703c++;
}

void JupiterLighthouse_LoadPalettesZero(s32 mode) {
  gData_02007040 = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gData_02007068, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWordsEntry(gData_02007088, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0. */
    s32 zero = 0;

    gData_0200703c = zero;
    Task_Add(PaletteCycle_Step, 1152);
  } else {
    Dma_Set(gData_0200708c, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

#include "field/event_inline.h"

#include "shared/actor_raise_from_cell.h"

/* VIEW(s167c): tag struct EventWork as struct EventWork__s167c; event_work.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1140: structure has no member
 * named `unk_d46'. */
struct EventWork__s167c {
  u8 unk_00[0x154];
  u16 unk_154;
  u8 unk_156[0xd44 - 0x156];
  u16 unk_d44;
  u16 unk_d46;
};

void Func_0200167c(void) {
  struct EventWork__s167c *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *motion;
  u8 *flag;
  s32 x;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unk_d46 = 0;
  motion = &actor->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(actor, 0);
  flag = (u8 *)linked + 0x63;
  while (*flag != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unk_d44 = 1;
  work->unk_d46 = 1;
  x = (linked->x >> 19) & ~1;
  facing = linked->facing;
  facing >>= 13;
  actor->x = (x + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  *(u8 *)&actor->priorityFlags = 1;
  *motion = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  *(u8 *)&shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFixed(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    System_Wait(1);
    i++;
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    System_Wait(1);
    i++;
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unk_d46 = 1;
  work->unk_154 = 0;
  Event_End();
}

#include "shared/position_flat_distance.h"

extern s32 gData_02007044[];

void Whirlwind_Update(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Position_FlatDistance(&leader->x, &self->x) <= 12 &&
            distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Trig_Cos(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gData_02007044);
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    Data_02007064 = zero;
  }
}

static __inline__ s32 MulQ16(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): MulQ16 forwarded inline. */
  return Iwram_MulQ16(a, b);
}

struct GustObject *Func_02001a54(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(SE_222);
  position.x = source->x + MulQ16(0x80000, Trig_Cos(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = (struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                  position.z);
  Actor_SelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = 0x18000;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - 0x60000, 0, 0, 0,
                    0x30000, &options);
  return object;
}

#include "shared/actor_emit_rising_sparks_b.h"

void Func_02001bcc(void) {
  struct PositionedActor *actor9 = Actor_GetById(9);
  struct PositionedActor *actor10 = Actor_GetById(10);
  struct PositionedActor *actor23 = Actor_GetById(23);
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 512, 248);
  Actor_SetPosition(23, 512, 248);
  Actor_SetPosition(9, 472, 256);
  Actor_SetPosition(10, 552, 256);
  Actor_GetById(24)->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(9), 4);
    Actor_SetSpriteColorMode(Actor_GetById(11), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(10), 4);
    Actor_SetSpriteColorMode(Actor_GetById(12), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#define MapLayer MapLayer__s1e20
#define SceneWork SceneWork__s1e20
#define Camera_SetBounds Camera_SetBounds__s1e20
#include "shared/mars_lighthouse_entrance_room.h"
#undef Camera_SetBounds
/* VIEW(s1e20): static Camera_SetBounds as Camera_SetBounds__s1e20; room.h has
 * static inline void FieldCamera_MoveToFixed(s32 x, s32 y, s32 z, s32
 * use_setter) { FieldCamera_MoveWithinMapBounds(x, y, z, use_setter); }; not
 * tried: Camera_SetBounds__s1e20 was not found. */
#undef SceneWork
/* VIEW(s1e20): tag struct SceneWork as struct SceneWork__s1e20; room.h has
 * another definition; not tried: struct SceneWork__s1e20 was not found. */
#undef MapLayer
/* VIEW(s1e20): tag struct MapLayer as struct MapLayer__s1e20; room.h has
 * another definition; not tried: struct MapLayer__s1e20 was not found. */

struct RideSlot {
  u16 id;
  u16 blocked;
};

struct RideWork {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

#include "shared/ride_update_carried.h"

extern u32 gData_02005d8c[];

void Actor_ResetScriptedFor(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_02005d8c);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001fb4(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Actor_ResetScriptedFor(object, 2);
    object->update = Ride_UpdateCarried;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1ffc): tag struct RideWork as struct RideWork__s1ffc;
 * mars_lighthouse_room_f.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1356: request for member `unk02'
 * in something not a structure or union. */
struct RideWork__s1ffc {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

void Ride_UpdateLanding(struct Actor *object) {
  struct RideWork__s1ffc *work =
      (struct RideWork__s1ffc *)(gInventorySnapshot + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y == other->terrainHeight && Actor_IsTouching(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.unk02 = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->unk00 = 1;
      else
        direction->unk00 = 0xffff;
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Actor_IsBeside(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

void Ride_UpdateDrift(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_02005d8c; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Actor_SetupScriptedPart(struct Actor *object, s32 part) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_02005d8c);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_020021a0(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateCarried;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_020021e8(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_02002230(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02005498_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

void Actor_SetupScriptedFor(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gData_02005d8c);
  FieldActor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Ride_SpawnSpark(s32 *position, s32 speed) {
  struct Actor *object =
      Func_02005498_(285, position[0], position[1], position[2] + 0xfffe0000);
  s32 spin;

  if (object != NULL) {
    object->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    object->timer = spin;
    Actor_SetupScriptedFor(object, 2);
    object->update = (void (*)(void))Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

/* VIEW(s242c): tag struct Actor as struct Actor__s242c; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f.c:1445:
 * structure has no member named `flag5b'. */
typedef struct Actor__s242c {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

/* VIEW(s242c): routine Ride_SpawnSpark as void Ride_SpawnSpark(); the parts
 * before it has another declaration; not tried: Ride_SpawnSpark's bound
 * declaration was not found. */
extern void Func_020023ac__s242c() __asm__("Ride_SpawnSpark");

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

void Beam_DrawTargets(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  Camera *camera;
  Position *position;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)(gInventorySnapshot + 0x188);
  camera = IWRAM->camera;
  entry = state->entries;
  position = &camera->position;
  cameraX = position->x;
  cameraX &= 0xffff0000;
  cameraZ = position->z & 0xffff0000;
  saved[1] = (*(s32 **)camera)[1];
  saved[0] = IWRAM->unk_6c.word;
  if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)
    state->callback();
  v = (u16)state->unk_6;
  state->unk_8 = v;
  state->unk_6 = 0;
  for (i = 3; i >= 0; i--, entry++) {
    Actor *actor;
    s32 flag, x, y, unk6, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (*(s16 *)(saved[0] + 358) != 0)
      flag |= 1;
    mark = &actor->flag5b;
    *mark = flag;
    if (state->unk_e != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dx = actor->x;
    dz = actor->z;
    dx -= cameraX;
    /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank.
     */
    asm("" : : "r"(dx));
    y = actor->y;
    dz -= cameraZ;
    y = dz - y;
    unk6 = entry->unk_6;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (unk6 == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 240;
      switch (y) {
      case 128:
        Func_020023ac__s242c(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 48:
        Func_020021a0(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 32:
        Func_02002230(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 16:
        Func_020021e8(&entry->actor->x, entry->unk_c, unk6);
        break;
      case 0:
        Func_02001fb4(&entry->actor->x, entry->unk_c, unk6);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
#undef IWRAM

void Func_020025b0(void) {
  struct MarsLighthouseRoomFRideWork *work =
      (struct MarsLighthouseRoomFRideWork *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseRoomFAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

/* VIEW(s25b0): tag struct RideWork as struct RideWork__s25b0;
 * mars_lighthouse_room_f.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1499: structure has no member
 * named `handle'. */
struct RideWork__s25b0 {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

void Actor_SetupScriptedFor(struct Actor *object, s32 unused);

void Func_02002604(void) {
  struct RideWork__s25b0 *work =
      (struct RideWork__s25b0 *)(gInventorySnapshot + 0x188);

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Func_02002618(s16 value) {
  ((struct RideWork__s25b0 *)(gInventorySnapshot + 0x188))->value0e = value;
}

#include "shared/beam_start.h"

#undef MarsLighthouseRoomF_ContinueSlide
/* VIEW(ride): tag struct RideSlot as struct RideSlot__ride;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1522: redefinition of `struct
 * RideSlot'. */
#define RideSlot RideSlot__ride
struct RideSlot {
  u16 value;
  u8 unk02[2];
};

/* VIEW(ride): tag struct RideWork as struct RideWork__ride;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1529: redefinition of `struct
 * RideWork'. */
#define RideWork RideWork__ride
struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void Func_020041ec(void *handle);

s32 Func_02002758(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_02002768(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_02002778(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->counter;
}

#include "shared/ride_advance_counter.h"

/* VIEW(ride): routine Ride_SetEntry as void Ride_SetEntry(s32 index, s32
 * value); scene/mars_lighthouse_room_f.h has s32 Ride_SetEntry(s32 index, s32
 * value); not tried: Ride_SetEntry's bound declaration was not found. */
void Func_020027ac__ride(s32 index, s32 value) __asm__("Ride_SetEntry");
#define Ride_SetEntry Func_020027ac__ride
#include "shared/ride_set_entry.h"

void Func_020027cc(s32 index, s16 value) {
  ((struct RideWork *)(gInventorySnapshot + 0x188))->entries[index].c = value;
}

s32 Func_020027dc(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

#include "shared/ride_set_slot.h"

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(ride): routine Unit_Get as this part declared it; the joined file has
 * another; tried as the definition's void *Unit_Get(s32 owner), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1584: request for member `hp' in
 * something not a structure or union. */
struct BattleUnit *Unit_Get__ride(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__ride

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect/advance_spin.c"

#undef RideSlot
#undef RideWork
#undef Ride_SetEntry

/* VIEW(flash): tag struct RideSlot as struct RideSlot__flash;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1595: redefinition of `struct
 * RideSlot'. */
#define RideSlot RideSlot__flash
struct RideSlot {
  u16 value;
  u8 unk02[2];
};

/* VIEW(flash): tag struct RideWork as struct RideWork__flash;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:1602: redefinition of `struct
 * RideWork'. */
#define RideWork RideWork__flash
struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[8];
  struct Actor *unk6c;
  void *handle;
};

#include "shared/actor_blink.h"

/* VIEW(flash): routine MapGrid_CheckCollision as s32
 * MapGrid_CheckCollision(struct Actor *object); the definition has s32
 * MapGrid_CheckCollision(struct Actor *object, s32 *position); this part passes
 * only the actor, and the bytes set no second argument; not tried:
 * MapGrid_CheckCollision's bound declaration was not found. */
extern s32 MapGrid_CheckCollisionOfActor(struct Actor *actor) __asm__(
    "MapGrid_CheckCollision");

#if TLA_PATCH_1
void Func_0200297c(void) {
#else
s32 Func_0200297c(void) {
#endif
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
#if TLA_PATCH_1
  struct Vector3 position;
#else
  /* FAKEMATCH(temporary): the position held in a union beside a halfword. */
  union {
    struct Vector3 v;
    s16 h;
  } positionU;
#define position positionU.v
#endif

  if (main->renderMode == 0) {
    struct Actor *actor;

    // UB: Party_TakeFallDamage takes no arguments and returns nothing; the
    // bytes call it with two through an int-returning type.
#ifdef UBFIX
    Party_TakeFallDamage();
#else
    ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
    actor = Actor_Get(main->currentOwner);
    work = (struct Work020023c4 *)(gInventorySnapshot + 0x188);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = Actor_Blink;
    actor->facing = TRIG_TURN / 4;
    Actor_SelectAnimation(actor, 49);
    for (;;) {
      if (MapGrid_GetCellAttributeLowNibbleOrBump(actor->terrainId, actor->x,
                                                  actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union. */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = Q16_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
          position.z = actor->z + (work->unk6a << 19);
          if (MapGrid_CheckCollisionOfActor(actor) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          System_Wait(1);
        }
        break;
      } else {
        s32 result;
#if TLA_PATCH_1
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;
#else
        s32 x, z, off, oldZ;
#endif

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result = MapGrid_CheckCollisionOfActor(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = TRIG_TURN / 4;
          Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          /* FAKEMATCH(wrapper): Import_FieldCheckSpheresOverlap forwarded
           * through Value4. */
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())Import_FieldCheckSpheresOverlap, (s32)&other->x,
                     radius, (s32)&position, other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
#endif
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
#if TLA_PATCH_1
        z = actor->z + (work->unk6a << 17);
#else
        do {
          oldZ = actor->z;
        } while (0);
        z = oldZ + (work->unk6a << 17);
#endif
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z = z - off;
        work->unk04 = 0;
        System_Wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = 0;
    Actor_SetSpriteColorMode(actor, 0);
  }
  work->unk04 = 0;
#if !TLA_PATCH_1 && defined(UBFIX)
  return 0;
#endif
}
#if !TLA_PATCH_1
#undef position
#endif

void Func_02002b88(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Func_0200297c();
}

void ScreenTransition_Start(s32 mode, s32 frames);

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
s32 MarsLighthouseRoomF_SlideOnIce(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct RideWork *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  if (*(u16 *)&event->screenStatus == 0) {
    ScreenTransition_Start(event->screenEffect, event->delay);
    event->screenStatus = 1;
  }
  Flag_Clear(FLAG_MENU_OPEN);
  event->unk166 = 0;
  event->unk15a = 0;
  *(u16 *)&event->unk15c[2] = 0;
  *(u16 *)&event->unk15c[0] = 0;
  event->encounterEnemyGroup = 0;
  // UB: Party_TakeFallDamage takes no arguments and returns nothing; the bytes
  // call it with two through an int-returning type.
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  /* FAKEMATCH(wrapper): called through a cast to the int-returning two-argument
   * type. */
  ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideWork *)(gInventorySnapshot + 0x188);
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->unk6c->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
      leader->x += ((s32 *)ride->unk6c->unk70)[0];
      leader->z += ((s32 *)ride->unk6c->unk70)[2];
    } else {
      if (((s32 *)ride->unk6c->unk70)[0] != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (((s32 *)ride->unk6c->unk70)[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/ride_if_active.h"

s32 Func_02002cfc(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
}

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16, as FieldSprite has it
  u8 vramBlock;
  u8 retainCacheEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u32 gData_02005dbc[];
s32 VramCache_FreeEntry(u32 slot);

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_02002d08(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gData_02005dbc);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = Q16_ONE / 2;
      sprite = object->sprite;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->retainCacheEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

extern u32 gData_02005e00[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + (Random16() >> 4) - (Random16() >> 4);
      angle = 0x4000 + rise;
      radius = Random16() * 8;
      size = Random16();
      Actor_StartScript(object, gData_02005e00);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(temporary): the scale is taken before the velocity
       * stores. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = 0x8000;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

void Func_02002f6c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}
#undef position
#undef WORK

#define MapLayer MapLayer__s2f80
#include "map.h"
#undef MapLayer
/* VIEW(s2f80): tag struct MapLayer as struct MapLayer__s2f80; room.h has
 * another definition; not tried: struct MapLayer__s2f80 was not found. */

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2f80): tag struct Work020023c4 as struct Work020023c4__s2f80; flash.c
 * has another definition; not tried: struct Work020023c4__s2f80 was not found.
 */
struct Work020023c4__s2f80 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);
void Func_02002d08(struct Actor *actor);
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed);
extern u8 gInventorySnapshot[];

void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct WorkEvent *work2;
    struct Work020023c4__s2f80 *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->unk166 != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    FieldVector_AddPolarOffset(0x80000, actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = 0x4000;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      /* FAKEMATCH(wrapper): forwarded. */
      MapGrid_SetShake(5, 5, 1);
      /* FAKEMATCH(wrapper): forwarded. */
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_573);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_572);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
      }
    }

    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    FieldActor_SetMoveTargetFixed(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->eventWork;
    other = (struct Work020023c4__s2f80 *)(gInventorySnapshot + 0x188);
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->unk154 = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= 0x1000;
      actor->scaleY -= 0x1000;
      Func_02002d08(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
#undef ABS

#undef RideSlot
#undef RideWork
void Func_02002f6c(struct Actor *object);

void Actor_Attach(s32 id, s32 target, s32 kind, s32 a3, const void *script) {
  struct Actor *source = Actor_Get(id);
  struct Actor *object = Actor_Get(target);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = a3;
  BYTE_AT(object, 98) = kind;
  object->update = Func_02002f6c;
}

#include "field/event_inline.h"

void Actor_RollAndCrumble(struct Actor *actor);

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Actor_RollAndCrumble as its update hook and its speed, clears its
/// scale and action, has it watch what self watches, and calls
/// Event_PlaySound(SE_104). The twin of overlay 6ac's Func_02002e6c.
void Func_02003304(struct Actor *self) {
  struct Actor *actor = Actor_Get(((u8 *)self)[0x62]);
  s32 angle = (s16)self->unk66;
  struct FieldSprite *sprite = actor->sprite;
  volatile s32 position[3];

  position[0] = self->x;
  position[1] = self->y - PIXELS(12);
  position[2] = self->z;
  FieldVector_AddPolarOffset(PIXELS(32), angle, (struct Vector3 *)position);
  actor->facing = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],
                                       position[2]);
  actor->update = Actor_RollAndCrumble;
  actor->actionFlags = 0;
  actor->speedLimit = 0x80000;
  actor->acceleration = 0x2000;
  sprite->rotation = angle - 0x4000;
  actor->scaleX = 0;
  do {
    actor->scaleY = 0;
  } while (0);
  actor->timer = 0;
  actor->watchedObject = self->watchedObject;
  Event_PlaySound(SE_104);
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

/* VIEW(s328c): tag struct RideSlot as struct RideSlot__s328c;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:2052: redefinition of `struct
 * RideSlot'. */
#define RideSlot RideSlot__s328c
struct RideSlot {
  u16 value;
  u8 unk02[2];
};

/* VIEW(s328c): tag struct RideWork as struct RideWork__s328c;
 * scene/mars_lighthouse_room_f has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_room_f.c:2059: redefinition of `struct
 * RideWork'. */
#define RideWork RideWork__s328c
struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[8];
  struct Actor *unk6c;
  void *handle;
};

extern u8 gInventorySnapshot[];
extern u16 gData_02004cf0[];
void Func_020041ec(void *handle);
void Actor_SetupScriptedFor(struct Actor *object, s32 unused);

s32 Actor_Park(struct Actor *object) {
  Func_02003304(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

#include "../field/object/set_sprite_priority.c"

#undef RideSlot
#undef RideWork
#include "../field/field_effect/move.c"

#undef DEFINE_FUNC_02000444
#undef DEFINE_FUNC_02000CF4
#undef ABS
#undef IWRAM
#undef position
#undef WORK
#undef ABS
#undef MapLayer
#undef SceneWork
#undef Camera_SetBounds
#undef RideSlot
#undef RideWork
#undef Ride_SetEntry
#undef Unit_Get
#undef RideSlot
#undef RideWork
#undef MapLayer
#undef RideSlot
#undef RideWork
// @file
#include "field/event_inline.h"

void Actor_StartScript(struct Actor *object, const void *script);
extern s32 gFieldEffect_DurationScripts[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

extern struct CellTarget gData_020070a8;
#define CELL_TABLE gData_020070a8
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

s32 CrackedTiles_Find(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_020070a8;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *cells;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cx, cz;
  s32 k;

  Iwram_ClearWordsEntry(table, 4);
  table->kind = kind;
  table->firstFlag = flag;
  table->flagByte = flagByte;
  table->x = x;
  table->z = z;
  table->current = -1;
  table->count = 0;
  map = (struct CellMapWork *)Ram_HeapSlots->mapWork;
  layer = &map->layers[0];
  cells = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell
   */
  /* copy's masks out of them and 321 of 268 instructions differ. */
  j = 0;
  rows = cells;
  if (j < height) {
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
    column:
      event = cell->event;
      if (event != 0 && event >= kind) {
        last = kind + 1;
        if (event <= last) {
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            if (event == last)
              Event_SetFlag(entry->flag);
          } else if (Event_GetFlag(entry->flag)) {
            MapGrid_CopyCells(table->x, table->z + 1, 1, 1,
                              cx | (s16)(table->x & 64),
                              cz | (s16)(table->z & 64));
          }
          entry++;
          table->count++;
        }
      }
      i++;
      cell++;
      if (i < width)
        goto column;
    }
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    search:
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      entry++;
      k++;
      if (k < table->count)
        goto search;
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;

    k = 63;
  fill:
    *word++ = 0x11111111;
    if (--k >= 0)
      goto fill;
  }
  table->tile = VramCache_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(CrackedTiles_Update, 0x480);
  return table->count;
}

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; Func_02003f18 calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall_event.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
#include "field/event_inline.h"

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetShakeFixed(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s3d80): routine CrackedTile_FallEvent as this part declared it; the
 * joined file has another; not tried: CrackedTile_FallEvent's declaration holds
 * a FAKEMATCH tag. */
void CrackedTile_FallEvent__s3d80(
    struct WarpWork *work,
    struct WarpEntry *entry) __asm__("CrackedTile_FallEvent");
#define CrackedTile_FallEvent CrackedTile_FallEvent__s3d80

s32 Func_02003f18(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct WarpWork *work = &gData_020070a8;
  struct WarpEntry *entry = work->entries;
  s32 index;

  index = 0;
  if (index >= work->count)
    goto end;
top:
  if (entry->x == x && entry->z == z) {
    if (!Event_GetFlag(entry->flag)) {
      CrackedTile_Crack(work, entry);
      Event_SetFlag(entry->flag);
      work->current = index;
      goto end;
    }
    if (index == work->current)
      goto end;
    work->current = index;
    CrackedTile_FallEvent(work, entry);
    GameFlag_SetByte(work->positionFlag, x);
    GameFlag_SetByte(work->positionFlag + 8, z);
    return 1;
  }
  index++;
  entry++;
  if (index < work->count)
    goto top;
end:
  return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct WarpWork *work = &gData_020070a8;
  s32 x = FieldGameFlag_GetByte(work->positionFlag);
  s32 z = FieldGameFlag_GetByte(work->positionFlag + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->positionFlag, 0);
    GameFlag_SetByte(work->positionFlag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    other = *(struct Actor **)(event + 460);
    other->x = object->x;
    other->z = object->z;
    Map_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}

extern s16 gData_02005ef8[];

void Actor_SteerByPad(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(FLAG_MENU_OPEN))
    angle = -1;
  else
    angle = gData_02005ef8[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Actor_SelectAnimation(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Actor_SelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

#include "shared/leader_stop_if_pending.h"

// A point on the map: 16.16 fixed-point pixels.
extern s16 gData_02005f38[];

void Leader_SlideOnIce(void) {
  struct Vector3 probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Vector3 *position;
  struct Vector3 *next;
  s32 heading;
  s32 nextBehavior;
  s32 distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 dx;
  s32 dz;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);

top:
  Leader_StopIfPending();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z;
  dx = x - actor->x;
  distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);
  if (distance < PIXELS(4)) {
    heading = gData_02005f38[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          PIXELS(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->timer = 0;
  FieldActor_SetMoveTargetFixed(actor, startX, actor->y, startZ);
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  FieldActor_WaitForMove(actor);
  actor->update = Actor_SteerByPad;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Import_FieldCheckSpheresOverlap(&other->x, radius, &probe,
                                          other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTargetFixed(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  {
    struct Vector3 *step;

    step = &probe;
    FieldVector_AddPolarOffset(PIXELS(16), heading, step);
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the
     * address. */
    step = NULL;
  }
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  FieldActor_SetMoveTargetFixed(actor, x, actor->y, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Actor_SelectAnimation(actor, 1);
}

#define SLIDE_FROM_EDGE Leader_SlideOnIceFromEdge
#define SLIDE_STOPPED gMarsLighthouseRoomF_SlideStopped
#define SLIDE_STOP_LEADER Leader_StopIfPending
#define SLIDE_ANGLES gData_02005f38
#define SLIDE_SET_ANIMATION Actor_SelectAnimation
#define SLIDE_HAS_ARRIVED Actor_HasArrived
#define SLIDE_KEEP_SLIDING Actor_SteerByPad
#define SLIDE_SPHERES_OVERLAP Import_FieldCheckSpheresOverlap
#include "shared/slide_on_ice_from_edge.c"
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP

void Leader_SetSlideStopped(s32 value) {
  gMarsLighthouseRoomF_SlideStopped = value;
}

void Leader_SlideOnIceAhead(void) {
  struct Vector3 probe;
  s32 heading;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 radius;
  struct Actor *actor;
  struct Vector3 *next;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Vector3 *position;
  u8 *flags;
  s32 i;
  s32 cell;
  s32 angle;
  s32 zero;
  struct Vector3 *step;
  s32 startX;
  s32 startZ;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((actor->facing + 0x2000) & -0x4000);

top:
  Leader_StopIfPending();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  /* Default: one zero, spilled and reloaded at each use. */
  zero = 0;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  /* FAKEMATCH(temporary): cell holds the masked x. */
  cell = actor->x & 0xfff00000;
  i = PIXELS(8);
  position = &probe;
  position->x = startX = cell + i;
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + i;
  x = startX;
  z = startZ;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*(layer = &actor->terrainId),
                                                 startX, startZ);
  FieldVector_AddPolarOffset(PIXELS(16), heading, position);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > i) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = x;
  position->z = z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->timer = zero;
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Actor_SteerByPad;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading
   * nextBehavior twice. */
  asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Import_FieldCheckSpheresOverlap(&other->x, radius, &probe,
                                          other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTargetFixed(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  FieldVector_AddPolarOffset(PIXELS(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address.
   */
  step = NULL;
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  FieldActor_SetMoveTargetFixed(actor, x, actor->y, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Actor_SelectAnimation(actor, 1);
}

extern u16 gPushKinds[];
extern s8 gPushFootprints[][4];

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004938(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->sprite)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gPushKinds[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gPushKinds[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gPushFootprints[j][0]) + ABS(gPushFootprints[j][2])) >> 4;
  height = (ABS(gPushFootprints[j][1]) + ABS(gPushFootprints[j][3])) >> 4;
  x = (object->x + (gPushFootprints[j][0] << 16)) >> 20;
  z = (object->z + (gPushFootprints[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Func_02004a44(0, x, z, width, height, 255);
  Func_02004a44(2, x, z, width, height, 255);
  return 1;
}

void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  u8 *cells = map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += (x + (z << 7)) << 2;
  for (row = 0; row < height; row++) {
    u8 *cell = cells + (row << 9);

    for (column = 0; column < width; column++) {
      cell[2] = value;
      cell += 4;
    }
  }
}

extern s32 gPushLeaderSteps[];

static inline s32 PushTarget_CheckCollision(struct Actor *object,
                                            s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

// Finds the object the leader is pushing and steps its footprint, one cell
// at a time in the leader's direction, until a cell of it is blocked
// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
#include "shared/push_target_find_slide.h"

#undef PIXELS
// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

#include "shared/push_target_find_ahead.h"

#undef PIXELS

void PushTarget_Push(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (object->z + (gPushFootprints[footprint][1] << 16)) >> 20;
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  Func_02004a44(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.currentOwner, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFixed(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.currentOwner, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (gPushLeaderSteps[direction] << 16) >> 31,
                             (gPushLeaderSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_298);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints[footprint][0] << 16);
  target.z = target.z + (gPushFootprints[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Func_02004a44(0, target.x, target.z, width, height, 255);
  Func_02004a44(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (from[2] + (gPushFootprints[footprint][1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Func_02004a44(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

#include "shared/actor_stop_at_push_target.h"
#undef ABS
#undef PIXELS
#define PIXELS(value) ((value) << 16)

#define MapLayer MapLayer__s5048
#include "map.h"
#undef MapLayer
/* VIEW(s5048): tag struct MapLayer as struct MapLayer__s5048; room.h has
 * another definition; not tried: struct MapLayer__s5048 was not found. */

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
void LightSprites_Draw(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020074cc].offset >> 5;
  viewY = map->origin[1];
  oam = Data_0200f510;

  for (i = 0; i < gData_020074ce; i++) {
    struct Actor *actor = gData_020074d0[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_020075d0;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_020075d2 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_020075d2;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_020075d2 == -1)
          oam->priority = BattleEffect_GetObjectPriority(actor);
        else
          oam->priority = gData_020075d2;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void LightSprites_InitA(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWordsEntry(gData_020074d0, 64);
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gData_020074cc = VramCache_FindFreeEntry();
  VramCache_Load(gData_020074cc, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void LightSprites_InitB(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWordsEntry(gData_020074d0, 64);
  Decompress_TaggedLz(gField_CachedTilesB, buffer);
  gData_020074cc = VramCache_FindFreeEntry();
  Vram_Load(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void LightSprites_InitC(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWordsEntry(gData_020074d0, 64);
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gData_020074cc = VramCache_FindFreeEntry();
  Vram_Load(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 1;
  gData_020075d2 = -1;
}

void LightSprites_Add(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_020074ce;
    gData_020074d0[*count] = object;
    (*count)++;
  }
}

void Func_020053bc(s32 value) { gData_020075d2 = value; }

s32 gData_020056c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
u32 gData_020056d4[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(PIXELS(680), 0, PIXELS(304)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(PIXELS(760), 0, PIXELS(304)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gData_02005748[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(PIXELS(584), 0, PIXELS(304)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(PIXELS(664), 0, PIXELS(304)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gData_020057bc[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_FLAG_CHECK(0x220),
    SC_IFNOT(SC_TO(1)),
    SC_LABEL(2),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(2)),
};

u32 gData_02005890[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(168), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_0200593c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(232), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(360), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_02005a3c[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                   
      SC_SET(SW_Y, PIXELS(32)),                                                
      SC_SET(SW_GROUND, PIXELS(32)),                                           
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_END,                                                                  
  };

u32 gData_02005a88[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s16 gData_02005b5c[] = {
    24, 25, 26, 24, 25, 0,
};

u32 gData_02005b68[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005bac[] = {
    0, 42, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u16 gData_02005bf0[] = {
    8, 67, 0,
};

u16 gData_02005bf6[] = {
    10, 1, 0,
};

u16 gData_02005bfc[] = {
    18, 17, 19, 49, 20, 19, 21, 3,
    0,
};

u16 gData_02005c0e[] = {
    22, 0, 0,
};

u16 gData_02005c14[] = {
    13, 17, 14, 33, 15, 49, 16, 131,
    0,
};

u16 gData_02005c26[] = {
    19, 1, 0,
};

u16 gData_02005c2c[] = {
    13, 3, 0,
};

u16 gData_02005c32[] = {
    14, 1, 0,
};

u16 gData_02005c38[] = {
    8, 3, 9, 3, 10, 3, 0,
};

u16 gData_02005c46[] = {
    12, 0, 0,
};

u16 gData_02005c4c[] = {
    13, 3, 14, 3, 15, 3, 0,
};

u16 gData_02005c5a[] = {
    10, 0, 0,
};

u16 gData_02005c60[] = {
    24, 3, 25, 3, 26, 3, 0,
};

u16 gData_02005c6e[] = {
    18, 0, 19, 0, 20, 0, 21, 0,
    22, 0, 0,
};

s32 gData_02005c84[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0xf80000, 0x200000, 0x2a00000, 1,  4,          0xf80000, 0x160000,         0x2a00000, 1,   
      4,  0xf80000,         0x160000, 0x2b00000, 1,  22, 37, (s32)Func_020006f4, 46, (s32)Actor_Park, 17,      
  };

s32 gData_02005ce0[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0xb80000, 0x400000, 0x2300000, 1,  4,          0xb80000, 0x360000,         0x2300000, 1,   
      4,  0xb80000,         0x360000, 0x2400000, 1,  22, 37, (s32)Func_02000d80, 46, (s32)Actor_Park, 17,      
  };

s32 gData_02005d3c[] = {                                                               
      SC_CALL(Actor_PopUp), SC_MOVE(PIXELS(872), PIXELS(32), PIXELS(128)),             
      SC_SYNC,        SC_MOVE(PIXELS(872), PIXELS(22), PIXELS(128)),             
      SC_SYNC,        SC_MOVE(PIXELS(872), PIXELS(22), PIXELS(144)),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

u32 gData_02005d8c[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

u32 gData_02005dbc[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005e00[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

s16 gData_02005ef8[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384, 16384, 8192, 24576, 16384,
    -1, 0, -32768, -1,
};

s16 gData_02005f18[] = {
    -1, 0, -0x8000, -1, -0x4000, -1, -1, -0x4000,
    0x4000, -1, -1, 0x4000, -1, 0, -0x8000, -1,
};

s16 gData_02005f38[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384, 16384, 0, -32768, 16384, -1,
    0, -32768, -1,
};

u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8},  {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32},  {-32, -8, 32, 8},  {-8, -32, 8, 32},
};

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };
// clang-format on

#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP
#undef ABS
#undef PIXELS
#define PIXELS(value) ((value) << 16)
#undef CrackedTile_FallEvent
#undef MapLayer

/* VIEW(data): routine Func_02001010 as module/scene/airs_rock.h declares it;
 * the joined file has another; not tried: Func_02001010's declaration holds a
 * FAKEMATCH tag. */
void Func_02001010__data(void) __asm__("Func_02001010");
#define Func_02001010 Func_02001010__data
#include "module/scene/airs_rock.h"

void Func_020000d0(void);
void Func_020002ec(void);
// Func_020002ec is from patch 1; the Japanese table contains 0.
#if !TLA_PATCH_1
#define MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC 0
#else
#define MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC ((s32)Func_020002ec)
#endif
void Func_02000670(void);
void Func_02000778(void);
void Func_02000784(void);
void Func_0200079c(void);
void Func_020007b4(void);
void Func_020007cc(void);
void Func_02000900(void);
void Func_02000aac(void);

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseRoomF1),
    SCENE_EXIT(1, MarsLighthouseRoomM4, 2),
    SCENE_EXIT(2, MarsLighthouseRoomF1, 3),
    SCENE_EXIT(3, MarsLighthouseRoomF1, 2),
    SCENE_EXIT(4, MarsLighthouseRoomM6, 3),
    SCENE_EXITS(MarsLighthouseRoomF2),
    SCENE_EXIT(1, MarsLighthouseRoomM2, 2),
    SCENE_EXIT(2, MarsLighthouseRoomF2, 3),
    SCENE_EXIT(3, MarsLighthouseRoomF2, 2),
    SCENE_EXIT(4, MarsLighthouseRoomF2, 5),
    SCENE_EXIT(5, MarsLighthouseRoomF2, 4),
    SCENE_EXIT(6, MarsLighthouseRoomF2, 7),
    SCENE_EXIT(7, MarsLighthouseRoomF2, 6),
    SCENE_EXIT(8, MarsLighthouseRoomM6, 2),
    SCENE_EXIT(9, MarsLighthouseRoomF2, 9),
    SCENE_EXITS(MarsLighthouseRoomF3),
    SCENE_EXIT(1, MarsLighthouseRoomM3, 2),
    SCENE_EXIT(2, MarsLighthouseRoomF3, 3),
    SCENE_EXIT(3, MarsLighthouseRoomF3, 2),
    SCENE_EXIT(4, MarsLighthouseRoomF3, 5),
    SCENE_EXIT(5, MarsLighthouseRoomF3, 4),
    SCENE_EXIT(6, MarsLighthouseRoomF3, 6),
    SCENE_EXIT(7, MarsLighthouseRoomF3, 7),
    SCENE_EXIT(8, MarsLighthouseRoomF3, 9),
    SCENE_EXIT(9, MarsLighthouseRoomF3, 8),
    SCENE_EXIT(10, MarsLighthouseRoomF3, 10),
    SCENE_EXIT(11, MarsLighthouseRoomF3, 12),
    SCENE_EXIT(12, MarsLighthouseRoomF3, 11),
    SCENE_EXIT(13, MarsLighthouseRoomF3, 14),
    SCENE_EXIT(14, MarsLighthouseRoomF3, 13),
    SCENE_EXIT(15, MarsLighthouseRoomM6, 4),
    SCENE_EXIT(16, MarsLighthouseRoomF3, 16),
    SCENE_EXITS(MarsLighthouseRoomF4),
    SCENE_EXIT(1, MarsLighthouseRoomM5, 2),
    SCENE_EXIT(6, MarsLighthouseRoomF4, 7),
    SCENE_EXIT(7, MarsLighthouseRoomF4, 6),
    SCENE_EXIT(8, MarsLighthouseRoomF4, 9),
    SCENE_EXIT(9, MarsLighthouseRoomF4, 8),
    SCENE_EXIT(10, MarsLighthouseRoomF4, 11),
    SCENE_EXIT(11, MarsLighthouseRoomF4, 10),
    SCENE_EXIT(12, MarsLighthouseRoomF4, 13),
    SCENE_EXIT(13, MarsLighthouseRoomF4, 12),
    SCENE_EXIT(14, MarsLighthouseRoomF4, 15),
    SCENE_EXIT(15, MarsLighthouseRoomF4, 14),
    SCENE_EXIT(5, MarsLighthouseRoomM6, 1),
    SCENE_EXITS_END,
};

// clang-format off
LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room4Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(315, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_02005748, 664, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_020056d4, 760, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(254, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room1Placements[] = {
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_020057bc, 104, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gData_020056c8, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_020056c8, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_020056c8, 248, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 248, 0, 848, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(293, CONDITION_ALWAYS, 7, 144, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(294, CONDITION_ALWAYS, 7, 104, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(295, CONDITION_ALWAYS, 7, 104, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(415, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room3Placements[] = {
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_02005890, 88, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_0200593c, 360, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_02005a3c, 120, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_02005a88, 968, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_020056c8, 872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA u32 gData_02006980[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_Y, PIXELS(80)),                                                
      SC_SET(SW_GROUND, PIXELS(80)),                                           
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(PIXELS(216), 0, PIXELS(512)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(PIXELS(152), 0, PIXELS(512)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room2Placements[] = {
    ACTOR_PLACEMENT(294, CONDITION_ALWAYS, 7, 568, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(293, CONDITION_ALWAYS, 7, 544, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(293, CONDITION_ALWAYS, 7, 544, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gData_02006980, 152, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gData_020056c8, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_020056c8, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gData_020056c8, 184, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(295, CONDITION_ALWAYS, 7, 600, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room4Triggers[] = {
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(Func_02000168)},
    {STEP_TRIGGER_WALKING, 31, CONDITION_ALWAYS, EVENT(Func_02000170)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x51, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02000248)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_0200025c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_02000248)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_0200025c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 10, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 11, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FUNC_020002EC},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 33, 0xa38, EVENT(Func_0200021c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0xa38, EVENT(Func_020001f4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(Func_020002b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(Func_020002b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT(Func_020002b8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT(Func_020002b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT(Func_020002b8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, CONDITION_ALWAYS, 0},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200297c)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 33, 0x220, EVENT(Func_020004f8)},
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(Func_02000418)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02002b88)},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200297c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20, 0xa44, EVENT(Func_02000444)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 21, 0xa45, EVENT(Func_02000480)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 22, 0xa46, EVENT(Func_020004bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(Func_020000bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 11, CONDITION_ALWAYS, EVENT(Func_02000554)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11, CONDITION_ALWAYS, EVENT(Func_02000670)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room3Triggers[] = {
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_02000944)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 2, CONDITION_ALWAYS, EVENT(Func_02000754)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 3, CONDITION_ALWAYS, EVENT(Func_02000760)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 4, CONDITION_ALWAYS, EVENT(Func_0200076c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 5, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 6, CONDITION_ALWAYS, EVENT(Func_02000784)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 7, CONDITION_ALWAYS, EVENT(Func_02000790)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, EVENT(Func_0200079c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 9, CONDITION_ALWAYS, EVENT(Func_020007a8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10, CONDITION_ALWAYS, EVENT(Func_020007b4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11, CONDITION_ALWAYS, EVENT(Func_020007c0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12, CONDITION_ALWAYS, EVENT(Func_020007cc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13, CONDITION_ALWAYS, EVENT(Func_020007d8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14, CONDITION_ALWAYS, EVENT(Func_020007e4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x358, EVENT(Func_020007f0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0xa37, EVENT(Func_020008a0)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_020008bc)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02000900)},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200297c)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_SlideOnIce)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_ContinueSlide)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS, EVENT(Func_02000b40)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(Func_02002b88)},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Func_0200297c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_020000bc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 16, CONDITION_ALWAYS, EVENT(Func_02000aac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 16, CONDITION_ALWAYS, EVENT(Func_02000ad8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x220, EVENT(Func_02000cf4)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x221, EVENT(Func_02000d38)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gData_0200703c = -1;

LATE_DATA s32 gData_02007040 = 1;

LATE_DATA s32 gData_02007044[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
#undef MarsLighthouseRoomF_ContinueSlide
#undef Func_02001010
