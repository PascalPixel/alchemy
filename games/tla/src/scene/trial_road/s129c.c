/// @file
/// Trial Road's effect spawner and the sand stage's stream.

#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 a);
/* VIEW(s129c): routine Actor_StartScript as void Actor_StartScript(struct
 * FieldEffect *effect, const s32 *script); scene/shared/trial_road.h has void
 * Actor_StartScript(struct Actor *object, const void *script). */
void Actor_StartScript__s129c(struct FieldEffect *effect,
                              const s32 *script) __asm__("Actor_StartScript");
/* VIEW(s129c): routine Actor_SetSpriteFlags as void Actor_SetSpriteFlags(struct
 * FieldEffect *effect, s32 flags); field/field.h has void
 * Actor_SetSpriteFlags(struct Actor *object, u32 flags). */
void Actor_SetSpriteFlags__s129c(struct FieldEffect *effect,
                                 s32 flags) __asm__("Actor_SetSpriteFlags");

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript__s129c(
      effect, gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags__s129c(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
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
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript__s129c(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "../../field/field_effect/advance_motion_priority.c"

#include "system.h"
#include "actor.h"
#include "frame.h"

void TrialRoad_DriftWithStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 velocity[3];

  if (gMainRoutineWork.renderMode == 4)
    return;
  leader->z += 0x10000;
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  velocity[1] = 0;
  velocity[2] = 0x10000;
  velocity[0] = 0;
  velocity[0] += ((Random16() << 17) >> 16) - 0x10000;
  velocity[2] += ((Random16() << 16) >> 16) - 0x8000;
  FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x1000001, &options);
}

// ---- scene/trial_road/s15a8.c ----

/// Trial Road's rooms set up, the rival Moapa's race, the peak, the
/// fountains and crumbling pillars, the race's progress display and the tables.

#include "../shared/map_cells.h"
#include "../shared/effects.h"
#include "../shared/trial_road.h"
#include "event_inline.h"
#include "event_work.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/scene/trial_road.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "resource.h"
#include "serial.h"
#include "task.h"
#include "heap_slots.h"

void TrialRoad_LiftCaveRock(void) { GameFlag_Set(0x289); }

void TrialRoad_CutBushes(void) { Func_02005a50_(68, 6, 1, 21, 13, 64, 64); }

void TrialRoad_CutMadPlantBushes(void) {
  Func_02005a50_(68, 6, 1, 21, 13, 64, 64);
  if (!Event_GetFlag(0x9b9)) {
    Func_02005a60(249, 72, 152);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b9;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 4);
  }
}

s32 TrialRoad_ReturnTrue(void) { return 1; }

#define DEFINE_TRIAL_ROAD_SET_POOL_TILES(name, n1, n2)                         \
  void name(void) {                                                            \
    MapGrid_CopyCells(n1, 30, 1, 1, 10, 11);                                   \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 7);                                    \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 15);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 74, 11);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 7);                                    \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 15);                                   \
  }

DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_RevealPoolTiles, 33, 35)

DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_HidePoolTiles, 34, 36)

void TrialRoad_OpenSummitGate(s32 sound) {
  if (Event_GetFlag(0x9ff)) {
    if (gTrialRoadState->mirrored == 0) {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
      MapGrid_CopyCellAttributes(78, 18, 1, 1, 77, 18);
    } else {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
      MapGrid_CopyCellAttributes(81, 18, 1, 1, 82, 18);
    }
    if (sound)
      Event_PlaySound(138);
  }
  Event_SetFlag(0x283);
}

void TrialRoad_ChallengeMoapa(void) {
  struct MainRoutineWork *party;
  s32 count = Event_GetFlag(0x381);

  count += Event_GetFlag(0x382);
  count += Event_GetFlag(0x383);
  count += Event_GetFlag(0x384);
  if (Event_GetFlag(0x385))
    TrialRoad_ReachPeakFirst();
  else
    TrialRoad_ReachPeakSecond();
  party = &gMainRoutineWork;
  party->battleRule24b = 2;
  do {
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov697Room2, 4);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov697Room2, 5);
  Field_RequestBattleFromEncounterTable(13, 4 - count);
}

/// When the object stands in column 8, sets flag 0x9fc, stops its motion and
/// lifts it one pixel.
void TrialRoad_SetSummitPillar(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 8) {
    Event_SetFlag(0x9fc);
    object->motionFlags = 0;
    object->y -= 0x10000;
  }
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 gTrialRoad_Triggers[];
extern const u8 gTrialRoad_Room3Triggers[];
extern const u8 gTrialRoad_Room4Triggers[];
extern const u8 gTrialRoad_Room2Triggers[];
extern const u8 *const gTrialRoad_Room1Triggers[];

const u8 *Scene_GetEvents(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room1)
    return gTrialRoad_Room1Triggers[pattern & 7];
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room3)
    return gTrialRoad_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room4)
    return gTrialRoad_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov697Room2)
    return gTrialRoad_Room2Triggers;
  return gTrialRoad_Triggers;
}

