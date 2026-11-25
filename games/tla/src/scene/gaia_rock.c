/// @file
/// The outside of Gaia Rock, the great rock near Izumo where the Serpent
/// lives, in three areas of this overlay.
///
/// The path from the world map runs north under stone arches to the cliffs,
/// where a dark cave mouth leads into the rock (gaia_rock_inside.c); the way
/// up is outside. On the first cliffs (SceneId_GaiaRockSusa) Felix pushes a
/// stone pillar west along a ledge until it drops into a gap and bridges it
/// (GaiaRock_FinishMovingPillar), climbs walls of handholds and rides slides,
/// and sees a stone statue blow whirlwinds at a hanging vine to set it
/// swinging. Whirlwind on another vine, over a gap, swings him across, and a
/// second pillar Moved east off the ledge beyond drops into a notch below. On
/// the next cliffs (SceneId_GaiaRockStatueSpots) more statues blow at vines,
/// and on a great wall of handholds three stone faces lie in wait: when Felix
/// climbs onto one it bursts out under him and knocks him off the holds
/// (GaiaRock_ClimbOntoLowestStoneFace). Beyond, a pillar slides in a slot, and
/// on the mesa past it Reveal shows the Dancing Idol on an altar ringed by
/// pillars (GaiaRock_RevealIdol), for Uzume in Izumo. Once the party has
/// fought the Serpent inside, they come out to find Susa lying worn out by
/// the cave mouth, and he asks whether they will tell Kushinada they slew it
/// (GaiaRock_SusaAsksAboutTheSerpent).

#include "shared/tables.h"
#include "shared/gaia_rock.h"
#include "scene/event_layer.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
/// Set once Susa has said he only needs rest ("I'm not dying... I'm just so
/// tired...").
#define FLAG_SUSA_RESTED 0xa61
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/actor.h"
#include "version.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "system/iwram_call.h"
#include "field/hanging_vine.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "system/task.h"
#include "field/actor_view.h"
#include "field/event_commands.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

/// The first cliffs, from the world map's path to the cave mouth
/// (SceneId_GaiaRockSusa): the eight hanging vines, the two pillars Moved into
/// the gaps, the statue that blows whirlwinds, a whirlwind's row parked off the
/// map (the whirlwinds the statue blows are spawned), and Susa, who lies by
/// the cave mouth after the Serpent ("Kushinada's safe... She's safe! Nothing
/// else matters.").
enum {
  ACTOR_FIRST_VINE = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_VINE + 8,
  ACTOR_WIND_STATUE,
  ACTOR_WHIRLWIND,
  ACTOR_SECOND_PILLAR,
  ACTOR_SUSA
};

/// The next cliffs, with the great wall of handholds
/// (SceneId_GaiaRockStatueSpots): seven hanging vines, two statues that blow
/// wind, and the three stone faces that burst out of the wall under Felix.
enum {
  ACTOR_FIRST_RISING_STATUE = ACTOR_FIRST_VINE + 9,
  ACTOR_SECOND_RISING_STATUE,
  ACTOR_THIRD_RISING_STATUE
};

/// The top, with the altar's mesa (SceneId_GaiaRockDancingIdol): the pillar
/// that slides in its slot, the altar and the Dancing Idol on it.
enum {
  ACTOR_PUSHED_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_ALTAR = ACTOR_FIRST_PLACED + 7,
  ACTOR_DANCING_IDOL
};

/// Set once each stone face has burst out of the great wall, so that it stays
/// out until the party enters another area.
#define FLAG_FIRST_STATUE_RISEN FLAG_AREA_TEMP_00
#define FLAG_SECOND_STATUE_RISEN FLAG_AREA_TEMP_01
#define FLAG_THIRD_STATUE_RISEN FLAG_AREA_TEMP_02

/// The two pillars Moved into the gaps: each stands sunk into its gap, its top
/// level with the ledge it was Moved along, for good.
#define FLAG_FIRST_PILLAR_DROPPED 0x920
#define FLAG_SECOND_PILLAR_DROPPED 0x921

