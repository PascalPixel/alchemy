
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"
/// @file
/// Four small places the ship reaches, sharing one overlay: the Gondowan
/// Settlement (rooms 1 to 3), whose bush Cyclone cuts to a stair; the Hesperia
/// Settlement (4 to 6), with its vine, crate and Tinder; Kalt Island (7 and 8),
/// with its apple, frozen river and Gel; and SW Atteka Islet (9 and 10), whose
/// pillar and crate go over the falls, and its boulder and Jupiter Djinni.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#undef EFFECT_SPAWN

#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "field/event_inline.h"

s32 Trig_Cos(s32 angle);

extern u8 MsgSettlements01[];

#include "system/frame.h"

extern s16 gSettlements_FallsObjects[];

void Settlements_SpawnSmoke(s32 x, s32 y, s32 z);
void Settlements_CycleWaterColors(void);
void Scroll_StartHBlank(void);
void Settlements_SinkPillar(void);
void Settlements_SlideCrateDownFalls(void);
void Settlements_RestoreWallCells(void);
void Settlements_DropCrate(void);
void Leader_SlideOnIce(void);
void Leader_StopMoving(void);
void Event_ResolveAllPendingActions(void);
void Scene_QueueMusic(s32 a);
#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void Settlements_EnterDoor(void);
void Settlements_DriftSmoke(struct Actor *object);
void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_Release(struct Actor *object);
extern s32 gSettlements_WaterfallLine;
extern u16 gSettlements_WaterfallScrollAbove;
extern u16 gSettlements_WaterfallScrollBelow;
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 a, s32 b, void (*callback)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 a, s32 b);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

extern u32 gSettlements_Room5Regions[];
extern u32 gSettlements_Room3Regions[];

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}
#include "field_event.h"
#include "field/field.h"
#include "types.h"
#include "graphics/palette_snapshots.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "system/io_reg.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"
#include "map.h"
#include "system/io_write.h"
#include "system/field_effect.h"
#include "actor.h"
#include "field/event_inline.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "field/field_sequence_event.h"
#include "call.h"
#include "system/input.h"
#include "shared/tables.h"

extern s16 gSlideOnIce_AnglesByKey[];

void Settlements_StepOntoWall(void);

void Leader_StopIfPending(void);

void Settlements_MeetTinder(void);

void Settlements_CatchApple(void);
void Settlements_CutLeftBush(void);

void Settlements_CutStairsBush(void);

void Settlements_EnterDoor5(void);

void Settlements_EnterDoor3(void);

void Settlements_StepOntoIce(void);

void Settlements_StepOffIce(void);
void Settlements_StepOnIceSlide(void);
void Settlements_LashRope(void);

void Settlements_UnblockFallsObjects(void);

void Settlements_BlockFallsObject(void);

void Settlements_MoveCrate(void);

void Settlements_StepOffWall(void);

void Settlements_UnblockCrate(void);

void Settlements_BlockCrate(void);

