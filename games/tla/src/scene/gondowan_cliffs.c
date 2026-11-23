/// @file
/// The Gondowan Cliffs, the maze of ledges, vines and slides west of Madra on
/// the way to Kibombo, and the river channel below them (SceneId_GondowanCliffs
/// and SceneId_GondowanCliffsPass, which share this overlay). Coming from
/// Madra, Felix finds a dog lying in the grass by the east entrance; Scoop on
/// the yellow sand beside it digs up a geyser that carries him up onto the
/// cliffs to the west. Whirlwind clears the weeds from a cave mouth nearby,
/// where Kibombo warriors are hiding. Further on, Move pushes a tree stump off
/// its ledge into the stream as a stepping stone, and close by Kindle, a Mars
/// Djinni, waits to be fought; the tightrope across the river in the far west
/// leads on towards Kibombo. Two puddles can be frozen with Frost once
/// Felix has it, the eastern one on a later visit for the Healing Fungus. If
/// the party comes back here after meeting Piers in Kibombo and before the
/// ceremony at the Gabomba Statue is over, Kraden, Jenna and Sheba turn Felix
/// back. Late in the game, Grind breaks the boulder in the river channel so
/// the ship can sail on south to the western sea.

#include "system/task.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/actor_script.h"
#include "flags.h"
#include "djinn_ids.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "field/call.h"
#include "graphics/vram_tab.h"
#include "macros.h"

extern struct SceneEntrance gGondowanCliffs_Entrances[];
extern struct SceneEntrance gGondowanCliffs_PassEntrances[];

#include "shared/tile_sprite_reset.h"

/// The cliffs' actors by their rows (Jenna's and Sheba's rows take their own
/// numbers).
enum {
  /// The posts of the tightrope across the river, strung once Briggs is
  /// caught.
  ACTOR_TIGHTROPE_POST_A = 8,
  ACTOR_TIGHTROPE_POST_B,
  /// The posts of the rope Lash ties from the lower one up to the cliff.
  ACTOR_ROPE_POST_A,
  ACTOR_ROPE_POST_B,
  /// The two puddles Frost freezes into pillars of ice.
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_WEEDS, ///< over the mouth of the cave Whirlwind opens
  /// The stump that stands in the stream once
  /// FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER is set.
  ACTOR_STUMP,
  ACTOR_DOG,
  ACTOR_GEYSER, ///< the unseen point the geyser rises from
  /// The two Kibombo warriors stranded by the river until Briggs is caught.
  ACTOR_FIRST_WARRIOR,
  ACTOR_SECOND_WARRIOR,
  ACTOR_KINDLE = 21,
  ACTOR_KRADEN
};

/// The river channel's actors: after the unseen one that resets the tile
/// sprites, the tightrope's two posts and the boulder Grind breaks.
enum {
  ACTOR_PASS_TIGHTROPE_POST_A = 9,
  ACTOR_PASS_TIGHTROPE_POST_B,
  ACTOR_PASS_SEA_ROCK
};

/// The river channel's entrance an event sends the party in by, aboard the
/// ship.
enum { ENTRANCE_PASS_BY_SHIP = 99 };