/// The cell behaviors of the trigger tables.
enum GaiaRockCellBehavior {
  /// The tufts of weeds below the cliff east of the cave mouth, which Cyclone
  /// blows away.
  CELL_BEHAVIOR_CAVE_MOUTH_WEEDS = 20,
  /// The great wall's three trap holds, lowest first, in its overhead layer
  /// (GaiaRock_TriggerWallCell reads them as Felix climbs).
  CELL_BEHAVIOR_LOWEST_STONE_FACE = 20,
  CELL_BEHAVIOR_MIDDLE_STONE_FACE,
  CELL_BEHAVIOR_HIGHEST_STONE_FACE,
  /// The path just south of where Susa lies by the cave mouth.
  CELL_BEHAVIOR_PAST_SUSA = 42
};

/// The passages within the first two areas. Each of their maps holds two
/// parts, each with a camera of its own: on the first cliffs, the path to the
/// cave mouth in the south and the cliffs above it in the north; on the next
/// cliffs, the cliffs of vines in the west and the great wall under the two
/// wind statues in the east. A passage's exit and the entrance it leads to.
enum GaiaRockPassage {
  GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH = 2,
  GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS,
  GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH = 2,
  GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS,
  GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL = 2,
  GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES,
  GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL = 2,
  GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES
};

/// Whether the party arrives on the next cliffs' great wall side: by its
/// passage, or down from the mesa (the entrance after it).
#define ARRIVES_AT_GREAT_WALL()                                                \
  (gMainRoutineState.entrance >= GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES &&     \
   gMainRoutineState.entrance <= GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES + 1)

/// The whirlwind's circling leaves.
enum { WHIRLWIND_LEAVES = 16 };

struct Actor;

extern struct SceneRegion gGaiaRock_SusaExitRegions[];
extern struct SceneRegion gGaiaRock_StatueSpotsExitRegions[];

void StoneHead_BurstOut(s32 id, s32 flag);

extern struct ActorPlacement gGaiaRock_Placements[];
extern struct ActorPlacement gGaiaRock_SusaActorPlacements[];

extern struct ActorPlacement gGaiaRock_StatueSpotsActorPlacements[];
extern struct ActorPlacement gGaiaRock_DancingIdolActorPlacements[];

extern struct Trigger gGaiaRock_SusaTriggers[];
extern struct Trigger gGaiaRock_StatueSpotsTriggers[];
extern struct Trigger gGaiaRock_Triggers[];
extern u32 gGaiaRock_LeafScript[];

extern s32 gGaiaRock_WindScript[];

void GaiaRock_StartWallTriggers(void);
void StoneHead_Place(s32 id, s32 x, s32 y, s32 z);

extern s32 gStoneHead_RiseScript[];

extern u32 gDebris_Script[];
extern u32 gGaiaRock_PillarScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_SUSA)
    return gGaiaRock_SusaExitRegions;
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return gGaiaRock_StatueSpotsExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_SUSA)
    return gGaiaRock_SusaActorPlacements;
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return gGaiaRock_StatueSpotsActorPlacements;
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_DANCING_IDOL)
    return gGaiaRock_DancingIdolActorPlacements;
  return gGaiaRock_Placements;
}

/// Climbing the great wall of handholds, Felix reaches the lowest of its three
/// trap holds, near the middle of the wall: a stone face bursts out of the
/// rock under him in a shower of stones, and he flails and loses his hold on
/// the wall: he is left standing on the face, and when the player climbs on he
/// drops off it, facing out, down the wall to its foot. The hold is cell
/// (111, 50) of the wall's overhead layer, drawn at (47, 50); the face stays
/// out, and the hold is solid rock, until the party enters another area.
void GaiaRock_ClimbOntoLowestStoneFace(void) {
  StoneHead_BurstOut(ACTOR_FIRST_RISING_STATUE, FLAG_FIRST_STATUE_RISEN);
}