void Settlements_LiftBoulder(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gSettlements_BobScript[];
extern u32 gSettlements_CrateBobScript[];

struct OrbitSprite {
  u8 unk00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 unk06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 unk0a[6];
  u8 palette;
  u8 unk11[10];
  u8 state;
};

struct OrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[12];
  s32 orbitAngle;
  u8 unk34[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 unk44[12];
  struct OrbitSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 state;
  u8 unk57[5];
  u8 active;
  u8 unk5d[4];
  u8 visible;
  u8 unk62[10];
  u32 callback;
};

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SettlementsSpawn {
  s32 unk00;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

extern char SceneId_Settlements1;
extern char SceneId_Settlements2;
extern char SceneId_Settlements4;
extern char SceneId_Settlements7;
extern char SceneId_Settlements8;
extern char SceneId_Settlements9;
extern char SceneId_Settlements10;

void Rope_Initialize(void);

extern char SceneId_Settlements6;

void Settlements_StepOntoWall(void);

void FieldEffect_StartBlankOverlay(s32 a);

void Actor_PlaceBlocks(const void *data);

extern u16 gSettlements_WaterColors[];
extern u16 gSettlements_WaterColorsSaved[];

void Settlements_SpawnSplashRing(s32 objectId);

void Settlements_MovePillar(void);

void Settlements_MeetGel(void) { Djinn_JoinParty(10, 1, 14); }

void Settlements_MeetTinder(void) { Djinn_JoinParty(11, 2, 14); }

void Settlements_MeetLostJupiterDjinni(void) { Djinn_JoinParty(23, 3, 19); }

s32 Settlements_ClearSpriteFlags(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

#include "shared/actor_bob.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u32 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;
  u32 *placements;

  if (scene == (s32)&SceneId_Settlements5)
    return gSettlements_Room5Regions;
  placements = 0;
  if (scene == (s32)&SceneId_Settlements3)
    placements = gSettlements_Room3Regions;
  return placements;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gSettlements_Placements[];
extern struct ActorPlacement gSettlements_Room1Placements[];
extern struct ActorPlacement gSettlements_Room2Placements[];
extern struct ActorPlacement gSettlements_Room4Placements[];
extern struct ActorPlacement gSettlements_Room5Placements[];
extern struct ActorPlacement gSettlements_Room9Placements[];
extern struct ActorPlacement gSettlements_Room10Placements[];
extern struct ActorPlacement gSettlements_Room7Placements[];
extern struct ActorPlacement gSettlements_Room8Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Settlements1)
    return gSettlements_Room1Placements;
  if (scene == (s32)&SceneId_Settlements2)
    return gSettlements_Room2Placements;
  if (scene == (s32)&SceneId_Settlements4)
    return gSettlements_Room4Placements;
  if (scene == (s32)&SceneId_Settlements5)
    return gSettlements_Room5Placements;
  if (scene == (s32)&SceneId_Settlements7)
    return gSettlements_Room7Placements;
  if (scene == (s32)&SceneId_Settlements8)
    return gSettlements_Room8Placements;
  if (scene == (s32)&SceneId_Settlements9)
    return gSettlements_Room9Placements;
  if (scene == (s32)&SceneId_Settlements10)
    return gSettlements_Room10Placements;
  return gSettlements_Placements;
}

void Settlements_CatchApple(void) {
  s32 mode;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0xf5d);
  ItemDisplay_ShowItem(ITEM_APPLE, 3);
  mode = 0;
  Party_GiveItem(ITEM_APPLE, mode);
  Event_End();
}

void Settlements_WarnOfRiddleOfRock(s32 actor) {
  s32 msg = (s32)MsgSettlements01;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Settlements_CutLeftBush(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Settlements_CutStairsBush(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.currentOwner, 2, 0);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 88, 296);
  }
  TileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(0xa60);
}

void Settlements_EnterDoor(void) {
  s32 next = gFieldWork->stepTrigger;

  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->actionFlags &= 0xfe;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  FieldActor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_Get(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  FieldActor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(next);
  Event_End();
}

void Settlements_EnterDoor4(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, 16, 0);
  Settlements_EnterDoor();
}

void Settlements_EnterDoor5(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  Actor_WalkByAndWait(4, -14, -8);
  Settlements_EnterDoor();
}

void Settlements_EnterDoor3(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(4, 192 << 8, 0);
  Settlements_EnterDoor();
}

void Settlements_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
  Event_SetFlag(FLAG_SCENE_TEMP_30);
}

extern struct Trigger gSettlements_Triggers[];
extern struct Trigger gSettlements_Room1Triggers[];
extern struct Trigger gSettlements_Room2Triggers[];
extern struct Trigger gSettlements_Room4Triggers[];
extern struct Trigger gSettlements_Room5Triggers[];
extern struct Trigger gSettlements_Room7Triggers[];
extern struct Trigger gSettlements_Room8Triggers[];
extern struct Trigger gSettlements_Room9Triggers[];
extern struct Trigger gSettlements_Room10Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Settlements1)
    return gSettlements_Room1Triggers;
  if (scene == (s32)&SceneId_Settlements2)
    return gSettlements_Room2Triggers;
  if (scene == (s32)&SceneId_Settlements4)
    return gSettlements_Room4Triggers;
  if (scene == (s32)&SceneId_Settlements5)
    return gSettlements_Room5Triggers;
  if (scene == (s32)&SceneId_Settlements7)
    return gSettlements_Room7Triggers;
  if (scene == (s32)&SceneId_Settlements8)
    return gSettlements_Room8Triggers;
  if (scene == (s32)&SceneId_Settlements9)
    return gSettlements_Room9Triggers;
  if (scene == (s32)&SceneId_Settlements10)
    return gSettlements_Room10Triggers;
  return gSettlements_Triggers;
}

