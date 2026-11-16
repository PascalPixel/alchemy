/// @file
/// The Sea of Time, the open water before Lemuria: its waves roll in cycles
/// (SeaOfTime_StartWaves, SeaOfTime_TrackWaves), currents carry the ship
/// (SeaOfTime_RideCurrent) or sweep it back (SeaOfTime_SweptBack), and a
/// whirlpool draws it down (SeaOfTime_SinkIntoWhirlpool). The ship's Grind
/// clears the rocks in its channel one by one (SeaOfTime_GrindFirstRock to
/// SeaOfTime_GrindLastRock) until it crosses to Lemuria
/// (SeaOfTime_CrossToLemuria).

#include "sprite_ids.h"
#include "graphics/palette_buffers.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"
#include "flags.h"
#include "types.h"
#include "macros.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/actor.h"
#include "field/map_grid.h"
#include "system/field_effect.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "field/scene_tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_ids.h"
#include "system/main_routine_work.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"

extern volatile u32 gSeaOfTime_WaveStartFrame;
extern u32 gSeaOfTime_WaveState;
extern u32 gSceneState[];
extern struct MapCell gMapGridCellBuffer[];

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
FIXED Trig_Cos(s32 angle);
void Event_End(void);
void FieldCamera_WaitForArrival(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
s32 SeaOfTime_GetCellCode(u32 layer, s32 x, s32 y);
s32 SeaOfTime_SetCellCode(u32 layer, s32 x, s32 y, s32 value);
s32 Position_FlatDistance(FIXED *a, FIXED *b);
void SeaOfTime_RemoveRock(s32 x, s32 z, s32 actor);
void SeaOfTime_PlaceRock(s32 x, s32 z, s32 actor);

/// Trig_Cos, through an inline as the swirl's turn calls it.
static inline s32 SeaOfTime_Cos(s32 angle) { return Trig_Cos(angle); }

struct Actor *Scene_CreateImportedActor(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, 15);
    object->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_UNDERFOOT;
    return object;
  }
  return NULL;
}

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

// The Sea of Time, east of Lemuria: currents and swirls that carry the ship
// round and sweep it back to the entrance, the laps and waves Yepp's songs
// tell the way by, and rocks Grind breaks; the rest of the overlay is in
// sea_of_time/.

extern struct ActorPlacement gSeaOfTime_Room2Placements[];
extern u32 gSeaOfTime_LapStamp;

void SeaOfTime_TrackWaves(void);

extern struct Trigger gSeaOfTime_Room1Triggers[];

extern struct Trigger gSeaOfTime_EntranceTriggers[];
extern struct Trigger gSeaOfTime_Triggers[];
extern s16 gSeaOfTime_RedPulseStep[1];
void SeaOfTime_RideCurrent(void);

void SeaOfTime_CountLaps(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gSeaOfTime_ShipRests[];

s32 Actor_HasArrived(struct Actor *actor);
u16 gSeaOfTime_RippleLines[160] BSS_DATA ALIGNED(16);

/// Starts an actor's animation from its first frame, on map layer 1.
s32 SeaOfTime_ResetAnimationFrame(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->flags = 0;
  object->layer = 1;
  return 1;
}

/// A map cell's collision code and second attribute, as one number: the code in
/// the high byte. Layers above 2 read the cell buffer.
s32 SeaOfTime_GetCellCode(u32 layer, s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + (y << 7);
  return (cell->collisionCode << 8) | cell->attributeB;
}

/// Sets a map cell's collision code and second attribute from one number, as
/// SeaOfTime_GetCellCode reads them, and returns the old one.
s32 SeaOfTime_SetCellCode(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

#include "shared/position_flat_distance.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gSeaOfTime_Placements[];

/// The party's ship: placed blank, it wears the boat sheet (sprite 11) on the
/// water, and the currents, the swirl and the whirlpool move it.
enum {
  ACTOR_SHIP = ACTOR_FIRST_PLACED,
  /// The five rocks (sprite 424) in the channel to Lemuria, which the ship's
  /// Grind clears one by one.
  ACTOR_ROCK_A,
  ACTOR_ROCK_B,
  ACTOR_ROCK_C,
  ACTOR_ROCK_D,
  ACTOR_ROCK_E
};

/// Set as Grind clears each rock; a cleared rock is not placed again.
enum {
  FLAG_ROCK_A_CLEARED = 0xa90,
  FLAG_ROCK_B_CLEARED,
  FLAG_ROCK_C_CLEARED,
  FLAG_ROCK_D_CLEARED,
  FLAG_ROCK_E_CLEARED,
  FLAG_LAST_ROCK_CLEARED
};

/// While the waves run (SeaOfTime_StartWaves), the channels and the lap cells
/// do not answer.
#define FLAG_WAVES_RUNNING FLAG_SCENE_TEMP_00
/// Set the first time the sea sweeps the ship back, which then leaves by exit 4
/// rather than 1, and on crossing to Lemuria.
#define FLAG_SWEPT_BACK_ONCE 0x8dd

/// The cell behaviors the Sea of Time's triggers answer: currents that carry
/// the ship one way (eight headings), the swirl (which way it turns), the
/// whirlpool, the channels (four headings), the cell that sweeps the ship back,
/// and the cells around the whirlpool that count its laps.
enum SeaOfTimeCellBehavior {
  CELL_BEHAVIOR_CURRENT_NORTH = 30,
  CELL_BEHAVIOR_CURRENT_NORTHWEST,
  CELL_BEHAVIOR_CURRENT_WEST,
  CELL_BEHAVIOR_CURRENT_SOUTHWEST,
  CELL_BEHAVIOR_CURRENT_SOUTH,
  CELL_BEHAVIOR_CURRENT_SOUTHEAST,
  CELL_BEHAVIOR_CURRENT_EAST,
  CELL_BEHAVIOR_CURRENT_NORTHEAST,
  CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE = 40,
  CELL_BEHAVIOR_SWIRL_CLOCKWISE,
  CELL_BEHAVIOR_WHIRLPOOL = 44,
  CELL_BEHAVIOR_CHANNEL_EAST,
  CELL_BEHAVIOR_CHANNEL_SOUTH,
  CELL_BEHAVIOR_CHANNEL_WEST,
  CELL_BEHAVIOR_CHANNEL_NORTH,
  CELL_BEHAVIOR_SWEPT_BACK,
  CELL_BEHAVIOR_LAP_FIRST
};

/// The raised triggers of the waves: the laps counted, and the waves ended.
enum SeaOfTimeRaisedTrigger { RAISED_START_WAVES = 200, RAISED_STOP_WAVES };

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_SEA_OF_TIME_TO_LEMURIA)
    return gSeaOfTime_Room2Placements;
  return gSeaOfTime_Placements;
}

