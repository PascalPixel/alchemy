/// @file
/// Gaia Rock's outside, near Izumo, in the rooms of this overlay: pillars
/// Moved across gaps, slides and climbing walls, vines that swing in the wind
/// of the statues (HangingVine_Place), and the altar where Reveal shows the
/// Dancing Idol the party carries inside (gaia_rock_inside.c); after the
/// Serpent, Susa rejoices here that Kushinada is safe.

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"
#include "version.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "call.h"
#include "field/field_sequence_event.h"
#include "system/iwram_call.h"
#include "hanging_vine.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "map.h"
#include "system/task.h"
#include "actor_view.h"
#include "module/field/event_commands.h"
#include "objcmd.h"
#include "text/msg_ids.h"

/// The two pillars Moved into the gaps: the first dropped in at the west end,
/// the second at (79, 12).
#define FLAG_FIRST_PILLAR_DROPPED 0x920
#define FLAG_SECOND_PILLAR_DROPPED 0x921

struct Actor;

extern s16 gGaiaRock_Room1Regions[];
extern u32 gGaiaRock_Room2Regions[];

void GaiaRock_StatueRisesUnderLeader(s32 id, s32 flag);

extern struct ActorPlacement gGaiaRock_Placements[];
extern struct ActorPlacement gGaiaRock_Room1Placements[];

extern struct ActorPlacement gGaiaRock_Room2Placements[];
extern s16 gGaiaRock_Room3Placements[];

void Scene_TalkToActor(s32 a);

extern struct Trigger gGaiaRock_Room1Triggers[];
extern struct Trigger gGaiaRock_Room2Triggers[];
extern struct Trigger gGaiaRock_Triggers[];
extern u32 gGaiaRock_LeafScript[];

void Actor_SelectAnimation(struct Actor *object, s32 a);

extern s32 gGaiaRock_WindScript[];

void GaiaRock_StartWallTriggers(void);
void GaiaRock_PlaceStatue(s32 id, s32 x, s32 y, s32 z);

extern s32 gGaiaRock_StatueRiseScript[];

extern u32 gGaiaRock_DustScript[];
extern u32 gGaiaRock_CrateScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct ActorPlacement *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock1)
    return (const struct ActorPlacement *)gGaiaRock_Room1Regions;
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock2)
    return (const struct ActorPlacement *)gGaiaRock_Room2Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct Trigger *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock1)
    return (const struct Trigger *)gGaiaRock_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock2)
    return (const struct Trigger *)gGaiaRock_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock3)
    return (const struct Trigger *)gGaiaRock_Room3Placements;
  return (const struct Trigger *)gGaiaRock_Placements;
}

void GaiaRock_StepOnFirstStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(17, 0x300);
}

void GaiaRock_StepOnSecondStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(18, 0x301);
}

void GaiaRock_StepOnThirdStatueSpot(void) {
  GaiaRock_StatueRisesUnderLeader(19, 0x302);
}

// Sets the object's motion flags to 3 while it moves, for at most 60 frames
// until its vertical speed is 0, then clears them ten frames later. The
// editions after the Japanese one also run the given cue first.
#if !TLA_PATCH_1
void GaiaRock_DropPillar(s32 id) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the counter fixed to r6. */
  register s32 frames asm("r6");

  object->motionFlags = 3;
  for (frames = 60; frames != 0; frames--) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
#else
void GaiaRock_DropPillar(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address fixed to r2. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter fixed to r6. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): an empty asm on flags. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): a copy of the flags' address. */
  held = flags;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  *held = 0;
}
#endif

void GaiaRock_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(7, 90, 1, 1, 8, 91);
}

// The Japanese edition drops an object with no cue; the later editions play
// cue 181 as it drops. The first drop goes through the call layer.
#if !TLA_PATCH_1
#define DROP_OBJECT_CALLED(id) GaiaRock_DropPillar(id)
#define DROP_OBJECT(id) GaiaRock_DropPillar(id)
#else
#define DROP_OBJECT_CALLED(id) GaiaRock_DropPillar(id, 181)
#define DROP_OBJECT(id) GaiaRock_DropPillar(id, 181)
#endif

