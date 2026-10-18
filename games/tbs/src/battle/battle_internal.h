#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

// What every part of the battle object shares: its includes, in their
// original order, and the names two or more parts use. battle.c includes it
// before the parts, and each part includes it too, so that a part read on
// its own still resolves. It places no bytes.

#include "vram_tab.h"
#include "item_ids.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/object/object.h"
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
#include "scene.h"
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
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "battle_calc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
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
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "owner_value.h"
#include "songs.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"
#include "macros.h"

// DMA control words, as io_reg.h composes them: copy n words or halfwords
// at once, or one halfword each HBlank to a register reloaded every line.
#define DMA_COPY_WORDS(n) ((DMA_ENABLE | DMA_32BIT) << 16 | (n))
#define DMA_COPY_HALFWORDS(n) (DMA_ENABLE << 16 | (n))
#define DMA_HBLANK_HALFWORD_PER_LINE                                           \
  ((DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD) << 16 | 1)

// The battle's windows: window 0 shows BG0 to BG2, sprites and blending,
// window 1 BG0, BG2, sprites and blending, and outside them only BG0 and
// blending.
#define BATTLE_WININ                                                           \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ |         \
   WININ_WIN1_CLR)
#define BATTLE_WINOUT (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

// The battle background's colours: BG palettes 6 to 13, filled from
// BattleWork.palette, darkened or not.
#define BATTLE_BG_PALETTE ((u16 *)&BG_PLTT_COLOR(6, 0))

// The opening curtain's tiles: 4-bit tiles 128 to 136 of character block 1,
// which BG1 shows from its map in screen block 12.
#define CURTAIN_TILE(n) (BG_CHAR_BLOCK(1) + (128 + (n)) * TILE_SIZE_4BPP)

// BG1 during a battle: 256 colours, priority 3, its map in screen block 31.
#define BATTLE_BG1CNT                                                          \
  (BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31))

// The kinds of argument a message's placeholders take, for
// UiWork_PushValueSlotFar: a unit's name, an item's, a class's, an
// ability's, and a number.
#define VALUE_UNIT_NAME 1
#define VALUE_ITEM_NAME 2
#define VALUE_CLASS_NAME 3
#define VALUE_ABILITY_NAME 4
#define VALUE_NUMBER 5

// Unit ids from 128 are the enemies'; in a link battle the opponent's party
// takes the first three.
#define FIRST_ENEMY_UNIT 128
#define LINK_PARTY_SIZE 3

// The party fights four at a time, three in a link battle; six enemies at
// most, three in a link battle.
#define PARTY_BATTLE_SIZE 4
#define ENEMY_BATTLE_SIZE 6

#define ABILITY_ATTACK 1 // the ability "Attack"
#define ALLY_MAX 7       // the last party member's unit id

// The length of a unit's name, its terminator excluded.
#define UNIT_NAME_LENGTH 14

// This console's number in a link session (SIOCNT bits 4 and 5, which the
// hardware sets in multi-player mode): 0 for the parent, 1 for the child.
#define SIO_MULTI_PLAYER_ID() ((REG_SIOCNT << 26) >> 30)

// gLinkStatus: both consoles are connected.
#define LINK_BOTH_CONNECTED 3

// The frames a link battle tolerates without a connection before it gives
// up.
#define LINK_MAX_MISSED_FRAMES 24

// The Djinn recovery list's two tables (Djinn_GetRecoveryTableFar): the
// party's and the link opponent's.
#define DJINN_RECOVERY_PARTY 0
#define DJINN_RECOVERY_OPPONENT 1

// DjinnRecoveryEntry.turns of a Djinni waiting for the battle to end.
#define DJINN_RECOVERY_AFTER_BATTLE -1

#define DJINN_PER_ELEMENT 20

// BattleWork.battleType, from the enemy formation
// (BattleFormation_BuildEnemyList): the random formations stand left to right
// in the order they were drawn, any other is laid out from the centre.
#define FORMATION_SHUFFLED 0
#define FORMATION_INTERLEAVED 1

