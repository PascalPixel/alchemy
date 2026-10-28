#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on
/// its own still resolves. It places no bytes.

#include "vram_cache.h"
#include "item_ids.h"
#include "window.h"
#include "module/battle/battle.h"
#include "module/overworld/overworld.h"
#include "module/runtime/runtime.h"
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
#include "unit.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "sequence_parent_work.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "task.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "heap_slots.h"
#include "battle_display.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "battle_work.h"
#include "battle_type.h"
#include "battle_summon.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "battle_command.h"
#include "resource.h"
#include "heap.h"
#include "flags.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "menu_list.h"
#include "sound_driver_symbols.h"
#include "ui.h"
#include "battle_calc.h"
#include "objdisp.h"
#include "battle_actor.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "particle.h"
#include "battle_flee.h"
#include "battle_target.h"
#include "battle_obj.h"
#include "playback_queue.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "blitter.h"
#include "battle_effect_work.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "sounds.h"
#include "ramcode.h"
#include "map_grid_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "frame.h"
#include "module/overworld/event_commands.h"
#include "macros.h"
#include "enemy_ids.h"
#include "debug_party.h"

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

/// In a link battle each console sends LINK_PARTY_SIZE places, at most three
/// party members (LinkBattle_SendParty), and the opponent's take the first
/// three enemy ids from FIRST_ENEMY_UNIT.
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
/// the sync or transfer under way (link.c, turn.c).
#define LINK_MAX_MISSED_FRAMES 24

/// The two battle sprites drawn in two parts: the sprite after each is its
/// second part, which EnemySprite_LoadIntoSlots loads into further
/// enemy sprite slots and BattleObject_SpawnObjectsForList shows as the
/// actor's second sprite.
#define SPRITE_WITH_SECOND_PART_A                                              \
  476 ///< the Kraken's (ENEMY_KRAKEN, battle_sprites.c)
#define SPRITE_WITH_SECOND_PART_B                                              \
  483 ///< the Fusion Dragon's (ENEMY_FUSION_DRAGON)

/// The inventory's weapon slot, for Inventory_FindEquippedFar.
#define EQUIP_WEAPON 1

// Actor.spriteStorageKind holds an enum ActorSpriteKind in its low four
// bits; a group's sprite word points to a table of four sprites.
#define ACTOR_SPRITE_KIND_MASK 15
#define ACTOR_SPRITE_GROUP_SIZE 4

/// BattleActionRun_Build's results besides 0, which stages the action.
#define ACTION_REPORTED -1 ///< the action failed and its message is shown
#define ACTION_NOTHING_TO_SHOW -2 ///< nothing to stage or show

/// BattleStaging_HideField's mode: a summon hides its user too.
#define STAGING_HIDE_ACTOR 1

/// BattleStaging_RunPsynergyCast's mode: what is cast, which decides the
/// glow.
enum BattleCastKind {
  CAST_PSYNERGY = 0, ///< the star field gathers on the caster
  CAST_DJINNI = 1,   ///< no glow; the effect plays its prelude
  CAST_ENEMY = 2     ///< the element's streaks
};

/// BattleAI_ChooseTarget's results when it picks no one: the side it
/// may target lists no one, or no one there is worth the ability.
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE -1

/// BattleAction.target of a held unit's lost action (COMMAND_NONE, from
/// Battle_BuildOpponentActions and
/// Battle_BuildUnitActions): as a place code, place 0 of the other
/// side, 0x100 the party's for an enemy and 0x180 the enemies' for a party
/// member (PLACE_PARTY, PLACE_ENEMY).
#define TARGET_NONE_ENEMY 0x100
#define TARGET_NONE_PARTY 0x180

/// The camera's yaw while a party member acts, and while an enemy does, in
/// 1/65536 turns: an eighth of a turn either side of 0. The camera task
/// eases toward it (BattleCamera_Update).
#define YAW_PARTY_ACTION 0x2000
#define YAW_ENEMY_ACTION -0x2000

/// The camera's yaw from behind the enemies, which an enemy's staged effect
/// and cast take in place of YAW_ENEMY_ACTION.
#define YAW_BEHIND_ENEMIES 0x5000

/// REG_BLDCNT: blend the semi-transparent sprites (SPRITE_SEMI_TRANSPARENT)
/// over every layer, to fade them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// The keys that take the player on past a message: A, B, L and R.
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS)

#endif
