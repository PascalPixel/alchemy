/// @file
/// Four small places the ship reaches, sharing one overlay: the Gondowan
/// Settlement (rooms 1 to 3), whose bush Cyclone cuts to a stair; the Hesperia
/// Settlement (4 to 6), with its vine, crate and Tinder; Kalt Island (7 and 8),
/// with its apple, frozen river and Gel; and SW Atteka Islet (9 and 10), whose
/// pillar and crate go over the falls, and its boulder and Jupiter Djinni.
/// ★ Named from their area names: "Gondowan Settlement", "Hesperia
/// Settlement", "Kalt Island", "SW Atteka Islet".

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#undef EFFECT_SPAWN

// ---- scene/settlements/s0280.c ----

/// The settlements' events, continued; settlements.c says which places these
/// are.

#include "shared/actors.h"
#include "shared/effects.h"
#include "settlements.h"
#include "field_event.h"
#include "field.h"
#include "module/scene/settlements.h"
#include "palette_snapshots.h"

void Settlements_MeetGel(void) { Djinn_JoinParty(10, 1, 14); }

void Settlements_MeetTinder(void) { Djinn_JoinParty(11, 2, 14); }

void Settlements_MeetLostJupiterDjinni(void) { Djinn_JoinParty(23, 3, 19); }

s32 Settlements_ClearSpriteFlags(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

DEFINE_ACTOR_BOB(Actor_Bob)

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u32 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;
  u32 *placements;

  if (scene == (s32)&SceneId_Ov690Room5)
    return gSettlements_Room5Regions;
  placements = 0;
  if (scene == (s32)&SceneId_Ov690Room3)
    placements = gSettlements_Room3Regions;
  return placements;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"
#include "io_reg.h"
#include "version.h"

extern u8 gSettlements_Placements[];
extern u8 gSettlements_Room1Placements[];
extern u8 gSettlements_Room2Placements[];
extern u8 gSettlements_Room4Placements[];
extern u8 gSettlements_Room5Placements[];
extern u8 gSettlements_Room9Placements[];
extern u8 gSettlements_Room10Placements[];
extern u8 gSettlements_Room7Placements[];
extern u8 gSettlements_Room8Placements[];

u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gSettlements_Room1Placements;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gSettlements_Room2Placements;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gSettlements_Room4Placements;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gSettlements_Room5Placements;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gSettlements_Room7Placements;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gSettlements_Room8Placements;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gSettlements_Room9Placements;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gSettlements_Room10Placements;
  return gSettlements_Placements;
}

void Settlements_CatchApple(void) {
  s32 mode;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0xf5d);
  ItemDisplay_ShowItem(193, 3);
  mode = 0;
  Party_GiveItem(193, mode);
  Event_End();
}

/// ★ Named from the game: "Don't tell me, you're here to solve the riddle of
/// the rock!" (MsgSettlements01)
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
  s32 next = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

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
  Event_PlaySound(123);
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
  Event_SetFlag(0x230);
}

extern u8 gSettlements_Triggers[];
extern u8 gSettlements_Room1Triggers[];
extern u8 gSettlements_Room2Triggers[];
extern u8 gSettlements_Room4Triggers[];
extern u8 gSettlements_Room5Triggers[];
extern u8 gSettlements_Room7Triggers[];
extern u8 gSettlements_Room8Triggers[];
extern u8 gSettlements_Room9Triggers[];
extern u8 gSettlements_Room10Triggers[];

u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gSettlements_Room1Triggers;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gSettlements_Room2Triggers;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gSettlements_Room4Triggers;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gSettlements_Room5Triggers;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gSettlements_Room7Triggers;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gSettlements_Room8Triggers;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gSettlements_Room9Triggers;
  if (scene == (s32)&SceneId_Ov690Room10)
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
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
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
  if (Event_GetFlag(0x109) == 0)
    actor->y += 0x100000;
  actor->flags23 &= 0xfe;
  actor->visible = 1;
  transfer = HeapSlot_Alloc(68, 0x608);
  Item_LoadIcon(193);
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

extern const u8 gSettlements_WallCrate[];

void Settlements_StartWaterfall(void);

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room7)
    Settlements_SetUpKaltIsland();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    FieldEffect_StartBlankOverlay(0);
    Actor_PlaceBlocks(gSettlements_FallsObjects);
    if (Event_GetFlag(0x206))
      Actor_SetPosition(10, 0, 0);
    Actor_GetById(10)->priorityFlags |= 2;
    Actor_SetAnimation(18, 2);
    Actor_SetAnimation(19, 2);
    Actor_SetAnimation(21, 2);
    if (Event_GetFlag(0x205))
      Actor_SetAnimation(10, 2);
    Settlements_StartWaterfall();
