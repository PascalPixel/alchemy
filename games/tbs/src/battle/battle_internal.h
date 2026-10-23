#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on
/// its own still resolves. It places no bytes.

#include "vram_tab.h"
#include "item_ids.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/overworld/overworld.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "battle_display.h"
#include "battle_message.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "field_offset.h"
#include "battle_work.h"
#include "battle_type.h"
#include "battle_summon.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flags.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "battle_calc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "script_layer.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "battle_obj.h"
#include "battle_event.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "blitter.h"
#include "battle_anim_work.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "owner_value.h"
#include "songs.h"
#include "ramcode.h"
#include "map_grid_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"
#include "unit/overworld/psynergy_effects.h"
#include "macros.h"
#include "enemy_ids.h"

/// The battle's windows: window 0 shows BG0 to BG2, sprites and blending,
/// window 1 BG0, BG2, sprites and blending, and outside them only BG0 and
/// blending.
#define BATTLE_WININ                                                           \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ |         \
   WININ_WIN1_CLR)
#define BATTLE_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

/// The battle background's colours: BG palettes 6 to 13, filled from
/// BattleWork.palette, darkened or not.
#define BATTLE_BG_PALETTE ((u16 *)&BG_PLTT_COLOR(6, 0))

/// The opening curtain's tiles: 4-bit tiles 128 to 136 of character block 1,
/// which BG1 shows from its map in screen block 12.
#define CURTAIN_TILE(n) (BG_CHAR_BLOCK(1) + (128 + (n)) * TILE_SIZE_4BPP)

/// BG1 during a battle: 256 colours, priority 3, its map in screen block 31.
#define BATTLE_BG1CNT                                                          \
  (BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31))

// The kinds of argument a message's placeholders take, for
// MessageArgument_PushFar: a unit's name, an item's, a class's, an
// ability's, and a number.
#define VALUE_UNIT_NAME 1
#define VALUE_ITEM_NAME 2
#define VALUE_CLASS_NAME 3
#define VALUE_ABILITY_NAME 4
#define VALUE_NUMBER 5

/// Unit ids from 128 are the enemies'. In a link battle each console sends
/// LINK_PARTY_SIZE places, at most three members (LinkBattle_SendParty), and
/// the opponent's take enemy ids 128 to 130.
#define FIRST_ENEMY_UNIT 128
#define LINK_PARTY_SIZE 3

/// The party fights four at a time, three in a link battle; six enemies at
/// most, three in a link battle.
#define PARTY_BATTLE_SIZE 4
#define ENEMY_BATTLE_SIZE 6

#define ALLY_MAX 7 ///< the last party member's unit id

// BattleWork.defeatState: whether the last enemy fell alone, or with others
// before it, or a party member's action ended the battle with a single
// defeat. The messages are "has been downed!" and "'s party goes down!",
// MsgDowned plus the state; DEFEAT_BY_PARTY_MEMBER shows neither.
#define DEFEAT_ONE 0
#define DEFEAT_SEVERAL 1
#define DEFEAT_BY_PARTY_MEMBER 3

/// The game flag of an enemy the party has defeated, set by
/// BattleEnemy_RecordDefeat outside the monster battles; the monster battles
/// choose only formations of defeated enemies (BattleFormation_AverageLevel).
#define ENEMY_DEFEATED_FLAG(classId) ((classId) + 0x600)

/// The length of a unit's name, its terminator excluded.
#define UNIT_NAME_LENGTH 14

/// This console's number in a link session (SIOCNT bits 4 and 5, which the
/// hardware sets in multi-player mode): 0 for the parent, 1 for the child.
#define SIO_MULTI_PLAYER_ID() ((REG_SIOCNT << 26) >> 30)

/// gLinkStatus: both consoles are connected.
#define LINK_BOTH_CONNECTED 3

/// The frames a link battle tolerates without a connection before it gives
/// up: a count above it pauses the battle (BattleWork.linkPaused) or fails
/// the sync or transfer under way (link.c, round.c).
#define LINK_MAX_MISSED_FRAMES 24

/// BattleWork.layout, from the enemy formation
/// (BattleFormation_BuildEnemyList): the random formations stand left to right
/// in the order they were drawn, any other is laid out from the centre.
#define FORMATION_LAYOUT_SHUFFLED 0
#define FORMATION_LAYOUT_INTERLEAVED 1

/// The two battle sprites drawn in two parts: the sprite after each is its
/// second part, which EnemySprite_LoadIntoSlots loads into further
/// enemy sprite slots and BattleObject_SpawnObjectsForList shows as the
/// object's second sprite.
#define SPRITE_WITH_SECOND_PART_A                                              \
  476 ///< the Kraken's (ENEMY_KRAKEN, sprites.tsv)
#define SPRITE_WITH_SECOND_PART_B                                              \
  483 ///< the Fusion Dragon's (ENEMY_FUSION_DRAGON)

/// The inventory's weapon slot, for Inventory_FindEquippedFar.
#define EQUIP_WEAPON 1

