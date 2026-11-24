/// @file
/// The Mars Lighthouse's dragon hall, high in the frozen lighthouse, where two
/// Flame Dragons sleep in a block of ice below a great dragon's head carved in
/// the north wall, with the corridor and the small room that lead to it.
///
/// Two torches stand on stands before the frozen dragons, the east one
/// burning. Blaze from the burning torch lights the other, which sends two
/// sparkles round the hall, each launching a fireball into the ice, which
/// cracks; Burst on the crack frees the dragons, which wake and fight. Beaten,
/// they are Agatio and Karst, too weak and cold to go on: they ask Felix to
/// light the beacon and leave him the Mars Star. Taken up into the Mythril Bag
/// and set in the dragon head's face, the star turns the lighthouse back to
/// fire and its ice is gone, here and in every room below.

#include "graphics/window_sprite_list.h"
#include "system/task.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "scene/flame_throwers.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "scene/mars_lighthouse_star_room.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "version.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "system/io_reg.h"
#include "battle/battle_unit.h"
#include "field/call.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "field/map_grid.h"
#include "macros.h"
#include "system/input.h"
#include "system/field_effect.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "field/sprite.h"
#include "tla_edition.h"
#include "game/character_ids.h"
#include "graphics/glyph.h"
#include "text/msg_ids.h"
#include "system/io_write.h"

#define FLAG_MARS_STAR_TAKEN 0xa3a ///< Felix took it from where Agatio left it
#define FLAG_ADEPTS_WOKE 0xa3b     ///< Agatio and Karst woke after the battle
/// The east fireball cracked the ice.
#define FLAG_DRAGON_ICE_CRACKED 0xa4a
/// Set on coming back from the Flame Dragons' battle.
#define FLAG_FLAME_DRAGONS_BEATEN 0xa4b
/// From patch 1: the Mars Star was set in the statue once, and the lighthouse
/// thawed; set again, it only rises and falls back.
#define FLAG_MARS_STAR_SET 0xa9e
/// Both fireballs have reached the ice: Burst can break it.
#define FLAG_BURST_CAN_BREAK_DRAGON_ICE FLAG_SCENE_TEMP_33
/// From patch 1: the ice is burst, and fireballs still on their way no longer
/// crack it.
#define FLAG_DRAGON_ICE_BURST FLAG_SCENE_TEMP_38

/// The room's own ways between its parts: the dragon hall's south end and the
/// west doorway of a long corridor lead to each other, and the corridor's east
/// doorway to the south end of a small room, whose north doorway leads out.
/// Each exit leads to the entrance of the same name.
enum {
  ENTRANCE_HALL_SOUTH_END = 1,
  ENTRANCE_CORRIDOR_WEST_DOORWAY,
  ENTRANCE_CORRIDOR_EAST_DOORWAY,
  ENTRANCE_SMALL_ROOM_SOUTH_END
};

/// The exits, each to the entrance of the same name.
enum {
  EXIT_TO_CORRIDOR_WEST_DOORWAY = 1,
  EXIT_TO_HALL_SOUTH_END,
  EXIT_TO_SMALL_ROOM_SOUTH_END,
  EXIT_TO_CORRIDOR_EAST_DOORWAY
};

/// Raised by Burst once the ice is gone: the dragons wake.
#define DRAGON_WAKE_TRIGGER 20
/// The cells of the ice the dragons sleep in, which Burst breaks.
#define CELL_DRAGON_ICE 20
/// The cell before the great dragon head on the north wall, which asks for
/// something to be placed in it, and on which the Mars Star is used.
#define CELL_STATUE_FACE 21

enum {
  /// The two torches on stands before the frozen dragons: the east one burns,
  /// the west one is unlit. Blaze's cast starts at the east one and hits the
  /// west one, which sends out the sparkles.
  ACTOR_EAST_TORCH = 8,
  ACTOR_WEST_TORCH = 9,
  /// The crack the fireballs leave in the ice the dragons sleep in.
  ACTOR_ICE_CRACK = 10,
  ACTOR_AGATIO_DRAGON = 11, ///< the Flame Dragon that was Agatio
  ACTOR_KARST_DRAGON = 12,  ///< the Flame Dragon that was Karst
  ACTOR_AGATIO = 13,
  ACTOR_KARST = 14,
  /// The lights that rise from the dragons as they turn back into Adepts.
  ACTOR_AGATIO_LIGHT = 15,
  ACTOR_KARST_LIGHT = 16,
  /// The sparkles Blaze sends from the west torch round each side of the
  /// hall; at the end of its way each launches its fireball, which rolls into
  /// the ice and cracks it.
  ACTOR_EAST_SPARKLE = 17,
  ACTOR_WEST_SPARKLE = 18,
  ACTOR_EAST_FIREBALL = 19,
  ACTOR_WEST_FIREBALL = 20
};

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

/// A torch's script call: the torches stand without a shadow.
s32 MarsLighthouseStarRoom_SetUpTorch(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Felix casts Blaze at the burning torch on the east stand before the frozen
/// dragons, and its jet of flame comes out long and fast. Cast westward from
/// beside it, the jet runs through the torch and on across the floor to the
/// unlit torch on the west stand, which flashes white and gives off sparks.
///
/// The cast sets `blazeLongSpray`, which gives Blaze its long jet in place of
/// the short one; every cast clears it again as it starts.
void MarsLighthouseStarRoom_StartBlazeOnEastTorch(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  event->blazeLongSpray = 1;
}

/// Blaze's flame reaches the unlit torch on the west stand before the frozen
/// dragons, which sends out the two sparkles that launch the fireballs.
void MarsLighthouseStarRoom_BlazeWestTorch(void) {
  MarsLighthouseStarRoom_ShootFireballs();
}

/// Nothing happens when Blaze ends at the west torch; the sparkles and
/// fireballs go on alone.
void MarsLighthouseStarRoom_EndBlazeOnWestTorch(void) {}

FIXED Trig_Cos(s32 angle);
void Actor_SetupScriptedPart(struct Actor *actor, s32 priority);

/// Creates one of the dragon hall's effects at a point in the room.
static __inline__ struct Actor *
MarsLighthouseStarRoom_CreateActor(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

void Actor_StartScript(struct Actor *actor, const void *script);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 value);
extern s16 gSlideOnIce_SteerAngles[];

extern u32 gDebris_Script[];

/// Six chips of ice burst from an actor and bounce away: the wind stone's
/// sprite, tinted to ice. Each flies off on a random heading at up to 8 pixels
/// a frame, and rises at 2 to 6, higher the further round its heading is.
void MarsLighthouseStarRoom_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  for (i = 0; i < 6; i++) {
    FIXED x = source->x.fixed + (Random16() << 4) - (Random16() << 4);
    struct Actor *actor = MarsLighthouseStarRoom_CreateActor(
        SPRITE_WIND_STONE, x, (Random16() << 3) + source->y.fixed + toFIXED(4),
        source->z.fixed + (Random16() << 4) - (Random16() << 4));

    if (actor != NULL) {
      s32 heading;
      s32 speed;
      s32 scale;

      heading = Random16();
      speed = Random16();
      scale = ((u32)Random16() >> 1) + FIXED_ONE / 4;
      Actor_StartScript(actor, gDebris_Script);
      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = heading * 4 + toFIXED(2);
      actor->velocityZ = Iwram_MulFX(speed * 8, Trig_Cos(heading));
      actor->velocityX = Iwram_MulFX(speed * 8, Math_Sin(heading));
      actor->acceleration = 0;
      actor->gravity = toFIXED(33) / 100;
      actor->scaleX = scale;
      actor->scaleY = scale;
      actor->bounce = FIXED_ONE / 2;
      Actor_SetSpriteColorMode(actor, SPRITE_COLOR_GRB);
    }
  }
}