/// As GaiaRock_ClimbOntoLowestStoneFace, for the middle trap hold, a little
/// higher and to the east: cell (113, 42) of the overhead layer, drawn at
/// (49, 42).
void GaiaRock_ClimbOntoMiddleStoneFace(void) {
  StoneHead_BurstOut(ACTOR_SECOND_RISING_STATUE, FLAG_SECOND_STATUE_RISEN);
}

/// As GaiaRock_ClimbOntoLowestStoneFace, for the highest trap hold, a little
/// west of the middle one: cell (112, 33) of the overhead layer, drawn at (48,
/// 33).
void GaiaRock_ClimbOntoHighestStoneFace(void) {
  StoneHead_BurstOut(ACTOR_THIRD_RISING_STATUE, FLAG_THIRD_STATUE_RISEN);
}

/// The Japanese edition lets a pillar land with no sound, through the routine
/// of Taopo Swamp and the Kibombo Mountains, which tests for landing before it
/// counts the frame down; the later editions take the Ankohl Ruins' routine and
/// play SE_BOOM as it lands.
#if !TLA_PATCH_1
#include "shared/actor_wait_to_land.h"
#else
#include "shared/actor_wait_to_land_with_cue.h"
#endif

/// Nothing visible happens as Felix's Move takes hold of the stone pillar on
/// the west ledge of the first cliffs, standing at cell (7, 27); the game
/// changes one cell of the map's lower half beside it so that it can slide.
/// GaiaRock_FinishMovingPillar changes it back.
void GaiaRock_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(7, 90, 1, 1, 8, 91);
}

#if !TLA_PATCH_1
#define DROP_PILLAR(id) Actor_WaitToLand(id)
#else
#define DROP_PILLAR(id) Actor_WaitToLandWithCue(id, SE_BOOM)
#endif

/// A pillar of the first cliffs stops after a push or a Move. The pillar on
/// the west ledge, once pushed or Moved west over the gap at cell (5, 27),
/// drops in, its top level with the ledge, so that Felix can walk
/// across. The pillar on the ledge past the swinging vine, at cell (37, 46),
/// once Moved east off the ledge's end drops onto the ledge below, and one
/// Move more sets it for good in the notch where a grey block shows, cell
/// (40, 46), its top a square Felix hops across. (The walkthrough says to Move
/// it to the left; the game takes it east.)
void GaiaRock_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  MapGrid_CopyCellAttributes(8, 90, 1, 1, 8, 91);
  if (id == ACTOR_FIRST_PILLAR &&
      Event_GetFlag(FLAG_FIRST_PILLAR_DROPPED) == 0 &&
      FIELD_HALF_CELL(actor->x.fixed) <= 11) {
    DROP_PILLAR(ACTOR_FIRST_PILLAR);
    Event_SetFlag(FLAG_FIRST_PILLAR_DROPPED);
    MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
  }
  if (id == ACTOR_SECOND_PILLAR &&
      Event_GetFlag(FLAG_SECOND_PILLAR_DROPPED) == 0) {
    if (FIELD_HALF_CELL(actor->x.fixed) == 77)
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
    if (FIELD_HALF_CELL(actor->x.fixed) == 79 &&
        FIELD_HALF_CELL(actor->y.fixed) == 12) {
      DROP_PILLAR(ACTOR_SECOND_PILLAR);
      MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
    }
    if (FIELD_HALF_CELL(actor->x.fixed) == 81) {
      Event_SetFlag(FLAG_SECOND_PILLAR_DROPPED);
      MapGrid_CopyCellAttributes(41, 46, 1, 1, 39, 46);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    }
  }
}

/// Nothing visible happens as Felix starts to push or Move the stone pillar at
/// the east edge of the summit where he arrives, which slides north and south
/// in its slot (cells (13, 14) to (13, 18)); the game frees the slot's cell
/// under it so that it can slide.
void GaiaRock_StartPushingPillar(void) {
  MapGrid_CopyCellAttributes(
      13, 18, 1, 1, 13, FIELD_CELL(Actor_Get(ACTOR_PUSHED_PILLAR)->z.fixed));
}