void TrialRoad_SetUpPillarStage(void) {
  if (!Event_GetFlag(0x307))
    OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  else
    Actor_SetPosition(20, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
  Task_Add(TrialRoad_TrackLeaderOnPillars, 0x480);
}

void TrialRoad_SetUpTorchStage(void) {
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_AddPillarTop(20);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
  TrialRoad_BlockCellUnder(20);
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  if (!Event_GetFlag(0x214))
    Actor_SetAnimation(17, 5);
  else
    TrialRoad_SetPillarTopLift(17, -16);
  if (!Event_GetFlag(0x215))
    Actor_SetAnimation(18, 5);
  else
    TrialRoad_SetPillarTopLift(18, -16);
  if (!Event_GetFlag(0x216))
    Actor_SetAnimation(19, 5);
  else
    TrialRoad_SetPillarTopLift(19, -16);
  if (!Event_GetFlag(0x217))
    Actor_SetAnimation(20, 5);
  else
    TrialRoad_SetPillarTopLift(20, -16);
}

void TrialRoad_SetUpBushPillarStage(void) {
  if (!Event_GetFlag(0x327))
    OBJECT_BYTE(Actor_GetById(15), 98) = 1;
  else
    Actor_SetPosition(20, 0, 0);
  if (!Event_GetFlag(0x328))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetPosition(21, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  TrialRoad_BlockCellUnder(18);
  TrialRoad_BlockCellUnder(19);
}

void TrialRoad_SetUpPoundStage(void) {
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  OBJECT_BYTE(Actor_GetById(19), 98) = 1;
  OBJECT_BYTE(Actor_GetById(20), 98) = 1;
  OBJECT_BYTE(Actor_GetById(21), 98) = 1;
  if (!Event_GetFlag(0x339))
    OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  else
    Actor_SetPosition(22, 0, 0);
  TrialRoad_AddPillarTop(15);
  TrialRoad_AddPillarTop(16);
  TrialRoad_AddPillarTop(17);
  TrialRoad_AddPillarTop(18);
  TrialRoad_AddPillarTop(19);
  TrialRoad_AddPillarTop(20);
  TrialRoad_AddPillarTop(21);
  TrialRoad_BlockCellUnder(15);
  TrialRoad_BlockCellUnder(16);
  TrialRoad_BlockCellUnder(17);
  if (!Event_GetFlag(0x335)) {
    TrialRoad_RaisePillar(18);
    TrialRoad_LowerPillar(19);
  } else {
    TrialRoad_RaisePillar(19);
    TrialRoad_LowerPillar(18);
  }
  if (!Event_GetFlag(0x337)) {
    TrialRoad_RaisePillar(20);
    TrialRoad_LowerPillar(21);
  } else {
    TrialRoad_RaisePillar(21);
    TrialRoad_LowerPillar(20);
  }
}

void TrialRoad_SetUpCrumblingStage(void) {
  TrialRoad_RestoreCrumblingPillars(gTrialRoad_CrumblingPillars);
}

/// Copies two blocks of map cells, sets byte 98 of objects 16 to 18 and clears
/// their collision flags; then runs TrialRoad_BlockLogs, and
/// TrialRoad_AddPillarTop and TrialRoad_BlockCellUnder for object 15.
void TrialRoad_SetUpLogStage(void) {
  MapGrid_CopyCellAttributes(11, 22, 11, 8, 11, 64);
  MapGrid_CopyCellAttributes(79, 22, 11, 8, 79, 64);
  OBJECT_BYTE(Actor_GetById(16), 98) = 1;
  OBJECT_BYTE(Actor_GetById(17), 98) = 1;
  OBJECT_BYTE(Actor_GetById(18), 98) = 1;
  {
    /* FAKEMATCH(pin): the zero pinned to r6; as a plain local, which reuses r5
     * where the 1 was held, 12 of 52 instructions differ in tla-en. */
    register s32 clear asm("r6") = 0;

    Actor_GetById(16)->collisionFlags = clear;
    Actor_GetById(17)->collisionFlags = clear;
    Actor_GetById(18)->collisionFlags = clear;
  }
  TrialRoad_BlockLogs();
  TrialRoad_AddPillarTop(15);
  TrialRoad_BlockCellUnder(15);
}

/// One of the fountain stage's fountains and the spout drawn over it.
struct TrialRoadFountain {
  s16 id;     ///< its actor; 0 for an empty slot
  s16 phase;  ///< frames into its period
  s16 height; ///< how far it has risen, in pixels
  s16 period; ///< frames in one rise and fall
  s16 riseAt; ///< the phase it starts to rise at
  s16 fallAt; ///< the phase it starts to fall at
  s16 high;
  s16 low;
  s16 unk10;
  s16 speed; ///< pixels a frame, 4 rising and -8 falling
  s16 unk14;
  s16 bob;             ///< the spout's sine phase
  struct Actor *spout; ///< drawn over it
};

/// The fountain stage's work, kept in the inventory snapshot's spare room.
struct TrialRoadFountains {
  s16 vramEntry;
  s16 shape; ///< the first of the shape grid's values for the fountains
  struct TrialRoadFountain fountains[16];
  u32 *sprites;
};

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high);

/// Sets up the Trial Road room with five movers: objects 16 to 20 in slots 0
/// to 4, the first three over a range of 64 and the last two over 32, each
/// with 240, 120 and 150; objects 16 to 18 are put away
/// (TrialRoad_OpenSpoutCell) once flags 0x363 to 0x365 are set and placed
/// otherwise; then object 15.
void TrialRoad_SetUpFountainStage(void) {
  struct TrialRoadFountain *fountain;

  TrialRoad_StartFountains(0, 240);
  fountain = TrialRoad_AddFountain(0, 16, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(1, 17, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(2, 18, 0, 0, 64);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(3, 19, 0, 0, 32);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  fountain = TrialRoad_AddFountain(4, 20, 0, 0, 32);
  fountain->period = 240;
  fountain->riseAt = 120;
  fountain->fallAt = 150;
  if (!Event_GetFlag(0x363))
    Actor_SetPosition(16, 0, 0);
  else
    TrialRoad_OpenSpoutCell(16);
  if (!Event_GetFlag(0x364))
    Actor_SetPosition(17, 0, 0);
  else
    TrialRoad_OpenSpoutCell(17);
  if (!Event_GetFlag(0x365))
    Actor_SetPosition(18, 0, 0);
  else
    TrialRoad_OpenSpoutCell(18);
  ((struct MapWorkByte23 *)Ram_HeapSlots->mapWork)->bits2 = 1;
  TrialRoad_AddPillarTop(15);
  TrialRoad_BlockCellUnder(15);
}

void TrialRoad_SetUpSandStage(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, 15, 16);
  if (!Event_GetFlag(0x374))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetPosition(17, 0, 0);
}

void TrialRoad_SetUpCave(void) {}

void TrialRoad_GaspFlees(void) {
  struct Actor *partner = Actor_GetById(8);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;
  s32 y;

  if (Event_GetFlag(648))
    return;
  dx = (leader->x - partner->x) >> 16;
  dz = (leader->z - partner->z) >> 16;
  near = dx * dx + dz * dz;
  dx = (leader->x >> 16) - 296;
  dz = (leader->z >> 16) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Actor_SetMoveTarget(partner, (296 - dx) << 16, partner->y,
                        (248 - dz) << 16);
  if (far > 15)
    return;
  Event_PlaySound(146);
  Event_PlaySound(122);
  partner->speedLimit = 0x20000;
  partner->acceleration = 0x20000;
  partner->motionFlags = 2;
  partner->terrainHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = 0xa0000;
  Actor_SetScript(8, gTrialRoad_GaspHopScript);
  y = partner->y;
  Actor_SetMoveTarget(partner, 344 << 16, y, 264 << 16);
  Event_SetFlag(648);
}

extern s32 gTrialRoad_PaletteCycleRunning;

/// Unless flag 123 is set, readies object 8: before flag 0x288 it raises it
/// 0x88000 with motion 5, after it grounds it with motion 3; then schedules
/// TrialRoad_GaspFlees.
void TrialRoad_SetUpPoolRoom(void) {
  if (!Event_GetFlag(123)) {
    struct Actor *object = Actor_GetById(8);
    s32 started = Event_GetFlag(0x288);
    u8 *flags = &object->motionFlags;

    if (!started) {
      *flags = 5;
      object->y += 0x88000;
    } else {
      *flags = 3;
      object->y = 0;
    }
    Task_Add(TrialRoad_GaspFlees, 0x480);
  }
}

void TrialRoad_SetPaletteCycleRunning(s32 value) {
  gTrialRoad_PaletteCycleRunning = value;
}

extern s32 gTrialRoad_PaletteCycleFrame;

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gTrialRoad_PaletteCycleFrame,
                             gTrialRoad_PaletteCycleRunning,
                             gTrialRoad_PaletteCycleColors, 0x050000a4)

#include "system.h"

extern s32 gTrialRoad_PaletteCycleFrame;
extern s32 gTrialRoad_PaletteCycleRunning;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void TrialRoad_StartPaletteCycle(s32 mode) {
  gTrialRoad_PaletteCycleRunning = 1;
  if (mode != 2)
    Iwram_CopyWords(gTrialRoad_PaletteCycleColors, (void *)&BG_PLTT_COLOR(5, 0),
                    32);
  if (mode == 1) {
    s32 zero = 0;

    gTrialRoad_PaletteCycleFrame = zero;
    Task_Add(PaletteCycle_StepAt, 0x480);
  }
}

#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "field.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void TrialRoad_LeaveMoapaWaiting(void);

/// The mountain's peak: after the battle, the victory or the defeat; on
/// arrival, Moapa and his knights wait.
void TrialRoad_SetUpPeak(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == 4) {
    InventorySnapshot_Restore();
    TrialRoad_BeatMoapa();
    Import_EventSetExitRequest(4);
  } else if (entrance == 5) {
    InventorySnapshot_Restore();
    TrialRoad_LoseToMoapa();
    Import_EventSetExitRequest(5);
  } else if (Event_GetFlag(0x9ff)) {
    if (Event_GetFlag(0x283))
      TrialRoad_OpenSummitGate(0);
    if (Event_GetFlag(0x385))
      TrialRoad_LeaveMoapaWaiting();
    else
      TrialRoad_PlaceMoapaAtPeak();
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  TrialRoad_StartPaletteCycle(1);
  TrialRoad_AddPillarTop(10);
  if (Event_GetFlag(0x9fc))
    Actor_SetPosition(10, PIXELS(136), PIXELS(328));
}

/* VIEW(s1fc4): variable gTrialRoad_StageSetUps as extern s32 (*const
 * gTrialRoad_StageSetUps[])(void); the definition has void
 * (*gTrialRoad_StageSetUps[])(void); called through the void type, the
 * call's result register moves 2 of 84 instructions of Scene_Initialize in
 * tla-en. */
extern s32 (*const gTrialRoad_StageSetUps__s1fc4[])(void) __asm__(
    "gTrialRoad_StageSetUps");

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 344) != 0)
    return 0;
  if (scene == (s32)&SceneId_Ov697Room1) {
    if (entrance <= 7) {
      Event_SetFlag(324);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
      gTrialRoad_StageSetUps__s1fc4[pattern & 7]();
    } else if (entrance == 55) {
      TrialRoad_ShowTraps();
    }
  }
  if (scene == (s32)&SceneId_Ov697Room3)
    TrialRoad_SetUpCave();
  if (scene == (s32)&SceneId_Ov697Room4)
    TrialRoad_SetUpPoolRoom();
  if (scene == (s32)&SceneId_Ov697Room2) {
    Event_SetFlag(324);
    TrialRoad_SetUpPeak();
  }
  return 0;
}