/// The west fireball reaches the ice: a small crack shows, unless the east one
/// came first.
void MarsLighthouseStarRoom_WestFireballCracksIce(void) {
  struct Actor *actor = Actor_Get(ACTOR_ICE_CRACK);

#if !TLA_PATCH_1
  if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
    Event_PlaySound(SE_ICE_CRACK);
    actor->x.fixed = INTtoFIXED(actor->homeX);
    actor->z.fixed = INTtoFIXED(actor->homeZ);
    if (Event_GetFlag(FLAG_DRAGON_ICE_CRACKED) == 0) {
      actor->scaleX = FIXED_ONE * 3 / 5;
      actor->scaleY = FIXED_ONE * 3 / 5;
    }
  }
}

/// The east fireball reaches the ice: the whole crack shows, and Burst can
/// break it.
void MarsLighthouseStarRoom_EastFireballCracksIce(void) {
  struct Actor *actor = Actor_Get(ACTOR_ICE_CRACK);

#if !TLA_PATCH_1
  if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !TLA_PATCH_2
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
  if (Event_GetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
      Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
    Event_PlaySound(SE_ICE_CRACK);
    Event_SetFlag(FLAG_BURST_CAN_BREAK_DRAGON_ICE);
    actor->x.fixed = INTtoFIXED(actor->homeX);
    actor->z.fixed = INTtoFIXED(actor->homeZ);
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
    Event_SetFlag(FLAG_DRAGON_ICE_CRACKED);
  }
}

void MarsLighthouseStarRoom_ClearDragonIce(void);

/// Felix casts Burst on the cracked ice the Flame Dragons sleep in, cells
/// (41, 14) to (43, 14): a flash, the ice gone, chips flying, and the dragons
/// wake.
void MarsLighthouseStarRoom_BurstIce(s32 phase, s32 frame) {
  if (phase == 1) {
    Event_PlaySound(SE_FLASH);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(60);
    MarsLighthouseStarRoom_ClearDragonIce();
    Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
#if TLA_PATCH_1
    Event_SetFlag(FLAG_DRAGON_ICE_BURST);
#endif
    gFieldState->raisedTrigger = DRAGON_WAKE_TRIGGER;
  }
  // From frame 97 to 271, chips burst from the ice every 30 frames.
  if ((u32)(frame - 97) <= 174 && frame % 30 == 0)
    MarsLighthouseStarRoom_ScatterIceChips(ACTOR_ICE_CRACK);
  if (frame == 276) {
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_Interpolate(20);
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

#include "shared/actor_freeze.h"

void MarsLighthouseStarRoom_WakeAfterBattle(void);
void MarsLighthouseStarRoom_CreateMarsStar(s32 rise);
void MarsLighthouseStarRoom_LayDownAdept(s32 actor);
void FlameThrowers_Create(void);
void CrackedTile_DropIn(void);
void MapGrid_CenterOnFollowedPoint(void);
void FieldEffect_StartBlankOverlay(s32 a);
struct Actor *gMarsLighthouseStarRoom_MarsStar BSS_DATA;

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineState.next;
  s32 entrance = gMainRoutineState.entrance;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gMarsLighthouseStarRoom_MarsStar = NULL;
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  FlameThrowers_Create();
  FlameThrowers_Start(0, 0, 0, STEP_TRIGGER_FLAME_HIT);
  if (scene == (s32)&SceneId_MarsLighthouseStarRoom) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
      // The ice is gone from the hall.
      MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
      MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
    }
    // Through the crack in the floor above: the party lands below it.
    if (entrance ==
        MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_B) {
      struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
      struct FieldState *state = Ram_HeapSlots->fieldState;
      s32 column = FieldGameFlag_GetByte(FLAG_BYTE_FALL_COLUMN);
      s32 row = FieldGameFlag_GetByte(FLAG_BYTE_FALL_ROW);
      struct Actor *other;
      s32 x;

      x = FIELD_CELL_CENTER(column);
      leader->x.fixed = x;
      leader->z.fixed = FIELD_CELL_ORIGIN(row) - toFIXED(296);
      other = state->camera;
      other->x.fixed = x;
      other->z.fixed = leader->z.fixed;
      MapGrid_CenterOnFollowedPoint();
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
    if (entrance ==
            MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_EAST ||
        entrance ==
            MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_WEST) {
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
    if (entrance == ENTRANCE_HALL_SOUTH_END ||
        entrance == MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE) {
      if (!Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
        FieldActor_SetSpritePriority(ACTOR_AGATIO_DRAGON, 3);
        FieldActor_SetSpritePriority(ACTOR_KARST_DRAGON, 3);
        Actor_GetById(ACTOR_KARST_DRAGON)->scaleX = -FIXED_ONE;
      }
      if (Event_GetFlag(FLAG_FLAME_DRAGONS_BEATEN)) {
        MarsLighthouseStarRoom_ClearDragonIce();
        Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
      } else if (Event_GetFlag(FLAG_DRAGON_ICE_CRACKED)) {
        Event_SetFlag(FLAG_BURST_CAN_BREAK_DRAGON_ICE);
      } else {
        Actor_SetPosition(ACTOR_ICE_CRACK, 0, 0);
      }
      if (Event_GetFlag(FLAG_ADEPTS_WOKE)) {
        if (!Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
          Actor_SetPosition(ACTOR_AGATIO_DRAGON, 0, 0);
          Actor_SetPosition(ACTOR_KARST_DRAGON, 0, 0);
        }
        MarsLighthouseStarRoom_ClearDragonIce();
        System_Wait(10);
        if (!Event_GetFlag(FLAG_MARS_STAR_TAKEN)) {
          MarsLighthouseStarRoom_CreateMarsStar(0);
          if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
            MarsLighthouseStarRoom_LayDownAdept(ACTOR_AGATIO);
            MarsLighthouseStarRoom_LayDownAdept(ACTOR_KARST);
          }
        }
        if (Actor_GetById(ACTOR_AGATIO)->x.fixed > toFIXED(400)) {
          MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
          MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
        }
      } else if (entrance ==
                 MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE) {
        MarsLighthouseStarRoom_WakeAfterBattle();
      }
    }
  }
  return 0;
}