/// Nothing visible happens when the pillar in its slot at the east edge of the
/// first summit stops; the game makes its cell a square Felix hops across, so
/// that he can jump over the pillar's top. Moved to the slot's south end, cell
/// (13, 18), it opens the way on to the altar.
void GaiaRock_FinishPushingPillar(void) {
  MapGrid_CopyCellAttributes(
      11, 14, 1, 1, 13, FIELD_CELL(Actor_Get(ACTOR_PUSHED_PILLAR)->z.fixed));
}

/// Felix casts Cyclone among the tufts of weeds below the cliff east of the
/// cave mouth (cells (49, 48), (51, 48), (50, 49), (58, 48), (59, 47) and a few
/// more), and its whirl of leaves blows away the tufts within its reach: cast
/// from cell (50, 50), the two either side of the sapling below the cave
/// mouth; from cell (57, 49), the one just east of Felix. The thick bushes
/// south of them stay as they were. The game takes each tuft's picture from
/// the overhead layer's block at cells (110, 64) to (123, 75) of map.txt into
/// its place, in the block from (110, 47) that the overhead layer shows here.
void GaiaRock_CycloneBlowsAwayWeeds(void) {
  FieldTileCopies_CopyTouchedCells(110, 64, 14, 12, 110, 47, 1);
}

/// Casting Reveal at the top of the cliffs shows the Dancing Idol standing on
/// the altar, on the mesa ringed by six pillars, until the party has taken it.
void GaiaRock_RevealIdol(void) {
  struct Actor *actor = Actor_Get(FIELD_TREASURE_ACTOR);
  s32 done;

  if (actor == NULL)
    return;
  done = Event_GetFlag(FLAG_DANCING_IDOL_TAKEN);
  if (done == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 657, 246);
    actor->motionFlags = done;
    actor->y.fixed = toFIXED(80);
  }
}

/// As Reveal wears off, the Dancing Idol vanishes from the altar again.
void GaiaRock_HideIdol(void) {
  if (Actor_Get(FIELD_TREASURE_ACTOR) != NULL)
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 0, 0);
}

/// Felix examines the altar on the mesa ringed by six pillars: "Felix checked
/// the altar." With the Dancing Idol revealed on it, he takes it. Before that
/// he says "I'm...sensing something..." and the field menu opens, a hint to
/// cast Reveal; once it is taken, "I can't feel anything...".
void GaiaRock_CheckAltar(void) {
  struct Actor *actor = Actor_Get(FIELD_TREASURE_ACTOR);

  Message_ShowCentered(MsgCheckedTheAltar, 1);
  if (actor != NULL && actor->x.fixed != 0) {
    Scene_TalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
  } else if (Event_GetFlag(FLAG_DANCING_IDOL_TAKEN) == 0) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    Message_ShowCentered(MsgIMSensing, 1);
    state->fieldMenu = 1;
  } else {
    Message_ShowCentered(MsgICanT, 1);
  }
}

/// Once the Serpent is slain, Felix walks past Susa, who lies by the cave mouth
/// (on the cells just south of him, (45, 52) to (50, 52)), and Susa asks:
/// "You're going to tell Kushinada that you defeated the serpent, aren't
/// you?" Yes: "Ha ha ha! Of course!" No: he says they should accept the
/// credit. It plays once.
void GaiaRock_SusaAsksAboutTheSerpent(void) {
  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_SetMessageId(MsgYouReGoing);
    FieldActor_StartRepeatedMotion(ACTOR_SUSA, 1);
    FieldEvent_Wait(15);
    Event_AskYesNo(ACTOR_SUSA, 0);
    Event_SetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE);
    Event_End();
  }
}