/// Talking to the red Djinni on the ledge near the tree stump starts the battle
/// with Kindle, a Mars Djinni, who joins Felix once beaten.
void GondowanCliffs_FightKindle(void) {
  Field_RequestDjinnBattle(ACTOR_KINDLE, DJINNI_KINDLE);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassEntrances;
  return gGondowanCliffs_Entrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gGondowanCliffs_ActorPlacements[];
extern struct ActorPlacement gGondowanCliffs_PassActorPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassActorPlacements;
  return gGondowanCliffs_ActorPlacements;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"
// clang-format on

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

// The Gondowan Cliffs' events and set-up, and the geysers Scoop digs up out of
// the yellow sand: each rises and falls on its timer, carrying the leader up
// its column of water (Geyser_Update, Geyser_Draw).

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/// The rope Lash ties, for this visit.
#define FLAG_GONDOWAN_CLIFFS_ROPE_LASHED FLAG_SCENE_TEMP_00
/// Set as Frost freezes each puddle into a pillar of ice, for this visit.
#define FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_02

struct Actor;
struct EffectOptions;
/// MapWork.groundEffects read as its bit-fields: the footprints, and the kind
/// of wading (1 in rooms with a second level).
struct MapGroundEffects {
  u8 unusedBeforeGroundEffects[23]; ///< MapWork's fields before it
  u8 footprints : 2;
  u8 wading : 2;
  u8 unusedHighBits : 4; ///< no code names them
};
/// The geyser Scoop digs up: it rises and falls on its timer, carrying the
/// platform at its top, as Yampi Desert's geyser does.
struct Geyser {
  s16 id;              ///< its actor; 0 for a free entry
  s16 timer;           ///< frames into its period
  s16 height;          ///< pixels it has risen
  s16 period;          ///< frames in one rise and fall
  s16 riseAt;          ///< the timer it starts to rise at
  s16 fallAt;          ///< the timer it starts to fall at
  s16 max;             ///< pixels it rises to
  s16 min;             ///< pixels it falls to
  s16 unreadSetToOne;  ///< set to 1 by Geyser_Add and never read
  s16 step;            ///< pixels a frame: 4 rising, -8 falling, 0 at rest
  s16 unreadCleared;   ///< cleared by Geyser_Add and never read
  s16 phase;           ///< the platform's bob, 1/65536 turns
  struct Actor *rider; ///< the platform at its top, which the leader rides
};

/// The geysers' work, kept in the inventory snapshot's spare room.
struct GeyserWork {
  s16 entry;        ///< the VRAM cache entry holding the column's tiles
  s16 firstTerrain; ///< the terrain id and collision slot of the first entry
  struct Geyser entries[GEYSER_ENTRIES];
  /// The OAM words Geyser_Draw queues: GEYSER_SPRITES sprites of three words.
  void *block;
};

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Tightrope_Place(s32 a, s32 b, s32 c);
void Rope_Initialize(void);
void Geyser_Start(s32 unused, s32 firstTerrain);
void Tightrope_Initialize(void);

/// The leaping stump's update: its sprite turns a 32nd of a turn a frame.
void GondowanCliffs_SpinStump(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation -= TRIG_TURN / 32;
}

/// Waits until the actor has come down to height (16.16), a second at most.
void GondowanCliffs_WaitToLand(struct Actor *actor, s32 height) {
  s32 count = FRAMES_PER_SECOND;

  while (count != 0) {
    System_Wait(1);
    count--;
    if (actor->y.fixed <= height)
      break;
  }
}

/// Pushing the tree stump west with Move to the edge of its ledge, cell
/// (76, 34), sends it rolling off: it leaps into the stream and lands upright
/// in the water with a thump and a splash, at cell (74, 33), a stepping stone
/// Felix can hop onto to cross.
///
/// The Move row answers every push; only a push that leaves the stump in
/// column 76 starts the leap. The landing cell takes the hop flag of the cell
/// two rows north of it.
void GondowanCliffs_LeapStumpIntoWater(s32 mode, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  s32 unused[3];
  struct EffectOptions options;
  struct FieldSprite *sprite;
  u32 i;

  if (FIELD_CELL(actor->x.fixed) != 76)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
  if (FIELD_CELL(Actor_GetById(ACTOR_STUMP)->x.fixed) == 76) {
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    for (i = 0; i <= 17; i++) {
      System_Wait(1);
      sprite = actor->sprite;
      sprite->rotation -= TRIG_TURN / 256;
      actor->x.fixed -=
          Trig_Cos(((struct FieldSprite *)actor->sprite)->rotation) / 2;
      actor->targetX = ACTOR_NO_TARGET;
    }
    actor->update = GondowanCliffs_SpinStump;
    Actor_SetSpeed(id, 3);
    Actor_SetDestination(id, 1193, 530);
    actor->gravity = toFIXED(4) / 5;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->layer = 0;
    FieldActor_WaitForMoveById(id);
    GondowanCliffs_WaitToLand(actor, 0);
    Event_PlaySound(SE_THUMP);
    MapGrid_SetShake(5, 5, 1);
    Event_PlaySound(SE_RUMBLE);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    ((struct FieldSprite *)actor->sprite)->rotation = TRIG_TURN / 16;
    actor->update = NULL;
    options.type = SPRITE_SPLASH;
    options.startScaleX = FIXED_ONE / 2;
    options.targetScaleX = FIXED_ONE * 3 / 2;
    options.targetScaleY = FIXED_ONE * 6 / 5;
    options.startScaleY = FIXED_ONE * 4 / 5;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
                      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET,
                      &options);
    Event_PlaySound(SE_CAST);
    Actor_SetAnimation(id, STUMP_ANIMATION_SPLASH);
    FieldMapGrid_WaitForShakeToSettle();
    Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER);
  }
  Event_End();
}