/// In a swirl the ship is drawn to the middle of its 32-pixel square and turned
/// about it, one way or the other by the swirl's cell.
void SeaOfTime_CircleSwirl(void) {
  struct Actor *object = Actor_GetById(ACTOR_SHIP);
  s32 turn[1];
  FIXED center[3];
  s32 *pos;
  FIXED sine;
  FIXED dx;
  FIXED dz;
  FIXED x;
  FIXED z;

  center[0] = ((object->x.fixed >> 21) << 21) + toFIXED(16);
  center[2] = ((object->z.fixed >> 21) << 21) + toFIXED(16);
  pos = &object->x.fixed;
  if (Position_FlatDistance(pos, center) > 4) {
    if (*pos < center[0])
      *pos += 0x3333;
    else
      *pos -= 0x3333;
    if (object->z.fixed < center[2])
      object->z.fixed += 0x3333;
    else
      object->z.fixed -= 0x3333;
    if (SeaOfTime_GetCellCode(1, FIELD_CELL(center[0]),
                              FIELD_CELL(center[2])) >>
            8 ==
        CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE) {
      turn[0] = SeaOfTime_Cos(-TRIG_TURN / 128);
      sine = Trig_Sin(-TRIG_TURN / 128);
    } else {
      turn[0] = SeaOfTime_Cos(TRIG_TURN / 128);
      sine = Trig_Sin(TRIG_TURN / 128);
    }
    dx = object->x.fixed - center[0];
    dz = object->z.fixed - center[2];
    x = Iwram_MulFX(dx, turn[0]) - Iwram_MulFX(dz, sine);
    z = Iwram_MulFX(dx, sine) + Iwram_MulFX(dz, turn[0]);
    x += center[0];
    z += center[2];
    object->x.fixed = x;
    object->z.fixed = z;
  }
}

/// A current carries the ship toward the next square in its heading and turns
/// it about that square's middle, throwing up spray on both sides as it goes.
void SeaOfTime_RideCurrent(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  struct Actor *object = Actor_GetById(ACTOR_SHIP);
  struct {
    s32 x, y, z;
  } center;
  FIXED ends[4];
  struct EffectOptions spark;
  s32 turn[1];
  s32 *pos;
  FIXED dx;
  FIXED dz;
  s32 x;
  s32 z;
  s32 phase;
  s32 frame;
  s32 *counter;
  FIXED c;
  FIXED s;
  s32 flip;
  s32 speed;
  FIXED px;
  FIXED py;
  FIXED pz;

  switch (work->stepTrigger) {
  case CELL_BEHAVIOR_CURRENT_NORTH:
    center.x = 0;
    center.z = -1;
    break;
  case CELL_BEHAVIOR_CURRENT_WEST:
    center.x = -1;
    center.z = 0;
    break;
  case CELL_BEHAVIOR_CURRENT_SOUTH:
    center.x = 0;
    center.z = 1;
    break;
  case CELL_BEHAVIOR_CURRENT_EAST:
    center.x = 1;
    center.z = 0;
    break;
  case CELL_BEHAVIOR_CURRENT_NORTHWEST:
    center.x = -1;
    center.z = -1;
    break;
  case CELL_BEHAVIOR_CURRENT_SOUTHWEST:
    center.x = -1;
    center.z = 1;
    break;
  case CELL_BEHAVIOR_CURRENT_SOUTHEAST:
    center.x = 1;
    center.z = 1;
    break;
  case CELL_BEHAVIOR_CURRENT_NORTHEAST:
    center.x = 1;
    center.z = -1;
    break;
  }
  center.x = (((object->x.fixed >> 21) + center.x) << 21) + toFIXED(16);
  center.z = (((object->z.fixed >> 21) + center.z) << 21) + toFIXED(16);
  pos = &object->x.fixed;
  if (Position_FlatDistance(pos, &center) > 46) {
    if (*pos < center.x)
      *pos += toFIXED(1) / 2;
    else
      *pos -= toFIXED(1) / 2;
    if (object->z.fixed < center.z)
      object->z.fixed += toFIXED(1) / 2;
    else
      object->z.fixed -= toFIXED(1) / 2;
  }
  if (SeaOfTime_GetCellCode(1, FIELD_CELL(center.x), FIELD_CELL(center.z)) >>
          8 ==
      CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE) {
    turn[0] = SeaOfTime_Cos(-TRIG_TURN / 128);
    s = Trig_Sin(-TRIG_TURN / 128);
  } else {
    turn[0] = SeaOfTime_Cos(TRIG_TURN / 128);
    s = Trig_Sin(TRIG_TURN / 128);
  }
  dx = object->x.fixed - center.x;
  dz = object->z.fixed - center.z;
  {
    s32 (*mul)(s32 a, s32 b) = Iwram_MulFX;
    x = mul(dx, turn[0]);
    x -= mul(dz, s);
    z = mul(dx, s);
    z += mul(dz, turn[0]);
  }
  x += center.x;
  z += center.z;
  object->x.fixed = x;
  object->z.fixed = z;

  counter = &gFrameCount;
  frame = *counter;
  phase = 3;
  phase &= frame;
  if (phase == 0) {
    if ((frame & 10) == 0)
      Event_PlaySound(SE_CURRENT);
    spark.priority = 2;
    spark.palette = 7;
    spark.startScaleX = FIXED_ONE * 4 / 5;
    spark.startScaleY = FIXED_ONE * 4 / 5;
    c = Trig_Cos(object->facing);
    {
      FIXED sine = Trig_Sin(object->facing);
      ends[0] = c * 10;
      ends[1] = sine * 10;
      ends[2] = -(c * 10);
      ends[3] = -(sine * 10);
    }
    ends[0] += object->x.fixed;
    ends[1] += object->z.fixed;
    speed = toFIXED(1) / 2;
    ends[2] += object->x.fixed;
    ends[3] += object->z.fixed;
    if (ends[1] != object->z.fixed) {
      ends[1] += -toFIXED(12);
      ends[3] += -toFIXED(12);
    }
    flip = *counter & 4;
    px = ends[0];
    py = object->y.fixed;
    pz = ends[1];
    if (flip) {
      FieldEffect_Spawn(px + toFIXED(3), py, pz, speed, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                            EFFECT_USE_PALETTE,
                        &spark);
      FieldEffect_Spawn(
          ends[2] - toFIXED(3), object->y.fixed, ends[3], -speed, 0, 0,
          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &spark);
    } else {
      FieldEffect_Spawn(px - toFIXED(3), py, pz, -speed, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                            EFFECT_USE_PALETTE,
                        &spark);
      FieldEffect_Spawn(
          ends[2] + toFIXED(3), object->y.fixed, ends[3], speed, 0, 0,
          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &spark);
    }
  }
}