#if TLA_PATCH_1
    if (Event_GetFlag(0x204)) {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4) {
    if (Event_GetFlag(0x230)) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
      Actor_SetAnimation(9, 4);
    }
    if (Event_GetFlag(0x109) && Event_GetFlag(0x220)) {
      Event_ClearFlag(0x220);
      Settlements_StepOntoWall();
    }
    Actor_PlaceBlocks(gSettlements_WallCrate);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room3)
    FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room6)
    FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    if (Event_GetFlag(0x205)) {
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (Event_GetFlag(0x205))
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
#else
    if (Event_GetFlag(0x204)) {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room1) {
    if (Event_GetFlag(0xa60)) {
      MapGrid_CopyCellsTo(5, 46, 5, 47, 1, 1);
      MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
      MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}

#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"
#include "map.h"

void Settlements_StepOntoIce(void) { GameFlag_Set(0x201); }

void Settlements_StepOffIce(void) { GameFlag_Clear(0x201); }

void Settlements_StepOnIceSlide(void) { Settlements_SlideOnIce(); }

void Settlements_EnterCave(void) {
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop carries no loop notes: the bytes rebuild the zero on every
  // pass, and a for loop hoists it.
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
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4)
    MapGrid_CopyCellsTo(33, 30, 23, 14, 1, 2);
  else
    MapGrid_CopyCellsTo(27, 29, 40, 16, 1, 2);
  Event_PlaySound(158);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Settlements_LashRope(void) {
  Rope_Lash(1, 8, 9);
  Event_SetFlag(0x203);
}

extern s32 gSettlements_WaterColorFrame;

void Settlements_CycleWaterColors(void) {
  struct PaletteCycleWork *work =
      (struct PaletteCycleWork *)Ram_HeapSlots->eventWork;
  s32 *frame;
  s32 phase, i;

  if (work->unk166 != 0 || work->unkca4 != 0)
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
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 514;
  FieldTask_AddFrame(Settlements_CycleWaterColors, 0x480);
  Scene_QueueMusic(170);
  Scroll_StartHBlank();
}

// The hblank split: above the line Scroll_StartHBlank sets, BG1 scrolls by one
// value and blending is off; below it, by the other, with BG1 blended.
#include "system/io_write.h"

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

DEFINE_LAYER_TRACK_SCROLL(Layer_TrackScroll, gSettlements_WaterfallLine,
                          gSettlements_WaterfallScrollAbove,
                          gSettlements_WaterfallScrollBelow)

DEFINE_SCROLL_START_HBLANK(Scroll_StartHBlank, Scroll_HBlank, Layer_TrackScroll)

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

  if (!Event_GetFlag(0x204) && column == 11 && row == 12) {
    object->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      object->y -= 0x20000;
      Event_Wait(1);
    }
    Settlements_SplashCrate(9);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(21, PIXELS(184), PIXELS(184));
    Actor_GetById(21)->motionFlags = 0;
    Actor_GetById(21)->y -= 0x400000;
    MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    Event_SetFlag(0x204);
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
    // A goto loop carries no loop notes: as a for loop its two uses of the
    // object count double and the object takes the row's register.
    i = 8;
  sink:
    FieldEvent_Wait(1);
    object->y -= 0x20000;
    i--;
    if (i > 0)
      goto sink;
    Settlements_SplashCrate(10);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(0xc00000, -1, 0xe00000, 1);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldActor_WaitForMoveById(10);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
    Actor_SetSpeed(10, 0x26666, 0x13333);
    Actor_SetDestinationOffset(10, 0, 80);
    FieldActor_WaitForMoveById(10);
    Settlements_SplashCrate(10);
    if (!Event_GetFlag(0x204)) {
      FieldEvent_Wait(10);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldActor_WaitForMoveById(10);
      Actor_SetSpeed(10, 0x28000, 0x14000);
      Actor_SetDestinationOffset(10, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(10, 0, 0);
      Event_SetFlag(0x206);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(10, 0, 12);
      FieldActor_WaitForMoveById(10);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(10, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(0x205);
    }
    Event_End();
  } else if (!Event_GetFlag(0x207) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Event_SetFlag(0x207);
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
  if (!Event_GetFlag(0x221) && !Event_GetFlag(0x220)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    MapGrid_CopyCellAttributes(42, 11, 10, 4, 11, 75);
    Event_SetFlag(0x220);
  }
}

void Settlements_StepOffWall(void) {
  if (Event_GetFlag(0x220)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(0x220);
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

extern const u8 gSettlements_CrateDropScript[];

#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    object->y -= 0x20000;                                                      \
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
    Event_SetFlag(0x221);
    object->motionFlags = 0;
    SINK(32);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(0x205) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Event_SetFlag(0x205);
    Event_SetFlag(0x221);
    SINK(48);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    Event_SetFlag(0x221);
    SINK(32);
    Settlements_SpawnSplashRing(10);
    if (!Event_GetFlag(102)) {
      Actor_SetPosition(11, 0, 0);
      Actor_SetScript(10, gSettlements_CrateDropScript);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(0x204) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
    Event_SetFlag(0x204);
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

extern const u8 gSettlements_SmokeScript[];

/* VIEW(s0280): routine FieldEffect_Spawn as void FieldEffect_Spawn(s32 x, s32
 * y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags, struct SettlementsSpawn *spawn);
 * field/field_effect/spawn has void EFFECT_SPAWN(s32 x, s32 y, s32 z, s32
 * velocityX, s32 velocityY, s32 velocityZ, u32 flags, const struct
 * EffectOptions *options) {. */
void FieldEffect_Spawn__s0280(
    s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
    struct SettlementsSpawn *spawn) __asm__("FieldEffect_Spawn");

void Settlements_SplashCrate(s32 id) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 68 bytes it reserves (sub sp, #68), no instruction
   * addresses the 12 at sp+56 to sp+67, where Settlements_SpawnSplashRing keeps
   * its velocity; as without it, 4 of 41 instructions differ in tla-en. */
  UNUSED struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct Actor *object = Actor_Get(id);

  spawn.kind = 0x115;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0xcccc;
  spawn.speed = 0x10000;
  spawn.acceleration = 0x10000;
  spawn.script = gSettlements_SmokeScript;
  Event_PlaySound(154);
  FieldEffect_Spawn__s0280(object->x, object->y, object->z, 0, 0, 0, 0x1c0000,
                           &spawn);
}