/// Frost on the puddle in the sandy gap between two ledges, near the tree
/// stump, cell (81, 28), raises a pillar of ice there that Felix can hop onto
/// to cross the gap; it stays until he leaves the cliffs.
///
/// The pillar's sprite stops moving, and the puddle's cell takes the hop flag
/// of the cell two to its west.
void GondowanCliffs_FreezeFirstPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN);
}

/// Frost on the puddle in the grass at the east end of the cliffs, cell
/// (117, 35), raises a pillar of ice there that Felix can hop onto to jump the
/// gap on the way to the Healing Fungus; it stays until he leaves the cliffs.
///
/// The pillar's sprite stops moving and its height is set to 32 pixels, and
/// the puddle's cell takes the hop flag of the cell two to its west.
void GondowanCliffs_FreezeSecondPuddle(void) {
  struct Actor *actor = Actor_Get(ACTOR_SECOND_PUDDLE);

  actor->priorityFlags = 0;
  actor->motionFlags = 0;
  actor->groundHeight = toFIXED(32);
  actor->y.fixed = toFIXED(32);
  MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN);
}

/// Whirlwind on the weeds hanging over a cave mouth west of the dog blows it
/// away for good, opening the way into the cave where the Kibombo warriors
/// hide.
///
/// The leaves are Whirlwind's own; the event only sets the flag that keeps the
/// weeds gone and opens the cave's exit.
void GondowanCliffs_BlowAwayFoliage(void) {
  GameFlag_Set(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN);
}

/// Scoop on the patch of yellow sand beside the dog at the east end, cell
/// (119, 45), digs up a geyser that from then on rises and falls there, a lift
/// Felix can ride up to the cliffs above.
///
/// The Scoop row runs the event every frame of the dig; at `SCOOP_DUG_FRAME`
/// it redraws the dug ground from a spare cell of the map and moves the
/// geyser's unseen point onto it.
void GondowanCliffs_ScoopUpGeyser(s32 stage, s32 frame) {
  struct Actor *actor;

  if (frame == SCOOP_DUG_FRAME) {
    actor = Actor_Get(ACTOR_GEYSER);
    MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
    Actor_SetPosition(ACTOR_GEYSER, 1912, 728);
    actor->groundHeight = actor->y.fixed =
        Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    Event_SetFlag(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG);
  }
}

#include "text/msg_ids.h"

/// Talking to the white dog lying in the grass at the east end makes it bark
/// "Rrruff! Rrruff!" at Felix. When Felix is facing the yellow sand beside it,
/// cell (119, 45), the button opens the field menu instead, as it does where
/// there is nothing to talk to, so that he can choose Scoop there.
void GondowanCliffs_TalkToDog(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct {
    s32 x, y, z;
  } position;
  s32 *out = (s32 *)&position;

  out[0] = actor->x.fixed;
  out[1] = actor->y.fixed;
  out[2] = actor->z.fixed;
  FieldVector_AddPolarOffset(16, actor->facing, out);
  if (FIELD_CELL(position.x) == 119 && FIELD_CELL(position.z) == 45) {
    gFieldWork->fieldMenu = 1;
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceEachOther(ACTOR_DOG, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgRrruffRrruff);
    // "Rrruff! Rrruff!"
    Event_ShowMessage(ACTOR_DOG, 0);
    Actor_FaceDirection(ACTOR_DOG, FACING_WEST, 0);
    Event_End();
  }
}