void TrialRoad_CopyCellsBackward(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape = *srcShape;
      dstShape--;
      srcShape--;
    }
  }
}

void TrialRoad_OpenRivalDoor(s32 mirrored, s32 stage);

void TrialRoad_UpdateRival(void) {
  struct TrialRoadState *state = gTrialRoadState;
  struct TrialRoadState *road = gTrialRoadState;
  s32 side;

  if (state->stage[1] > 3)
    return;
  if (TrialRoad_RivalIsWaiting() != 0) {
    if (state->rivalTimer == 0) {
      side = TrialRoad_RivalIsAhead(1) ^ 1;
      TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[side + 8]);
      state->rivalTimer = side * 5 * 120 + 1;
    } else {
      if (state->stage[1] >= state->rivalBest)
        state->rivalBest = state->stage[1] + 1;
      if (++road->stage[1] <= 3) {
        s32 pattern =
            state->order[state->stage[1] * 2 + (s16)(state->mirrored ^ 1)];

        TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[pattern & 7]);
        Actor_GetById(8)->terrainHeight = 0;
        Actor_GetById(8)->y = 0;
      } else {
        Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
      }
      state->rivalTimer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(8)->animationKind;

    if (state->stage[0] == state->stage[1])
      *kind = 1;
    else
      *kind = 0;
  }
  if (state->rivalTimer == 0)
    return;
  if (GameFlag_Get(260))
    return;
  if (state->stage[0] == state->stage[1]) {
    switch (state->rivalTimer) {
    case 1:
    case 601:
      Sound_Play(124);
      if (state->mirrored != 0) {
        Actor_SetModeById(9, 6);
        if (state->rivalTimer > 600)
          Actor_SetModeById(10, 6);
      } else {
        Actor_SetModeById(11, 6);
        if (state->rivalTimer > 600)
          Actor_SetModeById(12, 6);
      }
      break;
    case 655:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(10, 5);
      else
        Actor_SetModeById(12, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(8)->facing = 0xc000;
      break;
    case 50:
    case 710:
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      TrialRoad_OpenRivalDoor(state->mirrored, state->stage[1]);
      Sound_Play(138);
      break;
    }
  }
  road->rivalTimer++;
}

extern s16 gTrialRoad_StageMapCells[][2];
extern s8 gTrialRoad_DemoSceneryStages[];
extern s8 gTrialRoad_SceneryStages[];

void *Unit_Get(s32 owner);
void TrialRoad_StartRival(s32 a);