/// Talking to Susa where he lies by the cave mouth: before he has asked about
/// the serpent, "If you're going to Izumo, tell my sister that I'm still
/// alive."; after, he asks again.
void GaiaRock_TalkToSusa(void) {
  if (Event_GetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
    FieldEvent_SetMessageId(MsgYouReGoing);
    Event_AskYesNo(ACTOR_SUSA, 0);
  } else {
    FieldEvent_SetMessageId(MsgIfYouRe);
    // "If you're going to Izumo, tell my sister that I'm still alive."
    Event_ShowMessage(ACTOR_SUSA, 0);
  }
}

/// Mind Read on Susa where he lies by the cave mouth: "I really got worked
/// over, but as long as Kushinada's safe, it was worth it." before he has
/// asked about the serpent, "Kushinada's safe... She's safe!" after.
void GaiaRock_MindReadSusa(void) {
  if (Event_GetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
    FieldEvent_SetMessageId(MsgKushinadaSSafe);
    // "Kushinada's safe... She's safe! Nothing else matters."
    Event_ShowMessage(ACTOR_SUSA, 0);
  } else {
    FieldEvent_SetMessageId(MsgIReallyGot);
    // "I really got worked over, but as long as Kushinada's safe, it was..."
    Event_ShowMessage(ACTOR_SUSA, 0);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_SUSA)
    return gGaiaRock_SusaTriggers;
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    return gGaiaRock_StatueSpotsTriggers;
  return gGaiaRock_Triggers;
}

/// A circling leaf as its update reads it: struct Actor's fields under a type
/// of its own, the timer unsigned and the update taking the leaf.
struct OrbitEffect {
  u8 unused[6]; ///< the script and its cursor, not read here
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unusedAfterZ[15];
  u8 priorityFlags;
  u8 unusedAfterPriority[4];
  s32 velocityY;
  u8 unusedAfterVelocityY[28];
  s32 gravity;
  u8 unusedAfterGravity[24];
  u16 timer;
  s16 pause; ///< Actor.userValue
  struct OrbitEffect *watchedActor;
  void (*update)(struct OrbitEffect *);
};

FIXED Trig_Cos(s32 angle);

