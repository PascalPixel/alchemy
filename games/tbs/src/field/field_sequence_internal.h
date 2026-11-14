#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes and the macros and
/// types more than one part uses. field_sequence.c includes it before the
/// parts, and each part includes it too, so that a part read on its own
/// resolves. It places no bytes.

#include "module/field/battle_music.h"
#include "module/field/world_map_encounter_rules.h"
#include "vram_cache.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "system.h"
#include "dma.h"
#include "io_write.h"
#include "flags.h"
#include "iwram_call.h"
#include "resource_ids.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "event.h"
#include "display_transition.h"
#include "text/msg_ids.h"
#include "ground_mask.h"
#include "io_write_queue.h"
#include "battle_status.h"
#include "field_psynergy_work.h"
#include "macros.h"
#include "ability_ids.h"
#include "field/encounter_zone.h"

/// The speakers' definitions (field_sequence_tables.c), read by
/// Speaker_FindDefinition.
extern const struct SpeakerDefinition gSpeakerDefinitions[];

/// Scene ids above the field scenes, which MainRoutine_Run runs without a
/// layout; each returns a result to Scene_ReturnFromSequence instead of an
/// exit.
union PolarOffsetPosition {
  struct Vector3 vector;
  s32 coordinates[3];
};

STATIC_ASSERT(sizeof(union PolarOffsetPosition) == 12);

/// A map region's bounds, in pixels, as 16.16 fixed point.
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

// A field Psynergy or item use, packed into one value (items.c, psynergy.c), as
// work->psynergy and work->item hold it. bits: 13..10 the user (13 is
// ACT_PACKED) | 9..0 the Psynergy or item.
#define ABILITY_ID(c) ((c) & 0x3ff) ///< the Psynergy
#define ACT_ITEM_ID(c) ((c) & 0x3ff)
#define ACT_UNIT(c) (((c) >> 10) & 15) ///< who uses it
#define ACT_PACKED 0x2000              ///< use it without paying PP
/// No user named: Field_UsePsynergy takes ISAAC, and Field_UseItem
/// whoever holds the item in the most inventory slots.
#define UNIT_NONE 15
#define ALLY_MAX 7 ///< the last party member; only users up to it pay PP

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

extern const struct SceneEncounterRule gScene_EncounterRules[];

/// A rule choosing a scene's field music (gScene_MusicRules,
/// scene_rules.s), ended by id -1.
struct MapMusicRule {
  s16 id;            ///< a scene, or an area
  s16 entrance : 15; ///< -1 for any entrance
  u16 idIsMap : 1;   ///< 1 when id is a scene, 0 when it is an area
  s16 flag;          ///< -1, or a flag that must be set
  s16 music;         ///< the song (sounds.h's MUS_ numbers)
};

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

/// One step of WINDOW_TRANSITION_SHAPE, a row of
/// gScreenTransition_Shapes chosen by the progress: the WININ and WINOUT
/// values, swapped as the transition opens or closes, and three runs of lines
/// written one after another from the top of the screen.
struct TransitionShape {
  u16 first;  ///< WINOUT while opening, WININ while closing
  u16 second; ///< WININ while opening, WINOUT while closing
  struct TransitionSpan spans[3];
};

extern struct TransitionShape gScreenTransition_Shapes[];

/// The party leader's actor placement table (scripts.s), which
/// Actor_ResetForLeader copies into the field work.
extern const struct ActorPlacement gField_DefaultActorPlacements[2];

/// Curves mapping a 5-bit colour channel to the colour buffer's channel,
/// shifted up 10 bits: plain, lightened and with more contrast.
///@{
extern u16 gColorBuffer_ChannelCurve[];
extern u16 gColorBuffer_ChannelCurveLight[];
extern u16 gColorBuffer_ChannelCurveContrast[];
///@}

#endif