// The two battle sprites whose next sprite is loaded with them (into a
// second free place), the pass after them loading animation + 1.
#define SPRITE_WITH_SECOND_PART_A 476
#define SPRITE_WITH_SECOND_PART_B 483

// The inventory's weapon slot, for Inventory_FindEquippedFar.
#define EQUIP_WEAPON 1

// MotionObject.recordStorageKind's low four bits: no sprite, one sprite in
// records, or records pointing to a table of up to four.
#define RECORDS_NONE 0
#define RECORDS_SINGLE 1
#define RECORDS_TABLE 2
#define RECORDS_KIND_MASK 15
#define RECORDS_TABLE_SIZE 4

// BattleUnit.poison: the poison takes a tenth of the maximum HP at the end of
// each turn, deadly poison a fifth.
#define POISON_NORMAL 1
#define POISON_DEADLY 2

// BattleUnit.guardLevel: Defend and Granite halve the damage, Total Defense and
// Flash cut it to a tenth.
#define GUARD_DEFEND 1
#define GUARD_TOTAL_DEFENSE 2

// BattlePlan.outcome: how BattlePresentation_DispatchAction shows the
// action, set by BattleCommand_BuildPlan from the ability (gBattle_ActionStatus
// where it gives one).
enum BattlePlanStaging {
  PLAN_SHOW_MESSAGES = 1,   // no animation: each target's events only
  PLAN_SHOW_ATTACK = 2,     // the actor runs up and strikes
  PLAN_SHOW_CAST = 3,       // a Psynergy, and Venus, Mercury, Mars, Jupiter
  PLAN_SHOW_ITEM = 4,       // an item
  PLAN_SHOW_SUMMON = 5,     // the larger summons, the actor hidden
  PLAN_SHOW_DJINNI = 6,     // a Djinni with no attack (Granite, Quartz...)
  PLAN_SHOW_FLEE = 7,       // a unit runs
  PLAN_SHOW_ENEMY_CAST = 8, // an enemy's Psynergy
  PLAN_SHOW_FULL_SCREEN = 9 // Gaia, Rockfall, Procne and their kin
};

// BattlePlan.presentationFlags: the effect's kind in the low twelve bits and
// its strength (a double or triple blow) above them; then how the effect is
// staged.
#define STAGING_KIND_MASK 0xfff
#define STAGING_VARIANT_MASK 0x3000
#define STAGING_VARIANT_SHIFT 12
#define STAGING_BY_ID_RANGE 0x4000 // BattleFx_DispatchByIdRangeFar
#define STAGING_SCENE 0x8000       // BattleFx_RunSceneEffectFar, actor's view
#define STAGING_ON_DOWNED 0x10000  // shown on downed targets too (revivals)
#define STAGING_OTHER_SIDE 0x20000 // played from the other side

// More of BattlePlan.presentationFlags: the camera takes the other side's
// view (an enemy's cast a view of its own), or turns right round.
#define STAGING_VIEW_ACROSS 0x40000
#define STAGING_VIEW_REVERSED 0x80000

// ItemDefinition.useType: what using an item in battle does to it.
#define ITEM_USE_CONSUMED 1  // one is used up
#define ITEM_USE_MAY_BREAK 2 // breaks one time in eight
#define ITEM_USE_EMPTIES 4   // Hermes' Water leaves an Empty Bottle

// BattleAct.targetMode: whom an ability may target, from its user's view.
#define TARGET_MODE_NONE 0
#define TARGET_MODE_FOES 1
#define TARGET_MODE_ALLIES 2
#define TARGET_MODE_SELF 4

// BattleActionRecord.target of an action that needs none: a held unit's
// lost turn (the enemies' and the party's), and an enemy's attack in auto
// battle, which picks a party place.
#define TARGET_NONE_ENEMY 0x100
#define TARGET_NONE_PARTY 0x180

// The camera's yaw while a party member acts, and while an enemy does.
#define YAW_PARTY_TURN 0x2000
#define YAW_ENEMY_TURN -0x2000

// REG_BLDCNT: blend the semi-transparent sprites (OBJ mode 1) over every
// layer, to fade them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

// The arrow that asks the player to go on
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS) // the keys that go on

#endif