void Map_CopyPalette(s32 scene, s32 a1);

s32 Scene_PrepareMap(void) {
  if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
    Map_CopyPalette(gMainRoutineState.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
                    (s32)BG_PLTT);
  if (gMainRoutineState.entrance ==
      MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE)
    Event_SetFlag(FLAG_FLAME_DRAGONS_BEATEN);
  return 0;
}

void Event_Start(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);

/// Free of the ice, the Flame Dragons rise and roar, and the battle begins.
void MarsLighthouseStarRoom_WakeFlameDragons(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  MarsLighthouseStarRoom_ClearDragonIce();
  Event_PlaySound(MUS_SILENCE);
  FieldEvent_Wait(40);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, 8);
  FieldActor_Jump(gMainRoutineState.leaderActorId, 4, 10);
  Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, 16);
  FieldActor_Jump(gMainRoutineState.leaderActorId, 6, 20);
  Event_PlaySound(MUS_BATTLE_WITH_FLAME_DRAGON);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 2);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 2);
  FieldEvent_Wait(24);
  Actor_SetPosition(ACTOR_AGATIO_DRAGON, 696, 232);
  Actor_SetPosition(ACTOR_KARST_DRAGON, 664, 232);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 3);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 3);
  Event_PlaySound(SE_TREMOR);
  MapGrid_SetShake(2, 4, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_BOSS_ROAR);
  Actor_SetAnimation(ACTOR_AGATIO_DRAGON, 4);
  Actor_SetAnimation(ACTOR_KARST_DRAGON, 4);
  FieldEvent_Wait(60);
  gMainRoutineState.battleType = BATTLE_TYPE_NO_ESCAPE;
  FieldEvent_SetAfterBattleDestination(
      (s32)&SceneId_MarsLighthouseStarRoom,
      MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE);
  gMainRoutineState.battleBackground = SCENE_GONDOWAN_CLIFFS_HIDEOUT;
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
}

void Event_End();
void Actor_PlaceOnActor();
void Scene_PlayMusic();
extern u8 MsgMarsLighthouseStarRoomWhyWhyAmILying[];
extern s32 gMarsLighthouseStarRoom_LightRiseScript[];
extern s32 gMarsLighthouseStarRoom_LightSinkScript[];
extern s32 gMarsLighthouseStarRoom_AgatioDragonShrinkScript[];
extern s32 gMarsLighthouseStarRoom_KarstDragonShrinkScript[];