/// Walks actor 8 along a run of cells in the facing the event work names,
/// throwing sparks from both sides of it every fourth frame.
void SeaOfTime_FollowChannel(void) {
  FIXED pos[4];
  struct EffectOptions spark;
  struct FieldWork *work;
  struct Actor *actor;
  s16 angle;
  s32 x, z, dx, dz;
  FIXED targetX, targetZ;
  FIXED cosine, sine;
  FIXED x0, y0, z0;
  s32 flip;

  work = Ram_HeapSlots->fieldWork;
  actor = Actor_GetById(ACTOR_SHIP);
  angle = (work->stepTrigger - CELL_BEHAVIOR_CHANNEL_EAST) << 14;
  Event_Start();
  Event_ResolvePendingActions(0);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  dx = FIXEDtoINT(Trig_Cos(angle));
  dz = FIXEDtoINT(Trig_Sin(angle));
  actor->speedLimit = toFIXED(4);
  actor->acceleration = toFIXED(4);
  targetZ = 0;
  targetX = 0;
  if (SeaOfTime_GetCellCode(1, x + (dz < 0 ? -dz : dz),
                            z + (dx < 0 ? -dx : dx)) >>
          8 ==
      work->stepTrigger) {
    targetX = (dz < 0 ? -dz : dz) * 15 << 16;
    targetZ = (dx < 0 ? -dx : dx) * 15 << 16;
  }
  targetX += FIELD_CELL_ORIGIN(x);
  targetZ += FIELD_CELL_ORIGIN(z);
  while (SeaOfTime_GetCellCode(1, x, z) >> 8 == work->stepTrigger) {
    x += dx;
    z += dz;
    targetX += FIELD_CELL_ORIGIN(dx);
    targetZ += FIELD_CELL_ORIGIN(dz);
    Actor_SetMoveTarget(actor, targetX, actor->y.fixed, targetZ);
    while (Actor_HasArrived(actor) == 0) {
      s32 phase = 3;
      phase &= gFrameCount;
      if (phase == 0) {
        if ((gFrameCount & 10) == 0) {
          Sound_Play(SE_CURRENT);
        }
        spark.priority = 2;
        spark.palette = 7;
        spark.startScaleX = FIXED_ONE * 7 / 5;
        spark.startScaleY = FIXED_ONE * 6 / 5;
        cosine = Trig_Cos(actor->facing);
        sine = Trig_Sin(actor->facing);
        pos[0] = cosine * 12;
        pos[1] = sine * 12;
        pos[2] = -(cosine * 12);
        pos[3] = -(sine * 12);
        pos[0] += actor->x.fixed;
        pos[1] += actor->z.fixed;
        pos[2] += actor->x.fixed;
        pos[3] += actor->z.fixed;
        if (pos[1] != actor->z.fixed) {
          pos[1] -= toFIXED(12);
          pos[3] -= toFIXED(12);
        }
        flip = gFrameCount & 4;
        y0 = actor->y.fixed;
        x0 = pos[0];
        z0 = pos[1];
        if (flip) {
          FieldEffect_Spawn(x0 + toFIXED(4), y0, z0, toFIXED(1) / 2, 0, 0,
                            EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                                EFFECT_USE_PALETTE,
                            &spark);
          FieldEffect_Spawn(pos[2] - toFIXED(4), actor->y.fixed, pos[3],
                            -toFIXED(1) / 2, 0, 0,
                            EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                                EFFECT_USE_PALETTE,
                            &spark);
        } else {
          FieldEffect_Spawn(x0 - toFIXED(4), y0, z0, -toFIXED(1) / 2, 0, 0,
                            EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                                EFFECT_USE_PALETTE,
                            &spark);
          FieldEffect_Spawn(
              pos[2] + toFIXED(4), actor->y.fixed, pos[3], toFIXED(1) / 2, 0, 0,
              EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
              &spark);
        }
      }
      wait(1);
    }
  }
  Event_End();
}

extern u16 gSeaOfTime_LapCount;
extern s16 gSeaOfTime_LapTurn;
extern u16 gSeaOfTime_LastLapCell;
extern struct Spots gSeaOfTime_LapSpots;
extern u8 gSeaOfTime_LapsNeeded[];