/// A pillar set down after a Move: the first drops into its gap once Moved far
/// enough west, the second bridges its gap as it passes over it to (79, 12).
void GaiaRock_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  struct Actor *p;

  MapGrid_CopyCellAttributes(8, 90, 1, 1, 8, 91);
  if (id == 16 && Event_GetFlag(FLAG_FIRST_PILLAR_DROPPED) == 0 &&
      (object->x >> 19) <= 11) {
    DROP_OBJECT_CALLED(16);
    Event_SetFlag(FLAG_FIRST_PILLAR_DROPPED);
    MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
  }
  if (id == 19 && Event_GetFlag(FLAG_SECOND_PILLAR_DROPPED) == 0) {
    if ((object->x >> 19) == 77)
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
    /* FAKEMATCH(temporary): the object read again through p for the last two
     * tests. */
    p = object;
    if ((p->x >> 19) == 79 && (p->y >> 19) == 12) {
      DROP_OBJECT(19);
      MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
    }
    if ((p->x >> 19) == 81) {
      Event_SetFlag(FLAG_SECOND_PILLAR_DROPPED);
      {
        s32 row = 46;

        MapGrid_CopyCellAttributes(41, row, 1, 1, 39, 46);
      }
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    }
  }
}

void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

void GaiaRock_StartPushingBlock(void) {
  MapGrid_CopyCellAttributes(13, 18, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void GaiaRock_FinishPushingBlock(void) {
  MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void GaiaRock_CycloneClearsWall(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 110, 64, 14, 12, 110, 47, 1);
}

void GaiaRock_RevealIdol(void) {
  struct Actor *object = Actor_Get(64);
  s32 done;

  if (object == NULL)
    return;
  done = Event_GetFlag(0xf74);
  if (done == 0) {
    Actor_SetPosition(64, 657, 246);
    object->motionFlags = done;
    object->y = px(80);
  }
}

void GaiaRock_HideIdol(void) {
  if (Actor_Get(64) != NULL)
    Actor_SetPosition(64, 0, 0);
}

void GaiaRock_CheckAltar(void) {
  struct Actor *object = Actor_Get(64);

  Message_ShowCentered(MsgCheckedTheAltar, 1);
  if (object != NULL && object->x != 0) {
    Scene_TalkToActor(4160);
  } else if (Event_GetFlag(0xf74) == 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Message_ShowCentered(MsgIMSensing, 1);
    work->fieldMenu = 1;
  } else {
    Message_ShowCentered(MsgICanT, 1);
  }
}

void GaiaRock_SusaRejoices(void) {
  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId(MsgYouReGoing);
    FieldActor_StartRepeatedMotion(20, 1);
    FieldEvent_Wait(15);
    Event_AskYesNo(20, 0);
    Event_SetFlag(0x9ab);
    Event_End();
  }
}

void GaiaRock_TalkToSusa(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgYouReGoing);
    Event_AskYesNo(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIfYouRe);
    Event_ShowMessage(20, 0);
  }
}

void GaiaRock_MindReadSusa(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgKushinadaSSafe);
    Event_ShowMessage(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIReallyGot);
    Event_ShowMessage(20, 0);
  }
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock1)
    return gGaiaRock_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock2)
    return gGaiaRock_Room2Triggers;
  return gGaiaRock_Triggers;
}

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 priorityFlags;
  u8 unk24[4];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

s32 Trig_Cos(s32 angle);

void GaiaRock_CircleLeaf(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Math_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 71)
    Actor_StartScript((struct Actor *)effect, gGaiaRock_LeafScript);
}