/// After the battle the dragons shrink into two lights that sink to the floor,
/// and Agatio and Karst lie there: lost in the frozen lighthouse, its eye took
/// their will, and they ask Felix to light the beacon with the Mars Star.
void MarsLighthouseStarRoom_WakeAfterBattle(void) {
  s32 answer;
  struct Actor *dragon, *agatioLight, *karstLight;

  dragon = Actor_Get(ACTOR_AGATIO_DRAGON);
  agatioLight = Actor_Get(ACTOR_AGATIO_LIGHT);
  karstLight = Actor_Get(ACTOR_KARST_LIGHT);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MarsLighthouseStarRoom_ClearDragonIce();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_AGATIO), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KARST), 0);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO_LIGHT), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST_LIGHT), SPRITE_COLOR_WHITE);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(dragon, dragon->x.fixed, 0,
                                       dragon->z.fixed - toFIXED(8));
  agatioLight->motionFlags = 0;
  agatioLight->scaleX = FIXED_ONE / 4;
  agatioLight->scaleY = FIXED_ONE / 4;
  karstLight->motionFlags = 0;
  karstLight->scaleX = FIXED_ONE / 4;
  karstLight->scaleY = FIXED_ONE / 4;
  FieldCamera_MoveTo(680, CAMERA_KEEP, 228, 0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 680, 264);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO_DRAGON), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST_DRAGON), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO), SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST), SPRITE_COLOR_WHITE);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_WARP);
  Actor_PlaceOnActor(ACTOR_AGATIO_LIGHT, ACTOR_AGATIO_DRAGON);
  Actor_PlaceOnActor(ACTOR_KARST_LIGHT, ACTOR_KARST_DRAGON);
  System_Wait(1);
  Actor_SetScript(ACTOR_AGATIO_DRAGON,
                  (s32)&gMarsLighthouseStarRoom_AgatioDragonShrinkScript);
  Actor_SetScript(ACTOR_KARST_DRAGON,
                  (s32)&gMarsLighthouseStarRoom_KarstDragonShrinkScript);
  Actor_SetScript(ACTOR_AGATIO_LIGHT,
                  (s32)&gMarsLighthouseStarRoom_LightRiseScript);
  Actor_RunScriptAndWait(ACTOR_KARST_LIGHT,
                         (s32)&gMarsLighthouseStarRoom_LightRiseScript);
  MarsLighthouseStarRoom_LayDownAdept(ACTOR_AGATIO);
  MarsLighthouseStarRoom_LayDownAdept(ACTOR_KARST);
  Actor_SetScript(ACTOR_AGATIO_LIGHT,
                  (s32)&gMarsLighthouseStarRoom_LightSinkScript);
  Actor_RunScriptAndWait(ACTOR_KARST_LIGHT,
                         (s32)&gMarsLighthouseStarRoom_LightSinkScript);
  FieldEvent_Wait(10);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_AGATIO), SPRITE_COLOR_NORMAL);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KARST), SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_SetAnimation(ACTOR_AGATIO, 5);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)&MsgMarsLighthouseStarRoomWhyWhyAmILying);
  // "Why... Why am I lying here?"
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 80);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 1);
  FieldEvent_Wait(20);
  Event_PlaySound(MUS_KARST_AND_AGATIO);
  // "Wait, I think I remember... We were going to light the beacon..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_KARST, 1);
  Actor_SetAnimation(ACTOR_KARST, 5);
  FieldEvent_Wait(20);
  // "But the lighthouse was frozen. We wandered lost inside, trying to..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 20);
  // "We must hurry... If we delay, Prox is destroyed."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_RunRepeatedMotion(ACTOR_AGATIO, 2);
  Actor_SetAnimation(ACTOR_AGATIO, 6);
  // "It's no use... I'm absolutely drained... I can't even stand."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_KARST, 6);
  // "And the cold... Why am I so cold? We of the Fire Clan are meant to..."
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 20);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I feel the cold too, Karst... I... I'm afraid we're going to die up..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_ShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "I can feel it now... The long, dark sleep creeping into my soul..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "Wait... I think I remember..."
  FieldEvent_ShowMessageThenWait(ACTOR_AGATIO, 0, 20);
  // "The eye... It told us that we lack the will to go any farther."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_KARST, 7);
  // "The eye... I remember now... I felt its gaze pierce me to the core..."
  Event_ShowMessage(ACTOR_KARST, 0);
  // "That's right... And I remember fighting against Felix..."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  // "I do, too... Was it a dream? An illusion?"
  FieldEvent_ShowMessageThenWait(ACTOR_KARST, 0, 40);
  Actor_ShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Felix? Is that you?"
  Event_OpenMessage(ACTOR_AGATIO, 0);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  if (answer == 0) {
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    Actor_SetAnimation(ACTOR_AGATIO, 7);
    Event_ShowMessage(ACTOR_AGATIO, 0);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Actor_WaitForAnimationChange(gMainRoutineState.leaderActorId);
    Actor_SetAnimation(ACTOR_AGATIO, 6);
    Event_ShowMessage(ACTOR_AGATIO, 0);
  }
  FieldEvent_Wait(40);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KARST, 2);
  Event_ShowMessage(ACTOR_KARST, 0);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  MarsLighthouseStarRoom_CreateMarsStar(1);
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetAnimationAndWait(ACTOR_AGATIO, 7);
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_AGATIO, 10);
  Actor_SetAnimation(ACTOR_KARST, 9);
  Event_SetFlag(FLAG_ADEPTS_WOKE);
  Scene_PlayMusic();
  Event_End();
}

/// The Mars Star's update: drawn at the leader's sprite priority.
void MarsLighthouseStarRoom_DrawStarWithLeader(struct Actor *actor) {
  struct Actor *owner = Actor_Get(gMainRoutineState.leaderActorId);

  ((struct FieldSprite *)actor->sprite)->priority =
      ((struct FieldSprite *)owner->sprite)->priority;
}

/// The Mars Star as an actor on the floor by Agatio, rising first when asked.
void MarsLighthouseStarRoom_CreateMarsStar(s32 rise) {
  struct Actor **slot = &gMarsLighthouseStarRoom_MarsStar;
  IconTransfer *glyph;
  struct Actor *actor;
  struct FieldSprite *sprite;
  s32 size;
  u8 clear = 0;

  actor =
      FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(702), 0, toFIXED(224));
  *slot = actor;
  if (actor == NULL)
    return;
  sprite = actor->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
  actor->update = MarsLighthouseStarRoom_DrawStarWithLeader;
  size = sizeof(IconTransfer);
  sprite->flags = clear;
  sprite->count = clear;
  glyph = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, size);
  Item_LoadIcon(ITEM_MARS_STAR);
  Vram_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  if (rise) {
    System_Wait(1);
    (*slot)->velocityY = toFIXED(5) / 2;
    System_Wait(4);
    Event_PlaySound(SE_CLICK);
    System_Wait(8);
    Event_PlaySound(SE_CLICK);
  }
}

void Event_End(void);

/// Felix talks to Karst, lying on the floor of the dragon hall where her
/// dragon fell: "Your hand...so warm... People have such warm hands..."
void MarsLighthouseStarRoom_HoldWarmHand(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(ACTOR_KARST, 5);
  FieldEvent_SetMessageId((s32)MsgMarsLighthouseStarRoomYourHandSoWarmPeople);
  // "Your hand...so warm... People have such warm hands... I had forgotten..."
  Event_ShowMessage(ACTOR_KARST, 0);
  Actor_SetAnimation(ACTOR_KARST, 9);
  Event_End();
}

s32 Inventory_AddItem(s32 owner, s32 item);
struct Unit *Unit_Get(u32 unitId);
void Inventory_Equip(s32 owner, s32 slot);
void Actor_Release(struct Actor *actor);
void FieldCamera_WaitForArrival(void);

