#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes and the macros and
/// types more than one part uses. field_sequence.c includes it before the
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "ability_ids.h"
#include "debug/debug.h"
#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/battle_music.h"
#include "field/encounter_zone.h"
#include "field/event.h"
#include "field/field.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "field/world_map_encounter_rules.h"
#include "game/game.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "system/dma.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "window/battle_status.h"
#include "text/msg_ids.h"

/// The speakers' definitions (field_sequence_tables.c), read by
/// Speaker_FindDefinition.
extern const struct SpeakerDefinition gSpeakerDefinitions[];

/// A position Vector_AddPolarOffset moves: as a vector, or as its three
/// coordinates.
union PolarOffsetPosition {
  struct Vector3 vector;
  s32 coordinates[3];
};

STATIC_ASSERT(sizeof(union PolarOffsetPosition) == 12);

/// No user named: Field_UsePsynergy takes ISAAC, and Field_UseItem
/// whoever holds the item in the most inventory slots.
#define UNIT_NONE 15

/// Shows message m with the user's and the Psynergy's names, and waits.
#define PSYNERGY_MESSAGE(who, act, m)                                          \
  (MessageArgument_PushFar((who), MESSAGE_ARGUMENT_ACTOR),                     \
   MessageArgument_PushFar((act), MESSAGE_ARGUMENT_ABILITY),                   \
   FieldMessage_ShowFar((m), FIELD_MESSAGE_ZOOM))

/// A scene's encounter zones (gScene_EncounterRules, encounter_rules.s), ended
/// by scene -1.
struct SceneEncounterRule {
  s16 sceneId;               ///< -1 ends the table
  s16 entrance;              ///< -1 for any entrance
  s16 flag : 15;             ///< -1, or a flag that turns the rule off once set
  s16 firstZoneOnGround : 1; ///< the first zone is for ground 0, not 1
  /// The first of up to seven consecutive zones, rows of encounter_zones.c; 0
  /// for none on any ground.
  s16 firstZone;
};

/// The scenes' encounter rules; the first that matches the scene and entrance,
/// with its flag unset, gives the encounter zones.
extern const struct SceneEncounterRule gScene_EncounterRules[];

/// A rule choosing a scene's field music (gScene_MusicRules,
/// scene_rules.s), ended by id -1.
struct MapMusicRule {
  s16 id;            ///< a scene, or an area
  s16 entrance : 15; ///< -1 for any entrance
  u16 idIsMap : 1;   ///< 1 when id is a scene, 0 when it is an area
  s16 flag;          ///< -1, or a flag that must be set
  s16 music;         ///< sounds.h's MUS_ numbers
};

/// The field music rules; the first that matches the scene or its area, the
/// entrance and the flag gives the music.
extern const struct MapMusicRule gScene_MusicRules[];

/// A rule naming the area a scene is in (gArea_NameRules, scene_rules.s),
/// ended by id -1.
struct AreaNameRule {
  s16 id;            ///< a scene, or an area
  s16 entrance : 15; ///< -1 for any entrance
  u16 useMapId : 1;  ///< 1 when id is a scene, 0 when it is an area
  /// The area name, as an offset that most readers add to MsgDebugAreaName.
  /// 0 is the Title's name (area 0's rule), which a scene no rule
  /// matches also gets.
  s32 areaNameIndex;
};

/// The area name rules; the first that matches the scene or its area and the
/// entrance gives the name.
extern const struct AreaNameRule gArea_NameRules[];

/// A run of screen lines in a transition shape: one window edge pair for every
/// line, or, when that is 0, edges moving from left x0 and right y0 to left
/// x1 and right y1 down the run.
struct TransitionSpan {
  u16 count; ///< lines; 0 for an unused run
  /// The (left << 8 | right) pair every line of the run takes, or 0 for the
  /// sloped edges below.
  u16 value;
  u8 x0; ///< screen pixels: the left edge on the run's first line
  u8 x1; ///< screen pixels: the left edge the run moves toward
  u8 y0; ///< screen pixels: the right edge on the run's first line
  u8 y1; ///< screen pixels: the right edge the run moves toward
};

/// A window transition's value: from 0, the screen covered, it opens to
/// TRANSITION_OPEN and closes again to TRANSITION_CLOSED; a mode draws the
/// opening half and the closing half mirrored.
#define TRANSITION_OPEN 32
#define TRANSITION_CLOSED 64
/// WININ or WINOUT showing every layer in both windows (bits 6 and 14 set too).
#define TRANSITION_SHOW_ALL 0x7f7f
/// WININ or WINOUT showing every layer in its low byte (window 0, or outside
/// both windows; bit 6 set too) and nothing in its high byte.
#define TRANSITION_SHOW_LOW_BYTE 0x007f
/// WININ or WINOUT showing BG0 alone, in both bytes.
#define TRANSITION_SHOW_BG0 (WININ_WIN0_BG0 | WININ_WIN1_BG0)

/// One step of WINDOW_TRANSITION_SHAPE, a row of
/// gScreenTransition_Shapes chosen by the progress: the WININ and WINOUT
/// values, swapped as the transition opens or closes, and three runs of lines
/// written one after another from the top of the screen.
struct TransitionShape {
  u16 first;  ///< WINOUT while opening, WININ while closing
  u16 second; ///< WININ while opening, WINOUT while closing
  struct TransitionSpan spans[3];
};

extern struct TransitionShape gScreenTransition_Shapes[TRANSITION_OPEN];

/// The party leader's actor placement table (leader_placement.c), which
/// Actor_ResetForLeader copies into the field state.
extern const struct ActorPlacement gField_DefaultActorPlacements[2];

/// Curves mapping a 5-bit colour channel to the colour buffer's channel,
/// shifted up 10 bits: plain, lightened and with more contrast.
///@{
extern u16 gColorBuffer_ChannelCurve[];
extern u16 gColorBuffer_ChannelCurveLight[];
extern u16 gColorBuffer_ChannelCurveContrast[];
///@}

#endif