/// Each frame, carries a leaf round a wobbling circle, now and then turning
/// faster for a moment, and releases it at its 71st frame.
void GaiaRock_CircleLeaf(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  FIXED dx;
  FIXED dz;

  radius = Random16() + toFIXED(2);
  angle = effect->facing;
  dx = Iwram_MulFX(radius, Trig_Cos(angle));
  dz = Iwram_MulFX(radius, Math_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->facing -= TRIG_TURN / 4096;
  if (effect->pause != 0) {
    effect->pause--;
    effect->facing += TRIG_TURN / 32;
  } else if (Random_Below(32) == 0) {
    effect->pause = Random_Below(16) + 8;
  }
  if (++effect->timer == 71)
    Actor_StartScript((struct Actor *)effect, gGaiaRock_LeafScript);
}

/// The whirlwind's 16 leaves, spawned in a column around it, each circling
/// from its own start.
s32 GaiaRock_SpawnCirclingLeaves(struct OrbitEffect *source) {
  s32 i;
  s32 timer;

  i = 0;
  timer = 20;
  for (; i < WHIRLWIND_LEAVES; i++) {
    {
      struct OrbitEffect *effect;
      s32 radius;
      FIXED position[3];

      position[0] = source->x;
      position[1] = source->y + i * (toFIXED(4) / 5) + toFIXED(4);
      position[2] = source->z;
      radius = Random16();
      FieldVector_AddPolarOffsetFX(radius * 5 + toFIXED(3), Random16(),
                                   position);
      effect =
          (struct OrbitEffect *)Value4((s32 (*)())Actor_Spawn, SPRITE_LEAF,
                                       position[0], position[1], position[2]);
      if (effect != NULL) {
        effect->update = GaiaRock_CircleLeaf;
        effect->timer = timer;
        effect->pause = 0;
        effect->gravity = toFIXED(1) / 10;
        effect->velocityY = toFIXED(3);
        effect->facing = Random16();
        effect->priorityFlags = 0;
        Actor_SetSpritePriorityDirect((struct Actor *)effect, 2);
      }
    }
    timer -= 2;
  }
  return 0;
}

/// The hanging vine of actor id, or NULL: this overlay's copy of
/// HangingVine_FindSlot.
static inline struct HangingVineSlot *FindSlot(s32 id) {
  struct HangingVineState *state = HANGING_VINE_STATE;
  struct HangingVineSlot *found = NULL;
  s32 i = 0;

  for (i = 0; i < ROPE_SLOT_COUNT; i++) {
    if (state->slots[i].id == id) {
      found = &state->slots[i];
      break;
    }
  }
  return found;
}

/// The statue's whirlwind reaches the vine beside it and starts it swinging.
void GaiaRock_WhirlwindBlowsVine(void) {
  struct HangingVineSlot *slot = FindSlot(ACTOR_FIRST_VINE);

  slot->phase = DEGtoANG(90);
  slot->swing = TRIG_TURN / 1024;
}

/// The statue's whirlwind swings the vine beside it wider.
void GaiaRock_WhirlwindBlowsVineHarder(void) {
  struct HangingVineSlot *slot = FindSlot(ACTOR_FIRST_VINE);

  slot->phase = DEGtoANG(90);
  slot->swing += TRIG_TURN / 512;
}

void Actor_PlaySoundInView(struct Actor *actor, s32 sound);

/// Every 256 frames the stone statue on the upper ledge of the first cliffs
/// blows a whirlwind west at the hanging vine beside it, setting it swinging.
void GaiaRock_StatueBlowsWind(void) {
  struct Actor *actor = Actor_Get(ACTOR_WIND_STATUE);
  u8 phase = gFrameCount;

  if (phase == 0) {
    Actor_PlaySoundInView(actor, SE_FLAME_FLARE);
    actor = Actor_Spawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
                        actor->z.fixed);
    if (actor != NULL) {
      actor->acceleration = toFIXED(2);
      actor->speedLimit = toFIXED(2);
      actor->scaleX = FIXED_ONE / 2;
      actor->scaleY = FIXED_ONE / 10;
      actor->priorityFlags = phase;
      Actor_SelectAnimation(actor, 2);
      FieldActor_SetMoveTargetFX(actor, actor->x.fixed - toFIXED(32),
                                 actor->y.fixed, actor->z.fixed);
      Actor_StartScript(actor, gGaiaRock_WindScript);
      Actor_SetSpritePriorityDirect(actor, 2);
    }
  }
}

/// The transition Gaia Rock opens with: the window's shape until patch 1, an
/// interlaced wipe from it.
#if !TLA_PATCH_1
#define SCREEN_EFFECT                                                          \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define SCREEN_EFFECT                                                          \
  SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

s32 HangingVine_Place();