void Settlements_DriftSmoke(struct Actor *object) {
  s32 step;

  object->x += object->timer << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->timer += 2;
  if (--*(s32 *)&object->watchedObject == 0)
    Actor_Release(object);
}

void Settlements_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct Actor *object = FieldActor_Create(0x11d, x, y, z);

  if (object != NULL) {
    s8 *animation;

    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= 0xfe;
    FieldActor_SetSpriteColor(object, 7);
    animation = object->sprite;
    animation[9] = (animation[9] & -13) | 4;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = Q16_ONE / 2;
    object->scaleY = Q16_ONE / 2;
    *(s32 *)&object->watchedObject = 60;
    object->update = (void (*)(void))Settlements_DriftSmoke;
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

void Settlements_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0)
    Settlements_SpawnSmoke(0xf80000, 0x300000, 0x1000000);
}

/// Moves the object around its orbit's centre (words 14 and 15) and steps the
/// angle by a random amount.
// As ☀️'s Scene_UpdateFloatingNut does.
s32 Settlements_CircleCentre(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Math_Sin(p[12]) * 2;

  if (d > 0)
    d = -d;
  p[2] = p[14] + Trig_Cos(p[12]) * 2;
  p[3] = p[15] + d;
  q[9] = Trig_Cos(p[12] + 0x8000) / 8;
  a = Random16();
  b = Random16();
  {
    u32 x;
    s32 angle;

    x = (u32)a << 9;
    angle = p[12];
    p[12] = angle + ((x >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  }
  return 0;
}

void Rope_Place(s32 a, s32 b, s32 c, s32 d);

static __inline__ void Settlements_Func_02002180(s32 a, s32 b, s32 c, s32 d) {
  Rope_Place(a, b, c, d);
}

void Settlements_SetUpKaltIsland(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  u8 *transfer;

  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    FieldTask_AddFrame(Settlements_PuffChimneySmoke, priority);
  }
  Rope_Initialize();
  Rope_Initialize();
  Settlements_Func_02002180(1, 0x203, 8, 9);
  if (Event_GetFlag(0xf5d)) {
    Actor_SetPosition(11, 0, 0);
    return;
  }
  actor = (struct OrbitObject *)Actor_Get(11);
  sprite = actor->sprite;
  sprite->flags09Mode = 1;
  sprite->flags05Bit5 = 0;
  sprite->flags09High = 0;
  sprite->state = 0;
  Actor_SetSpriteFlags((struct Actor *)actor, 0);
  actor->active = 0;
  actor->mode = 0;
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    actor->y += px(16);
  actor->flags23 &= 0xfe;
  actor->visible = 1;
  transfer = HeapSlot_Alloc(68, 0x608);
  Item_LoadIcon(ITEM_APPLE);
  transfer += 0x400;
  Vram_Load(sprite->palette, 128, transfer);
  HeapSlot_Release(68);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Settlements_CircleCentre;
  actor->state = 0;
}

extern s16 gSettlements_WallCrate[];

void Settlements_StartWaterfall(void);

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements7)
    Settlements_SetUpKaltIsland();
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements9) {
    FieldEffect_StartBlankOverlay(0);
    Actor_PlaceBlocks(gSettlements_FallsObjects);
    if (Event_GetFlag(FLAG_SCENE_TEMP_06))
      Actor_SetPosition(10, 0, 0);
    Actor_GetById(10)->priorityFlags |= 2;
    Actor_SetAnimation(18, 2);
    Actor_SetAnimation(19, 2);
    Actor_SetAnimation(21, 2);
    if (Event_GetFlag(FLAG_SCENE_TEMP_05))
      Actor_SetAnimation(10, 2);
    Settlements_StartWaterfall();
#if TLA_PATCH_1
    if (Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(21, 184, 184);
      Actor_GetById(21)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements4) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_30)) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
      Actor_SetAnimation(9, 4);
    }
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        Event_GetFlag(FLAG_SCENE_TEMP_20)) {
      Event_ClearFlag(FLAG_SCENE_TEMP_20);
      Settlements_StepOntoWall();
    }
    Actor_PlaceBlocks(gSettlements_WallCrate);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements3)
    FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements6)
    FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements9) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_05)) {
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (Event_GetFlag(FLAG_SCENE_TEMP_05))
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
#else
    if (Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(21, 184, 184);
      Actor_GetById(21)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements1) {
    if (Event_GetFlag(0xa60)) {
      MapGrid_CopyCellsTo(5, 46, 5, 47, 1, 1);
      MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
      MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}

void Settlements_StepOntoIce(void) { GameFlag_Set(FLAG_SCENE_TEMP_01); }

void Settlements_StepOffIce(void) { GameFlag_Clear(FLAG_SCENE_TEMP_01); }

void Settlements_StepOnIceSlide(void) { Leader_SlideOnIce(); }

void Settlements_EnterCave(void) {
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  if (gMainRoutineWork.scene == (s32)&SceneId_Settlements4)
    MapGrid_CopyCellsTo(33, 30, 23, 14, 1, 2);
  else
    MapGrid_CopyCellsTo(27, 29, 40, 16, 1, 2);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Settlements_LashRope(void) {
  Rope_Lash(1, 8, 9);
  Event_SetFlag(FLAG_SCENE_TEMP_03);
}

extern s32 gSettlements_WaterColorFrame;

void Settlements_CycleWaterColors(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  s32 *frame;
  s32 phase, i;

  if (work->psynergy != 0 || work->revealActive != 0)
    return;
  frame = &gSettlements_WaterColorFrame;
  if (*frame == -1) {
    u16 *colors = gSettlements_WaterColors;
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(colors, (void *)0x5000060, 32);
    copy(gSettlements_WaterColorsSaved, colors, 32);
    *frame = 0;
  }
  phase = (*frame / 8) % 4;
  for (i = 0; i <= 3; i++)
    gSettlements_WaterColorsSaved[12 + (phase + i) % 4] =
        gSettlements_WaterColors[12 + i];
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)gSettlements_WaterColorsSaved;
      q->count = count + 1;
      *entry++ = 0x5000060;
      *entry = 0x84000008;
    }
    IRQ_UNLOCK(saved);
  }
  gSettlements_WaterColorFrame++;
}