/// Tracks the ship around the ring of cells: counts steps made in one
/// direction, spawning an effect every fourth frame once past 3, and rewards a
/// full count.
void SeaOfTime_CountLaps(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(ACTOR_SHIP);
  s16 index;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
  u32 *frame;
  u32 *stamp;

  index = (SeaOfTime_GetCellCode(0, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed)) >>
           8) -
          1;
  frame = &gFrameCount;
  stamp = &gSeaOfTime_LapStamp;

  if (*frame - *stamp > 50) {
    gSeaOfTime_LapCount = 0;
    gSeaOfTime_LapTurn = 0;
  }
  last = gSeaOfTime_LastLapCell;
  now = work->stepTrigger;
  nowRaw = (u16)work->stepTrigger;
  if (last != now) {
    s16 *delta = &gSeaOfTime_LapTurn;
    s32 raw;
    s32 diff;
    u16 *count;
    s32 step;

    if (*delta == 0) {
      *delta = last - nowRaw;
    }
    if (*delta == 7) {
      *delta = -1;
    }
    raw = (u16)*delta;
    if ((s16)raw == -7) {
      *delta = 1;
    }
    diff = last - now;
    raw = (u16)*delta;
    step = (s16)raw;
    count = &gSeaOfTime_LapCount;
    if (diff == step || now - last == step * 7) {
      (*count)++;
      *stamp = *frame;
    } else {
      *count = 0;
      *delta = 0;
    }
  }
  gSeaOfTime_LastLapCell = work->stepTrigger;
  if (gSeaOfTime_LapCount > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gSeaOfTime_LapSpots;
      struct EffectOptions params;
      u32 z;
      u32 x;

      struct EffectOptions *p = &params;

      p->type = SPRITE_SPARKLE;
      p->priority = 2;
      x = Random16() & 31;
      z = Random16() & 31;
      Event_PlaySound(SE_GLITTER);
      x <<= 16;
      z <<= 16;
      FieldEffect_Spawn(spots.spot[(u16)index].x + x, 0,
                        spots.spot[(u16)index].z + z, 0, 0x13333, phase,
                        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1, p);
    }
  }
  if (gSeaOfTime_LapCount == gSeaOfTime_LapsNeeded[(u16)index]) {

    work->raisedTrigger = RAISED_START_WAVES;
    gSeaOfTime_LapCount = 0;
    gSeaOfTime_LapTurn = 0;
  }
}

/// A task while the waves run: ends them after their time is up and keeps their
/// start, length and the frame count for the scene's next visit.
void SeaOfTime_TrackWaves(void) {
  u32 *state = gSceneState;

  if (gFrameCount - gSeaOfTime_WaveStartFrame > 120)
    gFieldWork->raisedTrigger = RAISED_STOP_WAVES;
  *state++ = gSeaOfTime_WaveStartFrame;
  *state++ = gSeaOfTime_WaveState;
  *state = gFrameCount;
}

// Two fades of BG palette 3: toward RGB(11, 6, 23), then, in
// SeaOfTime_StopWaves, back to its saved colours.

extern u32 gSeaOfTime_WaveState;

/// Fades BG palette 3's entries 1 to 13 toward RGB(11, 6, 23), one unit per
/// channel every 10 frames, then hands the rest of the event to
/// SeaOfTime_TrackWaves's task.
void SeaOfTime_StartWaves(void) {
  volatile u16 *palette;
  s32 changed;
  s32 i;
  s32 frame;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_WAVES_RUNNING);
  Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = RGB_RED(color);
      u16 g = RGB_GREEN(color);
      u16 b = RGB_BLUE(color);

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    frame = gFrameCount;
    gSeaOfTime_WaveState += frame & 1;
  } while (changed);
  gSeaOfTime_WaveState = 360;
  gSeaOfTime_WaveStartFrame = frame;
  Task_Add(SeaOfTime_TrackWaves, TASK_PASS_VBLANK);
  Event_End();
}

struct SavedColors gSeaOfTime_SavedColors BSS_DATA;

/// Ends the waves: fades BG palette 3 back to its saved colours, one unit per
/// channel every 10 frames, and lets the channels and lap cells answer again.
void SeaOfTime_StopWaves(void) {
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Task_Remove(SeaOfTime_TrackWaves);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_RISE_WITH_SPARKS_HIGH);
  gSeaOfTime_WaveState = 1;
  // Each of the 14 colours steps one unit per channel toward its saved colour
  // every 10 frames, until none changes.
  do {
    wait(10);
    changed = 0;
    palette = &BG_PLTT_COLOR(3, 0);
    // i counts palette entries from 1 and j saved colours from 0; the saved
    // colours are read through a pointer set after i, each pass.
    for (i = 1, saved = &gSeaOfTime_SavedColors, j = 0; i != 15; i++, j++) {
      u16 color = palette[i];
      u16 target = saved->colors[j];
      u16 r = RGB_RED(color);
      u16 g = RGB_GREEN(color);
      u16 b = RGB_BLUE(color);
      u16 tr = RGB_RED(target);
      u16 tg = RGB_GREEN(target);
      u16 tb = RGB_BLUE(target);

      if (r < tr) {
        r++;
        changed = 1;
      }
      if (r > tr) {
        r--;
        changed = 1;
      }
      if (g < tg) {
        g++;
        changed = 1;
      }
      if (g > tg) {
        g--;
        changed = 1;
      }
      if (b < tb) {
        b++;
        changed = 1;
      }
      if (b > tb) {
        b--;
        changed = 1;
      }
      palette[i] = RGB(r, g, b);
    }
  } while (changed);
  gSeaOfTime_WaveState = 1;
  Event_ClearFlag(FLAG_WAVES_RUNNING);
  Event_End();
}

