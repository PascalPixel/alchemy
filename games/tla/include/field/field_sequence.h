#ifndef ALCHEMY_FIELD_FIELD_SEQUENCE_H
#define ALCHEMY_FIELD_FIELD_SEQUENCE_H

/// @file
/// What the field_sequence object defines for the code outside it.

#include "types.h"
#include "field/encounter_zone.h"
#include "field/field_bank.h"
#include "field/scene_tables.h"

struct Actor;

/// How a scene fades in and out: SCENE_TRANSITION's high byte, which
/// ScreenTransition_Start and ScreenTransition_Finish read.
enum ScreenTransitionStyle {
  TRANSITION_FADE = 0,          ///< from or to black
  TRANSITION_BACKDROP_FADE = 1, ///< from or to the backdrop colour
  TRANSITION_WINDOW = 2,        ///< a window shape the variant chooses
  TRANSITION_DISSOLVE = 3,
  /// Opening from, or closing to, the centre line; the variant picks how.
  TRANSITION_CENTRE_LINE = 4
};

/// A scene transition: its style and its variant.
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

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

/// A 16.16 coordinate's whole cell: a map grid cell is 16 pixels square.
#define FIELD_CELL_MASK 0xfff00000
/// The map grid cell a 16.16 coordinate lies in, counted from 0 along its
/// axis.
#define FIELD_CELL(coordinate) ((coordinate) >> 20)
/// The whole pixels of a 16.16 coordinate or distance, px() undone.
#define FIELD_PIXEL(coordinate) ((coordinate) >> 16)
/// A 16.16 coordinate's whole pixels, its fraction cleared.
#define FIELD_PIXEL_MASK 0xffff0000
/// The 16.16 coordinate of the middle of a map grid cell.
#define FIELD_CELL_CENTER(cell) (((cell) << 20) + px(8))

/// A rule choosing a scene's field music, ended by id -1: a scene or an area
/// (byScene), an entrance (-1 for any), a flag that must be set (-1 for none)
/// and the song.
struct SceneCueRule {
  s16 id;
  s16 entrance : 15;
  u16 byScene : 1;
  s16 flag;
  s16 cue;
};

/// A rule naming the area a scene is in, ended by id -1: a scene or an area
/// (useSceneId), an entrance (-1 for any) and the area name's index.
struct SceneAreaNameRule {
  s16 id;
  s16 entrance : 15;
  u16 useSceneId : 1;
  s32 areaNameIndex;
};

/// A row of the scene's trigger table as the Psynergy trigger search reads
/// it: its kind and flags (-1 ends the table), what it answers, its
/// condition and its value.
struct TriggerEvent {
  s32 control;
  s16 trigger;
  s16 condition;
  u32 value;
};

/// A row of a scene's actor placement table as the field reads it: its
/// sprite, condition, movement type, position, facing, the facing it takes
/// to talk and its flags.
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

s32 WorldPicture_Show(s32 mode);
void WorldMap_ZoomOutWhileLHeld(void);
void BattleBackground_SelectForEnvironment(s32 terrain);
s32 GameFlag_IsConditionActive(s32 condition);
void *Scene_FindActorTalkTrigger(s32 id);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Field_UseItem(s32 arg, s32 slot);
void Actor_ReleaseIfPresent(struct Actor *object);
void ItemDisplay_RaiseAboveLeader(struct Actor *actor);
void FieldPsynergy_Lift(void);
s32 Speaker_GetVoice(s32 id);

#endif