void Settlements_StartWaterfall(void) {
  gFieldWork->transition = 514;
  FieldTask_AddFrame(Settlements_CycleWaterColors, 0x480);
  Scene_QueueMusic(SE_170);
  Scroll_StartHBlank();
}

// The hblank split: above the line Scroll_StartHBlank sets, BG1 scrolls by one
// value and blending is off; below it, by the other, with BG1 blended.

/// The hblank split: above the line Scroll_StartHBlank sets, BG1 scrolls by one
/// value and blending is off; below it, by the other, with BG1 blended.
void Scroll_HBlank(void) {
  if (REG_VCOUNT < gSettlements_WaterfallLine) {
    Io_Write16(gSettlements_WaterfallScrollAbove, &REG_BG1HOFS);
    REG_BLDCNT = 0;
  } else {
    Io_Write16(gSettlements_WaterfallScrollBelow, &REG_BG1HOFS);
    REG_BLDALPHA = 0x100c;
    REG_BLDCNT = 0x3f42;
  }
}

#include "shared/layer_track_scroll.h"

#include "shared/scroll_start_hblank.h"

void Settlements_UnblockFallsObjects(void) {
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_BlockFallsObject(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 2;
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_SplashCrate(s32 id);

void Settlements_SinkPillar(void) {
  struct Actor *object = Actor_GetById(9);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;

  if (!Event_GetFlag(FLAG_SCENE_TEMP_04) && column == 11 && row == 12) {
    object->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      object->y -= px(2);
      Event_Wait(1);
    }
    Settlements_SplashCrate(9);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(21, 184, 184);
    Actor_GetById(21)->motionFlags = 0;
    Actor_GetById(21)->y -= px(64);
    MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  }
}

void Settlements_SlideCrateDownFalls(void) {
  struct Actor *object = Actor_GetById(10);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    object->motionFlags = 0;
    i = 8;
  sink:
    FieldEvent_Wait(1);
    object->y -= px(2);
    i--;
    if (i > 0)
      goto sink;
    Settlements_SplashCrate(10);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 224, 1);
    Actor_SetSpeed(10, 0.8);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldActor_WaitForMoveById(10);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_SetSpeed(10, 2.4);
    Actor_SetDestinationOffset(10, 0, 80);
    FieldActor_WaitForMoveById(10);
    Settlements_SplashCrate(10);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      FieldEvent_Wait(10);
      Actor_SetSpeed(10, 0.8);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldActor_WaitForMoveById(10);
      Actor_SetSpeed(10, 2.5);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(10, 0, 0);
      Event_SetFlag(FLAG_SCENE_TEMP_06);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(10, 0, 12);
      FieldActor_WaitForMoveById(10);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(10, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(FLAG_SCENE_TEMP_05);
    }
    Event_End();
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_07) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_07);
    Event_End();
  }
}