/// Felix talks to Agatio, lying on the floor of the dragon hall: "It's too late
/// for us now... Hurry, light the beacon." The first time, the Mars Star flies
/// up from the floor to Felix and goes into the Mythril Bag, and the view turns
/// to the great dragon head on the north wall, which glows and speaks: "You who
/// hold the star, open your heart..."
void MarsLighthouseStarRoom_TakeMarsStar(void) {
  struct Actor *leader;
  struct Actor *actor;
  s32 i;
  u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetAnimation(ACTOR_AGATIO, 5);
  Event_SetMessageId((s32)MsgMarsLighthouseStarRoomItsTooLateForUs);
  // "It's too late for us now... Hurry, light the beacon."
  Event_ShowMessage(ACTOR_AGATIO, 0);
  Actor_SetAnimation(ACTOR_AGATIO, 10);
  if (!Event_GetFlag(FLAG_MARS_STAR_TAKEN)) {
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_ARMS_RAISED);
    actor = gMarsLighthouseStarRoom_MarsStar;
    actor->motionFlags = 0;
    actor->speedLimit = toFIXED(4);
    actor->acceleration = toFIXED(2);
    FieldActor_SetMoveTargetFX(actor, leader->x.fixed,
                               leader->y.fixed + toFIXED(32), leader->z.fixed);
    FieldActor_WaitForMove(gMarsLighthouseStarRoom_MarsStar);
    Event_PlaySound(MUS_TREASURE_FOUND);
    PartyInventory_Remove(ITEM_MYTHRIL_BAG);
    Inventory_AddItem(FELIX, ITEM_MYTHRIL_BAG_MARS_STAR);
    // "Felix got the Mars Star!"
    Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixGotMarsStar, 1);
    for (i = 0; i < INVENTORY_SLOTS; i++) {
      if ((Unit_Get(FELIX)->inventory[i] & INVENTORY_ITEM_ID_MASK) ==
          ITEM_MYTHRIL_BAG_MARS_STAR) {
        Inventory_Equip(FELIX, i);
        break;
      }
    }
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
    Event_SetFlag(FLAG_MARS_STAR_TAKEN);
    FieldActor_SetPositionAndResetMotion(gMarsLighthouseStarRoom_MarsStar, 0, 0,
                                         0);
    System_Wait(1);
    Actor_Release(gMarsLighthouseStarRoom_MarsStar);
    System_Wait(1);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(680, CAMERA_KEEP, 146, 1);
    FieldCamera_WaitForArrival();
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
    PaletteFade_Interpolate(20);
    System_Wait(20);
    *color = RGB_WHITE;
    System_Wait(1);
    FieldMessage_ShowCentered((s32)MsgMarsLighthouseStarRoomYouWhoHoldStarOpen,
                              0, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(20);
    System_Wait(20);
  }
  Event_End();
}

/// The block of ice the dragons slept in is gone from the floor.
void MarsLighthouseStarRoom_ClearDragonIce(void) {
  MapGrid_CopyCellsTo(46, 68, 40, 73, 5, 6);
  MapGrid_CopyCellAttributes(104, 9, 5, 6, 40, 9);
}

void Actor_SetSpriteFlags(struct Actor *actor, u32 flags);

/// Agatio or Karst lies on the floor where the dragon fell.
void MarsLighthouseStarRoom_LayDownAdept(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetSpriteFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->collisionFlags = 0;
  if (id == ACTOR_AGATIO) {
    Actor_SetPosition(ACTOR_AGATIO, 700, 224);
    Actor_SetAnimation(ACTOR_AGATIO, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, 13);
  } else {
    Actor_SetPosition(ACTOR_KARST, 660, 216);
    Actor_SetAnimation(ACTOR_KARST, 9);
    MapGrid_CopyCellAttributes(31, 21, 2, 2, 40, 12);
  }
}

/// Felix examines the great dragon head on the north wall of the dragon hall,
/// cell (42, 8): "There's a space for you to place something." Then the field
/// menu opens, for the party to choose what to place.
void MarsLighthouseStarRoom_ExamineStatue(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  // "There's a space for you to place something."
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomTheresSpace, 1);
  state->fieldMenu = 1;
  Event_End();
}

void Scene_PlayMusic(void);
void MarsLighthouseStarRoom_SpawnRisingSparks(void);

#if !TLA_PATCH_1

/// Felix uses the Mars Star on the great dragon head on the north wall of the
/// dragon hall and sets it in the head's face: the room shakes, sparks rise,
/// the ice melts into lava and the lighthouse turns back to fire; the star
/// falls back out.
void MarsLighthouseStarRoom_UseMarsStar(void) {
  struct Actor **slot;
  struct Actor *actor;
  struct FieldSprite *sprite;
  IconTransfer *glyph;
  u16 *palette;

  palette = (u16 *)Ram_HeapSlots->paletteFade;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  // "Felix used the Mars Star!"
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  slot = &gMarsLighthouseStarRoom_MarsStar;
  actor = FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8),
                            toFIXED(147));
  *slot = actor;
  if (actor != 0) {
    sprite = actor->sprite;
    sprite->flags = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    actor->motionFlags = 0;
    actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
    sprite->count = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    glyph = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
    HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 680, 186);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 40);
  Event_PlaySound(MUS_SILENCE);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(MarsLighthouseStarRoom_SpawnRisingSparks, TASK_PASS_FRAME);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(MarsLighthouseStarRoom_SpawnRisingSparks);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Map_CopyPalette(gMainRoutineState.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
                  palette);
  /* FAKEMATCH(dead): a once-only block that places black, the colour the
   * palette's first entry takes, at its store; as black declared with the
   * routine's locals. */
  {
    s32 black = RGB(0, 0, 0);

    *palette = black;
  }
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  actor = *slot;
  if (actor != 0) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->velocityY = toFIXED(2);
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1);
    FieldActor_SetMoveTarget(actor, 672, 0, 160);
    FieldActor_WaitForMove(*slot);
    Event_PlaySound(SE_CLICK);
    actor = *slot;
    actor->acceleration = toFIXED(1) / 2;
    actor->velocityY = toFIXED(4);
    actor->speedLimit = toFIXED(1);
    FieldActor_SetMoveTarget(actor, 664, 0, 158);
    FieldActor_WaitForMove(*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  actor = *slot;
  if (actor != 0)
    Actor_Release(actor);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 680, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Scene_PlayMusic();
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
  gMainRoutineState.battleBackground = SCENE_NARIBWE;
  Event_End();
}

#else