extern struct Trigger gGondowanCliffs_PassTriggers[];
extern struct Trigger gGondowanCliffs_Triggers[];

/// Grind on the tall brown boulder standing in the river channel grinds it
/// away for good, so the ship can sail on through.
void GondowanCliffs_GrindTheRock(s32 unused, s32 id) {
  Actor_SetPosition(id, 0, 0);
  Event_SetFlag(FLAG_GONDOWAN_CLIFFS_ROCK_GROUND);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS)
    return gGondowanCliffs_PassTriggers;
  return gGondowanCliffs_Triggers;
}

/// The water's colours as the map loads them, which the shimmer turns through.
u16 gGondowanCliffs_SavedWaterColors[WATER_COLORS] BSS_DATA;

/// Makes the water shimmer: its four colours turn one step every 16 frames.
void GondowanCliffs_CycleWaterColors(void) {
  u16 *color = (u16 *)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR);
  u32 phase = gFrameCount >> 4;
  s32 i;

  for (i = 0; i < WATER_COLORS; i++) {
    u32 index = 3;

    index &= phase;
    *color++ = gGondowanCliffs_SavedWaterColors[index];
    phase++;
  }
}

/// Sets up the river channel: arriving aboard the ship, the party sails on, the
/// tightrope across the channel is strung and the water starts to shimmer.
void GondowanCliffs_SetUpPass(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_PASS_BY_SHIP) {
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_SHIP;
    gMainRoutineWork.ownerOverride = 1;
    Event_SetDestination(SCENE_GONDOWAN_CLIFFS_PASS,
                         GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_NORTH);
  }
  Tightrope_Initialize();
  Value3((s32 (*)())Tightrope_Place, 0, ACTOR_PASS_TIGHTROPE_POST_A,
         ACTOR_PASS_TIGHTROPE_POST_B);
  Iwram_CopyWords((s32)gGondowanCliffs_SavedWaterColors,
                  (s32)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR),
                  sizeof(gGondowanCliffs_SavedWaterColors));
  Task_AddFrame(GondowanCliffs_CycleWaterColors, TASK_PASS_FRAME);
}

void Actor_PlaceOnActor(s32 actor, s32 target);

/// Coming back to the cliffs after meeting Piers in Kibombo, before the
/// ceremony at the Gabomba Statue is over, the party stops as soon as it
/// enters: Kraden asks "Felix! Where do you intend to go?", Jenna and Sheba
/// side with him, everyone nods, and Felix walks back out to the world map to
/// go and help Piers.
void GondowanCliffs_TurnBackToHelpPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 552, 488);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 568, 480);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 584, 472);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 608, 472);
  FieldEvent_SetMessageId((s32)MsgWhereDoYou);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "Felix! Where do you intend to go?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 10);
  FieldActor_Jump(ACTOR_JENNA, 6, 15);
  FieldActor_Jump(ACTOR_JENNA, 6, 23);
  // "Kraden's right. I think we should try to help Piers, too."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_Stop(ACTOR_SHEBA);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
  // "I completely agree with Kraden and Jenna."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "It looks like the majority wins! Let's go back to Kibombo and help..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 584, 472);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 568, 480);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 552, 488);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 528, 488);
  Event_FinishScreenTransition();
  Event_PlaySound(SE_MAP_EXIT);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 512, 488);
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(GONDOWAN_CLIFFS_TO_WORLD_MAP_A);
}

void Rope_Place(s32 a, s32 b, s32 c, s32 d);
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high);