void Settlements_MovePillar(void) {
  Actor_PlaceMovedBlock();
  Settlements_SinkPillar();
}

void Settlements_MoveCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_SlideCrateDownFalls();
}

void Settlements_PushIntoFalls(void) {
  Actor_PlaceMovedBlock();
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags += 254;
  Settlements_SinkPillar();
  Settlements_SlideCrateDownFalls();
}

void Settlements_SaveWallCells(void) {
  MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 0);
  MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 10);
}

void Settlements_RestoreWallCells(void) {
  MapGrid_CopyCellAttributes(64, 0, 10, 10, 11, 73);
  MapGrid_CopyCellAttributes(64, 10, 10, 10, 42, 73);
}

void Settlements_StepOntoWall(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_21) &&
      !Event_GetFlag(FLAG_SCENE_TEMP_20)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    MapGrid_CopyCellAttributes(42, 11, 10, 4, 11, 75);
    Event_SetFlag(FLAG_SCENE_TEMP_20);
  }
}

void Settlements_StepOffWall(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_20)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(FLAG_SCENE_TEMP_20);
  }
}

void Settlements_UnblockCrate(void) {
  struct Actor *object = Actor_Get(10);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  Settlements_SaveWallCells();
  Actor_LiftTargetBlock(gSettlements_WallCrate);
  if (x == 12 && z == 15) {
    MapGrid_CopyCellAttributes(42, 3, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 13 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 16 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else {
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  }
}

extern s32 gSettlements_CrateDropScript[];

#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    object->y -= px(2);                                                        \
    i--;                                                                       \
  } while (i > 0)

void Settlements_DropCrate(void) {
  struct Actor *object;
  s32 column;
  s32 row;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  EventCamera_FollowActor(10, 1);
  object = Actor_Get(10);
  column = object->x >> 20;
  row = object->z >> 20;
  if (column == 12 && row == 15) {
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    object->motionFlags = 0;
    SINK(32);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_05) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    SINK(48);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    SINK(32);
    Settlements_SpawnSplashRing(10);
    if (!Event_GetFlag(102)) {
      Actor_SetPosition(11, 0, 0);
      Actor_SetScript(10, gSettlements_CrateDropScript);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_04) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Event_End();
}

void Settlements_BlockCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_RestoreWallCells();
  Settlements_DropCrate();
}

extern u32 gSettlements_SmokeScript[];

void Settlements_SplashCrate(s32 id) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 68 bytes it reserves. */
  UNUSED struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct Actor *object = Actor_Get(id);

  spawn.kind = 0x115;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0xcccc;
  spawn.speed = 0x10000;
  spawn.acceleration = px(1);
  spawn.script = gSettlements_SmokeScript;
  Event_PlaySound(SE_CAST);
  FieldEffect_Spawn(object->x, object->y, object->z, 0, 0, 0, 0x1c0000, &spawn);
}

#include "../field/field_effect/advance_motion.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef SINK
// @file
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Settlements_SpawnSplashRing(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Trig_Cos(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    FieldEffect_Spawn(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                      &params);
  }
}

void Settlements_LiftBoulder(void) { GameFlag_Set(FLAG_SCENE_TEMP_08); }

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];
void Actor_SelectAnimation(struct Actor *object, s32 mode);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;