/// Felix uses the Mars Star on the great dragon head on the north wall of the
/// dragon hall and sets it in the head's face: the room shakes, sparks rise,
/// the ice melts into lava and the lighthouse turns back to fire; the star
/// falls back out.
void MarsLighthouseStarRoom_UseMarsStar(void) {
  s32 flag;
  struct Actor **slot, **placedStarSlot;
  struct Actor *actor, *placedStar;
  struct FieldSprite *sprite, *placedStarSprite;
  IconTransfer *glyph, *placedStarGlyph;
  u16 *palette;

  flag = Event_GetFlag(FLAG_MARS_STAR_SET);
  if (flag == 0) {
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    // "Felix used the Mars Star!"
    Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    System_Wait(1);
    Event_PlaySound(SE_GEM_SET);
    slot = &gMarsLighthouseStarRoom_MarsStar;
    actor = (struct Actor *)FieldActor_Create(
        SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8), toFIXED(147));
    *slot = actor;
    if (actor != 0) {
      sprite = actor->sprite;
      sprite->flags = 0;
      sprite->fullColor = 0;
      sprite->palette = 0;
      sprite->priority = 2;
      actor->motionFlags = 0;
      actor->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
      sprite->count = 0;
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      glyph = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
      Item_LoadIcon(ITEM_MARS_STAR);
      Vram_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
      HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 680, 186);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 80);
    Actor_ShowEmote(gMainRoutineState.leaderActorId,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    actor = *slot;
    if (actor != 0) {
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      actor->velocityY = toFIXED(2);
      actor->speedLimit = toFIXED(2);
      actor->acceleration = toFIXED(1);
      FieldActor_SetMoveTarget(actor, 672, 0, 160);
      FieldActor_WaitForMove((s32)*slot);
      Event_PlaySound(SE_CLICK);
      actor = *slot;
      actor->acceleration = toFIXED(1) / 2;
      actor->velocityY = toFIXED(4);
      actor->speedLimit = toFIXED(1);
      FieldActor_SetMoveTarget(actor, 664, 0, 158);
      FieldActor_WaitForMove((s32)*slot);
    }
    FieldEvent_Wait(20);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
    Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
        ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 670, 170);
    FieldEvent_Wait(1);
    Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
        ACTOR_FACES_MOTION;
    actor = *slot;
    if (actor != 0)
      Actor_Release(actor);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
    Scene_PlayMusic();
    Event_End();
    return;
  }
  palette = (u16 *)Ram_HeapSlots->paletteFade;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  // "Felix used the Mars Star!"
  Message_ShowCentered((s32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(1);
  Event_PlaySound(SE_GEM_SET);
  placedStarSlot = &gMarsLighthouseStarRoom_MarsStar;
  placedStar = FieldActor_Create(SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8),
                                 toFIXED(147));
  *placedStarSlot = placedStar;
  if (placedStar != 0) {
    placedStarSprite = placedStar->sprite;
    placedStarSprite->flags = 0;
    placedStarSprite->count = 0;
    placedStarSprite->fullColor = 0;
    placedStarSprite->palette = 0;
    placedStarSprite->priority = 2;
    placedStar->motionFlags = 0;
    placedStar->retainSpriteCache = ACTOR_KEEP_SPRITE_VRAM;
    placedStar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    placedStarGlyph =
        HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
    Item_LoadIcon(ITEM_MARS_STAR);
    Vram_Load(placedStarSprite->slot, ICON_TILE_BYTES, placedStarGlyph->tiles);
    HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 680, 186);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 40);
  Event_PlaySound(MUS_SILENCE);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  MapGrid_SetShake(1, 1, 1);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(MarsLighthouseStarRoom_SpawnRisingSparks, TASK_PASS_FRAME);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(80);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove(MarsLighthouseStarRoom_SpawnRisingSparks);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Map_CopyPalette(gMainRoutineState.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
                  palette);
  *palette = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(680, CAMERA_KEEP, 186, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  placedStar = *placedStarSlot;
  if (placedStar != 0) {
    placedStar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    placedStar->velocityY = toFIXED(2);
    placedStar->speedLimit = toFIXED(2);
    placedStar->acceleration = toFIXED(1);
    FieldActor_SetMoveTarget(placedStar, 672, 0, 160);
    FieldActor_WaitForMove((s32)*placedStarSlot);
    Event_PlaySound(SE_CLICK);
    placedStar = *placedStarSlot;
    placedStar->acceleration = toFIXED(1) / 2;
    placedStar->velocityY = toFIXED(4);
    placedStar->speedLimit = toFIXED(1);
    FieldActor_SetMoveTarget(placedStar, 664, 0, 158);
    FieldActor_WaitForMove((s32)*placedStarSlot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1.6);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 670, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  placedStar = *placedStarSlot;
  if (placedStar != 0)
    Actor_Release(placedStar);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
  Scene_PlayMusic();
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
  Event_SetFlag(FLAG_MARS_STAR_SET);
  gMainRoutineState.battleBackground = SCENE_NARIBWE;
  Event_End();
}

#endif

#define RISING_SPARK_FRAMES 60 ///< a spark's life
/// The frames a rising spark has left, kept where an actor keeps its watched
/// actor.
#define RISING_SPARK_FRAMES_LEFT(actor) (*(s32 *)&(actor)->watchedActor)

/// A spark rises and grows for a second, then is gone.
void MarsLighthouseStarRoom_UpdateRisingSpark(struct Actor *actor) {
  s32 growth;

  actor->y.fixed += toFIXED(1) / 2;
  growth = FIXED_ONE / 50;
  actor->scaleX += growth;
  actor->scaleY += growth;
  RISING_SPARK_FRAMES_LEFT(actor)--;
  if (RISING_SPARK_FRAMES_LEFT(actor) == 1)
    FieldActor_SetPositionAndResetMotion(actor, 0, 0, 0);
  else if (RISING_SPARK_FRAMES_LEFT(actor) == 0)
    Actor_Release(actor);
}

void Actor_SetBlendMode(struct Actor *actor, u32 value);

/// The two strips of the hall's floor the sparks rise from, each 28 pixels
/// wide and 120 deep: their west edges and their north edge.
#define WEST_SPARKS_X 620
#define EAST_SPARKS_X 712
#define SPARKS_Z 460

/// While the lighthouse thaws, sparks rise from the floor every other frame.
void MarsLighthouseStarRoom_SpawnRisingSparks(void) {
  u32 frame = gFrameCount;
  struct Actor *actor;
  struct FieldSprite *sprite;
  u32 x, r;
  s32 z;
  s32 left;

  if (frame & 1)
    return;
  // Each side makes its own spark, in turn: x is a random 28 pixels from the
  // strip's west edge, z a random 120 from SPARKS_Z.
  if ((frame >> 1) & 1) {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    left = toFIXED(WEST_SPARKS_X);
    z <<= 3;
    x <<= 16;
    x += left;
    z = FIXEDtoINT((u32)z);
    z = INTtoFIXED(z);
    z += toFIXED(SPARKS_Z);
    actor = MarsLighthouseStarRoom_CreateActor(SPRITE_DUST_CLOUD, x,
                                               toFIXED(32), z);
  } else {
    r = Random16();
    x = r << 3;
    x -= r;
    z = Random16() * 15;
    x <<= 2;
    x >>= 16;
    left = toFIXED(EAST_SPARKS_X);
    z <<= 3;
    x <<= 16;
    x += left;
    z = FIXEDtoINT((u32)z);
    z = INTtoFIXED(z);
    z += toFIXED(SPARKS_Z);
    actor = MarsLighthouseStarRoom_CreateActor(SPRITE_DUST_CLOUD, x,
                                               toFIXED(32), z);
  }
  if (actor != NULL) {
    actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite = actor->sprite;
    sprite->priority = 2;
    Actor_SetSpriteFlags(actor, 0);
    actor->scaleX = FIXED_ONE / 2;
    actor->scaleY = FIXED_ONE / 2;
    RISING_SPARK_FRAMES_LEFT(actor) = RISING_SPARK_FRAMES;
    actor->update = MarsLighthouseStarRoom_UpdateRisingSpark;
    Actor_SelectAnimation(actor, 5);
    Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
  }
}

extern s32 gMarsLighthouseStarRoom_WestSparkleRoute[];
extern s32 gMarsLighthouseStarRoom_EastSparkleRoute[];

/// The west torch sends its two sparkles round the hall, west and east; each
/// launches its fireball at the ice, the west one rolling east and the east
/// one west.
void MarsLighthouseStarRoom_ShootFireballs(void) {
  Actor_Attach(ACTOR_WEST_TORCH, ACTOR_WEST_SPARKLE, ACTOR_WEST_FIREBALL,
               FACING_EAST, gMarsLighthouseStarRoom_WestSparkleRoute);
  Actor_Attach(ACTOR_WEST_TORCH, ACTOR_EAST_SPARKLE, ACTOR_EAST_FIREBALL,
               FACING_WEST, gMarsLighthouseStarRoom_EastSparkleRoute);
}

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/flame_update_forward.h"

extern s32 gScriptedPart_Script[];

#include "shared/actor_reset_scripted_for.h"

/// A flame thrower's flame sent forward from `position` at `speed`; `flag`
/// keeps its sprite when off screen.
void FlameThrower_SendForward(s32 *position, s32 speed, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                          position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = speed;
    Actor_ResetScriptedFor(actor, 2);
    actor->update = Flame_UpdateForward;
    if (flag != 0)
      actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    Actor_SelectAnimation(actor, 2);
  }
}

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);