/// Sets up the scene being entered. The river channel has its own set-up; the
/// cliffs turn the party back while Piers still needs their help, and
/// otherwise tie the rope and string the tightrope, put back the weeds, the
/// frozen puddles, the geyser and the stump as their flags say, and start the
/// water shimmering.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 height;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS_PASS) {
    GondowanCliffs_SetUpPass();
    return 0;
  }
  if (GameFlag_Get(FLAG_KIBOMBO_PIERS_MET) &&
      !GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED)) {
    GondowanCliffs_TurnBackToHelpPiers();
    return 0;
  }
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS) {
    Rope_Initialize();
    Rope_Place(0, FLAG_GONDOWAN_CLIFFS_ROPE_LASHED, ACTOR_ROPE_POST_A,
               ACTOR_ROPE_POST_B);
    if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
      Tightrope_Initialize();
      Tightrope_Place(0, ACTOR_TIGHTROPE_POST_A, ACTOR_TIGHTROPE_POST_B);
    }
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN))
      Actor_SetPosition(ACTOR_WEEDS, 0, 0);
    else
      Actor_GetById(ACTOR_WEEDS)->scaleY = FIXED_ONE * 5 / 4;
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN))
      MapGrid_CopyCellAttributes(115, 35, 1, 1, 117, 35);
    Geyser_Start(0, GEYSER_FIRST_TERRAIN);
    Geyser_Add(0, ACTOR_GEYSER, 0, 0, GEYSER_MAX_HEIGHT);
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_GEYSER_DUG)) {
      actor = Actor_GetById(ACTOR_GEYSER);
      MapGrid_CopyCellsLayered(124, 1, 1, 1, 119, 43);
      Actor_SetPosition(ACTOR_GEYSER, 1912, 728);
      height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
      actor->y.fixed = height;
      actor->groundHeight = height;
    }
    Actor_SetAnimation(ACTOR_DOG, 4);
    if (GameFlag_Get(FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER)) {
      MapGrid_CopyCellAttributes(74, 31, 1, 1, 74, 33);
      Actor_SetPosition(ACTOR_STUMP, 1193, 530);
      Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Actor_GetById(ACTOR_STUMP)->collisionFlags = 0;
      Actor_SetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
    } else {
      Actor_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
    }
    Iwram_CopyWordsEntry(((u8 *)gGondowanCliffs_SavedWaterColors),
                         (void *)&BG_PLTT_COLOR(0, FIRST_WATER_COLOR),
                         sizeof(gGondowanCliffs_SavedWaterColors));
    Task_AddFrame(GondowanCliffs_CycleWaterColors, TASK_PASS_FRAME);
    ((struct MapGroundEffects *)Ram_HeapSlots->mapGrid)->wading = 1;
  }
  return 0;
}

extern s16 gGondowanCliffs_CameraBoxes[];

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_GONDOWAN_CLIFFS)
    ((struct MapWork *)Ram_HeapSlots->mapGrid)->cameraBoxes =
        gGondowanCliffs_CameraBoxes;
  return 0;
}

#include "shared/geyser_update.h"

#include "shared/geyser_draw.h"

#include "shared/geyser_start.h"

/// Makes actor id the geyser of entry slot, rising from low to high pixels
/// every six seconds, with an unseen platform at its top for the leader.
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 low, s32 high) {
  struct GeyserWork *work = GEYSER_WORK;
  struct Geyser *entry = &work->entries[slot];
  struct Actor *actor = Actor_Get(id);
  struct Actor *rider;

  Actor_Get(id);
  Actor_SetSpriteFlags(actor, 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = GEYSER_PERIOD_FRAMES;
    entry->max = high;
    entry->riseAt = GEYSER_RISE_FRAME;
    entry->fallAt = GEYSER_FALL_FRAME;
    entry->min = low;
    entry->unreadSetToOne = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    entry->step = 0;
    entry->phase = 0;
    entry->unreadCleared = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  rider = (FieldActor_Create(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
                             actor->x.fixed, actor->y.fixed, actor->z.fixed));
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  entry->rider = rider;
  return entry;
}

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

// The Gondowan Cliffs' tables, which the scene's getters return.

#include "text/msg_ids.h"

void GondowanCliffs_FreezeSecondPuddle(void);
void GondowanCliffs_BlowAwayFoliage(void);
extern s32 gGondowanCliffs_ResetTileSpriteScript[];

void GondowanCliffs_FightKindle(void);
void GondowanCliffs_FreezeFirstPuddle(void);
void GondowanCliffs_TalkToDog(void);

LATE_DATA s32 gGondowanCliffs_ResetTileSpriteScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
};