// Places `layer` `column` cells into the map: its offset (column << 20), its
// cells and terrain from that column on, and its position from the view. The
// offset takes the literal, the cells and terrain the column's variable:
// with the variable in the offset too, CSE keeps a shift of the register;
// with the literal in the cells, the buffer and offset fold into one literal;
// through gMapCellBuffer's name, the address stays in a register across calls.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = (column) << 20;                                                   \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulQ16(*viewX, words[4]) + words[2];                        \
  words[1] = Iwram_MulQ16(*viewZ, words[5]) + words[3]

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;

  if (scene == (s32)&SceneId_Ov697Room1) {
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct TrialRoadState *state = gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    u32 i;

    if ((u32)(entrance - 31) <= 1) {
      if (!GameFlag_Get(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(896);
      }
      state->mirrored = (entrance - 1) & 1;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      value = state->order[state->mirrored ^ 1];
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if ((u32)(entrance - 33) <= 1) {
      if (!GameFlag_Get(896)) {
        unit = Unit_Get(gMainRoutineWork.currentOwner);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n <= 13; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(896);
      }
      {
        /* FAKEMATCH(temporary): side, for both the store and its flip; as
         * state->mirrored, 2 of 879 instructions differ in tla-en. */
        s16 side = (entrance - 1) & 1;

        state->mirrored = side;
        state->stage[0] = 3;
        state->stage[1] = 3;
        state->leaderBest = 4;
        state->rivalBest = 4;
        side ^= 1;
        {
          /* FAKEMATCH(temporary): index; as state->order[side + 6], 2 of 879
           * instructions differ in tla-en. */
          s32 index = side + 6;

          value = state->order[index];
        }
      }
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination((s32)&SceneId_Ov697Room1, n);
    } else if (entrance == 50) {
      unit = Unit_Get(gMainRoutineWork.currentOwner);
      Iwram_ClearWordsEntry(state, 32);
      value = 0;
      for (n = 0; n <= 13; n++)
        value += unit[n];
      TrialRoad_DealStageOrder(value);
      state->mirrored = 0;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      state->pattern = state->order[0];
      Event_SetDestination((s32)&SceneId_Ov697Room1, 55);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 row;

      if (entrance == 55) {
        s32 x1;
        s32 x2;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        Actor_SetSpriteColor(Actor_GetById(9), 3);
        Actor_SetSpriteColor(Actor_GetById(10), 3);
        Actor_SetSpriteColor(Actor_GetById(11), 3);
        Actor_SetSpriteColor(Actor_GetById(12), 3);
        Map_CopyRawCellsRect(0, 12, 1, 21, 0, 13);
        Map_CopyRawCellsRect(x1, 12, 1, 21, x1, 13);
        Map_CopyRawCellsRect(x2, 12, 1, 21, x2, 13);
        Map_CopyRawCellsRect(0, 13, 33, 21, 1, 13);
        Map_CopyRawCellsRect(x1, 13, 33, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(x2, 13, 33, 21, x2 + 1, 13);
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 1, 13);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, x2 + 1, 13);
        // The native neither hoists the table's address nor strength-reduces
        // the index; as a for loop, 10 more instructions differ (376).
        i = 0;
      hideUnused55:
        if (gTrialRoad_DemoSceneryStages[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused55;
        Map_CenterOnFollowedPoint();
      } else {
        s32 x1;
        s32 x2;
        s32 destinationY;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        value = 1;
        if (entrance & value) {
          TrialRoad_CopyCellsBackward(0, 0, 102, 13, 9, 0);
          Map_CopyRawCellsRect(0, 0, 10, 1, 1, 0);
          Map_CopyRawCellsRect(0, 0, 10, 12, 0, 1);
          Map_CopyRawCellsRect(x1, 0, 10, 12, x1, 1);
          Map_CopyRawCellsRect(x2, 0, 10, 12, x2, 1);
          TrialRoad_CopyCellsBackward(0, 34, 102, 10, 9, 34);
          Map_CopyRawCellsRect(0, 33, 10, 10, 0, 34);
          Map_CopyRawCellsRect(x1, 33, 10, 10, x1, 34);
          Map_CopyRawCellsRect(x2, 33, 10, 10, x2, 34);
        } else {
          Map_CopyRawCellsRect(10, 0, 92, 13, 0, 0);
          Map_CopyRawCellsRect(23, 0, 10, 1, 24, 0);
          Map_CopyRawCellsRect(23, 0, 10, 12, 23, 1);
          Map_CopyRawCellsRect(57, 0, 10, 12, 57, 1);
          Map_CopyRawCellsRect(91, 0, 10, 12, 91, 1);
          Map_CopyRawCellsRect(10, 34, 92, 10, 0, 34);
          Map_CopyRawCellsRect(24, 33, 10, 10, 24, 34);
          Map_CopyRawCellsRect(58, 33, 10, 10, 58, 34);
          Map_CopyRawCellsRect(92, 33, 10, 10, 92, 34);
          Actor_SetPosition(9, 160 << 14, 152 << 16);
          Actor_SetPosition(10, 192 << 13, 152 << 16);
          Actor_SetPosition(11, 184 << 16, 152 << 16);
          Actor_SetPosition(12, 200 << 16, 152 << 16);
        }
        Actor_SetSpriteColor(Actor_GetById(9), 3);
        Actor_SetSpriteColor(Actor_GetById(10), 3);
        Actor_SetSpriteColor(Actor_GetById(11), 3);
        Actor_SetSpriteColor(Actor_GetById(12), 3);
        destinationY = 13;
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 10, destinationY);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, 44, destinationY);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, 78, destinationY);
        // A goto loop for the same reason as the one above.
        i = 0;
      hideUnused:
        if (gTrialRoad_SceneryStages[i] != value)
          Actor_SetHorizontalPositionWithTerrain(i + 64, 0, 0);
        i++;
        if (i <= 9)
          goto hideUnused;
        Map_CenterOnFollowedPoint();
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        if (!GameFlag_Get(265))
          leader->y = leader->terrainHeight =
              Map_GetTerrainHeight(0, leader->x, leader->z);
        if (GameFlag_Get(2559)) {
          struct TrialRoadState *road;

          GameFlag_Set(324);
          // A pointer of its own keeps the state's address in r5 across the
          // calls; through gTrialRoadState each test reloads it (5 more).
          road = gTrialRoadState;
          // The call's argument reads through the other struct: through the
          // same one GCSE moves the load into both predecessors (11 more of
          // gap).
          if (road->stage[0] < road->leaderBest || GameFlag_Get(642)) {
            TrialRoad_OpenLeaderDoor(road->mirrored, road->stage[0]);
            GameFlag_Set(642);
            GameFlag_Set(644);
          }
          if (road->stage[0] < road->rivalBest)
            TrialRoad_OpenRivalDoor(road->mirrored, road->stage[0]);
          TrialRoad_ShowProgress(gMainRoutineWork.currentOwner, 8);
          TrialRoad_StartRival((s16)(state->mirrored ^ 1));
          Task_Add(TrialRoad_UpdateRival, 0x480);
        } else {
          TrialRoad_OpenLeaderDoor(state->mirrored, state->stage[0]);
          GameFlag_Set(642);
          GameFlag_Set(644);
          TrialRoad_OpenRivalDoor(state->mirrored, state->stage[0]);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov697Room3) {
  } else if (scene == (s32)&SceneId_Ov697Room4) {
  } else if (scene == (s32)&SceneId_Ov697Room2 && GameFlag_Get(2559) &&
             !GameFlag_Get(901)) {
    if (gTrialRoadState->mirrored != 0)
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
// the random state is restored after.

extern s8 gTrialRoad_KeyStages[];

/// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
/// eight slots at the start of the stage state, again until both the even and
/// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
/// the random state is restored after.
void TrialRoad_DealStageOrder(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomState = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gTrialRoad_KeyStages[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomState = saved;
}

s32 TrialRoad_GetStage(s32 side) {
  struct TrialRoadState *state;

  state = gTrialRoadState;
  return state->stage[side & 1];
}

s32 TrialRoad_IsMirrored(void) { return gTrialRoadState->mirrored; }

/// A pixel x on the mirrored road, the right path's, when the leader took it.
s32 TrialRoad_MirrorX(s32 x) {
  if (TrialRoad_IsMirrored())
    return 512 - x;
  return x;
}

/// A cell x, in pixels, on the mirrored road when the leader took it.
s32 TrialRoad_MirrorCellX(s32 x) {
  if (TrialRoad_IsMirrored())
    return (64 - x) << 3;
  return x << 3;
}

s32 TrialRoad_MirrorOffset(s32 x) {
  if (TrialRoad_IsMirrored())
    return -x << 3;
  return x << 3;
}

s32 TrialRoad_MirrorAngle(s32 angle) {
  if (TrialRoad_IsMirrored())
    return (0x8000 - angle) & 0xffff;
  return angle & 0xffff;
}

/// Places an actor at a pixel position and facing, mirrored when the leader
/// took the right path.
void TrialRoad_PlaceActor(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetPositionAndFacing(id, TrialRoad_MirrorX(x) << 16, z << 16,
                             (u16)TrialRoad_MirrorAngle(angle));
}

void TrialRoad_LeaveMoapaWaiting(void) {}

void TrialRoad_PlaceMoapaAtPeak(void) {
  TrialRoad_PlaceActor(8, 288, 264, 0x8000);
  TrialRoad_PlaceActor(13, 296, 248, 0x8000);
  TrialRoad_PlaceActor(14, 296, 280, 0x8000);
}

void Actor_MoveToObject(s32 actor, s32 value);

/// ★ Named from the game: "You were quite swift! You may prove a challenge
/// yet!" (MsgTrialRoadSwiftChallenge)
void TrialRoad_ReachPeakFirst(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(0x1000000, -1, 0x1080000, 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkTo(4, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(7, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(5, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(6, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(7);
  Actor_FaceDirection(7, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(5, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(6);
  Actor_FaceDirection(6, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
  wait(20);
  Event_PlaySound(138);
  wait(20);
  TrialRoad_PlaceActor(8, 344, 296, 0xc000);
  object = Actor_GetById(8);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  Actor_SetMoveTarget(object, object->x, object->y + 0x580000, object->z);
  Actor_SelectAnimation(object, 2);
  FieldActor_WaitForMove(object);
  Actor_SelectAnimation(object, 1);
  object->motionFlags = 2;
  object->terrainHeight = 0x800000;
  object->velocityY = 0x40000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, object->x, object->y, object->z - 0x100000);
  FieldActor_WaitForMove(object);
  wait(12);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  FieldActor_SetMoveTarget(object, TrialRoad_MirrorCellX(41) << 16, object->y,
                           264 << 16);
  FieldActor_WaitForMove(object);
  wait(6);
  Event_PlaySound(152);
  object->velocityY = 0x60000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Actor_SetMoveTarget(object, TrialRoad_MirrorCellX(37) << 16, object->y,
                      264 << 16);
  FieldActor_WaitForMove(object);
  wait(6);
  Actor_MoveToObject(13, 8);
  Actor_MoveToObject(14, 8);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkTo(8, TrialRoad_MirrorCellX(36), 264);
  Actor_WalkTo(13, TrialRoad_MirrorCellX(37), 248);
  Actor_WalkTo(14, TrialRoad_MirrorCellX(37), 280);
  Actor_WaitForMoveById(8);
  Actor_FaceDirection(8, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_WaitForMoveById(13);
  Actor_FaceDirection(13, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_WaitForMoveById(14);
  Actor_FaceDirection(14, TrialRoad_MirrorAngle(0x8000), 0);
  Event_SetMessageId((s32)MsgTrialRoadSwiftChallenge);
  FieldActor_ShowEmote(8, 256, 10);
  Event_ShowMessage(8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeed(14, 0x1b333, 0xd999);
  Actor_WalkBy(4, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(8, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(5, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(13, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(7, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(14, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(6, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

/// ★ Named from the game: "You may have been slow, but you made it through
/// Trial Road to the mountain's peak!" (MsgTrialRoadSlowArrival)
void TrialRoad_ReachPeakSecond(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(264), 1);
  Actor_MoveToObject(5, 4);
  Actor_MoveToObject(6, 4);
  Actor_MoveToObject(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkTo(4, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(7, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(5, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(6, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(4);
  Actor_FaceDirection(4, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(7);
  Actor_FaceDirection(7, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(5);
  Actor_FaceDirection(5, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(6);
  Actor_FaceDirection(6, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  FieldActor_ShowEmote(8, 256, 10);
  Event_ShowMessage(8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeed(14, 0x1b333, 0xd999);
  Actor_WalkBy(4, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(8, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(5, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(13, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(7, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(14, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(6, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

void PartyInventory_Discard(s32 cue);

/// ★ Named from the game: "All right, all right. You've proven your strength,
/// and you are heroes indeed." (MsgTrialRoadHeroesProven)
void TrialRoad_BeatMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  TrialRoad_PlaceActor(4, 248, 272, 0);
  TrialRoad_PlaceActor(7, 224, 248, 0);
  TrialRoad_PlaceActor(5, 232, 264, 0);
  TrialRoad_PlaceActor(6, 224, 280, 0);
  TrialRoad_PlaceActor(9, 248, 256, 0);
  TrialRoad_PlaceActor(8, 272, 264, 0x4000);
  TrialRoad_PlaceActor(13, 288, 248, 0x4000);
  TrialRoad_PlaceActor(14, 288, 280, 0x4000);
  Actor_SetAnimation(8, 6);
  Actor_SetAnimation(13, 6);
  Actor_SetAnimation(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(13, 2);
  wait(4);
  Actor_StartRepeatedMotion(14, 2);
  wait(10);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(8, 7);
  Event_Wait(15);
  Actor_SetAnimation(13, 7);
  Event_Wait(10);
  Actor_SetAnimation(14, 7);
  Event_Wait(5);
  Actor_SetAnimation(8, 1);
  Event_Wait(15);
  Actor_FaceDirection(8, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_SetAnimation(13, 1);
  Event_Wait(10);
  Actor_FaceDirection(13, TrialRoad_MirrorAngle(0x8000), 0);
  Actor_SetAnimation(14, 1);
  Event_Wait(5);
  Actor_FaceDirection(14, TrialRoad_MirrorAngle(0x8000), 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, TrialRoad_MirrorCellX(32), 264);
  Actor_FaceEachOther(9, 7, 0);
  Actor_FaceEachOther(5, 6, 0);
  Actor_WaitForMoveById(8);
  Actor_FaceActor(9, 4, 0);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceActor(6, 4, 0);
  Actor_FaceActor(5, 4, 0);
  Event_Wait(20);
  PartyInventory_Discard(65);
  Party_GiveItem(215, 0);
  Actor_FaceActor(4, 8, 0);
  OBJECT_BYTE(Actor_GetById(8), 0x5a) &= ~1;
  Actor_WalkToAndWait(8, TrialRoad_MirrorCellX(35), 264);
  Event_Wait(1);
  /* FAKEMATCH(signature): the byte typed as a whole-byte bit-field, as in
   * overlay 685; as a plain byte, which stores in the order of a store that
   * aliases what follows, 4 of 304 instructions differ in tla-en. */
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(20);
  FieldActor_ShowEmote(8, 261, 10);
  Event_ShowMessage(8, 0);
  Actor_FaceDirectionAndWait(8, TrialRoad_MirrorAngle(0));
  Actor_FaceActor(13, 8, 0);
  Actor_FaceActor(14, 8, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(13, 3);
  wait(3);
  Actor_SetAnimationAndWait(14, 3);
  Actor_FaceActor(4, 9, 0);
  Actor_FaceActor(7, 9, 0);
  Actor_FaceActor(5, 9, 0);
  Actor_FaceActor(6, 9, 0);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(4, 3);
  wait(2);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  wait(1);
  Actor_SetAnimation(5, 3);
  wait(30);
}

/// ★ Named from the game: "You've lost. Now leave Shaman village."
/// (MsgTrialRoadLeaveVillage)
void TrialRoad_LoseToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  TrialRoad_PlaceActor(8, 264, 264, 0x8000);
  TrialRoad_PlaceActor(13, 280, 248, 0x8000);
  TrialRoad_PlaceActor(14, 280, 280, 0x8000);
  TrialRoad_PlaceActor(4, 240, 276, 0);
  TrialRoad_PlaceActor(7, 216, 254, 0);
  TrialRoad_PlaceActor(5, 232, 244, 0);
  TrialRoad_PlaceActor(6, 224, 296, 0);
  Actor_SetAnimation(4, 38);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(6, 19);
  Actor_SetAnimation(7, 19);
  FieldActor_SetSpriteRotationById(7, 0x4000);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetSpriteFlags(Actor_GetById(5), 0);
  Actor_SetSpriteFlags(Actor_GetById(6), 0);
  Actor_SetSpriteFlags(Actor_GetById(7), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(8, 0);
  Actor_StartRepeatedMotion(4, 2);
  wait(1);
  Actor_StartRepeatedMotion(6, 2);
  wait(2);
  Actor_StartRepeatedMotion(5, 2);
  Actor_StartRepeatedMotion(7, 2);
  Event_Wait(15);
}

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.

extern u32 gTrialRoad_LeftKnightLeavesScript[];

/// ★ Named from the game: "The room is filled with traps and snares. Use the
/// power of Contigo to avoid them." (MsgTrialRoadTrapInstructions)
void TrialRoad_ShowTraps(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, PIXELS(1016));
  Actor_SetPosition(8, PIXELS(1016), PIXELS(1016));
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightEntersScript);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightEntersScript);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(184 << 16, -1, 600 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 552 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 408 << 16, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120 << 16, -1, 120 << 16, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  Event_ShowMessageAndWait(8, 0);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(216 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(4, 0);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightToChestScript);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_WalkByAndWaitById(13, 0, 8);
  Event_Wait(1);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) |= 1;
  Event_Wait(20);
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_SetDestinationOffset(13, 0, -40);
  Actor_WaitForMoveById(13);
  Sound_Play(103);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  MapGrid_SetShake(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_WalkByAndWaitById(13, 0, 16);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(60);
  Actor_ArmCallback(13, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, -16, -16);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Sound_Play(124);
  Actor_SetModeById(9, 6);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_WalkToAndWaitById(13, 200, 168);
  Actor_FaceDirection(13, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(9, 5);
  Event_Wait(20);
  Sound_Play(138);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(13, gTrialRoad_LeftKnightLeavesScript);
  Actor_RefreshSelectorById(13);
  Sound_Play(123);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightToChestsScript);
  FieldCamera_MoveTo(328 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(14, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkByAndWait(14, 16, -16);
  Actor_ArmCallback(14, 0, 0);
  Sound_Play(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(14, 350, 168);
  Actor_FaceDirection(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(11, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(14, 364, 168);
  Actor_ArmCallback(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(12, 5);
  Event_Wait(20);
  Sound_Play(138);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_ArmCallback(14, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageAndWait(8, 0);
  FieldCamera_MoveWithinMapBounds(408 << 16, -1, 120 << 16, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gTrialRoad_RightKnightLeavesScript);
  Actor_RefreshSelectorById(14);
  Event_ShowMessageAndWait(8, 0);
  Import_EventSetExitRequest(10);
}

// Updates the sixteen movers at gInventorySnapshot + 0x188 each frame: at its
// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by
// 4 or -8 a frame until it reaches its high or low offset; its phase counts
// round its period unless flag 260 is set. Each mover's shape, collision
// height and cell attribute byte follow its actor and offset, and its marker
// actor is placed over it, bobbing on a sine while the offset is not 0.

/// Updates the sixteen movers at gInventorySnapshot + 0x188 each frame: at its
/// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by 4
/// or -8 a frame until it reaches its high or low offset; its phase counts
/// round its period unless flag 260 is set. Each mover's shape, collision
/// height and cell attribute byte follow its actor and offset, and its marker
/// actor is placed over it, bobbing on a sine while the offset is not 0.
void TrialRoad_UpdateFountains(void) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *mover = work->fountains;
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = work->shape;
  u8 *shapes = *(u8 **)((u8 *)map + 316);
  u8 *cells = map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x == 0 && actor->z == 0)
      continue;
    if (mover->phase == mover->riseAt) {
      Sound_Play(206);
      mover->speed = 4;
    }
    if (mover->phase == mover->fallAt) {
      Sound_Play(395);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->height += mover->speed;
      if (mover->height >= mover->high) {
        mover->height = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->height <= mover->low) {
        mover->height = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Get(260)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (actor->x >> 20) + ((actor->z >> 20) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((((s16 *)&actor->y)[1] + mover->height) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->height != 0) {
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->spout;
    if (marker != 0) {
      if (mover->height == 0) {
        marker->y = actor->y - 0x100000;
      } else {
        s32 wave;
        mover->bob += 0x1000;
        wave = Iwram_MulQ16(0x20000, Trig_Sin(mover->bob)) - 0xe0000;
        marker->y = actor->y + (mover->height << 16) + wave;
      }
      marker->x = actor->x;
      marker->z = actor->z;
    }
  }
}
#undef FIELD_AT
#undef SET_LAYER

// Trial Road's movers' sprites, the setup of its gauge, a rising platform's
// entry, the trap table's setup, TrialRoad_CheckCrumblingStep, the placing of
// actors 8, 9 and 10, TrialRoad_StepOnTrap and a second copy of
// Effect_AdvanceMotionAtLeaderPriority, in ROM order.
#include "frame.h"

/// Queues the Trial Road movers' sprites.
void TrialRoad_DrawFountains(void) {
  struct TrialRoadFountains *base =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *mover = base->fountains;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  s32 rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 bob;

  tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  entry = base->sprites;
  count = 0;
  rise = -0x20000;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->height == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->height) {
    next:
      if (count > 79)
        goto skip;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        goto skip;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Oam_AddSpriteAtDepth(entry, depth);
      entry += 3;
      count++;
    skip:
      lift += 16;
      if (lift < mover->height)
        goto next;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    height = object->y + (mover->height << 16) - viewY;
    z = object->z - viewZ - viewY;
    x = (x >> 16) - 8;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Trig_Sin(mover->bob));
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepth(entry, depth);
    entry += 3;
    count++;
  }
}

#include "actor.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

void TrialRoad_StartFountains(s32 unused, s32 shape) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!Event_GetFlag(0x109))
    Iwram_ClearWords(work, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Decompress_TaggedLz(gField_Tiles64D, buffer);
  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 0x800, buffer);
  Heap_Release(buffer);
  work->sprites = Heap_AllocEwram(0x3c0);
  work->shape = shape;
  Task_Add(TrialRoad_UpdateFountains, 0x48d);
  Task_Add(TrialRoad_DrawFountains, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high) {
  struct TrialRoadFountains *work =
      (struct TrialRoadFountains *)(gInventorySnapshot + 0x188);
  struct TrialRoadFountain *fountain = &work->fountains[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *spout;

  Actor_GetById(id);
  Actor_SetSpriteFlags(object, 0);
  if (!Event_GetFlag(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    fountain->id = id;
    fountain->height = 0;
    fountain->phase = timer;
    fountain->period = 360;
    fountain->high = high;
    fountain->riseAt = 60;
    fountain->fallAt = 240;
    fountain->low = low;
    fountain->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    fountain->speed = 0;
    fountain->bob = 0;
    fountain->unk14 = 0;
    object->priorityFlags |= 2;
  }
  spout = ((struct Actor *)Import_ActorCreate(0x8000, object->x, object->y,
                                              object->z));
  spout->collisionFlags = 0;
  spout->motionFlags = 0;
  spout->priorityFlags |= 128;
  fountain->spout = spout;
  return fountain;
}

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.

/// The trap table's setup: a trap whose second flag is set is put away; any
/// other marks its cell, writes a shape for its height into the shape grid, and
/// takes mode 0 when its first flag is set. Then the leader is lifted onto the
/// ground if it stands below it.
DEFINE_PILLARS_RESTORE_RAISED(TrialRoad_RestoreCrumblingPillars,
                              Actor_GetCellIndex, MapGrid_GetTerrainKind,
                              Actor_DisableById, MapCollision_FindOrAddTag,
                              Actor_SelectAnimation)

DEFINE_TRAPS_CHECK(TrialRoad_CheckCrumblingStep, Field_TryJumpForward,
                   Actor_FindAtCell, Actor_SelectAnimation,
                   TrialRoad_SpawnRubble, TrialRoad_SetLeaderCellShape,
                   TrialRoad_SpawnDustRing)

void TrialRoad_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        TrialRoad_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(object, 2);
        TrialRoad_SetActorCellShape(-4, OBJECT_BYTE(object, 34), id);
        TrialRoad_SpawnRubble(1);
        wait(16);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
        wait(2);
        Event_PlaySound(188);
        TrialRoad_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

void TrialRoad_SetPillarMode3(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 owner);
/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
DEFINE_PILLARS_RESTORE(TrialRoad_RestorePillars, TrialRoad_SetPillarMode3,
                       Actor_GetCellIndex, MapGrid_GetTerrainKind,
                       Actor_DisableById, MapCollision_FindOrAddTag)

void TrialRoad_TakePlaceOf(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

#include "field_effect.h"

void TrialRoad_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Actor_SelectAnimation(partner, 1);
      TrialRoad_TakePlaceOf(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Actor_SelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      TrialRoad_SpawnRubble(0);
      TrialRoad_TakePlaceOf(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Actor_SelectAnimation(partner, 0);
        TrialRoad_TakePlaceOf(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        TrialRoad_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        TrialRoad_SetLeaderCellShape(-2, trap->terrainId);
        TrialRoad_SpawnRubble(1);
        wait(16);
        TrialRoad_SetLeaderCellShape(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        TrialRoad_SpawnDustRing();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

/// A second copy of Effect_AdvanceMotionAtLeaderPriority in this overlay:
/// FieldEffect_AdvanceMotion, then the leader's sprite priority for the effect.
void TrialRoad_DriftRubble(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
  struct FieldSprite *sprite;
  union FieldObject *leader;
  struct EffectSpriteAttributes *to;
  struct EffectSpriteAttributes *from;
  struct EffectSpriteBits source;
  struct EffectSpriteBits bits;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  {
    /* FAKEMATCH(temporary): the x scale rate read into a block temporary,
     * which loads it before the x scale; as effect->scaleX +=
     * effect->scaleRateX, 2 of 59 instructions differ in tla-en. */
    s32 rate = effect->scaleRateX;

    effect->scaleX += rate;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->currentOwner);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

#include "field_sprite.h"

extern const s32 gTrialRoad_RubbleScript[];

void TrialRoad_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = TrialRoad_DriftRubble;
    }
    i++;
  } while (i <= 7);
}

extern const s32 gTrialRoad_DustScript[];

void TrialRoad_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Import_ActorCreate(285, leader->x, leader->y,
                                                      leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_DustScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Trig_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = TrialRoad_DriftRubble;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

DEFINE_ACTOR_SET_CELL_SHAPE(TrialRoad_SetActorCellShape, MapGrid_GetTerrainKind,
                            MapCollision_FindOrAddTag)

void TrialRoad_SetLeaderCellShape(s32 a, s32 b) {
  TrialRoad_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

extern const u8 gTrialRoad_HexDigits[];

/// Writes value as eight hexadecimal digits and a terminating null.
UNUSED void String_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;
  const u8 *digits = gTrialRoad_HexDigits;

  buf += 8;
  *buf = 0;
  buf--;
  i = 7;
  // A goto loop, which builds the 15 on every pass: as a for loop, the 15 is
  // moved out of it (differ 13).
next:
  *buf = digits[value & 15];
  value >>= 4;
  buf--;
  i--;
  if (i >= 0)
    goto next;
}

#include "field_event.h"

extern u8 gSceneState[];

void TrialRoad_Idle(void) {}

void TrialRoad_SetBalanceStateReady(void) {
  u16 *state = (u16 *)gSceneState;
  u16 ready = 9;

  *state = ready;
}

void TrialRoad_WaitForBalanceState(void) {
  s16 *state = (s16 *)gSceneState;

  while (*state != 9) {
    wait(1);
  }
}

// The Trial Road rival's per-frame update.

#include "../shared/trial_road.h"

/// Updates the Trial Road rival by the scene state's mode: 4 follows the
/// leader's position mirrored across the course; 3 places the rival and its
/// partner from the linked player's record; 1 replays the recorded path; 2
/// records the actor's position.
void TrialRoad_MoveRival(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  struct TrialRoadReplay *state = &gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x, z, turn, dx, dz;
  u16 heading;

  if (rival == 0)
    return;
  if (state->mode == 4) {
    x = leader->x;
    z = leader->z;
    if ((u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - (x & 0xffff0000);
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == 3) {
    s32 player;
    s16 *signature;
    s32 angle, link, ok, partner;
    s16 st;

    player = ((s32)GameFlag_GetByte(760)) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    st = *signature++;
    link = st >> 8;
    ok = st & 0xff;
    if (GameFlag_Get(dz = 529) && link != 0) {
      state->mode = 9;
      Actor_SelectAnimation(rival, 1);
      if (work->linkX < rival->x)
        x = work->linkX + 0xc0000;
      else
        x = work->linkX - 0xc0000;
      if (!GameFlag_Get(530)) {
        z = work->linkZ + 0x100000;
        rival->timer = work->linkAngle;
      } else {
        z = work->linkZ - 0x100000;
        rival->timer = work->unke6;
      }
      rival->acceleration = 0x4000;
      rival->speedLimit = 0x10000;
      Actor_SetMoveTarget(rival, x, 0, z);
      Actor_StartIdle(rival);
    } else {
      x <<= 16;
      z <<= 16;
      rival->facing = angle;
      rival->x = x;
      rival->z = z;
      Actor_SelectAnimation(rival, ok);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    ok = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x = x << 16;
    rival->facing = angle;
    rival->z = z << 16;
    Actor_SelectAnimation(rival, ok);
  } else if (state->mode == 1) {
    if (state->resource != work->resource) {
      Decompress_TaggedLz(Resource_GetTableEntry(state->resource), work->path);
      work->resource = state->resource;
    }
    x = work->path[state->point];
    z = work->path[state->point + 1];
    if (!GameFlag_Get(260))
      state->point += 2;
    if (x == 0 && z == 0) {
      state->mode = 9;
      return;
    }
    x <<= 16;
    z <<= 16;
    if (state->mirrored != 0 && (u32)(z - 0x1000000) > 0x1300000)
      x = 0x2100000 - x;
    if (gMainRoutineWork.entrance & 1)
      x += 0x1300000;
    else
      x -= 0x1300000;
    if (rival->x != x || rival->z != z) {
      dx = x - rival->x;
      dz = z - rival->z;
      heading = ArcTan2(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      rival->facing += turn;
      rival->x = x;
      rival->z = z;
      state->still = 0;
    } else {
      state->still++;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == 2) {
    x = rival->x >> 16;
    z = rival->z >> 16;
    work->path[state->point] = x;
    work->path[state->point + 1] = z;
    if (!GameFlag_Get(260))
      state->point += 2;
    if (state->point == 7198) {
      // A u16 read back as s16 at its use: the same value, the spelling that
      // matches.
      u16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[(s16)next] = 0;
      state->id = work->rival;
      state->point = 0;
      state->mode = 1;
    }
  }
}

// next object the overlay links, and the overlay's data.

#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"

/* VIEW(s51a0): tag struct TrialRoadLinkWork as struct TrialRoadLinkWork__s51a0;
 * s15a8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/scene/trial_road/s15a8.c:2523: structure has no member named
 * `vramEntry'. */
struct TrialRoadLinkWork__s51a0 {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 unke0;
  s16 a;
  s16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

extern const u8 gTrialRoad_ProgressPicture[];

s32 TrialRoad_GetStage(s32 side);

// The picture's buffer: 576 bytes of EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(576);
}

// left and right are the arms' offsets, each opening with its sign, so the
// subtractions compile in the order the native has them.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6; as a plain s32 *, 304 of 497             \
       instructions differ in tla-en. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9; as a plain s32, 365 of 497 instructions        \
       differ in tla-en. */                                                    \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0; as a plain s32, 44 of 497 instructions       \
       differ in tla-en. */                                                    \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (actor->x - work->originX) / PIXELS(16) left;                        \
      arg = which;                                                             \
    } else {                                                                   \
      x = (actor->x - work->originX) / PIXELS(16) right;                       \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = TrialRoad_GetStage(arg);                                            \
    y = (actor->z - *originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;    \
    /* Once-only: ends the block, so y + 4 is scheduled before x & 511. */     \
    do {                                                                       \
    } while (0);                                                               \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | 0xe400;                                   \
    Oam_AddSpriteAtDepth(entry++, 255);                                        \
  } while (0)

/// The Trial Road link display's scheduled callback, which
/// TrialRoad_ShowProgress starts. Its fade state falls during events or while
/// flag 262 is set and otherwise rises to 2 (unke0 holds it at 2); rising from
/// 0 to 1 it loads the palette and picture again, and at 0 it frees its VRAM.
/// Otherwise it queues two rows of sprites, each up to four pairs 36 pixels
/// apart down from the scrolled offset, sliding in 6 pixels per fade step (the
/// right row mirrored), and on frames 5 to 15 of every 16 a marker over each of
/// the two watched actors, projected at 1/16 scale from their positions and the
/// course side.
void TrialRoad_DrawProgress(void) {
  struct TrialRoadLinkWork__s51a0 *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gResourceTableEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = 0x40000000;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *actor;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Get(262) || event->unk166 != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(gTrialRoad_ProgressPalette, (void *)0x050003c0, 0x80000010,
              REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Oam_AddSpriteAtDepth(entry++, 255);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = ObjectTable_Get(work->a);
  if (actor != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  actor = ObjectTable_Get(work->b);
  if (actor == NULL || actor->x == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}

void TrialRoad_ShowProgress(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  owner = gMainRoutineWork.currentOwner;
  height = &link->height;
  unkf0 = &link->unkf0;
  unkda = &link->unkda;
  unke0 = &link->unke0;
  vramEntry = &link->vramEntry;
  if (a == owner) {
    *height = 0xa00000;
    *unkf0 = 0xf9a00000;
  } else {
    *height = 0x700000;
    *unkf0 = 0xf9a00000;
  }
  Actor_GetById(a);
  Actor_GetById(b);
  zero = 0;
  *unkda = zero;
  *unke0 = zero;
  Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
  *vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(TrialRoad_DrawProgress, 0x476);
  Heap_Release(buffer);
}

extern u8 gSceneState[];

void TrialRoad_StartRival(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;

  if (Event_GetFlag(0x109) == 0) {
    /* FAKEMATCH(temporary): a local reads the work value before the three
     * stores; as the value stored directly, 4 of 24 instructions differ in
     * tla-en. */
    s32 value = *(u16 *)(work + 228);

    control->field02 = a;
    control->field04 = value;
    control->field08 = 0;
  }
  Task_Add(TrialRoad_MoveRival, 0x485);
}

void TrialRoad_PlayRivalRecording(s32 a) {
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;
  s32 zero = 0;

  *(u16 *)((Ram_HeapSlots->unkec[0]) + 234) = zero;
  control->field0a = a;
  control->field06 = zero;
  control->state = 1;
  TrialRoad_MoveRival();
}

s32 TrialRoad_RivalIsWaiting(void) {
  s32 state = *(s16 *)gSceneState;

  return IsZero(state ^ 9);
}

void TrialRoad_DoNothing(void) {}

void TrialRoad_SettleOnGround(void);
void TrialRoad_FollowPillar(void);

s32 gTrialRoad_SettleScript[] = {
    46,
    (s32)TrialRoad_SettleOnGround,
    17,
};

s32 gTrialRoad_PillarTopScript[] = {
    46,
    (s32)TrialRoad_FollowPillar,
    17,
};

u32 gTrialRoad_PoundPartners[] = {
    0x14151213,
};

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
    1, 38, 17, 17,
};

u32 gTrialRoad_GaspHopScript[] = {
    23, 3, 0x1000, 0, 1, 13,
    0xffff, 0,
};

void (*gTrialRoad_StageSetUps[])(void) = {
    TrialRoad_SetUpPillarStage,
    TrialRoad_SetUpTorchStage,
    TrialRoad_SetUpBushPillarStage,
    TrialRoad_SetUpPoundStage,
    TrialRoad_SetUpCrumblingStage,
    TrialRoad_SetUpLogStage,
    TrialRoad_SetUpFountainStage,
    TrialRoad_SetUpSandStage,
};

u32 gTrialRoad_RivalRecordings[] = {
    450, 451, 452, 453, 454, 455,
    456, 457, 458, 459,
};

s16 gTrialRoad_StageMapCells[][2] = {
    {1, 48}, {15, 48}, {29, 48}, {1, 70},
    {15, 70}, {29, 70}, {1, 92}, {15, 92},
};

s8 gTrialRoad_DemoSceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

s8 gTrialRoad_SceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

s8 gTrialRoad_KeyStages[] = {
    0, 2, 3, 7, 1, 4, 5, 6,
};

u32 gTrialRoad_RightKnightEntersScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x1380000, 22, 7, 0x2a80000,
    39, 5, 4, 0x1380000, 0, 0x2780000,
    1, 4, 0x1580000, 0, 0x2680000, 1,
    4, 0x1980000, 0, 0x2680000, 1, 4,
    0x1a80000, 0, 0x2580000, 1, 4, 0x1a80000,
    0, 0x1c80000, 1, 17,
};

u32 gTrialRoad_LeftKnightEntersScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0xe80000, 22, 7, 0x2a80000,
    39, 5, 4, 0xe80000, 0, 0x2780000,
    1, 4, 0xc80000, 0, 0x2680000, 1,
    4, 0x880000, 0, 0x2680000, 1, 4,
    0x780000, 0, 0x2580000, 1, 4, 0x780000,
    0, 0x1c80000, 1, 17,
};

u32 gTrialRoad_LeftKnightToChestScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x780000, 22, 7, 0x1280000,
    39, 5, 4, 0x780000, 0, 0xd80000,
    1, 4, 0x880000, 0, 0xc80000, 1,
    4, 0xd80000, 0, 0xc80000, 1, 4,
    0xe80000, 0, 0xb80000, 1, 0, 2,
    22, 3, 0xc000, 17,
};

u32 gTrialRoad_LeftKnightLeavesScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0xd80000, 0, 0xa80000,
    1, 4, 0xe80000, 0, 0x980000, 1,
    4, 0xe80000, 0, 0x880000, 1, 22,
    5, 0, 22, 7, 0, 17,
};

u32 gTrialRoad_RightKnightToChestsScript[] = {
    22, 15, 0x10000, 22, 16, 0x8000,
    22, 5, 0x1580000, 22, 7, 0xb80000,
    39, 2, 4, 0x1380000, 0, 0xa80000,
    1, 39, 1, 0, 2, 22,
    3, 0x8000, 0, 2, 22, 3,
    0x6000, 0, 2, 22, 3, 0x4000,
    17,
};

u32 gTrialRoad_RightKnightLeavesScript[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0x1780000, 0, 0xc80000,
    1, 4, 0x1d80000, 0, 0xc80000, 1,
    4, 0x1e80000, 0, 0xb80000, 1, 4,
    0x1e80000, 0, 0x880000, 1, 39, 1,
    0, 2, 22, 3, 0xa000, 0,
    2, 22, 3, 0x8000, 0, 2,
    22, 3, 0x6000, 0, 2, 22,
    3, 0x4000, 17,
};
// clang-format on
#undef MARKER