// next object the overlay links.

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// Sends actor 8 to a point 8 pixels along z and, until Actor_HasArrived says
/// it is done, raises a pair of effects at its two sides every fourth frame.
void SeaOfTime_SinkIntoWhirlpool(void) {
  FIXED sides[4];
  struct EffectOptions options;
  struct Actor *actor;
  FIXED cosine, sine;
  FIXED x, y, z;
  FIXED velocity, reverseVelocity;
  s32 alternate;

  actor = Actor_GetById(ACTOR_SHIP);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed,
                      actor->z.fixed + toFIXED(8));
  while (Actor_HasArrived(actor) == 0) {
    if ((gFrameCount & 3) == 0) {
      options.priority = 2;
      options.palette = 7;
      options.startScaleX = FIXED_ONE * 7 / 5;
      options.startScaleY = FIXED_ONE * 6 / 5;
      cosine = Trig_Cos(actor->facing);
      sine = Trig_Sin(actor->facing);
      sides[0] = cosine * 12;
      sides[1] = sine * 12;
      sides[2] = -(cosine * 12);
      sides[3] = -(sine * 12);
      sides[0] += actor->x.fixed;
      sides[1] += actor->z.fixed;
      sides[2] += actor->x.fixed;
      sides[3] += actor->z.fixed;
      velocity = toFIXED(1) / 2;
      if (sides[1] != actor->z.fixed) {
        sides[1] -= toFIXED(12);
        sides[3] -= toFIXED(12);
      }
      alternate = gFrameCount & 4;
      reverseVelocity = -velocity;
      x = sides[0];
      y = actor->y.fixed;
      z = sides[1];
      if (alternate) {
        FieldEffect_Spawn(x + toFIXED(4), y, z, velocity, 0, 0,
                          EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                              EFFECT_USE_START_SCALE,
                          &options);
        FieldEffect_Spawn(sides[2] - toFIXED(4), actor->y.fixed, sides[3],
                          reverseVelocity, 0, 0,
                          EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                              EFFECT_USE_START_SCALE,
                          &options);
      } else {
        FieldEffect_Spawn(x - toFIXED(4), y, z, reverseVelocity, 0, 0,
                          EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                              EFFECT_USE_START_SCALE,
                          &options);
        FieldEffect_Spawn(
            sides[2] + toFIXED(4), actor->y.fixed, sides[3], velocity, 0, 0,
            EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
            &options);
      }
    }
    wait(1);
  }
  Event_End();
}

/// The sea sweeps the ship back out: by exit 4 the first time, by exit 1 after.
void SeaOfTime_SweptBack(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_SWEPT_BACK_ONCE)) {
    Import_EventSetExitRequest(4);
    Event_SetFlag(FLAG_SWEPT_BACK_ONCE);
  } else {
    Import_EventSetExitRequest(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Every other frame, throws up spray 16 pixels south of the ship, at a random
/// place across its width.
void SeaOfTime_UpdateSpray(struct Actor *object) {
  struct EffectOptions spark;
  s32 even = gFrameCount & 1;

  if (even == 0) {
    FIXED dx = toFIXED(Random16() & 31) + -toFIXED(14);

    spark.startScaleX = FIXED_ONE * 7 / 10;
    spark.startScaleY = FIXED_ONE * 7 / 10;
    spark.palette = 7;
    spark.priority = 1;
    FieldEffect_Spawn(
        object->x.fixed + dx, 0, object->z.fixed + toFIXED(16), 0, even, even,
        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
        &spark);
  }
}

/// The way to Lemuria opens: the ship sails up, the sea's colours fade back,
/// puffs burst over the water for 360 frames, faster and faster, and 16 larger
/// ones follow.
void SeaOfTime_CrossToLemuria(void) {
  volatile u16 fill;
  struct EffectOptions burst;
  struct EffectOptions rain;
  struct Actor *object;
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_SWEPT_BACK_ONCE);
  Actor_SetSpeed(ACTOR_SHIP, 1);
  Actor_GetById(ACTOR_SHIP)->facing = FACING_NORTH;
  Actor_WalkTo(ACTOR_SHIP, 352, 264);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(352, CAMERA_KEEP, 196, 1);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(ACTOR_SHIP);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_RISE_WITH_SPARKS_HIGH);
  // Fills palette 3's entries 1 to 14 with one colour.
  fill = RGB(11, 6, 23);
  Dma_Set((const void *)&fill, (void *)&BG_PLTT_COLOR(3, 1),
          (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 14, REG_DMA3);
  MapGrid_CopyCellPictureIndicesRect(49, 70, 19, 73, 6, 6);
  // Steps each colour's red, green and blue by one toward the saved palette
  // every 10 frames until none changes.
  do {
    wait(10);
    changed = 0;
    palette = &BG_PLTT_COLOR(3, 0);
    // i counts palette entries from 1 and j saved colours from 0; the saved
    // colours are read through a pointer set after i, each pass.
    for (i = 1, saved = &gSeaOfTime_SavedColors, j = 0; i != 15; i++, j++) {
      u16 color = palette[i];
      u16 target = saved->colors[j];
      u16 r = RGB_RED(color);
      u16 g = RGB_GREEN(color);
      u16 b = RGB_BLUE(color);
      u16 tr = RGB_RED(target);
      u16 tg = RGB_GREEN(target);
      u16 tb = RGB_BLUE(target);

      if (r < tr) {
        r++;
        changed = 1;
      }
      if (r > tr) {
        r--;
        changed = 1;
      }
      if (g < tg) {
        g++;
        changed = 1;
      }
      if (g > tg) {
        g--;
        changed = 1;
      }
      if (b < tb) {
        b++;
        changed = 1;
      }
      if (b > tb) {
        b--;
        changed = 1;
      }
      palette[i] = RGB(r, g, b);
    }
  } while (changed);
  Event_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then every
  // other frame, and plays sound 164 every 64 frames.
  for (i = 0; i != 360; i++) {
    if ((i & 63) == 0)
      Sound_Play(SE_SCATTER);
    if (i < 120) {
      if (i & 15)
        continue;
    } else if (i < 240) {
      if (i & 7)
        continue;
    } else if (i & 1)
      continue;
    {
      s32 x = Random16();
      s32 mask = 31;
      s32 z = Random16();

      burst.palette = 7;
      FieldEffect_Spawn(toFIXED(x & mask) + toFIXED(336), 0,
                        toFIXED(z & mask) + toFIXED(176), 0, 0, 0,
                        EFFECT_USE_PALETTE, &burst);
      wait(1);
    }
  }
  // Then 16 larger effects, one a frame.
  i = 0;
  while (i != 16) {
    {
      s32 x = Random16();
      s32 mask = 31;
      s32 z = Random16();
      FIXED offsetZ = toFIXED(z & mask);
      FIXED offsetX = toFIXED(x & mask);

      rain.startScaleX = FIXED_ONE * 3 / 2;
      rain.startScaleY = FIXED_ONE * 3 / 2;
      rain.palette = 7;
      rain.priority = 1;
      FieldEffect_Spawn(
          offsetX + toFIXED(336), 0, offsetZ + toFIXED(176), 0, toFIXED(1), 0,
          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &rain);
      wait(1);
    }
    i++;
  }
  object = Import_ActorCreate(SPRITE_POSEIDON, toFIXED(348), 0, toFIXED(184));
  object->update = (void (*)(void))SeaOfTime_UpdateSpray;
  Event_Wait(60);
  Actor_SelectAnimation(object, 1);
  Event_Wait(40);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination(SCENE_SEA_OF_TIME_TO_LEMURIA, 4);
  Field_RequestBattleFromEncounterTable(12, 6);
  Event_End();
}

/// Grind clears a rock: its 2 by 2 cells open and its actor is put away.
void SeaOfTime_RemoveRock(s32 x, s32 z, s32 actor) {
  s32 cx = FIELD_CELL(x);
  s32 cz = FIELD_CELL(z);

  SeaOfTime_SetCellCode(1, cx - 1, cz - 1, 0);
  SeaOfTime_SetCellCode(1, cx, cz - 1, 0);
  SeaOfTime_SetCellCode(1, cx - 1, cz, 0);
  SeaOfTime_SetCellCode(1, cx, cz, 0);
  Actor_SetPosition(actor, 0, 0);
}

#define DEFINE_GRIND_ROCK(name, rock, flag)                                    \
  void name(void) {                                                            \
    struct Actor *object;                                                      \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    object = Actor_GetById(rock);                                              \
    SeaOfTime_RemoveRock(object->x.fixed, Actor_GetById(rock)->z.fixed, rock); \
    GameFlag_Set(flag);                                                        \
    Event_End();                                                               \
  }

DEFINE_GRIND_ROCK(SeaOfTime_GrindFirstRock, ACTOR_ROCK_A, FLAG_ROCK_A_CLEARED)

DEFINE_GRIND_ROCK(SeaOfTime_GrindSecondRock, ACTOR_ROCK_B, FLAG_ROCK_B_CLEARED)

DEFINE_GRIND_ROCK(SeaOfTime_GrindThirdRock, ACTOR_ROCK_C, FLAG_ROCK_C_CLEARED)

DEFINE_GRIND_ROCK(SeaOfTime_GrindFourthRock, ACTOR_ROCK_D, FLAG_ROCK_D_CLEARED)

DEFINE_GRIND_ROCK(SeaOfTime_GrindFifthRock, ACTOR_ROCK_E, FLAG_ROCK_E_CLEARED)

DEFINE_GRIND_ROCK(SeaOfTime_GrindLastRock, ACTOR_ROCK_A, FLAG_LAST_ROCK_CLEARED)

#define DEFINE_LEAVE_BY_EXIT(name, exit)                                       \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Import_EventSetExitRequest(exit);                                          \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
    Event_End();                                                               \
  }