// clang-format off
LATE_DATA struct SceneEntrance gGondowanCliffs_Entrances[] = {
    {0, CONDITION_ALWAYS, 680, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {GONDOWAN_CLIFFS_FROM_WORLD_MAP_A, CONDITION_ALWAYS, 536, 0, 488, FACING_EAST, 0, 512, 0, 2048, 864, 0},
    {GONDOWAN_CLIFFS_FROM_WORLD_MAP_B, CONDITION_ALWAYS, 2024, 0, 792, FACING_WEST, 0, 512, 0, 2048, 864, 0},
    {GONDOWAN_CLIFFS_FROM_GONDOWAN_CLIFFS_HIDEOUT, CONDITION_ALWAYS, 1576, 0, 680, FACING_SOUTH, 0, 512, 0, 2048, 864, 0},
    {99, CONDITION_ALWAYS, 1272, 0, 552, FACING_SOUTH, 0, 512, 0, 2048, 864, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneEntrance gGondowanCliffs_PassEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(GondowanCliffs),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_TO_GONDOWAN_CLIFFS_HIDEOUT, GondowanCliffsHideout, GONDOWAN_CLIFFS_HIDEOUT_FROM_GONDOWAN_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanCliffsPass),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_CLIFFS_PASS_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gGondowanCliffs_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1560, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1624, 0, 760, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1304, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1880, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1576, 0, 672, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1232, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1928, 0, 728, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KIBOMBO_WARRIOR, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 888, 0, 232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 816, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_KINDLE), MOVEMENT_TYPE_STAND, 1336, 0, 648, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, IF_FLAG_SET(FLAG_KIBOMBO_PIERS_MET), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGondowanCliffs_PassActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)gGondowanCliffs_ResetTileSpriteScript, 330, 0, 79, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEA_ROCK, FLAG_GONDOWAN_CLIFFS_ROCK_GROUND, MOVEMENT_TYPE_STAND, 304, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LATE_DATA struct Trigger gGondowanCliffs_PassTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_PASS_SEA_ROCK, CONDITION_ALWAYS, EVENT(GondowanCliffs_GrindTheRock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGondowanCliffs_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, IF_FLAG_SET(FLAG_GONDOWAN_CLIFFS_FOLIAGE_BLOWN), 3},
    {TRIGGER_TALK, ACTOR_DOG, CONDITION_ALWAYS, EVENT(GondowanCliffs_TalkToDog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, MSGNO(MsgDigHereYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {TRIGGER_TALK, ACTOR_FIRST_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {TRIGGER_TALK, ACTOR_SECOND_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WARRIOR, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {TRIGGER_TALK, ACTOR_KINDLE, CONDITION_ALWAYS, EVENT(GondowanCliffs_FightKindle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, FLAG_GONDOWAN_CLIFFS_ROPE_LASHED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_GONDOWAN_CLIFFS_FIRST_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_GONDOWAN_CLIFFS_SECOND_PUDDLE_FROZEN, EVENT(GondowanCliffs_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS, CONDITION_ALWAYS, EVENT(GondowanCliffs_BlowAwayFoliage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CLIFFS_STUMP_IN_WATER, EVENT(GondowanCliffs_LeapStumpIntoWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_GONDOWAN_CLIFFS_GEYSER_DUG, EVENT(GondowanCliffs_ScoopUpGeyser)},
    {TRIGGER_TABLE_END},
};

/// The parts of the cliffs the camera keeps out of, left, top, right and
/// bottom in pixels.
LATE_DATA s16 gGondowanCliffs_CameraBoxes[] ALIGNED(4) = {
    512, 512, 1024, 864,
    1536, 0, 4096, 352,
    -1, -1, -1, -1,
};
// clang-format on