extern s16 gSlideOnIce_AnglesByKey[];
void Actor_SteerByPad(struct Actor *object);
void Leader_StopIfPending(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

#include "shared/slide_on_ice_from_edge.c"

void Settlements_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#include "text/msg_ids.h"

void Settlements_MeetGel(void);
void Settlements_MeetLostJupiterDjinni(void);
void Settlements_EnterDoor4(void);
void Settlements_GrowVine(void);
void Settlements_EnterCave(void);
void Settlements_PushIntoFalls(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gSettlements_FallsObjects[] = {
    9, 10, -1,
};

s16 gSettlements_WallCrate[] = {
    10, -1,
};

s16 gSlideOnIce_SteerAngles[] = {                                                               
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              
  };

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSettlements_BobScript[] = {
    39,
    3,
    22,
    26,
    0,
    23,
    6,
    -0x100000,
    46,
    (s32)Settlements_ClearSpriteFlags,
    17,
};

u32 gSettlements_SmokeScript[] = {
    39, 2, 17,
};

u32 gSettlements_CrateBobScript[] = {
    22, 26, 0, 17,
};

s32 gSettlements_CrateDropScript[] = {
    46,
    (s32)Actor_Bob,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 216, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 830, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSettlements_Room5Regions[] = {
    0x2e02a0, 0x2b00190, 0x1a0003e, 0xeffff, 0xffff, 0,
    0, 0,
};

u32 gSettlements_Room3Regions[] = {
    0xc00d4, 0xdc0084, 0x8c0014, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Settlements1),
    SCENE_EXIT_UNLESS(1, WorldMap, 48, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Settlements2, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Settlements3, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Settlements3, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements2),
    SCENE_EXIT_UNLESS(1, Settlements1, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements3),
    SCENE_EXIT_UNLESS(1, Settlements1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Settlements4, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements4),
    SCENE_EXIT_UNLESS(1, WorldMap, 49, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Settlements5, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Settlements5, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Settlements5, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Settlements5, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Settlements6, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements6),
    SCENE_EXIT_UNLESS(2, Settlements4, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements5),
    SCENE_EXIT_UNLESS(11, Settlements4, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, Settlements5, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, Settlements5, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, Settlements4, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements7),
    SCENE_EXIT_UNLESS(1, WorldMap, 50, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Settlements8, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements8),
    SCENE_EXIT_UNLESS(1, Settlements7, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements9),
    SCENE_EXIT_UNLESS(1, WorldMap, 47, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Settlements10, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(Settlements10),
    SCENE_EXIT_UNLESS(1, Settlements9, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSettlements_Placements[] = {
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 808, FACING_NORTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room1Placements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 192, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room2Placements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 632, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room4Placements[] = {
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, 7, 56, 0, 240, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, (s32)gSettlements_CrateBobScript, 280, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(245, 0x66, MOVEMENT_TYPE_STAND, 312, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room5Placements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 696, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room9Placements[] = {
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(288, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 72, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(329, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(246, 0x7f, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room10Placements[] = {
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 816, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room7Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(244, 0x52, MOVEMENT_TYPE_STAND, 312, 0, 296, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 88, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_Room8Placements[] = {
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSettlements_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gSettlements_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS,
     EVENT(Settlements_EnterDoor3)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 4, CONDITION_ALWAYS,
     EVENT(Settlements_EnterDoor4)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Settlements_WarnOfRiddleOfRock)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements06)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Settlements_CutLeftBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Settlements_CutStairsBush)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room2Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements05)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Settlements_EnterCave)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS,
     EVENT(Settlements_EnterDoor3)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 4, CONDITION_ALWAYS,
     EVENT(Settlements_EnterDoor4)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 5, CONDITION_ALWAYS,
     EVENT(Settlements_EnterDoor5)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 34, 0x220, EVENT(Settlements_StepOntoWall)},
    {TRIGGER_STEP, 35, 0x1220, EVENT(Settlements_StepOffWall)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements10)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Settlements_MeetTinder)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS,
     EVENT(Settlements_BlockCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 9, 0x230, EVENT(Settlements_GrowVine)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room5Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements08)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements12)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room7Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Settlements_EnterCave)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Settlements_StepOntoIce)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Settlements_StepOffIce)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Settlements_StepOnIceSlide)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x203, EVENT(Settlements_LashRope)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Settlements_MeetGel)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xf5d,
     EVENT(Settlements_CatchApple)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements13)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSettlements16)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgSettlements21))},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room9Triggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements18)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS,
     EVENT(Settlements_MeetLostJupiterDjinni)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements20)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x204, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x204, EVENT(Settlements_MovePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x205, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x205, EVENT(Settlements_MoveCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockFallsObject)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Settlements_PushIntoFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 22, 0x208,
     EVENT(Settlements_LiftBoulder)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_Room10Triggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSettlements19)},
    {TRIGGER_TABLE_END},
};

s32 gSettlements_WaterColorFrame = -1;