// MotionObject.spritesKind's low four bits: no sprite, one sprite in
// records, or records pointing to a table of up to four.
#define OBJECT_SPRITES_NONE 0
#define OBJECT_SPRITES_ONE 1
#define OBJECT_SPRITES_GROUP 2
#define OBJECT_SPRITES_KIND_MASK 15
#define OBJECT_SPRITES_GROUP_SIZE 4

/// BattleTurn.staging: how BattleStaging_DispatchAction shows the
/// action, set by BattleTurn_Build from the ability
/// (gAbilityStaging where it gives one).
enum BattleStaging {
  STAGING_MESSAGES = 1, ///< no animation: each target's messages only
  STAGING_ATTACK = 2,   ///< the actor runs up and strikes
  STAGING_CAST = 3,     ///< Psynergy, and Venus, Mercury, Mars, Jupiter
  STAGING_ITEM = 4,     ///< an item
  STAGING_SUMMON = 5,   ///< the larger summons, the actor hidden
  STAGING_DJINNI = 6,   ///< a Djinni with no attack (Granite, Quartz...)
  STAGING_FLEE = 7,     ///< a unit runs
  /// The action plays as a cast, the caster showing its element's streaks in
  /// place of the gathering star field. Where actstat.tsv gives no staging,
  /// it's how an enemy's ability plays when the ability isn't Psynergy
  /// (Ability_IsPsynergy) and its row of actflags.tsv isn't 0; an enemy's
  /// Psynergy plays as STAGING_CAST. actstat.tsv gives it to Slice and to
  /// seven weapon unleashes: Blizzard, Drown, Demon Fire, Vorpal Slash, Rapid
  /// Smash, Sonic Smash and Titan Blade.
  STAGING_ENEMY_CAST = 8,
  STAGING_FULL_SCREEN = 9 ///< Gaia, Rockfall, Procne and their kin
};

// BattleTurn.animation: the animation kind in the low twelve bits and
// its strength (a double or triple blow) above them; then how the animation is
// staged.
#define ANIM_ID_MASK 0xfff
#define ANIM_VARIANT_MASK 0x3000
#define ANIM_VARIANT_SHIFT 12
#define ANIM_BY_ID_RANGE 0x4000 ///< BattleAnim_DispatchByIdRangeFar
#define ANIM_SUMMON 0x8000      ///< BattleAnim_RunSummonEffectFar, actor's view
#define ANIM_ON_DOWNED 0x10000  ///< shown on downed targets too (revivals)
/// Plays the animation on the side opposite its targets. Unused: no row of
/// actflags.tsv in either game sets it and no code adds it, so the flip in
/// BattleStaging_HideField never happens.
#define ANIM_OTHER_SIDE 0x20000

/// The animation kind Severe Blow alone has: the status window drops its PP row
/// while it plays.
#define ANIM_ID_SEVERE_BLOW 0x87

/// More of BattleTurn.animation: the camera takes the other side's
/// view (an enemy's cast a view of its own), or turns right round.
#define ANIM_VIEW_ACROSS 0x40000
#define ANIM_VIEW_REVERSED 0x80000

/// BattleTurn_Build's results besides 0, which stages the turn.
#define TURN_REPORTED -1  ///< the action failed and its message is shown
#define TURN_NO_ACTION -2 ///< nothing to stage or show

/// BattleStaging_HideField's mode: a summon hides its user too.
#define STAGING_HIDE_ACTOR 1

/// BattleStaging_RunPsynergyCast's mode: what is cast, which decides the
/// glow.
enum BattleCastKind {
  CAST_PSYNERGY = 0, ///< the star field gathers on the caster
  CAST_DJINNI = 1,   ///< no glow; the animation plays its prelude
  CAST_ENEMY = 2     ///< the element's streaks
};

/// BattleAI_ChooseTarget's results when it picks no one: the side it
/// may target lists no one, or no one there is worth the ability.
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE -1

/// BattleAction.target of a held unit's lost turn (COMMAND_NONE, from
/// Battle_BuildOpponentActions and
/// Battle_BuildUnitActions): as a place code, place 0 of the other
/// side, 0x100 the party's for an enemy and 0x180 the enemies' for a party
/// member (PLACE_PARTY, PLACE_ENEMY).
#define TARGET_NONE_ENEMY 0x100
#define TARGET_NONE_PARTY 0x180

/// The camera's yaw while a party member acts, and while an enemy does, in
/// 1/65536 turns: an eighth of a turn either side of 0. The camera callback
/// eases toward it (BattleCamera_Update).
#define YAW_PARTY_TURN 0x2000
#define YAW_ENEMY_TURN -0x2000

/// The camera's yaw from behind the enemies, which an enemy's staged scene and
/// cast take in place of YAW_ENEMY_TURN.
#define YAW_BEHIND_ENEMIES 0x5000

/// REG_BLDCNT: blend the semi-transparent sprites (SPRITE_SEMI_TRANSPARENT)
/// over every layer, to fade them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// The keys that take the player on past a message: A, B, L and R.
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS)

#endif