s32 GaiaRock_SpawnCirclingLeaves(struct OrbitEffect *source) {
  s32 i;
  s32 timer;

  i = 0;
  timer = 20;
loop:
  {
    struct OrbitEffect *effect;
    s32 radius;
    s32 position[3];

    position[0] = source->x;
    position[1] = source->y + i * 0xcccc + 0x40000;
    position[2] = source->z;
    radius = Random16();
    FieldVector_AddPolarOffset(radius * 5 + 0x30000, Random16(), position);
    effect = (struct OrbitEffect *)Value4(
        (s32 (*)())Actor_Spawn, 308, position[0], position[1], position[2]);
    if (effect != NULL) {
      effect->update = GaiaRock_CircleLeaf;
      effect->timer = timer;
      effect->pause = 0;
      effect->speed = 0x1999;
      effect->scale = 0x30000;
      effect->angle = Random16();
      effect->priorityFlags = 0;
      Actor_SetSpritePriorityDirect((struct Actor *)effect, 2);
    }
  }
  timer -= 2;
  i++;
  if (i < 16)
    goto loop;
  return 0;
}

struct A4SlotValues {
  s16 id;
  s16 unk02;
  s32 value04;
  u8 unk08[8];
  s32 value10;
  u8 unk14[8];
};

static inline struct A4SlotValues *FindSlot(s32 id) {
  struct HangingVineWork *work = HANGING_VINE_WORK;
  struct A4SlotValues *found = NULL;
  s32 i;

  // Default: as HangingVine_FindSlot, the index zero is copied from found.
  i = 0;

  for (i = 0; i < 8; i++) {
    if (work->slots[i].id == id) {
      found = (struct A4SlotValues *)&work->slots[i];
      break;
    }
  }
  return found;
}

void GaiaRock_StartVineSwing(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 = 64;
}

void GaiaRock_SwingVineHarder(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 += 128;
}

void Event_PlayCueNearFocus(struct Actor *object, s32 a);

void GaiaRock_StatueBlowsWind(void) {
  struct Actor *object = Actor_Get(17);
  u8 phase = gFrameCount;

  if (phase == 0) {
    Event_PlayCueNearFocus(object, 202);
    object = Actor_Spawn(0x118, object->x, object->y, object->z);
    if (object != NULL) {
      object->acceleration = px(2);
      object->speedLimit = px(2);
      object->scaleX = Q16_ONE / 2;
      object->scaleY = 0x1999;
      object->priorityFlags = phase;
      Actor_SelectAnimation(object, 2);
      FieldActor_SetMoveTargetFixed(object, object->x - px(32), object->y,
                                    object->z);
      Actor_StartScript(object, gGaiaRock_WindScript);
      Actor_SetSpritePriorityDirect(object, 2);
    }
  }
}

#if !TLA_PATCH_1
#define SCREEN_EFFECT 0x201
#else
#define SCREEN_EFFECT 0x204
#endif

s32 HangingVine_Place();