#include "shared/flame_update_sideways.h"

#include "shared/actor_setup_scripted_part.h"

/// Defines a flame thrower's flame sent backward from `position` at `speed`
/// under `updateRoutine`; `flag` keeps its sprite when off screen.
#define DEFINE_RIDE_SPAWN_BACKWARD(name, updateRoutine)                        \
  void name(s32 *position, s32 speed, s32 flag) {                              \
    struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],   \
                                            position[1], position[2]);         \
                                                                               \
    if (actor != NULL) {                                                       \
      actor->bounce = 0;                                                       \
      actor->gravity = 0;                                                      \
      actor->sceneWord = -speed;                                               \
      Actor_SetupScriptedPart(actor, 3);                                       \
      actor->update = updateRoutine;                                           \
      if (flag != 0)                                                           \
        actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;                   \
      Actor_SelectAnimation(actor, 2);                                         \
    }                                                                          \
  }

DEFINE_RIDE_SPAWN_BACKWARD(FlameThrower_SendBackward, Flame_UpdateForward)

/// A flame thrower's flame that lands as it goes forward from `position`.
void FlameThrower_SendEast(s32 *position, s32 speed, s32 flag) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                          position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateSideways;
    if (flag != 0)
      actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    Actor_SelectAnimation(actor, 2);
  }
}

DEFINE_RIDE_SPAWN_BACKWARD(FlameThrower_SendWest, Flame_UpdateSideways)

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted_for.h"

void FlameThrower_UpdateSpark();