DEFINE_LEAVE_BY_EXIT(SeaOfTime_LeaveByExit4, 4)

DEFINE_LEAVE_BY_EXIT(SeaOfTime_LeaveByExit5, 5)

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_SEA_OF_TIME_WHIRLPOOL)
    return gSeaOfTime_Room1Triggers;
  if (gMainRoutineWork.entrance <= 4)
    return gSeaOfTime_EntranceTriggers;
  return gSeaOfTime_Triggers;
}

/// The Sea of Time's frame callback: rotates two runs of the background
/// palette, pulses the red of colour 15 (or steps it with the alarm), copies
/// the palette and ripples the layer's horizontal scroll line by line.
void SeaOfTime_AnimateSea(void) {
  u16 *line = gSeaOfTime_RippleLines;
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[2];
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 i;
  s16 color;

  if (event->psynergy != 0)
    return;
  if (fade->duration != 0)
    return;
  if (event->transitionActive == 0)
    return;

  if (gFrameCount - gSeaOfTime_WaveStartFrame > gSeaOfTime_WaveState) {
    s16 target;

    color = BG_PLTT[62];
    target = gSeaOfTime_SavedColors.colors[13];

    for (i = 14; i != 0; i--) {
      BG_PLTT[48 + i] = BG_PLTT[47 + i];
      gSeaOfTime_SavedColors.colors[i - 1] =
          gSeaOfTime_SavedColors.colors[i - 2];
    }
    BG_PLTT[49] = color;
    gSeaOfTime_SavedColors.colors[0] = target;
    gSeaOfTime_WaveStartFrame = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    color = BG_PLTT[78];

    for (i = 14; i != 0; i--)
      BG_PLTT[64 + i] = BG_PLTT[63 + i];
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      u16 red = RGB_RED(color);
      u16 green = RGB_GREEN(color);
      u16 blue = RGB_BLUE(color);

      if (GameFlag_Get(FLAG_SCENE_TEMP_00) == 0) {
        red += gSeaOfTime_RedPulseStep[0];
        if (red < 18) {
          red = 18;
          gSeaOfTime_RedPulseStep[0] = 1;
        } else if (red > 31) {
          red = RGB_LEVEL_MAX;
          gSeaOfTime_RedPulseStep[0] = -1;
        }
      } else if (gFieldWork->raisedTrigger == RAISED_START_WAVES) {
        if (red != 0)
          red--;
      } else if (gFieldWork->raisedTrigger == RAISED_STOP_WAVES) {
        if (red < 30)
          red++;
      }
      BG_PLTT[15] = RGB(red & RGB_LEVEL_MAX, green, blue);
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->paletteFade,
          DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  layer->speedY = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (s16)FIXEDtoINT(layer->x) +
        (Trig_Sin((i + *(u8 *)&gFrameCount + (s16)FIXEDtoINT(layer->y)) << 9) >>
         15);
  line = gSeaOfTime_RippleLines;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS,
          (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 |
              1,
          (volatile u32 *)REG_DMA0);
}

/// Places a rock (x and z in 8-pixel steps) and walls off its 2 by 2 cells.
void SeaOfTime_PlaceRock(s32 x, s32 z, s32 actor) {
  Actor_GetById(actor)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  Actor_SetPositionFX(actor, (x << 19) + toFIXED(8), (z << 19) + toFIXED(8));
  x >>= 1;
  z >>= 1;
  SeaOfTime_SetCellCode(1, x, z, CELL_BEHAVIOR_WALL << 8);
  SeaOfTime_SetCellCode(1, x + 1, z, CELL_BEHAVIOR_WALL << 8);
  SeaOfTime_SetCellCode(1, x, z + 1, CELL_BEHAVIOR_WALL << 8);
  SeaOfTime_SetCellCode(1, x + 1, z + 1, CELL_BEHAVIOR_WALL << 8);
}

/// A rock rises out of the sea: the camera goes to it, the screen shakes, water
/// splashes and the rock grows up out of it.
void SeaOfTime_RaiseRock(s32 x, s32 z, s32 actor) {
  struct EffectOptions spark;
  FIXED px;
  FIXED pz;
  s32 i;

  FieldCamera_SetSpeed(6);
  px = (x << 19) + toFIXED(8);
  pz = (z << 19) + toFIXED(8);
  FieldCamera_MoveToFX(px, -1, pz, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_CLATTER);
  MapGrid_SetShake(0, 16, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  spark.startScaleX = FIXED_ONE * 6 / 5;
  spark.startScaleY = FIXED_ONE * 3 / 2;
  spark.priority = 1;
  spark.type = SPRITE_SPLASH;
  FieldEffect_Spawn(
      px, 0, pz, 0, 0, 0,
      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &spark);
  Actor_GetById(actor)->scaleY = FIXED_ONE / 10;
  SeaOfTime_PlaceRock(x, z, actor);
  for (i = 0; i != 10; i++) {
    Actor_GetById(actor)->scaleY += FIXED_ONE / 10;
    Event_Wait(1);
  }
  Actor_GetById(actor)->scaleY = FIXED_ONE;
  Event_Wait(20);
}

/// The Sea of Time's opening: places the camera and the first effects, showers
/// sparks for 360 frames and larger effects for 16, then fades BG palette 3 to
/// RGB(11, 6, 23).
void SeaOfTime_Open(void) {
  struct Actor *actor;
  u32 i;
  s32 changed;
  volatile u16 *palette;
  struct EffectOptions params;
  struct EffectOptions burst;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(MUS_SILENCE);
  MapGrid_CopyCellsTo(49, 70, 19, 73, 6, 6);
  actor = FieldActor_Create(SPRITE_POSEIDON, toFIXED(348), 0, toFIXED(184));
  actor->update = (void (*)(void))SeaOfTime_UpdateSpray;
  FieldCamera_MoveTo(352, CAMERA_KEEP, 196, 0);
  Event_StartScreenTransition();
  wait(1);
  Task_Add(SeaOfTime_AnimateSea, TASK_PASS_FRAME);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  SeaOfTime_RaiseRock(21, 23, 9);
  SeaOfTime_RaiseRock(37, 17, 10);
  SeaOfTime_RaiseRock(47, 27, 12);
  SeaOfTime_RaiseRock(59, 19, 13);
  MapGrid_WaitForShakeToSettle();
  FieldCamera_MoveTo(352, CAMERA_KEEP, 196, 1);
  FieldCamera_WaitForArrival();
  Event_SetFlag(FLAG_WAVES_RUNNING);
  gFieldWork->raisedTrigger = RAISED_START_WAVES;
  Event_PlaySound(SE_SEA_OF_TIME_OPENS);
  Event_Wait(40);
  for (i = 0; i != 360; i++) {
    if ((i & 31) == 0)
      Event_PlaySound(SE_SCATTER);
    if (i < 120) {
      if (i & 15)
        continue;
    } else if (i < 240) {
      if (i & 7)
        continue;
    } else if (i & 1)
      continue;
    {
      s32 mask = 31;
      s32 x = Random16();
      s32 y = Random16();

      params.startScaleX = FIXED_ONE * 4 / 5;
      params.startScaleY = FIXED_ONE * 4 / 5;
      params.priority = 1;
      params.palette = 7;
      x &= mask;
      x <<= 16;
      x += toFIXED(336);
      FieldEffect_Spawn(x, 0, toFIXED(y & mask) + toFIXED(176), 0, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                            EFFECT_USE_PALETTE,
                        &params);
      wait(1);
    }
  }
  for (i = 0; i != 16; i++) {
    s32 mask = 31;
    s32 x = Random16();
    s32 y = Random16();

    burst.startScaleX = FIXED_ONE * 3 / 2;
    burst.startScaleY = FIXED_ONE * 3 / 2;
    burst.palette = 7;
    burst.priority = 1;
    x &= mask;
    x <<= 16;
    x += toFIXED(336);
    FieldEffect_Spawn(x, 0, toFIXED(y & mask) + toFIXED(176), 0, toFIXED(1), 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE,
                      &burst);
    wait(1);
  }
  Actor_Release(actor);
  Event_PlaySound(SE_RISE_WITH_SPARKS_LOW);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = RGB_RED(color);
      u16 g = RGB_GREEN(color);
      u16 b = RGB_BLUE(color);

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    gSeaOfTime_WaveState += gFrameCount & 1;
  } while (changed);
  Task_Remove(SeaOfTime_AnimateSea);
  Scene_PlayMusic();
  Event_SetFlag(FLAG_SEA_OF_TIME_OPENED);
  Event_End();
}

// The overlay's start: sets the screen effect, restores the clock and palette
// in the first room, draws the unlit cells in the second, and schedules the
// frame callback.

/// The overlay's start: sets the screen effect, restores the clock and palette
/// in the first room, draws the unlit cells in the second, and schedules the
/// frame callback.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineWork.next == SCENE_SEA_OF_TIME_WHIRLPOOL) {
    gSeaOfTime_WaveStartFrame = gFrameCount;
    gSeaOfTime_WaveState = 1;
    Dma_Set((const void *)&BG_PLTT_COLOR(3, 1), gSeaOfTime_SavedColors.colors,
            DMA_COPY_HALFWORDS(14), REG_DMA3);
    if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
      u32 *state = gSceneState;
      u32 offset, i;

      gSeaOfTime_WaveStartFrame = *state++;
      gSeaOfTime_WaveState = *state++;
      gFrameCount = *state;
      i = 1;
      offset = 98;
      do {
        *(volatile u16 *)(offset + 0x05000000) = RGB(11, 6, 23);
        i++;
        offset += 2;
      } while (i != 14);
      Task_Add(SeaOfTime_TrackWaves, TASK_PASS_VBLANK);
    }
  } else if (gMainRoutineWork.next == SCENE_SEA_OF_TIME_TO_LEMURIA) {
    gSeaOfTime_WaveStartFrame = gFrameCount;
    gSeaOfTime_WaveState = 0;
    if (gMainRoutineWork.entrance <= 4) {
      if (!GameFlag_Get(FLAG_SEA_OF_TIME_OPENED)) {
        Dma_Set((const void *)&BG_PLTT_COLOR(3, 1),
                gSeaOfTime_SavedColors.colors, DMA_COPY_HALFWORDS(14),
                REG_DMA3);
      } else {
        if (!GameFlag_Get(FLAG_ROCK_A_CLEARED))
          SeaOfTime_PlaceRock(21, 23, ACTOR_ROCK_A);
        if (!GameFlag_Get(FLAG_ROCK_B_CLEARED))
          SeaOfTime_PlaceRock(37, 17, ACTOR_ROCK_B);
        if (!GameFlag_Get(FLAG_ROCK_D_CLEARED))
          SeaOfTime_PlaceRock(47, 27, ACTOR_ROCK_D);
        if (!GameFlag_Get(FLAG_ROCK_E_CLEARED))
          SeaOfTime_PlaceRock(59, 19, ACTOR_ROCK_E);
        {
          void *block = Ram_HeapSlots->paletteFade;

          BG_PLTT_COLOR(0, 15) &= 0xffe0;
          Dma_Set((const void *)BG_PLTT, block, DMA_COPY_WORDS(112), REG_DMA3);
        }
        PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
      }
      if (gMainRoutineWork.entrance == 4 &&
          !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        SeaOfTime_Open();
    } else {
      if (!GameFlag_Get(FLAG_LAST_ROCK_CLEARED))
        SeaOfTime_PlaceRock(89, 85, ACTOR_ROCK_A);
    }
  }
  {
    s32 priority = TASK_PASS_FRAME;

    Task_Add(SeaOfTime_AnimateSea, priority);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  void *block = Ram_HeapSlots->paletteFade;

  Dma_Fill32(-1, block, 0x1c0);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  return 0;
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u8 gSeaOfTime_LapsNeeded[] = {16, 24, 16, 24, 16, 24};

struct Spots gSeaOfTime_LapSpots = {{
    {toFIXED(672), toFIXED(704)},
    {toFIXED(896), toFIXED(320)},
    {toFIXED(512), toFIXED(192)},
    {toFIXED(320), toFIXED(416)},
    {toFIXED(320), toFIXED(832)},
    {toFIXED(96), toFIXED(160)},
}};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSeaOfTime_ShipRests[] = {
    SC_CALL((s32)SeaOfTime_ResetAnimationFrame),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(SeaOfTimeWhirlpool),
    SCENE_EXIT_UNLESS(1, WorldMap, 42, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, SeaOfTimeToLemuria, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, SeaOfTimeWhirlpool, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemurianShipTiller, 20, EXIT_FLAG_NONE),
    SCENE_EXITS(SeaOfTimeToLemuria),
    SCENE_EXIT_UNLESS(1, SeaOfTimeWhirlpool, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, SeaOfTimeToLemuria, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, WorldMap, 44, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, WorldMap, 43, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, SeaOfTimeToLemuria, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSeaOfTime_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)gSeaOfTime_ShipRests,
                    120, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSeaOfTime_Room2Placements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)gSeaOfTime_ShipRests,
                    120, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u16 gSeaOfTime_LastLapCell = 0;

u16 gSeaOfTime_LapCount = 0;

s16 gSeaOfTime_LapTurn = 0;

u32 gSeaOfTime_LapStamp = 0;

volatile u32 gSeaOfTime_WaveStartFrame = 0;

u32 gSeaOfTime_WaveState = 0;

struct Trigger gSeaOfTime_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_SWEPT_BACK, CONDITION_ALWAYS,
     EVENT(SeaOfTime_SweptBack)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTHWEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_WEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_EAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTHEAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_COUNTERCLOCKWISE, CONDITION_ALWAYS,
     EVENT(SeaOfTime_CircleSwirl)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_CLOCKWISE, CONDITION_ALWAYS,
     EVENT(SeaOfTime_CircleSwirl)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WHIRLPOOL, CONDITION_ALWAYS,
     EVENT(SeaOfTime_SinkIntoWhirlpool)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_EAST, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_SOUTH, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_WEST, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 1, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 2, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 3, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 4, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 5, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 6, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LAP_FIRST + 7, FLAG_WAVES_RUNNING,
     EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_RAISED, RAISED_START_WAVES, CONDITION_ALWAYS,
     EVENT(SeaOfTime_StartWaves)},
    {TRIGGER_RAISED, RAISED_STOP_WAVES, CONDITION_ALWAYS,
     EVENT(SeaOfTime_StopWaves)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSeaOfTime_EntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 5, FLAG_SEA_OF_TIME_OPENED, EVENT(SeaOfTime_CrossToLemuria)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTHWEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_WEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_EAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT_NORTHEAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SWIRL_CLOCKWISE, CONDITION_ALWAYS,
     EVENT(SeaOfTime_CircleSwirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_A, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_B, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_C, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindThirdRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_D, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFourthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_E, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFifthRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSeaOfTime_Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(SeaOfTime_LeaveByExit4)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(SeaOfTime_LeaveByExit5)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_EAST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_SOUTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_WEST, CONDITION_ALWAYS,
     EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CHANNEL_NORTH, CONDITION_ALWAYS,
     EVENT(SeaOfTime_FollowChannel)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ROCK_A, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindLastRock)},
    {TRIGGER_TABLE_END},
};

s16 gSeaOfTime_RedPulseStep[] = {
    1,
};
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