s32 Scene_Initialize(void) {
  struct Actor *actor;
  struct FieldState *state;

  gFieldState->transition = SCREEN_EFFECT;
  gMainRoutineState.retreatScene = SCENE_GAIA_ROCK_SUSA;
  gMainRoutineState.retreatEntrance = GAIA_ROCK_SUSA_FROM_WORLD_MAP;
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_SUSA) {
    Task_Add((s32)GaiaRock_StatueBlowsWind, TASK_PASS_FRAME);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 1, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 2, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 3, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 4, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 5, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 6, 1);
    HangingVine_Place(ACTOR_FIRST_VINE + 7, 1);
    if (GameFlag_Get(FLAG_FIRST_PILLAR_DROPPED)) {
      actor = Actor_GetById(ACTOR_FIRST_PILLAR);
      actor->motionFlags = 0;
      actor->x.fixed = toFIXED(88);
      actor->y.fixed = toFIXED(128);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(6, 27, 1, 1, 5, 27);
    }
    actor = Actor_GetById(ACTOR_SECOND_PILLAR);
    actor->priorityFlags = 0;
    if (GameFlag_Get(FLAG_SECOND_PILLAR_DROPPED)) {
      actor->motionFlags = 0;
      actor->x.fixed = toFIXED(648);
      actor->y.fixed = toFIXED(64);
      Actor_SetSpriteFlags(actor, 0);
      MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      MapGrid_CopyCellAttributes(27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (FIELD_HALF_CELL(actor->x.fixed) != 75) {
        MapGrid_CopyCellAttributes(33, 102, 1, 2, 37, 102);
      }
      if (FIELD_HALF_CELL(actor->x.fixed) == 79) {
        actor->motionFlags = 0;
        MapGrid_CopyCellAttributes(38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
        !GameFlag_Get(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
      Actor_SetPosition(ACTOR_SUSA, 798, 800);
      // He lies across cells (49, 49) and (50, 49), in front of the sapling
      // by the cave mouth; both take the wall of cell (53, 52), so that no
      // one walks over him.
      Map_CopyCellAttributeRect(53, 52, 1, 1, 49, 49);
      Map_CopyCellAttributeRect(53, 52, 1, 1, 50, 49);
      Actor_SetAnimationById(ACTOR_SUSA, 5);
      if (!Event_GetFlag(FLAG_SUSA_RESTED)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        FieldActor_StartRepeatedMotion(ACTOR_SUSA, 3);
        FieldCamera_MoveToActorById(ACTOR_SUSA, 1);
        FieldEvent_Wait(30);
        FieldEvent_SetMessageId((s32)MsgIMNot);
        // "I'm not dying... I'm just so tired... I'm going to lie down for
        // a..."
        Event_ShowMessage(ACTOR_SUSA, 0);
        FieldActor_StartRepeatedMotion(ACTOR_SUSA, 2);
        FieldEvent_Wait(30);
        Event_SetFlag(FLAG_SUSA_RESTED);
        Event_End();
      }
    }
  } else if (gMainRoutineState.next == SCENE_GAIA_ROCK_STATUE_SPOTS) {
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 1, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 2, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 3, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 4, 1);
    Call2((void (*)())HangingVine_Place, ACTOR_FIRST_VINE + 5, 1);
    HangingVine_Place(ACTOR_FIRST_VINE + 6, 1);
    if (ARRIVES_AT_GREAT_WALL()) {
      GaiaRock_StartWallTriggers();
    }
    if (GameFlag_Get(FLAG_FIRST_STATUE_RISEN)) {
      StoneHead_Place(ACTOR_FIRST_RISING_STATUE, toFIXED(764), -toFIXED(391),
                      toFIXED(424));
    }
    if (GameFlag_Get(FLAG_SECOND_STATUE_RISEN)) {
      StoneHead_Place(ACTOR_SECOND_RISING_STATUE, toFIXED(789), -toFIXED(248),
                      toFIXED(424));
    }
    if (GameFlag_Get(FLAG_THIRD_STATUE_RISEN)) {
      StoneHead_Place(ACTOR_THIRD_RISING_STATUE, toFIXED(778), -toFIXED(119),
                      toFIXED(424));
    }
  } else if (gMainRoutineState.next == SCENE_GAIA_ROCK_DANCING_IDOL) {
    actor = Actor_GetById(FIELD_TREASURE_ACTOR);
    if (actor != 0) {
      Actor_SetPosition(FIELD_TREASURE_ACTOR, 0, 0);
    }
    state = Ram_HeapSlots->fieldState;
    actor = state->actors[ACTOR_DANCING_IDOL];
    state->actors[FIELD_TREASURE_ACTOR] = actor;
    actor = Actor_GetById(ACTOR_PUSHED_PILLAR);
    actor->motionFlags = 0;
    MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, FIELD_CELL(actor->z.fixed));
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_GAIA_ROCK_STATUE_SPOTS &&
      ARRIVES_AT_GREAT_WALL()) {
    struct MapLayer *layer = MapGrid_GetLayer(1);

    layer->offsetX = -toFIXED(128);
    layer->offsetY = toFIXED(1096);
  }
  return 0;
}