s32 Scene_Initialize(void) {
  struct Actor *actor;
  struct FieldWork *work;

  gFieldWork->transition = SCREEN_EFFECT;
  gMainRoutineWork.retreatScene = (s32)&SceneId_GaiaRock1;
  gMainRoutineWork.retreatEntrance = 5;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock1) {
    Task_Add((s32)GaiaRock_StatueBlowsWind, 0x480);
    Call2((void (*)())HangingVine_Place, 8, 1);
    Call2((void (*)())HangingVine_Place, 9, 1);
    Call2((void (*)())HangingVine_Place, 10, 1);
    Call2((void (*)())HangingVine_Place, 11, 1);
    Call2((void (*)())HangingVine_Place, 12, 1);
    Call2((void (*)())HangingVine_Place, 13, 1);
    Call2((void (*)())HangingVine_Place, 14, 1);
    HangingVine_Place(15, 1);
    if (GameFlag_Get(FLAG_FIRST_PILLAR_DROPPED)) {
      actor = Actor_GetById(16);
      actor->motionFlags = 0;
      actor->x = px(88);
      actor->y = px(128);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
    }
    actor = Actor_GetById(19);
    actor->priorityFlags = 0;
    if (GameFlag_Get(FLAG_SECOND_PILLAR_DROPPED)) {
      actor->motionFlags = 0;
      actor->x = px(648);
      actor->y = px(64);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if ((actor->x >> 19) != 75) {
        MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      }
      if ((actor->x >> 19) == 79) {
        actor->motionFlags = 0;
        MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Get(FLAG_SERPENT_SLAIN) && !GameFlag_Get(0x9ab)) {
      Actor_SetPosition(20, 798, 800);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 49, 49);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 50, 49);
      Actor_SetModeById(20, 5);
      if (!Event_GetFlag(0xa61)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        FieldActor_StartRepeatedMotion(20, 3);
        FieldCamera_MoveToActorById(20, 1);
        FieldEvent_Wait(30);
        FieldEvent_SetMessageId((s32)MsgIMNot);
        Event_ShowMessage(20, 0);
        FieldActor_StartRepeatedMotion(20, 2);
        FieldEvent_Wait(30);
        Event_SetFlag(0xa61);
        Event_End();
      }
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock2) {
    Call2((void (*)())HangingVine_Place, 8, 1);
    Call2((void (*)())HangingVine_Place, 9, 1);
    Call2((void (*)())HangingVine_Place, 10, 1);
    Call2((void (*)())HangingVine_Place, 11, 1);
    Call2((void (*)())HangingVine_Place, 12, 1);
    Call2((void (*)())HangingVine_Place, 13, 1);
    HangingVine_Place(14, 1);
    if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
      GaiaRock_StartWallTriggers();
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_00)) {
      GaiaRock_PlaceStatue(17, 0x2fc0000, -0x1870000, 0x1a80000);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_01)) {
      GaiaRock_PlaceStatue(18, 0x3150000, -0xf80000, 0x1a80000);
    }
    if (GameFlag_Get(FLAG_AREA_TEMP_02)) {
      GaiaRock_PlaceStatue(19, 0x30a0000, -0x770000, 0x1a80000);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock3) {
    actor = Actor_GetById(64);
    if (actor != 0) {
      Actor_SetPosition(64, 0, 0);
    }
    work = Ram_HeapSlots->fieldWork;
    actor = work->actors[16];
    work->actors[64] = actor;
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, actor->z >> 20);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GaiaRock2 &&
      (u16)(gMainRoutineWork.entrance - 3) <= 1) {
    s32 *layer = (s32 *)&Ram_HeapSlots->mapWork->layers[1];

    layer[2] = -0x800000;
    layer[3] = 0x4480000;
  }
  return 0;
}

void GaiaRock_TriggerWallCell(void) {
  if (gMainRoutineWork.renderMode == 2) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      work->stepTrigger = code;
  }
}