/// A spark puffed from a flame thrower, drifting off in a random direction.
void FlameThrower_SendSpark(s32 *position, s32 base, s32 mode) {
  struct Actor *actor = MarsLighthouseStarRoom_CreateActor(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 spin;

  if (actor != 0) {
    actor->bounce =
        Trig_Cos((Random_Below(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->sceneWord = (u16)(Random16() << 1) + base;
    spin = (u16)Random16() - DEGtoANG(180);
    actor->timer = spin;
    Actor_SetupScriptedFor(actor, 2);
    actor->update = FlameThrower_UpdateSpark;
    Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
  }
}

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

#include "shared/flame_throwers_release_flame.h"

void FlameThrowers_SetMode(s16 value) {
  FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

s32 FlameThrowers_GetCounter(void) { return FLAME_THROWERS_STATE->counter; }

#include "shared/flame_throwers_advance_counter.h"

#include "shared/flame_thrower_set.h"

#include "shared/flame_thrower_set_puffs_off_screen.c"

#include "shared/flame_thrower_get_flags.c"

#include "shared/flame_throwers_set_stop.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);

#include "shared/leader_knock_back.c"

/// Hit by the flames, the leader is knocked back.
void MarsLighthouseStarRoom_KnockBackIfHit(void) {
  if (FLAME_THROWERS_STATE->active)
    Leader_KnockBack();
}

/// A rolling fireball hits Felix in the dragon hall once the Flame Dragons are
/// beaten: he is hurt, sits down blinking, and is carried along with the
/// fireball until it stops.
///
/// The fireball is the hit state's `car`; Felix moves by its step each frame,
/// stopping at the centre of a cell where the floor ends.
s32 Fireball_CarryLeader(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct FlameHitState *ride;
  struct Actor *leader;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 unused[3];

  if (event->transitionActive == 0) {
    ScreenTransition_Start(event->transition, event->transitionFrames);
    event->transitionActive = 1;
  }
  Event_ClearFlag(FLAG_MENU_OPEN);
  event->psynergy = 0;
  event->fieldMenu = 0;
  event->pauseMenu = 0;
  event->aButton = 0;
  event->encounter = 0;
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  // UB: the fall damage takes no arguments and returns nothing; it is passed
  // the arguments it gives Party_AddHp.
  ((s32 (*)())Party_TakeFallDamage)(-FALL_DAMAGE, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  ride = FLAME_THROWERS_STATE;
  Event_SetFlag(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Event_ClearFlag(FLAG_MENU_OPEN);
  leader->update = (ObjectUpdate)Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
  while (ride->car->timer == 0) {
    if (MapGrid_GetTerrainKindOrBumpAt(0, leader->x.fixed, leader->z.fixed) !=
        TERRAIN_ROUNDED_SQUARE) {
      leader->x.fixed += ride->car->stepX;
      leader->z.fixed += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
      if (ride->car->stepZ != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = NULL;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/fireball_carry_leader_if_hit.h"

/// Whether a flame or fireball has hit the leader (FlameHitState.active).
s32 MarsLighthouseStarRoom_IsLeaderHit(void) {
  return FLAME_THROWERS_STATE->active;
}

extern u32 gCrumbs_Script[];

#include "shared/actor_scatter_crumbs.c"

extern u32 gIceChips_Script[];

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_event.c"

#include "shared/actor_roll_and_crumble.c"

#include "shared/actor_attach.c"

#include "shared/actor_launch_ahead.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

volatile s16 gSlideOnIce_Stopped BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
s32 Actor_HasArrived(struct Actor *actor);

#include "shared/slide_on_ice_from_edge.c"

/// Stops the leader's slide on the ice where it is (nonzero), or lets it run.
void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_RIDE_SPAWN_BACKWARD
#undef IWRAM
#undef STATE
#undef Position
extern const s32 *gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern s32 gMarsLighthouseStarRoom_TorchScript[];

#include "shared/debris_script.c"

// clang-format off
/// The west sparkle's way from the west torch round the west side of the hall
/// and up to the ice, where it leaves its fireball to roll in.
s32 gMarsLighthouseStarRoom_WestSparkleRoute[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(616, 0, 264),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(616, 0, 248),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(568, 0, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(568, 32, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(568, 32, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(576, 32, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(576, 22, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(592, 22, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseStarRoom_WestFireballCracksIce),
    SC_CALL(Actor_Park),
    SC_END,
};

/// The east sparkle's longer way round the east side of the hall to the ice.
s32 gMarsLighthouseStarRoom_EastSparkleRoute[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(680, 0, 264),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(680, 0, 296),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(728, 0, 296),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(728, 0, 264),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(808, 0, 264),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(808, 0, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(808, 24, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(792, 24, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(792, 32, 240),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(792, 32, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(784, 32, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(784, 22, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_MOVE(768, 22, 216),
    SC_SYNC,
    SC_WAIT(2),
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseStarRoom_EastFireballCracksIce),
    SC_CALL(Actor_Park),
    SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/field_effect_duration_scripts.c"

s32 gMarsLighthouseStarRoom_TorchScript[] = {
    SC_CALL(MarsLighthouseStarRoom_SetUpTorch),
    SC_END,
};

/// A light rises from a shrinking dragon, growing, for 40 frames.
s32 gMarsLighthouseStarRoom_LightRiseScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 64),
    SC_ADD(SW_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

/// The light sinks to the floor, shrinking, and is put away.
s32 gMarsLighthouseStarRoom_LightSinkScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_ADD(SW_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/// The dragons shrink to nothing over 40 frames and are put away; Karst's,
/// which faces the other way, mirrored.
s32 gMarsLighthouseStarRoom_AgatioDragonShrinkScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gMarsLighthouseStarRoom_KarstDragonShrinkScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseStarRoom),
    SCENE_EXIT(EXIT_TO_CORRIDOR_WEST_DOORWAY, MarsLighthouseStarRoom,
               ENTRANCE_CORRIDOR_WEST_DOORWAY),
    SCENE_EXIT(EXIT_TO_HALL_SOUTH_END, MarsLighthouseStarRoom,
               ENTRANCE_HALL_SOUTH_END),
    SCENE_EXIT(EXIT_TO_SMALL_ROOM_SOUTH_END, MarsLighthouseStarRoom,
               ENTRANCE_SMALL_ROOM_SOUTH_END),
    SCENE_EXIT(EXIT_TO_CORRIDOR_EAST_DOORWAY, MarsLighthouseStarRoom,
               ENTRANCE_CORRIDOR_EAST_DOORWAY),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST,
        MarsLighthouseEntranceSolBladeChest,
        MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST,
        MarsLighthouseEntranceValkyrieMailChest,
        MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A,
        MarsLighthouseEntranceSolBladeChest,
        MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST,
        MarsLighthouseEntranceValkyrieMailChest,
        MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseStarRoom_TorchScript, 712, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseStarRoom_TorchScript, 648, 0, 264,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 680, 0, 240, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 664, 0, 216, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 696, 0, 216, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, EXIT_TO_CORRIDOR_WEST_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS,
     EXIT_TO_HALL_SOUTH_END},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS,
     EXIT_TO_SMALL_ROOM_SOUTH_END},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, EXIT_TO_CORRIDOR_EAST_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, STEP_TRIGGER_FIREBALL_HIT,
     IF_FLAG_SET(FLAG_FLAME_DRAGONS_BEATEN), EVENT(Fireball_CarryLeader)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_StartBlazeOnEastTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_BlazeWestTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     (u32)(MarsLighthouseStarRoom_EndBlazeOnWestTorch)},
    {TRIGGER_RAISED, DRAGON_WAKE_TRIGGER, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_WakeFlameDragons)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_DRAGON_ICE, IF_FLAG_SET(FLAG_BURST_CAN_BREAK_DRAGON_ICE),
     EVENT(MarsLighthouseStarRoom_BurstIce)},
    {TRIGGER_TALK, ACTOR_KARST, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_HoldWarmHand)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
     MSGNO(MsgMarsLighthouseStarRoomImSoColdColderThan)},
    {TRIGGER_TALK, ACTOR_AGATIO, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_TakeMarsStar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AGATIO, CONDITION_ALWAYS,
     MSGNO(MsgMarsLighthouseStarRoomImSoTiredFog)},
    {TRIGGER_EXAMINE, CELL_STATUE_FACE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_ExamineStatue)},
    {USE_ITEM(ITEM_MYTHRIL_BAG_MARS_STAR),
     CELL_STATUE_FACE | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(MarsLighthouseStarRoom_UseMarsStar)},
    {TRIGGER_TABLE_END},
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
