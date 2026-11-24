#ifndef ALCHEMY_FIELD_FIELD_SEQUENCE_H
#define ALCHEMY_FIELD_FIELD_SEQUENCE_H

/// @file
/// What the field_sequence object defines for the code outside it.

#include "types.h"
#include "field/encounter_zone.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "field/actor.h"

struct Actor;

/// How the leader moves (MainRoutineState.movementMode): which actor script
/// Actor_StartMovementModeScript runs for it.
enum LeaderMovementMode {
  /// Walking: the pad moves the leader on a layered map.
  LEADER_MOVEMENT_WALK = 0,
  LEADER_MOVEMENT_WORLD_MAP = 1, ///< walking on the World Map
  /// Only the configured keys are checked; the leader is moved by others.
  LEADER_MOVEMENT_KEYS = 2,
  LEADER_MOVEMENT_ROPE = 3,        ///< crossing a tied rope
  LEADER_MOVEMENT_TIGHTROPE = 4,   ///< crossing a tightrope
  LEADER_MOVEMENT_SAND = 5,        ///< as sand, by Sand, on a layered map
  LEADER_MOVEMENT_SHIP_ON_MAP = 6, ///< the ship on a layered map
  LEADER_MOVEMENT_SHIP = 7,        ///< the ship on the World Map
  LEADER_MOVEMENT_FLYING_SHIP = 8, ///< the winged ship on the World Map
  LEADER_MOVEMENT_WORLD_SAND = 9,  ///< as sand on the World Map
  LEADER_MOVEMENT_HOVER = 10,      ///< hovering, by Hover
  LEADER_MOVEMENT_WALKING = 11,    ///< gActor_WalkingScript's
  LEADER_MOVEMENT_NONE = 12        ///< the idle script: it does not move
};

/// A window transition's shape: SCENE_TRANSITION(TRANSITION_WINDOW, mode)'s
/// low byte, which ScreenTransition_UpdateScanlineTable draws.
enum WindowTransitionMode {
  WINDOW_TRANSITION_BARS = 0,   ///< a bar on each line, at random
  WINDOW_TRANSITION_SHAPE = 1,  ///< a shape from gScreenTransition_Shapes
  WINDOW_TRANSITION_NOISE = 2,  ///< ragged lines from the left edge
  WINDOW_TRANSITION_CIRCLE = 3, ///< a circle at the screen's centre
  /// Alternate lines wiping in from the left and the right.
  WINDOW_TRANSITION_INTERLACED_WIPE = 4,
  /// As WINDOW_TRANSITION_LEADER with inside and outside swapped and the
  /// progress run the other way; it flickers.
  WINDOW_TRANSITION_LEADER_INVERSE = 7,
  /// An ellipse around a point (the state's centre) whose outline ripples:
  /// Reveal's.
  WINDOW_TRANSITION_RIPPLE = 8,
  /// An ellipse wider than tall around the party leader, 16 pixels above its
  /// feet.
  WINDOW_TRANSITION_LEADER = 9,
  /// An ellipse a little taller than wide, 8 pixels above the leader's feet.
  WINDOW_TRANSITION_LEADER_LOW = 77
};

/// A rule choosing a scene's field music, ended by id -1: a scene or an area
/// (byScene), an entrance (-1 for any), a flag that must be set (-1 for none)
/// and the music.
struct MapMusicRule {
  s16 id;
  s16 entrance : 15;
  u16 byScene : 1;
  s16 flag;
  s16 music;
};

/// A rule naming the area a scene is in, ended by id -1: a scene or an area
/// (useSceneId), an entrance (-1 for any) and the area name's index.
struct AreaNameRule {
  s16 id;
  s16 entrance : 15;
  u16 useSceneId : 1;
  s32 areaNameIndex;
};

/// A distance beyond any field Psynergy's reach, where the search for its
/// nearest target starts.
#define BEYOND_REACH 9999

/// struct ActorPlacement (scene_tables.h), a row of a scene's actor
/// placement table, as Scene_FindActorPlacement reads it: its sprite unsigned
/// and its spot in plain words.
struct ActionDescriptor {
  u16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
};

s32 MapScreen_Show(s32 mode);
void WorldMap_ZoomOutWhileLHeld(void);
void BattleBackground_SelectForEnvironment(s32 terrain);
s32 GameFlag_IsConditionActive(s32 condition);
void *Scene_FindActorTalkTrigger(s32 id);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_UseItem(s32 arg, s32 slot);
void Actor_ReleaseIfPresent(struct Actor *actor);
/// How ItemDisplay_ShowItem and ItemDisplay_ShowPicture show a found item.
enum ItemDisplayFlag {
  ITEM_DISPLAY_SPARKLES = 1, ///< sparkles rise from it
  ITEM_DISPLAY_RAISED = 2    ///< the leader raises it overhead
};

void ItemDisplay_RaiseAboveLeader(struct Actor *actor);
void FieldPsynergy_Lift(void);
s32 Speaker_GetVoice(s32 id);

/// A field Psynergy or item use, packed into one value, as FieldState.psynergy
/// and FieldState.item hold it. bits: 13..10 the user (13 is ACT_PACKED) | 9..0
/// the Psynergy or item.
#define ACT_ID_MASK 0x3ff                 ///< bits 9..0
#define ACT_UNIT_SHIFT 10                 ///< bits 13..10
#define ABILITY_ID(c) ((c) & ACT_ID_MASK) ///< the Psynergy
#define ACT_ITEM_ID(c) ((c) & ACT_ID_MASK)
#define ACT_UNIT(c) (((c) >> ACT_UNIT_SHIFT) & 15) ///< who uses it
#define ACT_PACKED 0x2000                          ///< use it without paying PP

#endif