#include "shared/gaia_rock_trigger_wall_cell.c"

#include "shared/stone_head_place.h"

#include "shared/stone_head_burst_out.h"

/// Starts GaiaRock_TriggerWallCell each frame.
void GaiaRock_StartWallTriggers(void) {
  Task_AddFrame(GaiaRock_TriggerWallCell, TASK_PASS_FRAME);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gGaiaRock_SusaExitRegions[] = {
    {771, -28, 484, 779, -20, 492, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH},
    {771, 180, 707, 779, 188, 715, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS},
    {SCENE_TABLE_END},
};

struct SceneRegion gGaiaRock_StatueSpotsExitRegions[] = {
    {195, 296, 323, 203, 304, 331, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL},
    {707, -554, 452, 715, -546, 460, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GaiaRockSusa),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH, GaiaRockSusa, GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS, GaiaRockSusa, GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockStatueSpots),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, GaiaRockSusa, GAIA_ROCK_SUSA_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, GaiaRockDancingIdol, GAIA_ROCK_DANCING_IDOL_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockDancingIdol),
    SCENE_EXIT_UNLESS(GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_DANCING_IDOL, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 gGaiaRock_PillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

struct ActorPlacement gGaiaRock_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_SusaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1000, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 936, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gGaiaRock_PillarScript, 120, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 295, 0, 442, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)gGaiaRock_PillarScript, 600, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_StatueSpotsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRock_DancingIdolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Unused: an empty trigger table nothing returns.
struct Trigger gGaiaRock_UnusedTriggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_SusaTriggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS},
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR},
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_WORLD_MAP, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_WORLD_MAP},
    {TRIGGER_STEP, CELL_BEHAVIOR_PAST_SUSA, FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE, EVENT(GaiaRock_SusaAsksAboutTheSerpent)},
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRock_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRock_MindReadSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 7, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_CAVE_MOUTH_WEEDS, CONDITION_ALWAYS, EVENT(GaiaRock_CycloneBlowsAwayWeeds)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_DROPPED, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_StatueSpotsTriggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA},
    {TRIGGER_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOWEST_STONE_FACE, FLAG_FIRST_STATUE_RISEN, EVENT(GaiaRock_ClimbOntoLowestStoneFace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MIDDLE_STONE_FACE, FLAG_SECOND_STATUE_RISEN, EVENT(GaiaRock_ClimbOntoMiddleStoneFace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_HIGHEST_STONE_FACE, FLAG_THIRD_STATUE_RISEN, EVENT(GaiaRock_ClimbOntoHighestStoneFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRock_Triggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS},
    {TRIGGER_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRock_CheckAltar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(GaiaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRock_FinishPushingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(GaiaRock_RevealIdol)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRock_HideIdol)},
    {TRIGGER_TABLE_END},
};

u32 gGaiaRock_LeafScript[] = {
    SC_RELEASE,
};

s32 gGaiaRock_WindScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LOOP(12, SC_TOP),
    SC_SET(SW_SCALE_X, FIXED_ONE * 17 / 10),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 13 / 10),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(GaiaRock_WhirlwindBlowsVine),
    SC_WAIT(2),
    SC_LABEL(1),
    SC_CALL(GaiaRock_WhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(3, SC_TO(1)),
    SC_CALL(GaiaRock_SpawnCirclingLeaves),
    SC_LABEL(2),
    SC_CALL(GaiaRock_WhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(14, SC_TO(2)),
    SC_CALL(GaiaRock_WhirlwindBlowsVineHarder),
    SC_WAIT(10),
    SC_RELEASE,
};

#include "shared/debris_script.c"

#include "shared/stone_head_rise_script.c"
// clang-format on