void GaiaRock_PlaceStatue(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 cellX = x / 0x100000;
  s32 cellZ = (z - y + 0x7fff) / 0x100000;

  cell += cellX + (cellZ << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (cellZ << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSpriteFlags(object, 0);
}

void GaiaRock_StatueRisesUnderLeader(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  pillar = Actor_Get(id);
  GaiaRock_PlaceStatue(id, leader->x, leader->y, leader->z);
  Actor_StartScript(pillar, gGaiaRock_StatueRiseScript);
  i = 15;
dust:
  {
    struct Actor *dust =
        FieldActor_Create(255, leader->x, leader->y + px(16), leader->z);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Actor_StartScript(dust, gGaiaRock_DustScript);
      angle = Random16();
      speed = Random16();
      size = Random16();
      scale = (size >> 1) + 0x2000;
      dust->velocityY = Iwram_MulQ16(speed * 8, Trig_Cos(angle));
      dust->velocityX = Iwram_MulQ16(speed * 8, Math_Sin(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = 0x547a;
      dust->bounce = Q16_ONE / 2;
    }
  }
  if (--i >= 0)
    goto dust;
  Event_PlaySound(SE_590);
  Event_PlaySound(SE_FLOOR_SWITCH);
  MapGrid_SetShake(2, 2, 0.9);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = 3;
  id = 0;
  leader->gravity = 0x547a;
  goto fall;
next:
  id++;
fall:
  if (id <= 179) {
    System_Wait(1);
    if (leader->y > leader->terrainHeight)
      goto next;
  }
  Event_PlaySound(SE_THUMP);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_REMOVE);
  Actor_SelectAnimation(leader, 38);
  MapGrid_SetShake(1, 4, 1);
  System_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  System_Wait(20);
  FieldCamera_MoveToFixed(leader->x, leader->y, leader->z, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SelectAnimation(leader, 1);
  leader->velocityY = px(4);
  System_Wait(20);
  gMainRoutineWork.renderMode = 0;
  Event_End();
  Event_SetFlag(flag);
}

void GaiaRock_StartWallTriggers(void) {
  Task_AddFrame(GaiaRock_TriggerWallCell, TASK_PASS_VBLANK);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gGaiaRock_Room1Regions[] = {
    771, -28, 484, 779, -20, 492, -1, 2,
    771, 180, 707, 779, 188, 715, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gGaiaRock_Room2Regions[] = {
    0x12800c3, 0xcb0143, 0x14b0130, 0x2ffff, 0xfdd602c3, 0x2cb01c4,
    0x1ccfdde, 0x3ffff, 0xffff, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GaiaRock1),
    SCENE_EXIT_UNLESS(1, GaiaRock2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GaiaRock1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GaiaRock1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GaiaRockInside2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, WorldMap, 39, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRock2),
    SCENE_EXIT_UNLESS(1, GaiaRock1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GaiaRock2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GaiaRock2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GaiaRock3, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRock3),
    SCENE_EXIT_UNLESS(1, GaiaRock2, 4, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gGaiaRock_CrateScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gGaiaRock_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_Room1Placements[] = {
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1000, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 936, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gGaiaRock_CrateScript, 120, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 295, 0, 442, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(280, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, (s32)gGaiaRock_CrateScript, 600, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(29, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_Room2Placements[] = {
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s16 gGaiaRock_Room3Placements[] = {
    272, -1, 1, 0, 0, 216, 0, 0,
    0, 232, 0x4000, 2, 272, -1, 1, 0,
    0, 680, 0, 0, 0, 216, 0x4000, 258,
    272, -1, 1, 0, 0, 712, 0, 0,
    0, 248, 0x4000, 258, 272, -1, 1, 0,
    0, 680, 0, 0, 0, 280, 0x4000, 258,
    272, -1, 1, 0, 0, 632, 0, 0,
    0, 280, 0x4000, 258, 272, -1, 1, 0,
    0, 600, 0, 0, 0, 248, 0x4000, 258,
    272, -1, 1, 0, 0, 632, 0, 0,
    0, 216, 0x4000, 258, 385, -1, 1, 0,
    0, 656, 0, 0, 0, 248, 0x4000, 2,
    397, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gGaiaRock_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 42, 0x9ab, EVENT(GaiaRock_SusaRejoices)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(GaiaRock_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT(GaiaRock_MindReadSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(GaiaRock_CycloneClearsWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, FLAG_SECOND_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, FLAG_AREA_TEMP_00, EVENT(GaiaRock_StepOnFirstStatueSpot)},
    {TRIGGER_STEP, 21, FLAG_AREA_TEMP_01, EVENT(GaiaRock_StepOnSecondStatueSpot)},
    {TRIGGER_STEP, 22, FLAG_AREA_TEMP_02, EVENT(GaiaRock_StepOnThirdStatueSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(GaiaRock_CheckAltar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingBlock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(GaiaRock_RevealIdol)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRock_HideIdol)},
    {TRIGGER_TABLE_END},
};

u32 gGaiaRock_LeafScript[] = {
    38,
};

s32 gGaiaRock_WindScript[] = {
    SC_ADD(SW_SCALE_X, 0x1999),
    SC_ADD(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LOOP(12, SC_TOP),
    SC_SET(SW_SCALE_X, 0x1b333),
    SC_SET(SW_SCALE_Y, 0x14ccc),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(GaiaRock_StartVineSwing),
    SC_WAIT(2),
    SC_LABEL(1),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(2),
    SC_LOOP(3, SC_TO(1)),
    SC_CALL(GaiaRock_SpawnCirclingLeaves),
    SC_LABEL(2),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(2),
    SC_LOOP(14, SC_TO(2)),
    SC_CALL(GaiaRock_SwingVineHarder),
    SC_WAIT(10),
    SC_FREE,
};

u32 gGaiaRock_DustScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gGaiaRock_StatueRiseScript[] = {                                                               
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_WAIT(20),                                                             
      SC_LOOP(3, SC_TOP),                                                      
      SC_END,                                                                  
  };
// clang-format on
