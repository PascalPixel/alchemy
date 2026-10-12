#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

// ほかのモジュールの関数 (共通ヘッダーにまだ宣言が無いもの)

// このファイルの関数 (定義より前で使うもの)

// このファイルで繰り返す処理

// 手に入れたアイテムのメッセージ (ドイツ語版はブラックオーブだけ別の文)
static inline void ItemGift_ShowGotItem(s32 item, s32 mode)
{
#if defined(TBS_EDITION_DE)
	if (item == ITEM_BLACK_ORB)
	{
		UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, mode);
		return;
	}
#endif
	UiWork_PushValueSlotFar(item, 2);
	UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, mode);
}

void MapGroupTable_SelectEntry(s32 kind)
{
	s16		   cur;
	const u32 *p;
	const u32 *start;
	u32		   word;
	s32		   id;
	s32		   result;
	s32		   sub;
	s32		   entry_sub;
	s32		   entry_kind;
	s32		   flag;

	cur = gGameState.scene;
	p = gOverlayArea.exits();
	result = 999;
	sub = 0;
	if (kind == 999)
		return;

	for (;;)
	{
		id = *p++;
		if (id & 0xfffff000)
			continue;
		id &= 0xfff;
		if (id == 0x1ff)
			break;
		if (id != cur)
			continue;
		start = p;
		for (;;)
		{
			word = *p++;
			id = word & 0xfff;
			entry_sub = (word & 0xff000) >> 12;
			entry_kind = (word & 0x0ff00000) >> 20;
			flag = word & 0x10000000;
			if (flag)
				flag = *p++;
			if (id == 0x1ff || entry_kind == 0)
			{
				result = *start & 0x1ff;
				break;
			}
			if (entry_kind == 0xff || entry_kind == kind)
			{
				if (flag == 0 || GameFlag_TestFar(flag) == 0)
				{
					result = id;
					sub = entry_sub;
					break;
				}
			}
		}
		break;
	}

	if (result != 999)
	{
		gGameState.scene = result;
		gGameState.entrance = sub;
	}
}

static __inline__ s32 Vitals_Ratio(s32 value, s32 max)
{
	s32 ratio = (value << 14) / max;
	s32 clamped;

	if (ratio > 0x4000)
		clamped = 0x4000;
	else if (ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

static __inline__ void Unit_UpdateGauges(struct BtlUnit *unit)
{
	unit->hp_gauge = Vitals_Ratio(unit->hp, unit->max_hp);
	if (unit->hp_gauge == 0 && unit->hp != 0)
		unit->hp_gauge = 1;
	unit->ep_gauge = Vitals_Ratio(unit->ep, unit->max_ep);
	if (unit->ep_gauge == 0 && unit->ep != 0)
		unit->ep_gauge = 1;
}

void Party_SetReturnPoint(s32 reason)
{
	struct BtlUnit *unit;
	s32				i;
	s32				scene;
	s32				entrance;

	gGameState.scene_change_reason = reason;
	if (reason == -1)
	{
		unit = UnitGetFar(gGameState.selected_actor);
		if (unit->hp == 0)
		{
			unit->hp = 1;
			Unit_UpdateGauges(unit);
		}
		if (GameFlag_TestFar(32))
		{
			for (i = 0; i < 2; i++)
			{
				unit = UnitGetFar(i);
				Unit_Recover(unit);
				Unit_UpdateGauges(unit);
			}
		}
		scene = gGameState.defeat_scene;
		entrance = gGameState.defeat_entrance;
		if (scene == -1 && entrance == -1)
		{
			gGameState.scene = gGameState.saved_scene;
			gGameState.entrance = gGameState.saved_entrance;
		}
		else
		{
			if (scene != -1)
				gGameState.scene = scene;
			else
				gGameState.scene = gGameState.default_scene;
			if (entrance != -1)
				gGameState.entrance = entrance;
			else
				gGameState.entrance = gGameState.default_entrance;
		}
	}
	else
	{
		scene = gGameState.next_scene;
		entrance = gGameState.next_entrance;
		if (scene != -1 || entrance != -1)
		{
			if (scene != -1)
				gGameState.scene = scene;
			else
				gGameState.scene = gGameState.default_scene;
			if (entrance != -1)
				gGameState.entrance = entrance;
			else
				gGameState.entrance = gGameState.default_entrance;
		}
		else
		{
			gGameState.scene = gGameState.default_scene;
			gGameState.entrance = gGameState.default_entrance;
			GameFlag_SetBitFar(0x109);
		}
	}
}

//--------------------------------------------------------------------------
// data_get_resource_group.c
// シーン表：シーンのグループ番号
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// シーンが属するグループを返す
// scene : シーン番号
//--------------------------------------------------------------------------
s32 BattleFx_GetResourceGroup(s32 scene)
{
	return Field_SceneTable[scene].group;
}

enum
{
	SCENE_BLANK = 0x1fb,
	SCENE_TABLE_LOAD = 0x1fc,
	SCENE_TABLE_RUN = 0x1fd,
	SCENE_BATTLE = 0x1fe
};

static __inline__ s32 Scene_RunTable(s32 entrance)
{
	void *palette = (void *)&BG_PLTT_COLOR(14, 0);
	void *saved = MemAlloc(64);
	s32	  result;

	Dma_Set(palette, saved, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
	/* FAKEMATCH: Runtime_BlankDisplayAndRunFar is called as s32 (*)(s32); as s32 (*)(void), its prototype, Scene_RunTable does not compile */
	result = ((s32 (*)(s32))Runtime_BlankDisplayAndRunFar)(entrance);
	Dma_Set(saved, palette, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
	MemFree(saved);
	return result;
}

static __inline__ s32 Scene_LoadTable(s32 entrance)
{
	void *palette = (void *)&BG_PLTT_COLOR(14, 0);
	void *saved = MemAlloc(64);
	s32	  result;

	Dma_Set(palette, saved, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
	/* FAKEMATCH: Runtime_BlankDisplayLoadValueAndRunFar is called as s32 (*)(s32); as s32 (*)(void), its prototype, Scene_LoadTable does not compile */
	result = ((s32 (*)(s32))Runtime_BlankDisplayLoadValueAndRunFar)(entrance);
	Dma_Set(saved, palette, DMA_ENABLE32 | DMA_32BIT | (64 / 4), REG_DMA3);
	MemFree(saved);
	return result;
}

void Game_ResetForNewGame(s32 mode)
{
	const struct SceneRecord *table;
	const struct SceneRecord *record;
	s32						  entrance;

	table = Field_SceneTable;
	if (gDebugMode)
	{
		if (mode == 1)
		{
			gGameState.scene = (s32)&SceneId_HaidiaIe;
			gGameState.entrance = mode;
			goto start;
		}
		if (mode == 2)
		{
			gGameState.scene = (s32)&SceneId_Clear;
			gGameState.entrance = 1;
			goto start;
		}
	}
	GameState_InitDefaultsFar();
	gGameState.scene = (s32)&SceneId_Title;
	gGameState.entrance = 2;
start:
	PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	Resource_InitializeTable();
	Scheduler_ResetTaskTable();
	Scheduler_ResetTaskTable();
	for (;;)
	{
		if (GameFlag_TestFar(0x101))
			GameFlag_ClearBitFar(0x101);
		else
			AudioCommand_PlayFar(0x120);
		record = &table[gGameState.scene];
		entrance = gGameState.entrance;
		{
			volatile u16 *dma0 = REG_DMA0;

			dma0[5] &= 0xc5ff;
			dma0[5] &= 0x7fff;
			dma0[5];
		}
		Scheduler_ResetTaskTable();
		Runtime_SetIrqHandler(1, 0, 0);
		Runtime_SetIrqHandler(2, 0, 0);
		Runtime_InitializeHeap();
		Bg0_ClearTilemap();
		Resource_InitializeTable();
		if (gGameState.scene >= SCENE_BLANK)
		{
			switch (gGameState.scene)
			{
			case SCENE_BATTLE:
				entrance = Battle_RunEncounterFar(entrance);
				break;
			case SCENE_TABLE_RUN:
				entrance = Scene_RunTable(entrance);
				break;
			case SCENE_TABLE_LOAD:
				entrance = Scene_LoadTable(entrance);
				break;
			case SCENE_BLANK:
				entrance = 0;
				break;
			}
			Party_SetReturnPoint(entrance);
			continue;
		}
		Scene_ResetFlagsOnEnter(gGameState.scene, GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION));
		Scene_ResolveInteractionResult();
		if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION))
		{
			if (GameFlag_TestFar(0x11a) || GameFlag_TestFar(0x11b))
				GameFlag_ClearBitFar(0x11a);
			else
				Audio_PlayCueFromEventWork();
		}
		else if (gGameState.return_cue != -1)
		{
			AudioCommand_PlayFar(gGameState.return_cue);
		}
		else
		{
			Audio_PlayCueFromEventWork();
		}
		gGameState.map = record->map_index;
		BattleFx_LoadResourceGroup(0);
		MapGroupTable_SelectEntry(Field_RunScene(entrance));
	}
}

//--------------------------------------------------------------------------
// data_load_resource_group.c
// シーン表：現在のシーンのコードを読み込む
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 現在のシーンのコードをオーバーレイ領域へ読み込む
//--------------------------------------------------------------------------
void BattleFx_LoadResourceGroup(s32 unused)
{
	const struct SceneRecord *tbl;

	tbl = Field_SceneTable;
	Resource_LoadCode(tbl[gGameState.scene].resource_id, &gOverlayArea);
}

#define SCENE_RECORD(id) (&Field_SceneTable[id])

void Scene_ResetFlagsOnEnter(s32 unused, s32 keep)
{
	s32 group = SCENE_RECORD(gGameState.scene)->group;
	s32 flag;

	if (keep == 0)
	{
		for (flag = 0x200; flag <= 0x2ff; flag++)
			GameFlag_ClearBitFar(flag);
		if (group != gGameState.scene_group)
		{
			for (flag = 0x300; flag <= 0x3ff; flag++)
				GameFlag_ClearBitFar(flag);
			GameFlag_SetBitFar(0x12f);
			gGameState.encounter_steps = 0;
			gGameState.hazard_steps = 0;
			GameFlag_ClearBitFar(0x110);
			GameFlag_ClearBitFar(0x111);
			GameFlag_ClearBitFar(0x112);
			GameFlag_ClearBitFar(0x113);
			gGameState.retreat_scene = gGameState.scene;
			gGameState.retreat_entrance = gGameState.entrance;
		}
		for (flag = 0x80; flag <= 0xdf; flag++)
			GameFlag_ClearBitFar(flag);
		GameFlag_ClearBitFar(0x16c);
		GameFlag_ClearBitFar(0x144);
		GameFlag_ClearBitFar(0x161);
		GameFlag_ClearBitFar(0x123);
		GameFlag_ClearBitFar(0x11c);
		gGameState.return_cue = -1;
	}
	gGameState.scene_group = group;
	GameFlag_SetBitFar((group & 0x7f) + 0x180);
	gGameState.scene_variant = SCENE_RECORD(gGameState.scene)->variant;
	if (gGameState.scene_variant == 2)
		GameFlag_SetBitFar(0x123);
	GameFlag_RefreshLureCapFar();
}

void Audio_PlayCueFromEventWork(void)
{
	AudioCommand_PlayFar(gGameState.cue);
}

//--------------------------------------------------------------------------
// encountr.c
// エンカウント：場所ごとの出現テーブルの選択と、敵グループの抽選
//--------------------------------------------------------------------------
// マップと部屋ごとの出現テーブル (Battle_LocationRules、map が -1 で終わり)
struct BattleLocationRule
{
	s16 map;
	s16 room;	   // -1 ならどの部屋でも
	s16 flag : 15; // -1 か、立っていない間だけ有効なフラグ
	s16 shift : 1; // 1 なら表の 1 番目を 0 番目へ移す
	s16 result;	   // 最初の出現テーブル番号
};

extern const struct BattleLocationRule Battle_LocationRules[];

// ワールドマップの地形ごとの出現テーブル (BattleFx_ResultRules、-1 で終わり)
typedef struct TerrainRule
{
	s16 kind;	 // 地形の種類 (マップブロックから)
	s16 terrain; // 地形の動作 (-1 ならどれでも)
	s16 flag;	 // -1 か、立っていない間だけ有効なフラグ
	s16 zone;	 // 出現テーブル番号
} TerrainRule;

// 出現テーブル
struct EncounterZone
{
	u16 rate;		// 基本の出現率
	u16 level;		// 想定レベル
	u16 enemies[8]; // 敵グループ
	u8	weights[8]; // 各グループの重み
};

//--------------------------------------------------------------------------
// 今のマップと部屋に合う規則から、出現テーブル番号の並びを作る
// use_rules : 0 なら規則を見ずに、すべて 0 (出現なし) にする
//--------------------------------------------------------------------------
void BattleFx_SelectLocationRule(s32 use_rules)
{
	struct EventWork				*work;
	s32								 map;
	s32								 room;
	u8								*out;
	const struct BattleLocationRule *rule;
	s32								 zone;
	s16								 shift;
	u32								 i;

	work = gEventWork;
	map = gGameState.scene;
	room = gGameState.entrance;
	out = work->encounter_groups;
	rule = Battle_LocationRules;
	zone = 0;
	shift = 0;
	if (use_rules != 0)
	{
		while (rule->map != -1)
		{
			if (rule->map == map && (rule->room == -1 || rule->room == room))
			{
				if (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0)
				{
					zone = rule->result;
					shift = rule->shift;
					break;
				}
			}
			rule++;
		}
	}

	// [0] は 0、[1]〜[7] は zone から順に 1 つずつ増やす
	*out++ = 0;
	for (i = 0; i <= 6; i++)
	{
		*out++ = zone;
		if (zone != 0)
			zone++;
	}
	if (shift != 0)
	{
		work->encounter_groups[0] = work->encounter_groups[1];
		work->encounter_groups[1] = 0;
	}
	work->encounter_bias = 0;
	work->encounter_threshold = 0x100000;
	BattleFx_SetSpecialFromTable();
}

//--------------------------------------------------------------------------
// ワールドマップの位置から出現テーブル番号を求め、戦闘の背景も選ぶ
//--------------------------------------------------------------------------
s32 BattleFx_LookupResult(void *pos)
{
	s32			 kind;
	TerrainRule *rule = BattleFx_ResultRules;
	s32			 terrain = GetWorldMapTerrainBehaviorFar(pos, &kind);
	s32			 zone = 0;

	while (rule->kind != -1)
	{
		if (rule->kind == kind && (rule->terrain == -1 || rule->terrain == terrain)
			&& (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0))
		{
			zone = rule->zone;
			break;
		}
		rule++;
	}
	BattleFx_SelectResultPointer(terrain);
	return zone;
}

//--------------------------------------------------------------------------
// 出現テーブル zone で敵が出るかを決め、出るなら敵グループを返す (出ないなら 0)
// steps : 積算量に掛ける係数 (Q16)。積算が encounter_threshold を越えたら出る
//--------------------------------------------------------------------------
s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps)
{
	struct EventWork	 *work;
	struct EncounterZone *entry;
	s32					  rate;
	s32					  level;
	s32					  bias;
	s32					  step;
	s32					  total;
	s32					  sum;
	s32					  pick;
	s32					  i;

	work = gEventWork;
	if (GameFlag_TestFar(351) != 0)
		goto encounter;
	if (GameFlag_TestFar(352) != 0 || GameFlag_TestFar(353) != 0)
		return 0;
	if (zone == 0)
		return 0;
	if (gGameState.cloaked != 0)
		return 0;
	entry = (struct EncounterZone *)Encounter_EnemyGroupTable + zone;
	rate = entry->rate;
	if (rate == 0)
		return 0;
	if (GameFlag_TestFar(5) != 0)
	{
		level = UnitGetFar(5)->experience;
		if (level > 130)
			return 0;
	}

	// 平均レベルが想定を上回る分 (0〜5) だけ出現率を 5 ずつ上げる
	// その分があるとき encounters_off なら出ない
	level = Party_GetAverageLevelFar() - entry->level;
	if (level < 0)
		level = 0;
	if (level > 5)
		level = 5;
	if (level > 0 && gGameState.avoid_distance != 0)
		return 0;
	rate += level * 5;

	// 4 つの乱数で作るばらつきを、出現のたびに選び直す
	bias = work->encounter_bias;
	if (bias == 0)
	{
		s32 a = Random16();
		s32 b = Random16();
		s32 c = Random16();

		bias = (a - b + c - (s32)Random16()) / 2;
		work->encounter_bias = bias;
	}
	step = Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
	total = gGameState.encounter_steps += Iwram_MulQ16(step, steps);
	if (total < work->encounter_threshold)
		return 0;

	// 重みに従って 8 つのグループから 1 つ選ぶ
encounter:
	work->encounter_bias = 0;
	sum = 0;
	for (i = 0; i < 8; i++)
		sum += entry->weights[i];
	if (sum == 0)
		return 0;
	pick = (Random16() * sum) >> 16;
	pick -= entry->weights[0];
	i = 0;
	if (pick >= 0)
	{
		do
		{
			i++;
			if (i > 7)
				break;
			pick -= entry->weights[i];
		} while (pick >= 0);
	}
	sum = entry->enemies[i];
	BattleFx_SelectBattleCue(zone, i);
	return sum;
}

//--------------------------------------------------------------------------
// data_lookup_routines.c
// フィールド：エンカウント・曲・地名の表引き
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 地形ごとのエンカウント
// terrain : 地形番号（0 は通常の地面）
// steps   : 歩数の係数。そのまま Encounter_SelectEnemyGroup へ渡す
//--------------------------------------------------------------------------
s32 EffectRuntime_LookupByTableEntry(u32 terrain, s32 steps)
{
	u8 *zone;

	zone = gEventWork->encounter_groups;
	return Encounter_SelectEnemyGroup(zone[terrain], steps);
}

//--------------------------------------------------------------------------
// ワールドマップのエンカウント（足元の地形からゾーンを引く）
//--------------------------------------------------------------------------
s32 BattleFx_ApplyLookupResult(s32 pos, s32 steps)
{
	return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)pos), steps);
}

//--------------------------------------------------------------------------
// ゾーンの敵グループをひとつ返す
// zone : ゾーン番号（1 件 14 ハーフワード）
// slot : 敵グループの枠
//--------------------------------------------------------------------------
u16 BattleFx_GetWeightedResult(s32 zone, s32 slot)
{
	return Encounter_EnemyGroupTable[zone * 14 + slot + 2];
}

//--------------------------------------------------------------------------
// 番号から（ゾーン, 枠）を引き、その敵グループを返す
// return 文はなく、BattleFx_GetWeightedResult の戻り値が r0 に残る
//--------------------------------------------------------------------------
s16 BattleFx_GetPhaseResult(s32 no)
{
	u16 *ent;

	ent = (u16 *)&Encounter_AreaEntryTable[no * 2];
	BattleFx_GetWeightedResult(ent[0], ent[1]);
}

struct SceneInteractionEntry
{
	s16 id;
	s16 value : 15;		// 入口（-1 はどの入口でも）
	u16 alt_source : 1; // 1: id はシーン番号、0: グループ番号
	s16 condition;		// 立っていれば有効になるフラグ（-1 は常に）
	s16 result;			// 曲
};

extern const struct SceneInteractionEntry Scene_InteractionRuleTable[];

//--------------------------------------------------------------------------
// シーンと入口から、そのシーンの曲を決める
//--------------------------------------------------------------------------
void Scene_ResolveInteractionResult(void)
{
	s16									cue = 18;
	s16									scene = gGameState.scene;
	s16									ent = gGameState.entrance;
	s16									grp = gGameState.scene_group;
	const struct SceneInteractionEntry *p = Scene_InteractionRuleTable;

	for (; p->id != -1; p++)
	{
		if (p->alt_source)
		{
			if (p->id != scene)
				continue;
			if (p->value != -1 && p->value != ent)
				continue;
			if (p->condition != -1 && GameFlag_TestFar(p->condition) == 0)
				continue;
			cue = p->result;
			break;
		}
		if (p->id != grp)
			continue;
		if (p->value != -1 && p->value != ent)
			continue;
		if (p->condition != -1 && GameFlag_TestFar(p->condition) == 0)
			continue;
		cue = p->result;
		break;
	}

	gGameState.cue = cue;
}

struct SceneAreaNameRule
{
	s16 id;
	s16 entrance : 15;	  // -1 はどの入口でも
	u16 use_scene_id : 1; // 1: id はシーン番号、0: グループ番号
	s32 area_name_index;  // 地名番号
};

extern const struct SceneAreaNameRule Scene_AreaNameRules[];

//--------------------------------------------------------------------------
// シーンと入口から地名の番号を返す（見つからなければ 0）
//--------------------------------------------------------------------------
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance)
{
	const struct SceneAreaNameRule *p = Scene_AreaNameRules;
	s32								name = 0;
	s32								grp = BattleFx_GetResourceGroup(scene);

	while (p->id != -1)
	{
		if (p->use_scene_id)
		{
			if (p->id == scene)
			{
				if (p->entrance == -1 || p->entrance == entrance)
				{
					name = p->area_name_index;
					break;
				}
			}
		}
		else
		{
			if (p->id == grp)
			{
				if (p->entrance == -1 || p->entrance == entrance)
				{
					name = p->area_name_index;
					break;
				}
			}
		}
		p++;
	}
	return name;
}

void Party_ResolveTablePair(void)
{
	s16		   first = gGameState.scene;
	s16		   second = gGameState.entrance;
	const s16 *entry = Party_PairResolveRules;

	while (entry[0] != -1)
	{
		if (entry[0] == first && (entry[1] == -1 || entry[1] == second))
		{
			gGameState.saved_scene = entry[2];
			gGameState.saved_entrance = entry[3];
			return;
		}
		entry += 4;
	}
}

s16 Event_GetSpecialValue(void)
{
	return gGameState.special;
}

void BattleFx_SetSpecialFromTable(void)
{
	s32	 target = gGameState.scene;
	s32 *table = Battle_BackdropScenes;
	s32	 entry = *table++;
	s32	 result; // 初期化しない：該当する行がなければ不定の値がそのまま入る

	if (entry != 0 && entry != target)
	{
		do
		{
			if (entry & 0x80000000)
			{
				result = entry & 0xFFFF;
			}
			entry = *table++;
		} while (entry != 0 && entry != target);
	}
	gGameState.special = result;
}

void BattleFx_SelectResultPointer(s32 arg0)
{
	u16 value;

	switch (arg0 - 1)
	{
	case 0:
		value = (u32)&ResourceId_GrasslandBackdrop;
		break;
	case 1:
		value = (u32)&ResourceId_ForestBackdrop;
		break;
	case 2:
		value = (u32)&ResourceId_DesertBackdrop;
		break;
	case 3:
	case 6:
		value = (u32)&ResourceId_BeachBackdrop;
		break;
	case 4:
	case 5:
		value = (u32)&ResourceId_SnowfieldBackdrop;
		break;
	default:
		value = (u32)&ResourceId_SoruShindenBackdrop;
		break;
	}
	gGameState.special = value;
}

void BattleFx_SelectBattleCue(s32 first, s32 second)
{
	u16		   key = (first << 4) + second;
	const u16 *tbl = gBattleCueTable;
	u16		   cue;
	u16		   value;

	if (GameFlag_TestFar(0x16c))
	{
		cue = 18;
	}
	else
	{
		for (;;)
		{
			value = *tbl++;
			if (value == 0)
				break;
			if (value == key)
				break;
			if (value & 0x8000)
				cue = value & 0xfff;
		}
	}
	gGameState.scene_cue = cue;
}

s32 Party_RemapCharacterIdByFlags(s32 arg0)
{
	s32 result;

	result = arg0;
	if (result <= 8)
	{
		if (GameFlag_TestFar(0x20) != 0)
		{
			if (result == 0)
			{
				result = 0x12;
			}
			if (result == 1)
			{
				result = 0x13;
			}
		}
		else if ((GameFlag_TestFar(0x21) != 0) && (result == 0))
		{
			result = 0x11;
		}
	}
	return result;
}

s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1)
{
	s32 result;

	result = arg0;
	if ((result <= 8) && (arg1 != 0))
	{
		if (result == 0)
		{
			result = 0x12;
		}
		if (result == 1)
		{
			result = 0x13;
		}
	}
	return result;
}

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

extern const struct ScenePlacement Field_DefaultPlacements[2];

void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot)
{
	u32						__umodsi3(u32 numerator, u32 denominator);
	struct FieldStepWork   *work;
	struct ObjectRuntime   *object;
	struct ObjectRuntime   *previous;
	struct AnimationObject *sprite;
	s32						i;
	s32						index;
	s32						character;
	s32						condition;
	u8						resource;
	u16						id;

	work = (struct FieldStepWork *)gEventWork;
	for (i = 0; i < 4; i++)
	{
		if (work->tables[i] == entry)
			break;
		if (work->tables[i] == 0)
		{
			work->tables[i] = entry;
			break;
		}
	}
	for (id = entry->sprite; (s16)id != -1 && slot <= 65; entry++, id = entry->sprite)
	{
		if (entry->sprite <= 7)
			index = entry->sprite;
		else if (entry->sprite <= 0x2705)
			index = slot++;
		condition = entry->condition;
		if (!GameFlag_IsConditionActive(condition))
			continue;
		if ((u32)(condition - 48) <= 79 && work->mode != 3
			&& !GameFlag_IsConditionActive(condition + 80))
			continue;
		character = Party_RemapCharacterIdByFlags(entry->sprite);
		object = ObjectTable_Get(index);
		if (object == 0)
		{
			object = Object_CreateFar(character, entry->x, entry->y, entry->z);
			if (entry->flags & 1)
			{
				previous = ObjectTable_Get(index - 1);
				if (previous->animation_kind == 1 && object->animation_kind == 1)
				{
					sprite = previous->animation;
					sprite->display_flags |= 1;
					resource = sprite->slot;
					sprite = object->animation;
					sprite->display_flags |= 1;
					Resource_ResetEntry(sprite->slot);
					sprite->slot = resource;
				}
			}
			if (GameFlag_TestFar(33) && (u32)(character - 18) <= 1)
				ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object, 226);
		}
		else if (!GameFlag_TestFar(0x109))
		{
			Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
		}
		if (object)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
			if (object->animation_kind == 1 && (sprite = object->animation) != 0)
				sprite->last_no = __umodsi3(Random16(), 30);
			object->angle = entry->facing;
			object->unknown_59 = 1;
			ObjectMotion_SetActionCallback(object, entry->behavior);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
			((struct ScriptObjectRuntime *)object)->home_x = object->x / 0x10000;
			((struct ScriptObjectRuntime *)object)->home_z = object->z / 0x10000;
			if (object->y != 0)
			{
				object->flags = 4;
				object->y += 0x8000;
			}
			if (work->mode == 3)
			{
				object->flags &= 0xfe;
				if (!GameFlag_TestFar(33))
					sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
			}
			else
			{
				object->terrain_height = Map_GetTerrainHeightFar(0, object->x, object->z);
				object->y += object->terrain_height;
			}
			object->unknown_23 = 1;
		}
		work->actors[index] = object;
	}
}

void ObjectTable_DestroyAtIndex(s32 index)
{
	struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
	void				 *object = ObjectTable_Get(index);

	if (object != NULL)
	{
		ObjectDispatch_ReleaseFar(object);
		state->actors[index] = NULL;
	}
}

void ObjectTable_ResetForObject(struct ScenePlacement *table)
{
	struct FieldStepWork  *work;
	struct ScenePlacement *entry;
	struct ObjectRuntime  *object;
	struct ObjectRuntime  *camera;
	struct MapCell		  *cell;
	struct MapCell		  *above;
	s32					   leader;
	s32					   pos;
	s32					   hgt;

	work = (struct FieldStepWork *)gEventWork;
	leader = gGameState.selected_actor;
	entry = (struct ScenePlacement *)((u8 *)work + 0x200);
	entry[0] = Field_DefaultPlacements[0];
	entry[1] = Field_DefaultPlacements[1];
	for (pos = 0; pos < 4; pos++)
		work->tables[pos] = 0;
	ObjectTable_ClearBattleSlots();
	entry->condition = -1;
	entry->sprite = leader;
	entry->x = gGameState.x;
	entry->y = 0;
	entry->z = gGameState.z;
	entry->facing = gGameState.heading;
	Event_SpawnObjectTable(entry, leader);
	Event_SpawnObjectTable(table, 8);

	object = work->actors[leader];
	object->terrain_id = gGameState.turn;
	pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
	cell = &MAP_CELLS[pos];
	above = &MAP_CELLS[pos - 128];
	if (gGameState.y != 0 && cell->collision_code == 0xfd && above->collision_code == 0xfd)
	{
		gGameState.movement_mode = 1;
		hgt = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
		object->y += hgt;
		object->terrain_height = object->y;
		object->flags = 0;
		ObjectDispatch_SetSingleChildFlagsFar(object, 0);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
	}
	else
	{
		gGameState.movement_mode = 0;
	}

	camera = Object_CreateFar(0x8000, object->x, object->y, object->z);
	camera->terrain_height = object->terrain_height;
	ObjectDispatch_InitFromTable4WithArgumentFar((struct DispatchObject *)camera, (s32)object);
	if (work->mode == 3)
	{
		struct AnimationEntry *meta =
			(struct AnimationEntry *)ResourceMetadata_RegisterFar(object->animation, 23);
		meta->param = 15;
		meta->priority = 9;
	}
	((struct MapScrollWork *)gMapWork[0])->origin = &camera->x;
	((struct EventWork *)work)->view_center = (struct FieldActor *)camera;
}

s32 ObjectTable_FindLastActiveId(void)
{
	struct FieldStepWork  *state = (struct FieldStepWork *)gEventWork;
	s32					   result = 7;
	s32					   index = 8;
	struct ObjectRuntime **value = &state->actors[ACTOR_FIRST_PLACED];

	do
	{
		struct ObjectRuntime *current = *value++;

		if (current != 0)
		{
			result = index;
		}
		index++;
	} while (index <= 65);
	result++;
	if (result == 66)
	{
		result = -1;
	}
	return result;
}

void *ObjectTable_GetSlotAddress(u32 index)
{
	return (u8 *)gEventWork + 0x14 + index * sizeof(void *);
}

//--------------------------------------------------------------------------
// bounds.c
// フィールド：画面の範囲による条件フラグと、置いた物体の片付け
//--------------------------------------------------------------------------

#define FLG_IN 0x164  // 範囲の中 (下ろしたフラグ)
#define FLG_OUT 0x165 // 範囲の外 (立てたフラグ)

#define PLACED_MAX 58 // 置いた物体の数

// 範囲で条件を決める表の一行
struct SceneRegionEntry
{
	s16 id; // -1 で終わり
	s16 flag;
	u8	unknown_04[4];
	s32 x;
	u8	unknown_0c[4];
	s32 z;
	u8	unknown_14[4];
};

//--------------------------------------------------------------------------
// フラグの決まっていない行に、マップの範囲の中か外かでフラグを割り当てる
//--------------------------------------------------------------------------
void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
	struct MapScrollWork *view = gMapWork[0];

	GameFlag_ClearBitFar(FLG_IN);
	GameFlag_SetBitFar(FLG_OUT);
	while (entry->id != -1)
	{
		/* FAKEMATCH: needed to match: the flag read through a u16 keeps loop.c to one hoisted bound address */
		u16 raw = entry->flag;
		s16 flag = raw;

		if (flag == 0)
		{
			s32 x = entry->x;
			s32 z = entry->z;

			if (view->min_x <= x && x <= view->max_x && view->min_y <= z && z <= view->max_y)
				entry->flag = FLG_IN;
			else
				entry->flag = FLG_OUT;
		}
		entry++;
	}
}

//--------------------------------------------------------------------------
// 画面の中心から離れた物体を消す
// x は ±160、z は -200 から +100 ドットの外。(0, 0) にある物体は残す
//--------------------------------------------------------------------------
void BattleEffect_ClearOutOfBoundsObjects(void)
{
	struct EventWork   *work = gEventWork;
	struct FieldActor  *center;
	s32					x;
	s32					z;
	s32					left;
	s32					right;
	s32					top;
	s32					bottom;
	struct FieldActor **slot;
	s32					mask;
	s32					i;

	center = work->view_center;
	x = center->x.fixed;
	left = x - 0xa00000;
	right = x + 0xa00000;
	z = center->z.fixed;
	top = z - 0xc80000;
	bottom = z + 0x640000;
	slot = work->placed_actors;
	mask = ~1;

	for (i = PLACED_MAX - 1; i >= 0; i--)
	{
		struct FieldActor *obj = *slot;

		if (obj != NULL)
		{
			s32 obj_x = obj->x.fixed;
			s32 obj_z = obj->z.fixed;

			if (obj_x != 0 || obj_z != 0)
			{
				if (obj_x < left || obj_x > right || obj_z < top || obj_z > bottom)
				{
					u8 *spr;
					u8 *removed = &obj->active;
					u32 flags;

					// 表示を止めて消す
					*removed = 1;
					spr = (u8 *)obj->sprite;
					flags = spr[29];
					spr[29] = flags & mask;
					ObjectDispatch_ReleaseFar(obj);
					spr = NULL;
					*slot = (struct FieldActor *)spr;
				}
			}
		}

		slot++;
	}
}

//--------------------------------------------------------------------------
// 置いた物体をすべて消し、場面の配置表から置き直す
//--------------------------------------------------------------------------
void BattleEffect_ClearAllObjects(void)
{
	struct EventWork	  *work = gEventWork;
	struct ScenePlacement *table;
	s32					   i;

	for (i = 0; i < PLACED_MAX; i++)
	{
		struct FieldActor *obj = work->placed_actors[i];

		if (obj != 0)
		{
			obj->active = 1;
			obj->sprite->unknown_1d_0 = 0;
			ObjectDispatch_ReleaseFar(obj);
			work->placed_actors[i] = 0;
		}
	}
	table = work->tables[1];
	work->tables[1] = 0;
	work->tables[2] = 0;
	work->tables[3] = 0;
	if (table != 0)
		Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

void ObjectTable_ClearBattleSlots(void)
{
	s32 *current;
	s32	 zero;
	s32	 count;
	zero = 0;
	count = 0x41;
	current = (s32 *)&((struct FieldStepWork *)gEventWork)->actors[65];
	do
	{
		count--;
		*current = zero;
		current--;
	} while (count >= 0);
}

void *ObjectTable_Get(u32 index)
{
	u8 *base = (u8 *)gEventWork;
	u32 offset;
	if (index > 0xbf)
		return 0;
	offset = (index * 4) + 0x14;
	return *(void **)(base + offset);
}

void ObjectTable_Snapshot(void)
{
	struct ObjectRuntime *copy = gObjectSnapshots;
	u8					 *frames = (u8 *)&gObjectSnapshots[32];
	u8					 *next_frames = frames + 32;
	u8					 *priorities = frames + 64;
	u8					 *indices = (u8 *)gObjectSnapshots - 32;
	u32					  count = 0;
	s32					  limit = 66;
	struct ObjectRuntime *object;
	s32					  i;
	u32					  frame;
	u32					  next;
	u32					  priority;

	if (((struct EventRuntime *)gEventWork)->mode_19e == 3)
		limit = 8;
	for (i = 0; i < limit; i++)
	{
		object = ObjectTable_Get(i);
		if (object == NULL)
			continue;
		*indices++ = i;
		Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
		if (object->animation_kind == 1)
		{
			frame = ((struct AnimationObject *)object->animation)->last_no;
			next = ((struct FieldSprite *)object->animation)->flags;
			priority = ((struct FieldSprite *)object->animation)->priority;
		}
		else
		{
			frame = 0;
			next = 0;
			priority = 0;
		}
		*frames++ = frame;
		*next_frames++ = next;
		*priorities++ = priority;
		count++;
		copy++;
		if (count > 31)
			break;
	}
	for (i = count; i < 32; i++)
		*indices++ = 255;
}

void ObjectTable_Restore(void)
{
	struct ObjectRuntime  *copy = gObjectSnapshots;
	u8					  *indices = (u8 *)gObjectSnapshots - 32;
	u8					  *frames = (u8 *)&gObjectSnapshots[32];
	u8					  *next_frames = frames + 32;
	u8					  *priorities = frames + 64;
	s32					   count;
	struct ObjectRuntime  *object;
	struct FieldSprite	  *sprite;
	struct ObjectRuntime  *camera;
	s32					  *view;
	s32					   index;
	s32					   frame;
	s32					   priority;
	s32					   y;
	s32					  *selected = &gGameState.selected_actor;
	struct EventWork	 **work = &gEventWork;
	struct MapScrollWork **map = (struct MapScrollWork **)gMapWork;

	count = 0;
	index = *indices++;
	while (index != 255)
	{
		object = ObjectTable_Get(index);
		if (object != NULL)
		{
			sprite = object->animation;
			Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
			frame = *frames;
			if (frame != 0)
				ObjectDispatch_ApplyArgumentToChildrenFar(object, frame);
			ObjectDispatch_SetSingleChildFlagsFar(object, *next_frames);
			priority = *priorities;
			sprite->priority = priority;
			sprite->second_priority = priority;
			object->animation = sprite;
			if (index == *selected)
			{
				camera = (struct ObjectRuntime *)(*work)->view_center;
				view = (*map)->origin;
				y = object->y;
				camera->terrain_height = y;
				camera->y = y;
				view[1] = y;
				ObjectDispatch_InitFromTable6Far(object);
			}
		}
		copy++;
		frames++;
		next_frames++;
		priorities++;
		count++;
		if (count > 31)
			break;
		index = *indices++;
	}
}

#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

struct StepOwner
{
	u8	unknown_000[0x38];
	s16 hp;
	u8	unknown_03a[0x131 - 0x3a];
	u8	doku;
};

enum StepTileEvent
{
	STEP_EVENT_FIRST = 1,
	STEP_EVENT_LAST = 239,
	STEP_TERRAIN_FIRST = 240,
	STEP_TERRAIN_LAST = 241,
	STEP_EVENT_HAZARD = 250,
	STEP_WARP_FIRST = 252,
	STEP_WARP_LAST = 254
};

#define MODE_WORLD_MAP 3 // ワールドマップ
#define STEP_FLAG_DOUBLE_SPEED 0x167
#define STEP_CUE_EFFECT 139

void BattleFx_ResetCounters(void)
{
	short							 cleared_value;
	struct BattleEffectCounterState *state;
	cleared_value = 0;
	state = (struct BattleEffectCounterState *)gEventWork;
	state->counters[0] = cleared_value;
	state->counters[1] = 0;
	state->counters[2] = 0;
	state->counters[3] = 0;
	state->counters[4] = 0;
	state->counters[5] = 0;
	state->counters[6] = 0;
	state->counters[7] = 0;
	state->counters[8] = 0;
	state->counters[9] = 0;
	state->counters[10] = 0;
	state->counters[11] = 0;
}

s32 BattleFx_SumCounters(void)
{
	struct BattleEffectCounterState *state = (struct BattleEffectCounterState *)gEventWork;

	return state->counters[0] + state->counters[1] + state->counters[2] + state->counters[3]
		   + state->counters[4] + state->counters[5] + state->counters[6] + state->counters[7]
		   + state->counters[6] + state->counters[8] + state->counters[9] + state->counters[10]
		   + state->counters[11];
}

s32 GetFocusedObjectCollision(void)
{
	struct EventRuntime	 *runtime;
	struct ObjectRuntime *object;
	struct MapState		 *map;
	struct FieldPosition  position;
	u8					 *tile;
	s32					  x;
	s32					  y;

	runtime = WORK_SLOT(HEAP_SLOT_EVENT);
	object = ((struct FieldStepWork *)runtime)->actors[gGameState.selected_actor];

	map = WORK_SLOT(8);

	if (object == 0)
		return 0;

	position.x = object->x;
	position.y = object->y;
	position.z = object->z;
	Vector_AddPolarOffset(0x100000, object->angle, &position);

	if (runtime->mode_19e == 3)
	{
		u32 tile_x;
		u32 tile_y;

		x = position.x;
		if (x < 0)
			x += 0x1fffff;
		tile_x = (x >> 21) & 31;

		y = position.z;
		if (y < 0)
			y += 0x1fffff;
		tile_y = (y >> 21) & 31;

		tile = (u8 *)gMapBlocks + ((tile_x + (tile_y << 5)) << 2);
	}
	else
	{
		tile = (u8 *)map->layers[0].cells;

		x = position.x;
		if (x < 0)
			x += 0xfffff;
		{
			u32 tile_x = x >> 20;

			y = position.z;
			if (y < 0)
				y += 0xfffff;

			tile = (u8 *)((u32)tile + ((tile_x + ((y >> 20) << 7)) << 2));
		}
	}

	return tile[2];
}

void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z)
{
	s32						  selected_value;
	struct EventRuntime		 *runtime;
	s32						  z;
	s32						  y;
	s32						  x;
	s16						  condition;
	s16						  min_z;
	s16						  max_y;
	s16						  min_y;
	s32						  max_z;
	s16						  max_x;
	s16						  min_x;
	const struct SceneRegion *region;

	x = position_x;
	y = position_y;
	z = position_z;
	region = gOverlayArea.regions();
	if (region != 0
		&& (runtime = (struct EventRuntime *)gEventWork, min_x = region->min_x, min_x != -1))
	{
	loop:
		min_y = region->min_y;
		min_z = region->min_z;
		max_x = region->max_x;
		max_y = region->max_y;
		max_z = region->max_z;
		condition = region->condition;
		selected_value = region->value;
		if (GameFlag_IsConditionActive(condition) != 0 && y >= REGION_FIXED(min_y)
			&& y < REGION_FIXED(max_y) && x >= REGION_FIXED(min_x) && x < REGION_FIXED(max_x)
			&& z >= REGION_FIXED(min_z) && z < REGION_FIXED(max_z))
		{
			runtime->value_170 = selected_value;
			AudioCommand_PlayFar(123);
			Battle_InitializeRenderObject();
			return;
		}
		runtime = (struct EventRuntime *)gEventWork;
		region++;
		min_x = region->min_x;
		if (min_x != -1)
			goto loop;
	}
}

void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z)
{
	/* FAKEMATCH: needed to match: the StepOwner read prefix preserves the party-load order and the 44-byte frame. Plain struct BtlUnit differs by 2: its alias set ties the doku store to the active-owner read. */
	struct FieldStepWork *work = WORK_SLOT(HEAP_SLOT_EVENT);
	struct MapState		 *map = WORK_SLOT(8);
	s32					  selected = gGameState.selected_actor;
	struct ObjectRuntime *actor = work->actors[selected];
	s16					  hp[8];
	struct MapCell		 *tile;
	struct StepOwner	 *owner;
	u32					  count;
	s32					  fell;
	u32					  i;
	u32					  event;
	s32					  speed;
	s32					  steps;
	s32					  n;
	s32					  sum;
	s32					  full;

	fell = 0;
	if (work == NULL)
		return;

	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
		hp[i] = UnitGetFar(gGameState.active_owners[i])->hp;

	if (work->mode == MODE_WORLD_MAP)
	{
		tile = &(
			(struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
	}
	else
	{
		if ((u32)layer <= 2)
			tile = map->layers[layer].cells;
		else
			tile = (struct MapCell *)gMapCellBuffer;
		tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
	}
	event = tile->collision_code;
	work->previous_tile = work->tile;
	work->tile = tile;
	if (event != 0)
		UpdateMapRegionAtPosition(x, y, z);
	if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
		work->step_event = event;
	if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
		work->warp_event = event;

	if (gGameState.movement_mode == 0 && actor != NULL && actor->target_x != ACTOR_NO_TARGET)
	{
		speed = actor->speed_limit;
		if (GameFlag_TestFar(STEP_FLAG_DOUBLE_SPEED))
			speed <<= 1;
		if (work->mode == MODE_WORLD_MAP)
		{
			full = 1;
			work->unknown_17c = BattleFx_ApplyLookupResult((s32)&actor->x, speed);
		}
		else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST)
		{
			full = work->encounter_full[event - (STEP_TERRAIN_FIRST - 1)];
			work->unknown_17c =
				EffectRuntime_LookupByTableEntry(event - (STEP_TERRAIN_FIRST - 1), speed);
		}
		else
		{
			full = work->encounter_full[0];
			work->unknown_17c = EffectRuntime_LookupByTableEntry(0, speed);
		}
		steps = Iwram_MulQ16(work->encounter_rate, actor->speed_limit);
		if (!full)
			steps /= 2;
		sum = work->encounter_steps + steps;
		work->encounter_steps = sum;
		if (sum > 0xffff)
		{
			n = sum / 0x10000;
			work->encounter_steps = sum & 0xffff;
			BattleParty_ApplyDrain(n, 0);
			if (BattlePlacement_UpdateTimedEntriesFar())
				AudioCommand_PlayFar(STEP_CUE_EFFECT);
			Event_ClearInvalidPackedValues();
			fell = BattleParty_ApplyStatusDamage();
		}
		if (gGameState.hazard_mode == 0 && event == STEP_EVENT_HAZARD)
		{
			if (work->previous_tile->collision_code == STEP_EVENT_HAZARD)
				gGameState.hazard_steps += actor->speed_limit / 0x10000;
			else
				gGameState.hazard_steps = gGameState.hazard_limit / 2;
		}
		if (gGameState.avoid_distance != 0 && gGameState.scene_variant != 2)
		{
			gGameState.avoid_distance -= actor->speed_limit;
			if (gGameState.avoid_distance <= 0)
			{
				gGameState.avoid_distance = 1;
				if (work->psynergy_request == 0)
					work->psynergy_request = 0x2096;
			}
		}
	}

	if (gGameState.hazard_mode == 1)
	{
		gGameState.hazard_steps++;
		if (gGameState.hazard_steps == gGameState.hazard_limit / 2)
			BattleFx_ConfigureLinkedObject(selected, 0x101);
		if (gGameState.hazard_steps == gGameState.hazard_limit)
			BattleFx_ConfigureLinkedObject(selected, 0x100);
	}
	if (gGameState.hazard_steps >= gGameState.hazard_limit)
	{
		n = gGameState.hazard_damage;
		gGameState.hazard_steps = 0;
		BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
		fell++;
	}

	if (fell)
	{
		work->fallen_count = 0;
		work->standing_count = 0;
		actor->velocity_y = 0x40000;
		BattleFx_ConfigureLinkedObject(selected, 0x102);
		for (i = 0; i < count; i++)
		{
			owner = (struct StepOwner *)UnitGetFar(gGameState.active_owners[i]);
			if (owner->hp > 0)
			{
				work->standing_count++;
			}
			else if (hp[i] != 0)
			{
				work->fallen[work->fallen_count++] = gGameState.active_owners[i];
				work->raised_trigger = 0xffff;
				owner->doku = 0;
			}
		}
	}
}

void BattleParty_ApplyDrain(s32 amount, s32 unused)
{
	s32 target_count = Party_CountActiveOwnersFar();
	s32 i;

	for (i = 0; i < target_count; i++)
		UnitEpAddFar(gGameState.active_owners[i], amount);
}

void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled)
{
	s32				count;
	s32				i;
	s32				value;
	struct BtlUnit *unit;

	if (amount < 0)
	{
		BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
		BattleFx_StartBufferInterpolation(4);
		if (amount < -10)
			AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
		else
			AudioCommand_PlayFar(133);
	}
	else
	{
		AudioCommand_PlayFar(SOUND_RECOVERY);
	}

	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
	{
		unit = UnitGetFar(gGameState.active_owners[i]);
		if (!scaled)
		{
			value = amount;
		}
		else
		{
			value = unit->max_hp * amount / 100;
			if (value == 0)
			{
				value = amount;
				if (value < 0)
					value = -value;
			}
		}
		Owner_AdjustFirstValueFar(gGameState.active_owners[i], value);
	}
}

s32 BattleParty_ApplyStatusDamage(void)
{
	s32 result = 0;
	s32 count = Party_CountActiveOwnersFar();
	s32 i;

	for (i = 0; i < count; i++)
	{
		struct BtlUnit *unit = UnitGetFar(gGameState.active_owners[i]);
		s32				amount;

		switch (unit->doku)
		{
		case 1:
			amount = -((unit->max_hp + 10) / 20);
			if (amount == 0)
				amount = -1;
			if (result <= 0)
				result = 1;
			break;
		case 2:
			amount = -((unit->max_hp + 5) / 10);
			if (amount == 0)
				amount = -1;
			if (result <= 1)
				result = 2;
			break;
		default:
			amount = 0;
			break;
		}
		Owner_AdjustFirstValueFar(gGameState.active_owners[i], amount);
	}

	if (result != 0)
	{
		BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
		BattleFx_StartBufferInterpolation(4);
		AudioCommand_PlayFar(133);
	}

	return result;
}

void Battle_SetObjectFlag5bWhenMode3(void)
{
	struct EventRuntime			  *work;
	struct DisplayTransitionState *blk;

	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	if (work->mode_19e == 3)
	{
		blk = Runtime_AllocateBlock(0x1F, 0x540);
		if ((blk != NULL) && (blk->step != 0))
		{
			blk->start = 0;
			blk->end = 0;
			blk->duration = 1;
			blk->step = 0;
		}
		((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 1;
		Map_ResumeAnimationFar();
	}
}

void Battle_ClearObjectFlag5bWhenMode3(void)
{
	struct EventRuntime *work;

	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	if (work->mode_19e == 3)
	{
		Map_LoadAreaGraphicsFar();
		((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 0;
	}
}

// イベントのワーク (gEventWork) のこのファイルでの見え方。共通の EventWork に欄のない 0xcb6 以降も持つ
struct FieldWork
{
	u8				   unknown_000[0x10];
	s32				   view;
	struct FieldActor *actors[86];
	s16				   action;
	s16				   input;
	s16				   exit;
	s16				   menu;
	s16				   examine;
	s16				   start;
	u16				   descriptor;
	s16				   tile;
	s16				   scene;
	s16				   command;
	s16				   item;
	s16				   event;
	s16				   count;
	s16				   survived;
	s16				   quantities[9];
	s16				   item_argument;
	u8				   unknown_19c[2];
	s16				   mode;
	u8				   unknown_1a0[8];
	s32				   unknown_1a8;
	u8				   unknown_1ac[4];
	s32				   unknown_1b0;
	s32				   unknown_1b4;
	u8				   unknown_1b8[4];
	u8				  *cells;
	s32				   transition;
	u16				   unknown_1c4;
	u16				   transition_active;
	s32				   transition_frames;
	u8				   unknown_1cc[0xaea];
	u16				   busy;
	s16				   effect_active;	// 効果が動いていれば 0 以外
	s16				   effect_frames;	// 残りのフレーム
	s16				   effect_center_x; // 効果の中心 (ドット)
	s16				   effect_center_y;
	s16				   unknown_cc0;
	u8				   unknown_cc2[4];
	s8				   resolving_action; // エナジーの演出中
	s8				   unknown_cc7;
	s16				   queued_sound;
};

struct MenuCtrl
{
	u8	unknown_00[4];
	u16 wait;
};

static __inline__ s32 Vitals_Clamp(s32 ratio)
{
	s32 clamped;

	if (ratio > 0x4000)
		clamped = 0x4000;
	else if (ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

s32 Field_RunScene(s32 entrance)
{
	volatile s32	   zero;
	struct FieldWork  *work;
	struct FieldActor *actor;
	s32				   mode;
	s32				   result = 0;
	s32				   code;

	work = Runtime_AllocateBlock(27, sizeof(struct FieldWork));
	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (sizeof(struct FieldWork) / 4), REG_DMA3);
	GameFlag_ClearBitFar(0x103);
	gGameState.default_scene = gGameState.scene;
	gGameState.default_entrance = gGameState.entrance;
	gGameState.next_scene = -1;
	gGameState.next_entrance = -1;
	gGameState.defeat_scene = -1;
	gGameState.defeat_entrance = -1;
	Scheduler_ResetTaskTable();
	Djinn_ResolvePendingEvent(0);
	if (gGameState.map == (s32)&SceneId_Clear)
	{
		Map_InitializePerspectiveSceneFar();
		gGameState.perspective_scene = 1;
		mode = 3;
		BattleFx_SelectLocationRule(0);
	}
	else
	{
		Map_LoadLayeredSceneFar(gGameState.map);
		mode = 2;
		BattleFx_SelectLocationRule(1);
	}
	work->mode = mode;
	ObjectSystem_InitializeFar(mode);
	UiWork_InitializeWithResourceCountersFar();
	BattleFx_ResetCounters();
	work->view = (s32)gOverlayArea.events();
	BattleMap_ApplyEntranceView();
	ObjectTable_ResetForObject((struct ScenePlacement *)gOverlayArea.placements());
	if (GameFlag_TestFar(0x109) != 0)
		ObjectTable_Restore();
	if (gGameState.pending_djinn_event != 0)
		FieldObject_PlaceSceneActors();
	if (mode == 3)
	{
		Map_UpdateCurrentTileBlockFar();
		WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
	}
	else
	{
		Map_ApplyWorkOriginAndSpanFar();
	}
	Battle_PlaceMapMarkers();
	BattleEffect_InitializeBuffers();
	BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
	work->transition = 0x100;
	work->transition_frames = 16;
	work->transition_active = 0;
	work->unknown_1b0 = 0x199;
	work->unknown_1b4 = 0;
	if (gGameState.scene_variant == 2)
	{
		work->unknown_1b0 = 0;
		GameFlag_SetBitFar(0x144);
	}
	work->cells = Ram_MapCellBuffer;
	if (GameFlag_TestFar(0x109) == 0)
	{
		Party_ResolveTablePair();
		gGameState.hazard_limit = 16;
		gGameState.hazard_mode = 0;
		gGameState.hazard_damage = 1;
		gGameState.paused_object = -1;
		gGameState.cloaked = 0;
	}
	BattleFx_ScheduleCloakedMapCheck();
	work->queued_sound = -1;
	gOverlayArea.initialize();
	code = work->exit;
	if (code != 0)
	{
		result = code;
		work->exit = 0;
		goto done;
	}
	GameFlag_ClearBitFar(0x109);
	if (BattleFx_SumCounters() == 0)
	{
		if (work->transition_active == 0)
		{
			DisplayTransition_Start(work->transition, work->transition_frames);
			work->transition_active = 1;
			PLTT_BACKDROP = 0;
			if (ObjectEffect_RunPendingFlagEvent() == 0)
				WaitFrames((work->transition_frames + 1) / 2);
		}
		if (GameFlag_TestFar(0x12f) != 0)
		{
			GameFlag_ClearBitFar(0x12f);
			/* FAKEMATCH: UiTimedNotice_CreateFar is called as void (*)(s32); as void (*)(void), its prototype, Field_RunScene does not compile */
			((void (*)(s32))UiTimedNotice_CreateFar)(gGameState.scene);
		}
		if (gGameState.pending_djinn_event != 0)
		{
			Djinn_ResolvePendingEvent(gGameState.pending_djinn_event, 1);
			gGameState.pending_djinn_event = 0;
		}
		if (gGameState.pending_item != 0)
		{
			Battle_SetObjectFlag5bWhenMode3();
			if (Party_CheckMemberValueTotal(gGameState.pending_item) == 0)
				PartyInventory_GiveItem(gGameState.pending_item, 0);
			Battle_ClearObjectFlag5bWhenMode3();
			gGameState.pending_item = 0;
		}
	}

	for (;;)
	{
		GameFlag_SetBitFar(0x104);
		actor = work->actors[gGameState.selected_actor];
		ObjectDispatch_InitFromTable6Far(actor);
		gGameState.x = actor->x.fixed;
		gGameState.y = 0;
		gGameState.z = actor->z.fixed;
		gGameState.heading = actor->facing;
		gGameState.turn = actor->terrain_id;

		while (BattleFx_SumCounters() != 0)
		{
			s32 next = work->exit;

			if (next != 0)
			{
				result = next;
				work->exit = 0;
				goto done;
			}
			if (work->event != 0)
			{
				work->busy = 1;
				if (work->event == -1)
				{
					u32 i;

					Battle_InitializeRenderObject();
					Battle_SetObjectFlag5bWhenMode3();
					for (i = 0; i < work->count; i++)
					{
						ObjectDispatch_ApplyArgumentToChildrenFar(actor, 22);
						UiWork_PushValueSlotFar(work->quantities[i], 1);
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldStrengthExhausted, 1);
					}
					if (work->survived == 0)
					{
						struct BtlUnit *vitals;

						if (gGameState.selected_actor == 0)
						{
							if (GameFlag_TestFar(32) != 0)
								ObjectDispatch_ApplyArgumentToChildrenFar(actor, 21);
							else
								ObjectDispatch_ApplyArgumentToChildrenFar(actor, 37);
						}
						else
						{
							ObjectDispatch_ApplyArgumentToChildrenFar(actor, 19);
						}
						AudioCommand_PlayFar(59);
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPartyAnnihilated, 1);
						vitals = UnitGetFar(gGameState.selected_actor);
						vitals->hp = 1;
						/* FAKEMATCH: needed to match: the one-pass block keeps the reloaded 1 below the hp store */
						do
						{
							vitals->hp_gauge = Vitals_Clamp((vitals->hp << 14) / vitals->max_hp);
							if (vitals->hp_gauge == 0 && vitals->hp != 0)
								vitals->hp_gauge = 1;
						} while (0);
						vitals->ep_gauge = Vitals_Clamp((vitals->ep << 14) / vitals->max_ep);
						if (vitals->ep_gauge == 0 && vitals->ep != 0)
							vitals->ep_gauge = 1;
						gGameState.scene = gGameState.saved_scene;
						gGameState.entrance = gGameState.saved_entrance;
						Battle_ClearObjectFlag5bWhenMode3();
						result = 999;
						goto done;
					}
					Battle_ClearObjectFlag5bWhenMode3();
				}
				else if (work->event == -888)
				{
					Battle_InitializeRenderObject();
					Map_ShowWorldMap((s32)&ResourceId_WorldMapPicture);
				}
				else if (work->event == -889)
				{
					Battle_InitializeRenderObject();
					BattleFx_RunVisibilityTransition();
				}
				else
				{
					BattleFx_RunKind6DescriptorAction(work->event);
				}
				work->busy = 0;
				work->event = 0;
			}
			else if (work->scene != 0)
			{
				Battle_InitializeRenderObject();
				ObjectTable_Snapshot();
				gGameState.return_cue = -1;
				gGameState.scene = 510;
				gGameState.entrance = work->scene;
				work->exit = 999;
				Scene_FadeColorFromWhite(work->scene);
				work->unknown_1a8 = 0;
				work->scene = 0;
				gGameState.encounter_steps = 0;
			}
			else if (work->action != 0)
			{
				work->busy = 1;
				BattleAction_RunDescriptor(work->action);
				work->busy = 0;
				work->action = 0;
			}
			else if (work->input != 0)
			{
				Battle_DispatchInputEvent(work->input);
				work->input = 0;
			}
			else if (work->examine != 0)
			{
				s32 object;

				UiTimedNotice_CloseIfActiveFar();
				object = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
				mode = 0;
				if (object != -1)
				{
					mode = BattleFx_FindDescriptorWithOverride(object) != 0;
				}
				if (mode != 0)
				{
					work->descriptor = object | 0x1000;
					work->menu = 0;
				}
				else
				{
					mode = Object_GetTriggerTileAheadOfCurrent();
					if (mode != 0)
					{
						work->tile = mode;
						work->menu = 0;
					}
					else
					{
						work->menu = 1;
					}
				}
				work->examine = 0;
			}
			else if (work->menu != 0)
			{
				UiTimedNotice_CloseIfActiveFar();
				Battle_InitializeRenderObject();
				AudioCommand_PlayFar(111);
				Battle_SetObjectFlag5bWhenMode3();
				GameFlag_SetBitFar(0x106);
				if (*(volatile u8 *)&gDebugMode != 0 && (gKeysHeld & KEY_B) != 0
					&& (gKeysHeld & KEY_SELECT) != 0)
				{
					Menu_RunSelectionWithCursorObjectFar();
				}
				else if (GameFlag_TestFar(0x107) != 0)
				{
					work->event = 250;
				}
				else
				{
					work->unknown_cc0 = 0;
					Menu_RunTopSelectionFar();
					work->unknown_cc0 = 1;
				}
				Battle_ClearObjectFlag5bWhenMode3();
				GameFlag_ClearBitFar(0x106);
				GameFlag_RefreshLureCapFar();
				work->menu = 0;
			}
			else if ((s16)work->descriptor != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				BattleFx_RunDescriptorAction(work->descriptor & 0xfff);
				Battle_ClearObjectFlag5bWhenMode3();
				work->descriptor = 0;
			}
			else if (work->tile != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				Field_RunTileAction(work->tile);
				Battle_ClearObjectFlag5bWhenMode3();
				work->tile = 0;
			}
			else if (work->command != 0)
			{
				UiTimedNotice_CloseIfActiveFar();
				Battle_SetObjectFlag5bWhenMode3();
				BattleCommand_ExecuteSelectedAction(work->command);
				Battle_ClearObjectFlag5bWhenMode3();
				work->command = 0;
			}
			else if (work->item != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				BattleCommand_ExecuteSelectedItem(work->item, work->item_argument);
				Battle_ClearObjectFlag5bWhenMode3();
				work->item = 0;
			}
			else if (work->start != 0)
			{
				AudioCommand_PlayFar(111);
				Battle_InitializeRenderObject();
				Battle_SetObjectFlag5bWhenMode3();
				GameFlag_SetBitFar(0x106);
				if (*(volatile u8 *)&gDebugMode != 0 && (gKeysHeld & KEY_B) != 0)
				{
					Menu_RunSelectionFar();
				}
				else if (*(volatile u8 *)&gDebugMode != 0 && (gKeysHeld & KEY_L) != 0)
				{
					Debug_RunPaletteEditor();
				}
				else if (GameFlag_TestFar(0x107) != 0)
				{
					work->event = 250;
				}
				else
				{
					Battle_ResetEffectCounter();
					UiTimedNotice_CloseIfActiveFar();
					gGameState.return_cue = AudioCommand_GetSecondaryStateByteFar();
					if (GameFlag_TestFar(0x17e) == 0)
					{
						struct MenuCtrl *ctrl = (struct MenuCtrl *)gMenuCtrlWork;
						u8				*owner = ResourceBlockOwners;
						s32				 free = 0;
						s32				 n = 512;

						do
						{
							if (*owner++ == 0xff)
								free++;
						} while (--n != 0);
						if (free - 136 < 0)
						{
							ctrl->wait = 1;
							WaitFrames(1);
						}
						/* FAKEMATCH: Menu_RunWorkspaceResultLoopFar is called as void (*)(s32); as s32 (*)(void), its prototype, Field_RunScene does not compile */
						((void (*)(s32))Menu_RunWorkspaceResultLoopFar)(0);
						ctrl->wait = 0;
					}
					else
					{
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPauseUnavailable, 1);
					}
				}
				Battle_ClearObjectFlag5bWhenMode3();
				GameFlag_ClearBitFar(0x106);
				work->start = 0;
			}
		}

		GameFlag_ClearBitFar(0x104);
		actor = work->actors[gGameState.selected_actor];
		if (actor != NULL)
		{
			if (gGameState.movement_mode == 2)
				ObjectDispatch_InitFromTable0Far(actor);
			else if (gGameState.movement_mode == 1)
				ObjectDispatch_InitFromTable1Far(actor);
			else if (work->mode == 3)
				ObjectDispatch_InitFromTable3Far(actor);
			else
				ObjectDispatch_InitFromTable2Far(actor);
		}
		do
		{
			WaitFrames(1);
			actor = work->actors[gGameState.selected_actor];
			if (*(volatile u8 *)&gDebugMode == 0 || GameFlag_TestFar(0x163) == 0)
				Field_ProcessStep(
					actor->terrain_id, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		} while (BattleFx_SumCounters() == 0);
	}

done:
	if (work->transition_active != 0)
	{
		DisplayTransition_Finish(work->transition, work->transition_frames);
		work->transition_active = 0;
		WaitFrames(work->transition_frames);
	}
	Runtime_ReleaseHeapBlock(27);
	return result;
}

// FieldActor と同じ物体。位置をひとつのベクトルとして渡すための見え方
struct Object
{
	u8						pad0[6];
	u16						angle;
	struct EffectVector		pos;
	u8						pad1[14];
	u8						kind;
	u8						pad2[0x2d];
	struct AnimationObject *sprite;
};

struct EntranceView
{
	s16 entrance;
	s16 flag;
	s16 x;
	s16 y;
	s16 z;
	u16 heading;
	s16 unknown_0c;
	s16 left;
	s16 top;
	s16 right;
	s16 bottom;
	s16 unknown_16;
};

#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

struct ActionDescriptorTables
{
	struct ActionDescriptor *tables[4];
};

s32 Object_GetTriggerTileAheadOfCurrent(void)
{
	u8				   *state;
	u8				   *map;
	struct Object	   *obj;
	struct EffectVector pos;
	u8				   *cell;
	u8				   *base;
	s32					kind;
	s32					height;
	s32					result;

	result = 0;
	obj = ObjectTable_Get(gGameState.selected_actor);
	state = *(u8 **)Ram_EventWork;
	map = *(u8 **)Ram_MapWork;
	if (obj != 0)
	{
		pos.x = obj->pos.x;
		pos.y = obj->pos.y;
		pos.z = obj->pos.z;
		Vector_AddPolarOffset(0x100000, obj->angle, &pos);
		if (((struct FieldWork *)state)->mode == 3)
		{
			cell = Ram_MapBlocks
				   + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
		}
		else
		{
			base = *(u8 **)(map + 0x130);
			cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
		}
		kind = cell[2];
		if (kind >= 242 && kind <= 247)
		{
			height = Map_GetTerrainHeightFar(obj->kind, pos.x, pos.z);
			if (height >= obj->pos.y && height <= obj->pos.y + 0x400000)
			{
				result = kind;
			}
		}
		else if (BattleFx_FindDescriptor(3, kind) != 0)
		{
			result = kind;
		}
	}
	return result;
}

void BattleMap_ApplyEntranceView(void)
{
	struct MapScrollWork *work = gMapWork[0];
	s32					  entrance = gGameState.entrance;
	struct EntranceView	 *view = (struct EntranceView *)gOverlayArea.entrances();
	s32					  found = 0;

	while (view->entrance != -1)
	{
		if (view->entrance == entrance && (view->flag == -1 || GameFlag_TestFar(view->flag)))
		{
			found = 1;
			break;
		}
		view++;
	}
	if (!found)
		view = (struct EntranceView *)gOverlayArea.entrances();

	if (!GameFlag_TestFar(0x109))
	{
		gGameState.x = view->x << 16;
		gGameState.y = view->y << 16;
		gGameState.z = view->z << 16;
		gGameState.heading = view->heading;
		gGameState.turn = 0;
	}

	if (view->left != -1)
		work->min_x = view->left << 16;
	if (view->top != -1)
		work->min_y = view->top << 16;
	if (view->right != -1)
		work->max_x = view->right << 16;
	if (view->bottom != -1)
		work->max_y = view->bottom << 16;

	if (work->min_x + (240 << 16) > work->max_x)
		work->min_x = work->max_x - (240 << 16);
	if (work->min_y + (160 << 16) > work->max_y)
		work->min_y = work->max_y - (160 << 16);
}

void Debug_RunPaletteEditor(void)
{
	s32	 palette = 0;
	s32	 channel = 1;
	s32	 color = 1;
	u16 *map;
	u16 *p;
	u16 *colors;
	u32	 tile;
	u32	 value;
	u32	 red;
	u32	 green;
	u32	 blue;
	u32	 i;

	Resource_DecompressHalfwords(Debug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
	tile = (palette << 12) + 0xd1;
	colors = &PALETTE[palette * 16 + 1];
	map = EDITOR_MAP;
	p = map;
	*p = HEX_DIGIT + palette;
	p += 32;
	*p = 0xf000 | 'R';
	p += 32;
	*p = 0xf000 | 'G';
	p += 32;
	*p = 0xf000 | 'B';
	map++;
	for (i = 1; i < 16; i++)
	{
		p = map;
		*p = tile++;
		value = *colors++;
		p += 32;
		*p = (value & 31) + HEX_DIGIT;
		p += 32;
		*p = ((value >> 5) & 31) + HEX_DIGIT;
		p += 32;
		*p = ((value >> 10) & 31) + HEX_DIGIT;
		map++;
	}
	WaitFrames(1);
	for (;;)
	{
		if (gKeysRepeat & KEY_UP)
		{
			channel--;
			if (channel <= 0)
				channel = 3;
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			channel++;
			if (channel > 3)
				channel = 1;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			color--;
			if (color <= 0)
				color = 15;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			color++;
			if (color > 15)
				color = 1;
		}
		if (gKeysRepeat & KEY_L)
		{
			palette--;
			if (palette < 0)
				palette = 13;
			goto redraw;
		}
		if (gKeysRepeat & KEY_R)
		{
			palette++;
			if (palette > 13)
				palette = 0;
			goto redraw;
		}
		if (gKeysRepeat & KEY_A)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = value & 31;
			green = (value >> 5) & 31;
			blue = (value >> 10) & 31;
			if (channel == 1 && red < 31)
				red++;
			if (channel == 2 && green < 31)
				green++;
			if (channel == 3 && blue < 31)
				blue++;
			*colors = (blue << 10) | (green << 5) | red;
			goto redraw;
		}
		if (gKeysRepeat & KEY_B)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = value & 31;
			green = (value >> 5) & 31;
			blue = (value >> 10) & 31;
			if (channel == 1 && red != 0)
				red--;
			if (channel == 2 && green != 0)
				green--;
			if (channel == 3 && blue != 0)
				blue--;
			*colors = (blue << 10) | (green << 5) | red;
			goto redraw;
		}
		if (gKeysRepeat & KEY_START)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = 0;
			for (;;)
			{
				WaitFrames(1);
				if (!(gKeysHeld & KEY_START))
					break;
				if (red == 0)
					*colors = 0x7fff;
				if (red == 10)
					*colors = value;
				if (red == 20)
					*colors = 0;
				if (red == 30)
					*colors = value;
				if (++red >= 40)
					red = 0;
			}
			*colors = value;
		}
		if (gKeysRepeat & KEY_SELECT)
			break;
		gFrameCount;
		WaitFrames(1);
	}
	Ui_LoadWindowGraphics();
	Bg0_ClearTilemap();
}

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id)
{
	struct ActionDescriptor *entry;
	s32						 i;
	s32						 group = 8;

	for (i = 0; i < 4; i++)
	{
		entry = ((struct ActionDescriptorTables *)gEventWork)->tables[i];
		if (entry == NULL)
			continue;
		if (id <= 7)
		{
			for (; (s16)entry->sprite != -1; entry++)
			{
				if ((s16)entry->sprite == id)
					goto found;
			}
		}
		else
		{
			for (; (s16)entry->sprite != -1; entry++)
			{
				if ((s16)entry->sprite > 7)
				{
					if (group == id)
						goto found;
					group++;
				}
			}
		}
	}
found:
	if ((s16)entry->sprite == -1)
		return NULL;
	return entry;
}

// イベントの作業領域（gEventWork）の一部
struct EffectDescriptorRuntime
{
	u8						 unused_00[16];
	struct EffectDescriptor *descriptors; // いまのシーンのイベント表（-1 で終わり）
	u8						 unused_14[0x15c];
	s16						 queued_result; // 出口の行き先
	u8						 unused_172[0x2a];
	s16						 limit; // 押し続けた数（EventWork の blocked_steps）
};

// 配置表の 1 件（struct ScenePlacement）
struct BattleEffectAction
{
	s16 id;
	u8	unused_02[20];
	s8	mode; // 話しかけたときの向き方（talk_facing）
	u8	unused_17;
};

// イベント表の 1 件にも、イベントの作業領域にも重ねる（触れる・出口の処理で使う）
typedef struct
{
	s32 flags;
	u8	unk_04[4];
	s32 result;
	u8	unk_0c[0x170 - 0xc];
	s16 queued_result;
	u8	unk_172[0x19c - 0x172];
	s16 limit;
} EffectDescriptorWorkView;

//--------------------------------------------------------------------------
// イベントの条件を調べる
// -1 は常に成り立つ。0x1000 が立っていればフラグが立っているとき、
// そうでなければフラグが立っていないときに成り立つ
//--------------------------------------------------------------------------
s32 GameFlag_IsConditionActive(s32 condition)
{
	s32 on;

	if (condition == -1)
	{
		return 1;
	}
	if (0x1000 & condition)
	{
		return GameFlag_TestFar(condition);
	}
	on = GameFlag_TestFar(condition);
	return on == 0;
}

//--------------------------------------------------------------------------
// 調べるイベント（種類 3）で、条件を見ずに成り立たせるものか
// 対象つき（flags & 0x1ff が 3 でない）で、result の上位 12bit が 0x005 でないとき 1
//--------------------------------------------------------------------------
u32 BattleFx_CheckDescriptorKind3Result(s32 flags, s32 result)
{
	u32 tmp;
	s32 kind;
	u32 ret;

	if ((flags & 0xf) != 3)
		return 0;
	kind = flags & 0x1ff;
	ret = 0;
	if (kind != 3)
	{
		tmp = (result & 0xfff00000) ^ 0x500000;
		ret = tmp | (0U - tmp);
		ret >>= 31; // tmp != 0 なら 1
	}
	return ret;
}

//--------------------------------------------------------------------------
// いまのシーンのイベント表から、種類と相手が合い、条件の成り立つものを探す
// flags 0x200 : 押し続けた数が 12（0x800 なら 2）を超えたときだけ
// flags 0x400 : 先頭の向きが flags の上位 4bit の向きから 1/8 周以内のときだけ
// kind  : 種類
// value : 相手
//--------------------------------------------------------------------------
struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
	struct EffectDescriptorRuntime *work = (struct EffectDescriptorRuntime *)gEventWork;
	struct EffectDescriptor		   *ev = work->descriptors;
	s32								flags;
	u32 facing = ((struct FieldActor *)ObjectTable_Get(gGameState.selected_actor))->facing;

	flags = ev->flags;
	while (flags != -1)
	{
		if ((flags & 0xf) == kind && ev->value == value
			&& (BattleFx_CheckDescriptorKind3Result(flags, ev->result) != 0
				|| (GameFlag_IsConditionActive(ev->condition) != 0 && (flags = ev->flags, 1))))
		{
			s32 ok = 0;
			s32 steps = 12;
			s32 need;

			if ((flags & 0x800) != 0)
				steps = 2;
			need = flags & 0x600;

			switch (need)
			{
			case 0:
				ok = 1;
				break;
			case 0x200: // 押し続け
				if (work->limit > steps)
				{
					ok = 1;
				}
				break;
			case 0x400: // 向き
				flags &= 0xf000;
				ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
				break;
			case 0x600: // 両方
				if (work->limit > steps)
				{
					flags &= 0xf000;
					ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
				}
				break;
			}
			if (ok)
			{
				return ev;
			}
		}
		ev++;
		if (ev->flags == -1)
			break;
		flags = ev->flags;
	}
	return 0;
}

//--------------------------------------------------------------------------
// オーバーレイ領域に読み込んだシーンの初期化を呼ぶ
//--------------------------------------------------------------------------
void Runtime_CallWorkBlockEntry(void)
{
	gOverlayArea.initialize();
}

//--------------------------------------------------------------------------
// 話すイベントを探す（止めている相手なら、種類 7 のほうを先に見る）
//--------------------------------------------------------------------------
s32 BattleFx_FindDescriptorWithOverride(s32 id)
{
	struct EffectDescriptor *ev = BattleFx_FindDescriptor(0, id);
	s32						 paused = gGameState.paused_object;

	if (paused == id)
	{
		struct EffectDescriptor *alt = BattleFx_FindDescriptor(7, paused);

		if (alt != 0)
		{
			return (s32)alt;
		}
	}
	return (s32)ev;
}

//--------------------------------------------------------------------------
// オブジェクト id に話しかける
// 止めている相手は種類 7 を先に使い、なくて種類 0 が関数なら
// MsgPonderEllipsis（「・・・。」）からのメッセージで済ませる
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleFx_RunDescriptorAction(s32 id)
{
	struct BattleEffectAction *place = (struct BattleEffectAction *)BattleAction_FindDescriptor(id);
	s32						   ret = -1;
	struct ObjectRuntime	  *obj = (struct ObjectRuntime *)Object_GetById(id);
	struct EffectDescriptor	  *ev;
	s32						   paused = 0;
	u32						   saved_value;
	u32						   mode;
	s32						   talk = 0;

	if (gGameState.paused_object == id)
	{
		paused = 1;
		ev = BattleFx_FindDescriptor(7, id);
		if (ev == 0)
		{
			ev = BattleFx_FindDescriptor(0, id);
			talk = 1;
			if (ev == 0)
			{
				return -1;
			}
			if (ev->result >= 0x10000)
			{
				s32 no = BattleFx_GetFlags(id);
				u32 rnd = Random16();
				s32 mes = (s32)MsgPonderEllipsis + no * 2 + (rnd * 2 >> 16);
				Battle_Reset();
				Event_SetMessageId(mes);
				BattleEv_RunWait(id, 0);
				BattleFx_FinishAction();
				goto finish;
			}
		}
	}
	else
	{
		ev = BattleFx_FindDescriptor(0, id);
	}

	if (ev != 0 && ev->result != 0)
	{
		if (!paused)
		{
			// 相手を止め、向き方が 0・1・3 なら先頭も止めて相手とつなぐ
			u8 *busy = &obj->movement_state;

			*busy = 1;
			ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)obj, 0);
			saved_value = obj->angle;
			mode = (u8)place->mode << 24;
			if (mode <= (1 << 24) || mode == (3 << 24))
			{
				struct ObjectRuntime *leader =
					(struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);

				leader->target_x = leader->x;
				leader->target_y = leader->y;
				leader->target_z = leader->z;
				leader->velocity_x = 0;
				leader->velocity_y = 0;
				leader->velocity_z = 0;
				Object_LinkPair(id, gGameState.selected_actor, 0);
			}
		}
		if (ev->result < 0x10000)
		{
			UiWork_SetBusyFlagsFar(talk);
			Battle_Reset();
			Event_SetMessageId(ev->result);
			BattleEv_RunWait(id, 0);
			BattleFx_FinishAction();
		}
		else
		{
			Battle_InitializeRenderObject();
			((void (*)(s32))ev->result)(id);
		}
		if (!paused)
		{
			// 相手の状態が 16 なら、向き方に合わせて動きを戻す
			if (obj->script[obj->step] == (void *)16)
			{
				if (place->mode == 3)
				{
					obj->linked_object = Object_GetById(gGameState.selected_actor);
					obj->action_flags |= 1;
					ObjectMotion_SetActionCallback(obj, ObjectMotion_LinkedActionScript);
				}
				else if (place->mode == 1)
				{
					obj->action = saved_value;
					ObjectDispatch_InitializeFar(
						(struct DispatchObject *)obj, (const s32 *)ObjectMotion_StepAngleScript);
				}
			}
			obj->movement_state = 0;
			ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)obj, 16);
		}
		ret = 0;
	}
finish:
	if (paused)
	{
		BattleFx_ResumeObject(gGameState.paused_object);
		gGameState.paused_object = -1;
	}
	return ret;
}

//--------------------------------------------------------------------------
// 種類 6 のイベントを動かす
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleFx_RunKind6DescriptorAction(s32 value)
{
	s32						 res;
	s32						 ret;
	struct EffectDescriptor *ev;

	ev = BattleFx_FindDescriptor(6, value);
	ret = -1;
	if (ev != NULL)
	{
		res = ev->result;
		if (res != 0)
		{
			if (res < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(ev->result);
				BattleEv_RunWait(-1, 0);
				ret = 0;
				BattleFx_FinishAction();
			}
			else
			{
				((void (*)(s32))res)(value);
				ret = 0;
			}
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// 触れたものの触れるイベント（種類 2）を動かし、なければ出口（種類 1）へ
// 出口は音を鳴らし、行き先を積んでおく
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleAction_RunDescriptor(s32 value)
{
	EffectDescriptorWorkView *touch;
	EffectDescriptorWorkView *exit;
	s32						  kind;
	s32						  ret;
	EffectDescriptorWorkView *work;

	touch = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(2, value);
	ret = -1;
	work = (EffectDescriptorWorkView *)gEventWork;
	if ((touch != 0) && (touch->result != 0))
	{
		if (touch->flags & 0x200)
		{
			work->limit = 0;
		}
		if (touch->result < 0x10000)
		{
			Battle_Reset();
			Event_SetMessageId(touch->result);
			BattleEv_RunWait(-1, 0);
			ret = 0;
			BattleFx_FinishAction();
		}
		else
		{
			((void (*)(s32))touch->result)(value);
			goto done;
		}
	}
	else
	{
		exit = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(1, value);
		if (exit != 0)
		{
			kind = exit->flags & 0x30;
			switch (kind)
			{
			case 0:
				AudioCommand_PlayFar(123);
				break;
			case 32:
				AudioCommand_PlayFar(128);
				ObjectEffect_BeginContextEffect26();
				break;
			case 48:
				AudioCommand_PlayFar(129);
				ObjectEffect_BeginContextEffect25();
				break;
			}
			work->queued_result = exit->result;
		done:
			ret = 0;
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// 入力イベント（0xf9・0xfc〜0xfe）の処理
// 押し続けた数が 12 を超えたら動かし、数え直す
//--------------------------------------------------------------------------
s32 Battle_DispatchInputEvent(s32 event)
{
	struct EffectDescriptorRuntime *work = (struct EffectDescriptorRuntime *)gEventWork;
	s32								leader = gGameState.selected_actor;

	switch (event)
	{
	case 0xfc: // 下を押していれば BattleFx_RunRisingObjectSequence
		if (work->limit > 12 && (gKeysHeld & KEY_DOWN))
		{
			BattleFx_RunRisingObjectSequence(leader, 6, 0);
			work->limit = 0;
		}
		break;
	case 0xf9:
	case 0xfe: // 前へ跳ぶ
		if (work->limit > 12)
		{
			Field_TryJumpForward();
			work->limit = 0;
		}
		break;
	case 0xfd: // 下なら FieldEffect_UpdateGridPlacement、上なら battle_owner_69
		if (work->limit > 12)
		{
			if (gKeysHeld & KEY_DOWN)
			{
				FieldEffect_UpdateGridPlacement();
			}
			else if (gKeysHeld & KEY_UP)
			{
				battle_owner_69();
			}
			work->limit = 0;
		}
		break;
	}
	return 0;
}

s32 Field_RunTileAction(s32 tile)
{
	const struct SceneEvent *descriptor;
	s32						 leader;
	u32						 n;
	s16						 condition;
	u32						 action;
	s32						 type;
	s32						 allowed;

	leader = gGameState.selected_actor;
	n = tile - 242;
	if (n <= 5)
	{
		Battle_InitializeRenderObject();
		tile = Field_TileMessageOffsets[n];
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + tile, 1);
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + tile, 1);
	}
	else
	{
		descriptor = (const struct SceneEvent *)BattleFx_FindDescriptor(3, tile);
		if (descriptor != NULL)
		{
			s32 flags = descriptor->control;

			condition = descriptor->condition;
			n = (flags >> 4) & 31;
			if ((descriptor->trigger & 0x400) == 0 && n != 0)
			{
				Battle_InitializeRenderObject();
				UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + n, 1);
				GameFlag_SetBitFar(0x142);
			}
			else
			{
				GameFlag_ClearBitFar(0x142);
			}
			action = descriptor->value;
			if ((action & 0x0f000000) != 0 || (action & 0xfff00000) == 0x400000)
			{
				if ((action & 0x0f000000) != 0)
				{
					if (GameFlag_IsConditionActive(condition) != 0)
						((void (*)(s32))descriptor->value)(gGameState.selected_actor);
					if (GameFlag_TestFar(0x142) != 0)
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
				}
				else
				{
					if (GameFlag_IsConditionActive(condition) != 0)
						UiText_ShowPositionedMessageAndWaitFar(
							(*(const u16 *)&descriptor->value), 1);
					else
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgButFoundNothing, 1);
				}
			}
			else
			{
				Battle_Reset();
				if (GameFlag_IsConditionActive(condition) != 0)
				{
					action = descriptor->value;
					allowed = 1;
					if ((action & 0xf0000) == 0x10000 && leader <= 7)
						allowed = 0;
					if (allowed != 0)
					{
						if ((descriptor->control & 0x1ff) == 19)
							EffectRuntime_SetMode4AndPlayCue(tile);
						type = descriptor->value & 0xfff00000;
						if (type == 0x300000)
						{
							void *object;

							if ((descriptor->control & 0x1ff) == 19)
								EffectRuntime_SetMode2(tile);
							object = EffectRuntime_GetCurrentObject(tile);
							EffectRuntime_PrepareRisingObject(object);
							AudioCommand_PlayFar(83);
							UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
							UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone, 3);
							BattleParty_ApplyDrain(999, 0);
							UiWindow_CreateWithLayoutBoundsFar(1);
							AudioCommand_PlayFar(126);
							UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 1, 1);
							UiWork_FinalizeAndReleaseBlock16Far();
							ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
							AudioCommand_PlayFar(246);
							Battle_WaitMode0(30);
							UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 2, 1);
							EffectRuntime_ClearCurrentFlags(tile);
							if (condition != -1)
								GameFlag_SetBitFar(condition);
						}
						else if (type == 0x500000)
						{
							struct EventRuntime *work = (struct EventRuntime *)gEventWork;

							if ((descriptor->control & 0x1ff) == 19)
								EffectRuntime_SetMode7AndLaunch(tile);
							if (condition != -1)
							{
								condition = gGameState.pending_djinn_event = condition | 0x1000;
							}
							work->value_17c =
								BattleFx_GetWeightedResult(99, (*(const u16 *)&descriptor->value));
							Battle_SetStart(2);
							BattleFx_SelectBattleCue(99, (*(const u16 *)&descriptor->value));
							AudioCommand_PlayFar(gGameState.scene_cue);
							UiText_ShowPositionedMessageAndWaitFar((s32)MsgChestWasMimic, 1);
						}
						else if (type == 0x200000)
						{
							void *object;

							object =
								BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, 0);
							WaitFrames(30);
							if ((descriptor->control & 0x1ff) == 19)
								EffectRuntime_SetMode2(tile);
							EffectRuntime_PrepareRisingObject(object);
							AudioCommand_PlayFar(83);
							UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
							UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotCoins, 3);
							Party_AdjustCoinsFar((*(const u16 *)&descriptor->value));
							if (condition != -1)
								GameFlag_SetBitFar(condition);
							ObjectDispatch_ReleaseFar(object);
						}
						else
						{
							void *object;
							s32	  owner;

							object = BattleFx_StartRandomParticleEmitter(
								gGameState.selected_actor, descriptor->value & 0xfff);
							WaitFrames(30);
							owner = PartyInventory_AddFar((*(const u16 *)&descriptor->value));
							n = 0xffff;
							if (owner == -1)
							{
								UiWork_PushValueSlotFar(descriptor->value & 0xfff, 2);
								UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem, 1);
								UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem + 4, 1);
								Object_DestroyIfPresent(object);
								if ((descriptor->control & 0x1ff) == 19)
									EffectRuntime_SetMode5AndPlayCue(tile);
							}
							else
							{
								if ((descriptor->control & 0x1ff) == 19)
									EffectRuntime_SetMode2(tile);
								EffectRuntime_PrepareRisingObject(object);
								AudioCommand_PlayFar(83);
								UiWork_PushValueSlotFar(descriptor->value & n, 2);
								if (owner == gGameState.selected_actor)
								{
									UiText_ShowPositionedMessageAndWaitFar(
										(s32)MsgKorosseoRobinGotItem, 3);
								}
								else
								{
									UiWork_PushValueSlotFar(owner, 1);
									UiText_ShowPositionedMessageAndWaitFar(
										(s32)MsgGaveItemToMember, 3);
								}
								if (condition != -1)
									GameFlag_SetBitFar(condition);
								ObjectDispatch_ReleaseFar(object);
							}
						}
					}
					else
					{
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgCantOpenIt, 1);
					}
				}
				else
				{
					UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
				}
				BattleFx_FinishAction();
				BattleFx_PlayQueuedSound();
			}
		}
		else
		{
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedGround, 1);
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgGroundDidntFindAnything, 1);
		}
		GameFlag_ClearBitFar(0x142);
	}
	return 0;
}

//--------------------------------------------------------------------------
// nearobj.c
// 向いている方の近くにあるオブジェクトを選ぶ
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// mode に対する探す距離 (ドット)
// BattleFx_TargetRangeByMode は mode と距離の組の並びで、-1 で終わる。なければ 16
//--------------------------------------------------------------------------
s32 BattleFx_MapKeyThroughTable(s32 mode)
{
	s16 *p = BattleFx_TargetRangeByMode;
	s32	 range = 16;
	s32	 key = *p;

	while (key != -1)
	{
		++p;
		if (mode == key)
		{
			range = *p;
			break;
		}
		++p;
		key = *p;
	}
	return range;
}

//--------------------------------------------------------------------------
// id のオブジェクトにいちばん近いオブジェクトを選ぶ (なければ -1)
// 高さの差 48 ドット未満、距離 32 ドット未満のもの。11 ドットより遠ければ
// 向きの ±0x2fff の中にあるものだけ。unknown_59 の 8 は除き、4 は 10/13 の距離とみる
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyObject(s32 id)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  i;
	s32					  dx;
	s32					  dy;
	s32					  dz;
	s32					  xx;
	s32					  yy;
	s32					  zz;
	s32					  sq;
	s32					  dist;
	s32					  angle;

	best = -1;
	best_dist = 32;
	src = ObjectTable_Get(id);
	if (src == 0)
		goto done;

	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		if (obj->y - src->y >= 0)
		{
			if (obj->y - src->y >= 48 << 16)
				continue;
		}
		else if (src->y - obj->y >= 48 << 16)
		{
			continue;
		}

		// ドット単位の距離
		dx = (obj->x - src->x) / 0x10000;
		dy = (obj->y - src->y) / 0x10000;
		dz = (obj->z - src->z) / 0x10000;

		xx = dx * dx;
		yy = dy * dy;
		zz = dz * dz;
		sq = xx + yy + zz;
		dist = Iwram_Sqrt(sq);
		if ((obj->unknown_59 & 4) != 0)
			dist = dist * 10 / 13;
		if (dist >= best_dist)
			continue;

		angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
		if (dist > 11)
		{
			s32 diff = (s16)(angle - src->angle);

			if (diff < -0x2fff)
				continue;
			if (diff > 0x2fff)
				continue;
		}

		best = i;
		best_dist = dist;
	}

done:
	return best;
}

//--------------------------------------------------------------------------
// id のオブジェクトの前にいちばん近いオブジェクトを選ぶ (なければ -1)
// 向きは 90 度単位に丸め、距離は x と z だけで測る。
// mode : 探す距離と、高さの差・向きの幅を決める
//--------------------------------------------------------------------------
s32 BattleEffect_SelectNearbyTargetObject(s32 id, s32 mode)
{
	struct ObjectRuntime *src;
	struct ObjectRuntime *obj;
	s32					  best;
	s32					  best_dist;
	s32					  dir;
	s32					  i;
	s32					  dy_max;
	s32					  dx;
	s32					  dz;
	s32					  cz;
	s32					  dist;
	s32					  angle;
	s32					  width;

	best = -1;
	best_dist = BattleFx_MapKeyThroughTable(mode);
	src = (struct ObjectRuntime *)ObjectTable_Get(id);
	if (src == 0)
		return best;

	dir = (src->angle + 0x2000) & 0xc000;
	for (i = 0; i <= OBJ_LAST; i++)
	{
		if (i == id)
			continue;

		obj = (struct ObjectRuntime *)ObjectTable_Get(i);
		if (obj == 0 || (obj->unknown_59 & 8) != 0)
			continue;

		// 高さの差 : ふつうは 8、mode 13 は 48、5 は 64、2 は 16 ドットまで
		dy_max = 8 << 16;
		if (mode == 13)
			dy_max = 48 << 16;
		if (mode == 5)
			dy_max = 64 << 16;
		if (mode == 2)
			dy_max = 16 << 16;

		{
			s32 dy = obj->y - src->y;
			if (dy >= 0)
			{
				if (dy > dy_max)
					continue;
			}
			else if (src->y - obj->y > dy_max)
			{
				continue;
			}
		}

		// ドット単位の距離 (0 へ向けて切り捨て)
		dx = obj->x - src->x;
		if (dx < 0)
			dx += 0xffff;
		dx >>= 16;

		dz = obj->z - src->z;
		if (dz < 0)
			dz += 0xffff;
		cz = dz >> 16;

		dist = Iwram_Sqrt(dx * dx + cz * cz);
		if ((obj->unknown_59 & 0x10) != 0)
			dist = dist * 2 / 3;
		if (dist >= best_dist)
			continue;

		// 向きの幅 : 19 ドットまでは ±0x1800、それより遠ければ ±0x1000、mode 2 は ±0x2000
		angle = (u16)ArcTan2(obj->z - src->z, obj->x - src->x);
		width = 0x1800;
		if (dist > 19)
			width = 0x1000;
		if (mode == 2)
			width = 0x2000;

		if (dist > 11)
		{
			s32 diff = (s16)(angle - dir);
			if (diff < 0)
				diff = -diff;
			if (diff >= width)
				continue;
		}

		best = i;
		best_dist = dist;
	}

	return best;
}

//--------------------------------------------------------------------------
// target のオブジェクトがいれば音を鳴らして mode 4 にし、12 フレームのちに
// 粒の発生を始める
//--------------------------------------------------------------------------
s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
	void *obj;
	s32	  ret;

	obj = Object_GetById(target);
	ret = 0;
	if (obj != 0)
	{
		AudioCommand_PlayFar(124);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 4);
		WaitFrames(12);
		ret = (s32)BattleFx_StartRandomParticleEmitter(effect, mode);
	}
	return ret;
}

typedef s32 (*BattleItemCallback)(s32 item, s32 actor, s32 slot);

void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette)
{
	struct AnimationObject *state;
	struct AnimationEntry **p;
	struct AnimationEntry  *current;
	s32						cnt;
	s32						replacement;
	u8						n;

	if ((obj->record_storage_kind & 15) == 1)
	{
		state = obj->records;
		replacement = palette - 1;
		if (palette == 0)
			replacement = Animation_ChildPaletteCycle[(gFrameCount >> 1) & 7];

		n = state->count;
		if (n != 0)
		{
			p = state->entries;
			cnt = n;
			do
			{
				current = *p++;
				if (current != 0 && current->script != 0 && current->param != 15)
					current->param = replacement;
				cnt--;
			} while (cnt != 0);
		}
		state->dirty = 1;
	}
}

void Battle_ResetEffectCounter(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	runtime->unknown_cb6 = 0;
	if (*(s16 *)runtime->unknown_cb8 != 0)
	{
		BattleFx_ExecutePackedAbilityEffect(0x2090);
	}
}

s32 Event_FindFacingTrigger(s32 source)
{
	struct FieldStepWork	*runtime = (struct FieldStepWork *)gEventWork;
	const struct SceneEvent *trigger = runtime->events;
	s32 facing = ((struct MotionObject *)ObjectTable_Get(gGameState.selected_actor))->angle;
	s32 selected = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
	s32 collision;

	source &= 0x1ff;
	collision = GetFocusedObjectCollision();

	while (trigger->control != -1)
	{
		s32 high_value = trigger->trigger & 0xf000;
		s16 has_facing = trigger->trigger & 0x0800;
		s32 low_value = trigger->trigger & 0xff;

		if ((trigger->control & 0x0f) == 4 && GameFlag_IsConditionActive(trigger->condition) != 0
			&& (has_facing == 0 || (u16)(high_value - facing + 0x17ff) <= 0x2ffe))
		{
			u32 flags = trigger->control;
			s32 owner = ((const u8 *)&trigger->control)[1];

			if (source == 0 || owner == source)
			{
				if ((flags & 0x10) != 0)
				{
					if (low_value == selected)
						return (s32)trigger;
				}
				else if (low_value == collision)
				{
					return (s32)trigger;
				}
			}
		}
		trigger++;
	}
	return 0;
}

s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot)
{
	s32						 result;
	s32						 item_id;
	s32						 actor;
	struct BtlUnit			*obj;
	const struct SceneEvent *event;
	s32						 j;
	s32						 i;
	s32						 best;
	s32						 matches;
	s32						 count;
	struct FieldWork		*runtime;

	result = -1;
	item_id = arg & 0x3ff;
	actor = (arg >> 10) & 0xf;
	{
		best = 0;
		count = Party_CountActiveOwnersFar();

		if (actor == 15)
		{
			actor = 0;
			for (i = 0; i < count; i++)
			{
				obj = UnitGetFar(gGameState.active_owners[i]);
				matches = 0;
				for (j = 0; j < 15; j++)
				{
					if ((obj->inventory[j] & 0x1ff) == item_id)
						matches++;
				}

				if (best < matches)
				{
					best = matches;
					actor = gGameState.active_owners[i];
				}
			}
		}
		else
		{
			obj = UnitGetFar(actor);
			for (j = 0; j < 15; j++)
			{
				if ((obj->inventory[j] & 0x1ff) == item_id)
					best++;
			}
		}

		if (best == 0)
		{
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
			return -1;
		}
	}

	event = (const struct SceneEvent *)Event_FindFacingTrigger(item_id);
	if (event != 0 && event->value != 0)
	{
		GameFlag_ClearBitFar(0x143);
		GameFlag_ClearBitFar(0x142);
		if (!(event->trigger & 0x400))
		{
			UiWork_PushValueSlotFar(actor, 1);
			UiWork_PushValueSlotFar(item_id, 2);
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgActorUsesItem, 1);
		}
		if ((s32)event->value < 0x10000)
		{
			s32 objref = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
			Battle_Reset();
			Event_SetMessageId(event->value);
			BattleEv_RunWait(objref, 0);
			BattleFx_FinishAction();
		}
		else
		{
			((BattleItemCallback)event->value)(item_id, actor, slot);
		}
		result = 0;
	}
	else
	{
		s32 action_id;

		GameFlag_ClearBitFar(0x143);
		GameFlag_SetBitFar(0x142);
		action_id = Item_GetDirectFar(item_id)->action_id;
		runtime = (struct FieldWork *)gEventWork;

		if (action_id != 0)
		{
			GameFlag_SetBitFar(0x145);
			GameFlag_ClearBitFar(0x142);

			if (action_id == 149 && !GameFlag_TestFar(0x144))
			{
				UiWork_PushValueSlotFar(item_id, 2);
				UiText_ShowPositionedMessageAndWaitFar((s32)MsgUseItemQuestion, 13);
				/* FAKEMATCH: needed to match: the owner index doubles as the declined flag, which ranks it above best in global allocation (r6, best r7) */
				i = Object_CallSpawnRoutineAtOrigin(1);
				UiWork_FinalizePendingCoreFar();
				if (i != 0)
					return 0;

				gGameState.scene = gGameState.retreat_scene;
				gGameState.entrance = gGameState.retreat_entrance;
				runtime->exit = 999;
			}

			UiWork_PushValueSlotFar(actor, 1);
			UiWork_PushValueSlotFar(item_id, 2);
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgActorUsesItem, 1);
			/* FAKEMATCH: needed to match: called as returning a value, the 0 is loaded after the action id (measured differ 2 as void) */
			((s32 (*)(s32, s32))BattleFx_LoadActionEffectResources)(action_id, 0);
			runtime->resolving_action = 1;
			BattleFx_Run();
			runtime->resolving_action = 0;
			BattleEffect_CleanupSceneObjects();

			if (Item_GetDirectFar(item_id)->use_type & 1)
				GameFlag_SetBitFar(0x143);
		}
	}

	if (GameFlag_TestFar(0x142))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
	if (GameFlag_TestFar(0x143))
		Inventory_RemoveFar(actor, slot);
	return result;
}

s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
	struct FieldStepWork	*runtime = (struct FieldStepWork *)gEventWork;
	const struct SceneEvent *event = runtime->events;
	s32 reference = ((struct MotionObject *)ObjectTable_Get(gGameState.selected_actor))->angle;
	s32 selected = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, group);
	s32 alternate;
	s32 ignore_flags = 0;

	*(s32 *)result = selected;
	alternate = GetFocusedObjectCollision();
	if (requested_flags == 0x70000005)
		ignore_flags = 1;

	while (event->control != -1)
	{
		s32 high_value = event->trigger & 0xf000;
		s16 has_reference = event->trigger & 0x0800;
		s32 low_value = event->trigger & 0xff;

		if ((event->control & 0x0f) == 5 && GameFlag_IsConditionActive(event->condition) != 0
			&& (has_reference == 0 || (u16)(high_value - reference + 0x17ff) <= 0x2ffe)
			&& BtlActGet(((const u8 *)&event->control)[1])->type_0c == group
			&& (ignore_flags || (event->control & 0x7000000f) == requested_flags))
		{
			if ((event->control & 0x80) != 0)
				return (s32)event;
			if ((event->control & 0x10) != 0)
			{
				if (low_value == selected)
					return (s32)event;
			}
			else if (low_value == alternate)
			{
				return (s32)event;
			}
		}
		event++;
	}
	return 0;
}

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
	s32	  output;
	s32	  index;
	s32	  mode;
	u8	  object;
	void *first;
	void *second;

	index = packed & 0x3FF;
	mode = ((u32)packed >> 10) & 0xF;
	object = BtlActGet(index)->type_0c;
	// The object is looked up and the result is not used.
	ObjectTable_Get(gGameState.selected_actor);
	first = (void *)BattleFx_FindMatchingEvent(0x30000005, object, &output);
	second = (void *)BattleFx_FindMatchingEvent(0x20000005, object, &output);
	BattleFx_LoadActionEffectResources(index, 0);
	BattleFx_SetupObjectPair(gGameState.selected_actor, output);
	BattleFx_RunEventAction(first, mode, output);
	FieldEvent_RunTypeHandler();
	EffectRuntime_StopCurrentObject();
	BattleFx_RunEventAction(second, mode, output);
	BattleEffect_CleanupSceneObjects();
	return 0;
}

//--------------------------------------------------------------------------
// action.c
// フィールドでのエナジー使用と、マップのマーカー物体の配置
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// エナジーの実行
//--------------------------------------------------------------------------
#define ACT_NO(c) ((c) & 0x3ff)		   // わざ番号
#define ACT_UNIT(c) (((c) >> 10) & 15) // 使う者
#define ACT_PACKED 0x2000			   // EPを使わずに実行する
#define UNIT_NONE 15
#define MIKATA_MAX 7 // 仲間の番号の上限

#define ENA_IMAJIN 144 // イマジン
#define ENA_RITAN 149  // リターン

#define EXIT_RITAN 999 // リターンによる出口

#define FLG_ENA_NONE 0x17e // エナジーが効かない
#define FLG_RITAN_NG 0x144 // リターンが打ち消される
#define FLG_ITEM_USE 0x145 // 道具から使った
#define FLG_ENA_EFX 0x140  // 標準の演出を行う
#define FLG_ENA_TGT 0x141  // 対象に向けて演出する

// エナジーのイベントの段階
#define ENA_EV(phase) ((phase) << 28 | EVENT_PSYNERGY)
#define ENA_EV_ANY ENA_EV(7) // 段階を問わない

// 使う者とわざの名前を入れたメッセージを出して待つ
#define ENA_MES(who, act, m)                                               \
	(UiWork_PushValueSlotFar((who), 1), UiWork_PushValueSlotFar((act), 4), \
		UiText_ShowPositionedMessageAndWaitFar((s32)(m), 1))

//--------------------------------------------------------------------------
// フィールドでエナジーを使う
// code : わざ番号と使う者
//--------------------------------------------------------------------------
s32 BattleCommand_ExecuteSelectedAction(u32 code)
{
	s32						 act_no = ACT_NO(code);
	struct FieldWork		*work = (struct FieldWork *)gEventWork;
	const struct SceneEvent *pre;
	const struct SceneEvent *post;
	const struct SceneEvent *mid;
	s32						 aid;
	s32						 tid;
	s32						 ritan;
	s32						 group;
	s32						 cost;
	s32						 no;

	group = BtlActGet(act_no)->type_0c;
	aid = ACT_UNIT(code);
	ObjectTable_Get(gGameState.selected_actor);
	ritan = 0;
	Battle_InitializeRenderObject();
	GameFlag_ClearBitFar(FLG_ITEM_USE);
	if (aid == UNIT_NONE)
		aid = 0;

	// 効かないとき
	if (GameFlag_TestFar(FLG_ENA_NONE))
	{
		ENA_MES(aid, act_no, MsgNothingHappened);
		return 0;
	}
	if (work->mode == MODE_WORLD_MAP && act_no == ENA_IMAJIN)
	{
		ENA_MES(aid, ENA_IMAJIN, MsgNothingHappened);
		return 0;
	}

	// リターン：確かめてから戻り先へ
	if (act_no == ENA_RITAN)
	{
		if (GameFlag_TestFar(FLG_RITAN_NG))
		{
			ENA_MES(aid, ENA_RITAN, MsgDoesNotWorkHere);
			return 0;
		}
		UiWork_PushValueSlotFar(ENA_RITAN, 4);
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgUseAbilityConfirm, 13);
		no = Object_CallSpawnRoutineAtOrigin(1); // 0 = はい
		UiWork_FinalizePendingCoreFar();
		if (no != 0)
			return 0;
		{
			struct GameState *st = &gGameState;
			s32				  scene, ent;
			scene = (u16)st->retreat_scene;
			st->scene = scene;
			ent = (u16)st->retreat_entrance;
			st->entrance = ent;
		}
		work->exit = EXIT_RITAN;
		ritan = 1;
	}
	if (code & ACT_PACKED)
		return BattleFx_ExecutePackedAbilityEffect(code);

	// 仲間ならEPを払う
	if (aid <= MIKATA_MAX)
	{
		cost = BtlActGet(act_no)->pp_cost;
		if (UnitGetFar(aid)->ep < cost)
		{
			ENA_MES(aid, act_no, MsgNotEnoughPp);
			if (ritan)
				work->exit = 0;
			return 0;
		}
		UnitEpAddFar(aid, -cost);
	}

	// マップのイベントを探し、演出の方法を決める
	pre = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(1), group, &tid);
	post = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(0), group, &tid);
	mid = (const struct SceneEvent *)BattleFx_FindMatchingEvent(ENA_EV(5), group, &tid);
	tid = -1;
	GameFlag_SetBitFar(FLG_ENA_EFX);
	GameFlag_SetBitFar(FLG_ENA_TGT);
	if (pre || post || mid)
	{
		tid = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, group);
		if (post && (post->trigger & 0x400)) // イベントが演出を受け持つ
		{
			GameFlag_ClearBitFar(FLG_ENA_EFX);
			GameFlag_ClearBitFar(FLG_ENA_TGT);
		}
	}
	else
		GameFlag_ClearBitFar(FLG_ENA_TGT);

	// 演出
	if (work->mode == MODE_WORLD_MAP)
		BattleEffect_ClearOutOfBoundsObjects();
	BattleFx_LoadActionEffectResources(act_no, 0);
	work->resolving_action = 1;
	BattleFx_SetupObjectPair(gGameState.selected_actor, tid);
	EventObject_Initialize();

	BattleFx_RunEventAction(pre, aid, tid);
	if (GameFlag_TestFar(FLG_ENA_EFX))
	{
		if (GameFlag_TestFar(FLG_ENA_TGT))
			BattleFx_DispatchRequestKind();
		else
			BattleFx_Run();
	}
	EffectRuntime_StopCurrentObject();
	BattleFx_RunEventAction(post, aid, tid);
	if (GameFlag_TestFar(FLG_ENA_EFX))
		BattleFx_ClearChildValueOnMismatch();
	GameFlag_ClearBitFar(FLG_ENA_EFX);
	GameFlag_ClearBitFar(FLG_ENA_TGT);
	work->resolving_action = 0;
	BattleEffect_CleanupSceneObjects();
	if (work->mode == MODE_WORLD_MAP)
		BattleEffect_ClearAllObjects();
	return 0;
}

//--------------------------------------------------------------------------
// このエナジーに応えるイベントがマップにあるか
//--------------------------------------------------------------------------
s32 BattleFx_HasMatchingEvent5(s32 group)
{
	s32 tid;
	s32 ev = BattleFx_FindMatchingEvent(ENA_EV_ANY, (u16)group, &tid);
	return (u32)((-ev) | ev) >> 31;
}

//--------------------------------------------------------------------------
// この道具に応えるイベントが目の前にあるか
//--------------------------------------------------------------------------
u32 BattleFx_HasTrigger(u16 item)
{
	s32 ev;

	ev = Event_FindFacingTrigger(item);
	return (u32)((0 - ev) | ev) >> 31;
}

//--------------------------------------------------------------------------
// 物体を地面の高さに合わせる
//--------------------------------------------------------------------------
void ObjectMotion_SnapToTerrain(void *object)
{
	struct ObjectRuntime *obj = object;
	s32					  height;

	height = Map_GetTerrainHeightFar(0, obj->x, obj->z);
	obj->y = height;
	obj->terrain_height = height;
}

//--------------------------------------------------------------------------
// マップのマーカー物体
//--------------------------------------------------------------------------
#define MARKER_MAX 10

// マップデータの先頭
struct MarkerMap
{
	u8	unknown_00[0x10];
	u8 *markers; // (列, 行, 番号) の並び、255, 255 で終わる
};

struct MarkerSlot
{
	struct ScriptObjectRuntime *object;
	u8							id; // イベントのトリガー番号
	u8							unknown_05;
	u8							column; // 16 ドットのマス
	u8							row;
};

// イベントワークのうち、マーカーの記録
struct MarkerWork
{
	u8				  unknown_000[0x11c];
	struct MarkerSlot slots[MARKER_MAX];
};

//--------------------------------------------------------------------------
// マーカーの位置に、対応するイベントの物体を置く
// 番号 100-239 のマーカーだけを扱い、最大 10 個まで記録する
//--------------------------------------------------------------------------
void Battle_PlaceMapMarkers(void)
{
	u32							id;
	struct MarkerMap		   *map = gMapWork[0];
	struct MarkerWork		   *work = gMapWork[HEAP_SLOT_EVENT - 8];
	s32							count = 0;
	u8						   *list = map->markers;
	struct MarkerSlot		   *slot = work->slots;
	volatile u32				fill;
	const struct SceneEvent	   *event;
	struct ScriptObjectRuntime *obj;
	u32							column;
	u32							row;

	fill = 0;
	Dma_Set((void *)&fill, slot,
		DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(struct MarkerSlot) * MARKER_MAX / 4),
		REG_DMA3);
	if (list == NULL)
		return;

	column = *list++;
	row = *list++;
	while (column != 255 || row != 255)
	{
		id = *list++;
		if (id < 100 || id > 239)
			goto next;
		event = gOverlayArea.events();
		for (; event->control != -1; event++)
		{
			if (event->trigger != id)
				continue;
			if ((event->control & 0x1ff) == 19)
			{
				// マスの中心に物体 20 を置く
				obj = (struct ScriptObjectRuntime *)Object_CreateFar(
					20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
				if (obj == NULL)
					continue;
				ObjectMotion_SnapToTerrain(obj);
				ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
				if (GameFlag_TestFar(event->condition))
				{
					if ((event->value & 0xfff00000) == 0x500000)
					{
						ObjectDispatch_ReleaseFar(obj);
						continue;
					}
					ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
				}
				ObjectDispatch_InitFromTable6Far(obj);
				obj->home_x = obj->x / 0x10000;
				obj->home_z = obj->z / 0x10000;
				obj->unknown_23 = 1;
				obj->flags_59 = 1;
				slot->id = event->trigger;
				slot->object = obj;
				slot->column = obj->x / 0x100000;
				slot->row = obj->z / 0x100000;
				slot++;
				count++;
				if (count >= MARKER_MAX)
					return;
			}
			else if ((event->control & 0x1ff) == 3)
			{
				// フラグが立っていなければ、物体 28 を置く
				if ((event->value & 0xfff00000) != 0x300000)
					continue;
				if (GameFlag_TestFar(event->condition))
					continue;
				obj = (struct ScriptObjectRuntime *)Object_CreateFar(
					28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
				if (obj == NULL)
					continue;
				ObjectMotion_SnapToTerrain(obj);
				ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
				ObjectDispatch_InitFromTable6Far(obj);
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
				obj->home_x = obj->x / 0x10000;
				obj->home_z = obj->z / 0x10000;
				obj->flags_59 = 1;
				obj->unknown_23 = 1;
				slot->object = obj;
				slot->id = event->trigger;
				slot->column = obj->x / 0x100000;
				slot->row = obj->z / 0x100000;
				slot++;
				count++;
				if (count >= MARKER_MAX)
					return;
			}
			else
			{
				continue;
			}
			break;
		}
	next:
		column = *list++;
		row = *list++;
	}
}

s32 EffectRuntime_FindSlotByObject(s32 id)
{
	s32						  index;
	s32						  found;
	struct EffectRuntimeSlot *slot;

	slot = ((struct EffectRuntime *)gEventWork)->slots;
	found = -1;
	index = 0;
	if (slot->active == id)
	{
		found = 0;
	}
	else
	{
	next_slot:
		index++;
		slot++;
		if (index <= 9)
		{
			if (slot->active == id)
				found = index;
			else
				goto next_slot;
		}
	}
	return found;
}

void EffectRuntime_ReservedNoOp(void)
{
}

// id のオブジェクトの絵のアニメを mode にし、音 cue を鳴らして 12 フレーム待つ
static __inline__ void SetModeAndPlayCue(s32 id, s32 mode, s32 cue)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, mode);
		AudioCommand_PlayFar(cue);
		WaitFrames(0xc);
	}
}

void EffectRuntime_SetMode5AndPlayCue(s32 id)
{
	SetModeAndPlayCue(id, 5, 0x7d);
}

void EffectRuntime_SetMode7AndLaunch(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		WaitFrames(0x12);
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
		AudioCommand_PlayFar(0x92);
		if (object != NULL)
		{
			object->velocity_y = 0x80000;
			ObjectDispatch_SetSingleChildFlagsFar(object, 1);
		}
	}
}

void EffectRuntime_SetMode4AndPlayCue(s32 id)
{
	SetModeAndPlayCue(id, 4, 0x7c);
}

void EffectRuntime_SetMode2(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
	}
}

struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id)
{
	s32						  slot = EffectRuntime_FindSlotByObject(id);
	struct EffectRuntimeSlot *slot_data;

	if (slot == -1)
		return NULL;
	slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
	return slot_data->object;
}

void EffectRuntime_ClearCurrentFlags(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			object->animation_kind = 0;
	}
}

void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 y)
{
	s32						  slot;
	s32						  height;
	s32						  object_x;
	s32						  object_y;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	object_y = y;
	object_x = x;
	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
		{
			if (object_x == -1)
				object_x = (slot_data->tileX << 20) + 0x80000;
			if (object_y == -1)
				object_y = (slot_data->tileY << 20) + 0x80000;
			object->x = object_x;
			object->z = object_y;
			height = Map_GetTerrainHeightFar(0, object_x, object_y);
			object->terrain_height = height;
			object->y = height;
		}
	}
}

//--------------------------------------------------------------------------
// emitter.c
// フィールド効果：手に入れたアイテムの表示ときらめき、近くの印、ウィンドウの HBlank DMA
//--------------------------------------------------------------------------

LAYOUT_OFFSET_GUARD(ParticleEmitter_travel_offset_offset, struct ObjectRuntime, velocity_y, 0x28);
LAYOUT_OFFSET_GUARD(ParticleEmitter_active_offset, struct ObjectRuntime, flags, 0x55);
LAYOUT_OFFSET_GUARD(ParticleEffectObject_child_offset, struct ObjectRuntime, animation, 0x50);

struct EfxVisual
{
	u8 pad0[5];
	u8 flatbs;
	u8 pad1[3];
	u8 flatla;
	u8 pad2[18];
	u8 slot; // VRAM のタイル枠
	u8 pad3[9];
	u8 unk1;
	u8 unk2;
};

struct TileBits
{
	u16 tile : 10;
};

struct EfxObj
{
	s32				  data; // スクリプト
	u8				  pad0[0x24];
	u32				  travel;
	u8				  pad1[0x1c];
	u32				  dy;
	u8				  pad2[4];
	struct EfxVisual *vis;
	u8				  pad3[24];
	void (*proc)(void); // 毎フレームの処理
};

// オブジェクト 64 個の表 (gEventWork の 88 バイト前)
#define EfxPool (*(struct EfxObj **)((u8 *)&gEventWork - 88))

// マップのイベント (flags が -1 で終わり)
struct MapEventEntry
{
	s32 flags; // 下位 4 ビットが種類
	s16 id;
	s16 flag;
	s32 position;
};

//--------------------------------------------------------------------------
// 操作キャラが印のマスの中心から 16 ドット以内にいれば、
// 中心から操作キャラの向きへ 20 ドット離れた位置へ出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticle(void)
{
	struct ObjectRuntime *actor = ObjectTable_Get(gGameState.selected_actor);
	struct MarkerSlot	 *cell = ((struct MarkerWork *)gEventWork)->slots;
	s32					  i = 0;
	s32					  x;
	s32					  z;
	s32					  cx;
	s32					  cz;
	s32					  dx;
	s32					  dz;

	if (cell->id != 0)
	{
		x = actor->x;
		z = actor->z;
		for (;;)
		{
			cx = cell->column << 20;
			dx = x - cx - (8 << 16);
			cz = cell->row << 20;
			dz = z - cz - (8 << 16);
			if ((dx >= -0xfffff && dx <= 0xfffff) && (dz >= -0xfffff && dz <= 0xfffff))
			{
				actor->x = cx + (8 << 16);
				actor->z = cz + (8 << 16);
				Vector_AddPolarOffset(
					20 << 16, (u16)ArcTan2(dz, dx), (struct EffectVector *)&actor->x);
				actor->target_x = ACTOR_NO_TARGET; // 移動先なし
				actor->target_y = ACTOR_NO_TARGET;
				actor->target_z = ACTOR_NO_TARGET;
				return;
			}
			i++;
			cell++;
			if (i > 9 || cell->id == 0)
				break;
		}
	}
}

//--------------------------------------------------------------------------
// アイテム表示の毎フレームの処理：1/10 の確率で、周り 16 ドット以内にきらめきを出す
//--------------------------------------------------------------------------
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter)
{
	struct EffectVector	  pos;
	struct ObjectRuntime *obj;
	u32					  random_angle;

	if (emitter->velocity_y >= -255 && emitter->velocity_y <= 255)
		emitter->flags = 0;

	if ((100 * Random16() >> 16) > 9)
		return;

	pos.x = emitter->x;
	pos.y = emitter->y;
	pos.z = emitter->z;
	random_angle = Random16();
	Vector_AddPolarOffset(random_angle << 4, Random16(), &pos);
	obj = Object_Spawn(285, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		s32 mask;
		u8	flags;

		// パーツ 0 の 9 バイト目：bit 2〜3 を 1 にする
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleScript);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		mask = 13;
		flags = ((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9];
		mask = -mask;
		mask &= flags;
		mask |= 4;
		((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9] = mask;
	}
}

//--------------------------------------------------------------------------
// キャラ actor の 16 ドット前のマスの中心、高さ 16 ドットにアイテム item の絵を出す
// 効果が動いていれば、先に残っているきらめきを消す
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item)
{
	struct EfxObj *obj;

	s32 work = (s32)gEventWork;
	u32 off = (actor * 4) + 0x14; // EventWork の actors[actor]

	s32					src_z = (s32) * (struct FieldActor **)(work + off);
	struct EffectVector pos;
	s32					x;

	if (src_z == 0)
		return 0;

	pos.x = ((struct FieldActor *)src_z)->x.fixed;
	pos.y = ((struct FieldActor *)src_z)->y.fixed;
	pos.z = ((struct FieldActor *)src_z)->z.fixed;
	Vector_AddPolarOffset(16 << 16, ((struct FieldActor *)src_z)->facing, &pos);
	x = (pos.x & 0xfff00000) + (8 << 16);
	src_z = (pos.z & 0xfff00000) + (8 << 16);

	if (((struct FieldWork *)work)->effect_active != 0)
	{
		struct EfxObj *ent = EfxPool;

		work = 63;

		do
		{
			if (ent->data != 0)
			{
				if (ent->proc == (void (*)(void))BattleFx_SpawnRandomParticleAtPosition)
				{
					ObjectDispatch_ReleaseFar(ent);
				}
				if (ent->data == (s32)BattleFx_ParticleScript)
				{
					ObjectDispatch_ReleaseFar(ent);
				}
			}
			work--;
			ent++;
		} while (work >= 0);
	}

	WaitFrames(3);
	obj = (struct EfxObj *)Object_CreateFar(22, x, 16 << 16, src_z);
	if (obj == 0)
		return 0;

	ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleEmitterScript);
	{
		struct EfxVisual *vis = obj->vis;
		void			 *buf;
		s32				  mask;
		s32				  zero = 0;

		vis->unk1 = zero;
		vis->unk2 = zero;

		vis->flatbs &= zero - 33;

		mask = vis->flatla & 0x0f;
		{
			s32 clr = 13;
			clr = -clr;
			mask &= clr;
		}
		mask |= 4;
		vis->flatla = mask;

		obj->travel = 0x20000;
		obj->dy = 0x4000;

		// アイテムの絵を作業領域 17 へ読み、VRAM のタイルへ送る
		buf = Runtime_AllocateHeapBlock(17, 0x608);
		Ui_PrepareTransferForItemFar(item);

		((struct TileBits *)((u8 *)vis + 8))->tile =
			VramBlock_LoadCached(vis->slot, 128, (u8 *)buf + 0x400);

		Runtime_ReleaseHeapBlock(17);
		obj->proc = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
	}

	return obj;
}

void Object_DestroyIfPresent(void *object)
{
	if (object != 0)
		ObjectDispatch_ReleaseFar(object);
}

//--------------------------------------------------------------------------
// アイテムの絵を操作キャラの 36 ドット上に置いて動かし、
// 操作キャラに動作 28 をとらせて向きを 0x4000 (90°) にする
//--------------------------------------------------------------------------
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object)
{
	struct ObjectRuntime *actor;

	if (object == 0)
		return;

	actor = ObjectTable_Get(gGameState.selected_actor);
	object->acceleration = 1 << 16;
	object->speed_limit = 2 << 16;
	object->flags = 0;
	Object_SetMoveTargetFar(object, actor->x, actor->y + (36 << 16), actor->z);
	WaitFrames(3);
	ObjectDispatch_ApplyArgumentToChildrenFar(actor, 28);
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_RisingPulseScript);
	actor->angle = 0x4000;
}

//--------------------------------------------------------------------------
// flags の bit 0 できらめきを付け、bit 1 で EffectRuntime_PrepareRisingObject
// 両方なら前後で 60 と 80 フレーム待つ。最後に操作キャラを動作 1 にする
//--------------------------------------------------------------------------
void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
	void *actor;

	if (object != NULL)
	{
		actor = ObjectTable_Get(gGameState.selected_actor);
		if (flags & 1)
		{
			ObjectDispatch_SetSingleChildFlagsFar(object, 0);
			ObjectDispatch_InitializeFar(object, BattleFx_ParticleEmitterScript);
			((struct EfxObj *)object)->travel = 0x20000;
			((struct EfxObj *)object)->dy = 0x4000;
			((struct EfxObj *)object)->proc =
				(void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
		}
		if (flags == 3)
		{
			WaitFrames(60);
		}
		if (flags & 2)
		{
			EffectRuntime_PrepareRisingObject(object);
		}
		if (flags == 3)
		{
			WaitFrames(80);
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
	}
}

//--------------------------------------------------------------------------
// 操作キャラの 36 ドット上にアイテム value の絵を 80 フレーム出す
// flags の bit 0 できらめきを付け、bit 1 で EffectRuntime_PrepareRisingObject
//--------------------------------------------------------------------------
void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
	struct FieldActor *actor = ObjectTable_Get(gGameState.selected_actor);
	void			  *handle = Runtime_AllocateHeapBlock(17, 0x608);
	struct EfxObj	  *object = (struct EfxObj *)Object_CreateFar(
		22, actor->x.fixed, actor->y.fixed + (36 << 16), actor->z.fixed);

	if (object != 0)
	{
		struct EfxVisual *visual = object->vis;
		s32				  mask;

		s32 zero = 0;
		visual->unk1 = zero;
		visual->unk2 = zero;

		visual->flatbs &= zero - 33;

		mask = visual->flatla & 0x0f;
		{
			s32 clear = 13;
			clear = -clear;
			mask &= clear;
		}
		mask |= 4;
		visual->flatla = mask;

		Ui_PrepareTransferForItemFar(value);
		VramBlock_LoadCached(visual->slot, 128, (u8 *)handle + 0x400);
		Runtime_ReleaseHeapBlock(17);

		if (flags & 1)
			object->proc = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
		if (flags & 2)
			EffectRuntime_PrepareRisingObject((struct ObjectRuntime *)object);

		WaitFrames(80);
		ObjectDispatch_ApplyArgumentToChildrenFar(actor, 1);
		ObjectDispatch_ReleaseFar(object);
	}
}

// ドイツ語・スペイン語・フランス語・イタリア語版はメニューを開いている間きらめきを止める
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)

#define PARTICLES_PAUSE_FOR_MENU 1
#endif

#if defined(PARTICLES_PAUSE_FOR_MENU)
#define MENU_OPEN_OFS 0xcca // gEventWork：メニューを開いていれば 0 以外

//--------------------------------------------------------------------------
// 効果が動いていれば、きらめきのオブジェクトをすべて消す
//--------------------------------------------------------------------------
void BattleFx_ClearRandomParticles(void)
{
	if (((struct FieldWork *)gEventWork)->effect_active != 0)
	{
		struct EfxObj *ent = EfxPool;
		s32			   n = 63;

		do
		{
			if (ent->data != 0)
			{
				if (ent->data == (s32)BattleFx_ParticleScript)
					ObjectDispatch_ReleaseFar(ent);
			}
			n--;
			ent++;
		} while (n >= 0);
	}
}

#endif

//--------------------------------------------------------------------------
// 印の毎フレームの処理：1/10 の確率で、周り 16 ドット以内にきらめきを出す
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomParticleAtPosition(const struct FieldActor *source)
{
	struct EffectVector	  pos;
	struct ObjectRuntime *obj;
	u32					  rnd;

#if defined(PARTICLES_PAUSE_FOR_MENU)
	if (*(s8 *)((u8 *)gEventWork + MENU_OPEN_OFS) != 0)
		return;
#endif
	if ((100 * Random16() >> 16) > 9)
		return;

	pos.x = source->x.fixed;
	pos.y = source->y.fixed;
	pos.z = source->z.fixed;
	rnd = Random16();
	Vector_AddPolarOffset(rnd << 4, Random16(), &pos);
	obj = Object_Spawn(285, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		s32 mask;
		u8	flags;

		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_ParticleScript);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		mask = 13;
		flags = ((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9];
		mask = -mask;
		mask &= flags;
		mask |= 4;
		((u8 *)&((struct AnimationObject *)obj->animation)->part[0])[9] = mask;
	}
}

//--------------------------------------------------------------------------
// 効果が動いていれば 1、止まっていれば 0
//--------------------------------------------------------------------------
u32 EffectRuntime_IsActive(void)
{
	s16					  count;
	u32					  active = 0;
	struct EffectRuntime *runtime = (struct EffectRuntime *)gEventWork;

	if (runtime != NULL)
	{
		count = ((struct FieldWork *)runtime)->effect_active;
		active = (0 - count) | count;
		active >>= 31;
	}
	return active;
}

//--------------------------------------------------------------------------
// 操作キャラから横 8 マス、縦 5 マス以内にある番号 100〜239 の場所のうち、
// 種類 3 のイベントがあって、そのフラグが立っていないものに印を出す
// (position の上位 12 ビットが 0〜3 か 5 のものだけ)
//--------------------------------------------------------------------------
void FieldEffect_SpawnNearbyMarkers(void)
{
	u8					 *list;
	struct ObjectRuntime *actor;
	struct MapEventEntry *entry;
	s32					  actor_x;
	s32					  actor_z;
	s32					  x;
	s32					  z;
	s32					  id;

	// マップの場所の表：(x, z, 番号) の 3 バイトずつ、x と z が 0xff で終わり
	list = *(u8 **)(((u8 *)gMapWork[0]) + 16);
	actor = ObjectTable_Get(gGameState.selected_actor);
	actor_x = actor->x >> 20;
	actor_z = actor->z >> 20;
	if (list != 0)
	{
		while (x = *list++, z = *list++, x != 0xff || z != 0xff)
		{
			id = *list++;
			if (EffectRuntime_GetCurrentObject(id) != NULL)
				continue;
			if (id < 100 || id > 239)
				continue;
			if (actor_x - x >= 0)
			{
				if (actor_x - x > 8)
					continue;
			}
			else if (x - actor_x > 8)
				continue;
			if (actor_z - z >= 0)
			{
				if (actor_z - z > 5)
					continue;
			}
			else if (z - actor_z > 5)
				continue;
			entry = (struct MapEventEntry *)gOverlayArea.events();
			if (entry->flags == -1)
				continue;
			do
			{
				if (entry->id == id && (entry->flags & 15) == 3)
				{
					switch (entry->position & 0xfff00000)
					{
					case 0:
					case 0x100000:
					case 0x200000:
					case 0x300000:
					case 0x500000:
						if (entry->flag != -1 && GameFlag_TestFar(entry->flag) == 0)
						{
							/* FAKEMATCH: needed to match: the spawned object reuses the actor variable, which keeps both in r5 */
							actor = (struct ObjectRuntime *)Object_CreateFar(
								22, (x << 20) + (8 << 16), 0, (z << 20) + (8 << 16));
							if (actor != 0)
							{
								ObjectDispatch_InitializeFar(
									(struct DispatchObject *)actor, BattleFx_MarkerParticleScript);
								ObjectDispatch_SetSingleChildFlagsFar(actor, 0);
								((struct EfxObj *)actor)->proc =
									(void (*)(void))BattleFx_SpawnRandomParticleAtPosition;
							}
						}
						break;
					}
				}
				entry++;
			} while (entry->flags != -1);
		}
	}
}

//--------------------------------------------------------------------------
// ウィンドウ 0/1 を有効にし、ページの値を WININ, WINOUT, WIN0H, WIN1H に入れてから
// 残りを HBlank ごとに DMA0 で WIN0H と WIN1H へ 1 ワードずつ送る
//--------------------------------------------------------------------------
void BattleFx_StartWindowHBlankDma(void)
{
	struct DisplayTransitionState *work = gDisplayTransition;
	u16							  *src = work->pages[work->page];
	volatile u16				  *channel = REG_DMA0;

	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	Io_Write16(*src++, &REG_WININ);
	Io_Write16(*src++, &REG_WINOUT);
	Io_Write16(*src++, &REG_WIN0H);
	Io_Write16(*src++, &REG_WIN1H);
	Io_Write16(160, &REG_WIN0V);
	Io_Write16(160, &REG_WIN1V);
	Dma_Set(src, (void *)&REG_WIN0H, 0xa6600001, (volatile u32 *)channel);
}

struct TransitionSpan
{
	u16 cnt;
	u16 val;
	u8	x0;
	u8	x1;
	u8	y0;
	u8	y1;
};

struct TransitionShape
{
	u16					  first;
	u16					  second;
	struct TransitionSpan spans[3];
};

extern struct TransitionShape DisplayTransition_Shapes[];

void DisplayTransition_UpdateScanlineTable(void)
{
	struct DisplayTransitionState *state;
	struct TransitionSpan		  *span;
	struct TransitionShape		  *shape;
	struct MapScrollWork		  *map;
	u16							  *dst;
	s32							  *view7;
	s32							  *view8;
	s32							  *view9;
	s32							  *view77;
	struct ObjectRuntime		  *obj;
	u32							   i;
	s32							   k;
	s32							   cnt;
	s32							   v;
	s32							   rr;
	s32							   d;
	u32							   h;
	s32							   lo;
	s32							   hi;
	s32							   cx;
	s32							   cy;

	s32 ay;
	s32 bx;
	s32 by;
	u32 w;
	s32 e;

	map = gMapWork[0];
	state = gMapWork[23];

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			state->duration = 0;
			if (state->next_mode == 0)
			{
				if (state->end == 64)
				{
					REG_DISPCNT &= 0x81ff;
				}
				else
				{
					REG_DISPCNT &= 0x9fff;
				}
				Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StartWindowHBlankDma);
				Scheduler_RemoveCallback((SchedulerCallback)DisplayTransition_UpdateScanlineTable);
				{
					volatile u16 *dma0 = REG_DMA0;

					dma0[5] &= 0xc5ff;
					dma0[5] &= 0x7fff;
					dma0[5];
				}
				return;
			}
			state->mode = state->next_mode;
		}
		else
		{
			state->value =
				state->start
				+ Iwram_SignedDivide((state->end - state->start) * ++state->step, state->duration);
		}
	}

	dst = state->pages[1 ^ state->page];
	UiWindow_FillFromSceneFar(dst + 2);

	switch (state->mode)
	{
	case 0:
		*dst++ = 0x7f7f;
		*dst++ = 1;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v = DisplayTransition_NoiseWidths[v];
		for (i = 0; i < 160; i++)
		{
			w = (Random16() * (241 - v)) >> 16;
			dst[0] = ((w << 8) | (w + v));
			dst += 2;
		}
		break;

	case 1:
		v = state->value;
		shape = &DisplayTransition_Shapes[v & 31];
		if (v & 32)
		{
			*dst++ = shape->first;
			*dst++ = shape->second;
		}
		else
		{
			*dst++ = shape->second;
			*dst++ = shape->first;
		}
		for (i = 0; i < 3; i++)
		{
			span = &shape->spans[i];
			cnt = span->cnt;
			hi = span->val;
			if (cnt != 0)
			{
				if (hi != 0)
				{
					for (k = 0; k < cnt; k++)
					{
						dst[0] = hi;
						dst += 2;
					}
				}
				else
				{
					rr = span->x0;
					lo = span->y0;
					d = span->x1;
					hi = span->y1;
					for (k = 0; k < cnt; k++)
					{
						dst[0] = (((rr + (d - rr) * k / cnt) << 8) + (lo + (hi - lo) * k / cnt));
						dst += 2;
					}
				}
			}
		}
		break;

	case 2:
		v = state->value - 1;
		if (v & 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
		}
		else
		{
			*dst++ = 0x7f7f;
			*dst++ = 1;
		}
		v &= 31;
		for (i = 0; i < 160; i++)
		{
			h = (v << 4) + ((16 * Random16()) >> 16);
			if (h > 255)
			{
				h = 255;
			}
			dst[0] = h;
			dst += 2;
		}
		break;

	case 3:
		v = state->value;
		*dst++ = 0x7f7f;
		*dst++ = 1;
		if ((u32)v > 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
			v = 64 - v;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - 80;
			h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
			lo = 120 - h;
			hi = h + 120;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;

	case 4:
		v = state->value;
		if (v & 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
		}
		else
		{
			*dst++ = 0x7f7f;
			*dst++ = 1;
		}
		v = (u32)(240 * (v & 31)) >> 5;
		for (i = 0; i < 160; i += 2)
		{
			e = ((240 - v) << 8) + 240;
			dst[0] = v;
			dst[2] = e;
			dst += 4;
		}
		break;

	case 7:
	{
		s32 ax;

		view7 = &map->view_x;
		ax = view7[0] & 0xffff0000;
		ay = view7[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->active;
		*dst++ = state->mask;
		v = state->value;
		if ((v & 32) == 0)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		if (gFrameCount & 1)
		{
			v = 0;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	case 8:
		view8 = &map->view_x;
		bx = view8[0] & 0xffff0000;
		by = view8[1] & 0xffff0000;
		cx = (state->centre_x - bx) / 0x10000;
		cy = (state->centre_y - by) / 0x10000 - 16;
		k = -cy * 2 + gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < hi)
			{
				d = DisplayTransition_EdgeOffsets[k & 31];
				lo -= d;
				hi += d;
				if (lo >= hi)
				{
					lo = 240;
					hi = 240;
				}
			}
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
			k += 2;
		}
		break;

	case 9:
	{
		s32 ax;

		view9 = &map->view_x;
		ax = view9[0] & 0xffff0000;
		ay = view9[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	case 77:
	{
		s32 ax;

		view77 = &map->view_x;
		ax = view77[0] & 0xffff0000;
		ay = view77[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 8;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	}

	state->page ^= 1;
}

void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
	struct DisplayTransitionState *state = Runtime_AllocateBlock(31, sizeof(*state));
	volatile u32				   zero;

	zero = 0;
	Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
	state->mode = mode;
	state->value = 0;
	state->mask = 0x3f3f;
	state->active = 1;
	Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
	Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
	Scheduler_EnableCallbacks(&DisplayTransition_UpdateScanlineTable);
	Scheduler_EnableCallbacks(&BattleFx_StartWindowHBlankDma);
}

struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void)
{
	volatile u32				   clear_value;
	struct DisplayTransitionState *state;

	state = Runtime_AllocateBlock(31, sizeof(*state));
	clear_value = 0;
	Dma_Set((const void *)&clear_value, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
	return state;
}

void DisplayTransition_Start(s32 mode, s32 frames)
{
	struct DisplayTransitionWindow *display;
	s32								value;
	s32								kind;

	kind = (mode >> 8) & 0xff;
	display = gMapWork[0];
	value = mode & 0xff;
	switch (kind)
	{
	case 0:
		Blend_SetDarkenTarget16(0);
		Blend_SetDarkenTarget0(frames);
		WaitFrames(1);
		break;
	case 1:
		BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
		BattleFx_StartBufferInterpolation(frames);
		WaitFrames(1);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		BattleFx_SetPrimaryBufferValue(0);
		return;
	case 2:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 0;
		state->mask = 63;
		state->active = 1;
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
		Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
		WaitFrames(1);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		state->start = 0;
		state->end = 32;
		state->duration = frames;
		state->step = 0;
		return;
	}
	case 3:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		DisplayTransition_FillTilemapAndSolidTile(15);
		WaitFrames(1);
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		state->start = 0;
		state->end = 32;
		state->duration = frames;
		state->step = 0;
		return;
	}
	case 4:
	{
		struct DisplayTransitionState *state;

		display = gMapWork[0];
		state = DisplayTransition_AllocateAndClearState();
		display->first_line = 80;
		display->second_line = 80;
		WaitFrames(1);
		if (value == 0)
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 80;
			state->end = 0;
			state->duration = frames;
			state->step = 0;
		}
		else
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 80;
			state->end = 0;
			state->duration = frames;
			state->step = 0;
		}
		break;
	}
	}
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
}

void DisplayTransition_Finish(s32 mode, s32 frames)
{
	s32 value;
	s32 kind;

	kind = (mode >> 8) & 0xff;
	value = mode & 0xff;
	switch (kind)
	{
	case 0:
		Blend_SetDarkenTarget0(0);
		Blend_SetDarkenTarget16(frames);
		break;
	case 1:
		BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
		BattleFx_StartBufferInterpolation(frames);
		break;
	case 2:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		state->mask = 63;
		state->active = 1;
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
		Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
		WaitFrames(1);
		state->start = 32;
		state->end = 64;
		state->duration = frames;
		state->step = 0;
		break;
	}
	case 3:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		DisplayTransition_FillTilemapAndSolidTile(0);
		WaitFrames(1);
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
		state->start = 32;
		state->end = 64;
		state->duration = frames;
		state->step = 0;
		break;
	}
	case 4:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		if (value == 0)
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 0;
			state->end = 80;
			state->duration = frames;
			state->step = 0;
		}
		else
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 0;
			state->end = 80;
			state->duration = frames;
			state->step = 0;
		}
		break;
	}
	}
}

void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
	struct DisplayTransitionWindow *state;

	state = gMapWork[0];
	if (state != NULL)
	{
		if (clear_0200 != 0)
		{
			state->dispcnt &= 0xFDFF;
		}
		if (clear_0400 != 0)
		{
			state->dispcnt &= 0xFBFF;
		}
		if (clear_0800 != 0)
		{
			state->dispcnt &= 0xF7FF;
		}
	}
}

void DisplayTransition_Update(void)
{
	struct DisplayTransitionFrame  *state = *Ram_DisplayWork;
	struct DisplayTransitionWindow *display = *(struct DisplayTransitionWindow **)Ram_MapWork;
	s8							   *duration = &state->duration;
	u32								display_value;

	if (*duration != 0)
	{
		s8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_Update);
			Runtime_SetIrqHandler(1, 0, 0);
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			s32 value;

			(*step)++;
			value = Iwram_SignedDivide(delta * *step, *duration);
			state->value = state->start + value;
		}
	}

	display_value = state->value;
	if (display_value > 79)
	{
		display->first_line = 200;
		display->second_line = 250;
	}
	else
	{
		display->first_line = display_value;
		display->second_line = 159 - display_value;
	}
}

void DisplayTransition_UpdateFromCentre(void)
{
	struct DisplayTransitionFrame  *state = *Ram_DisplayWork;
	struct DisplayTransitionWindow *window = *(struct DisplayTransitionWindow **)Ram_MapWork;
	s8							   *duration = &state->duration;
	u16								value;

	if (*duration != 0)
	{
		s8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
			Runtime_SetIrqHandler(1, 0, 0);
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			s32 offset;

			(*step)++;
			offset = Iwram_SignedDivide(delta * *step, *duration);
			state->value = state->start + offset;
		}
	}

	value = state->value;
	if (value > 79)
	{
		window->first_line = 200;
		window->second_line = 250;
	}
	else if (value != 0 && (gFrameCount & 1))
	{
		window->first_line = value + 80;
		window->second_line = 80 - value;
	}
	else
	{
		window->first_line = 0;
		window->second_line = 159;
	}
}

void DisplayTransition_UpdateScanline(void)
{
	u32								line;
	struct DisplayTransitionWindow *state;
	u16								value;

	line = REG_VCOUNT;
	state = gMapWork[0];

again:
	switch (state->irq_phase)
	{
	case 3:
		if (line >= state->irq_first_line)
		{
			value = REG_DISPCNT;
			REG_DISPCNT = (value & 0xFFF8) | 2;
			value = 9;
			state->irq_phase = value;
		}
		break;
	case 2:
		if (line >= state->irq_second_line)
		{
			value = REG_DISPCNT;
			REG_DISPCNT = value & 0xFFF8;
			value = 9;
			state->irq_phase = value;
		}
		break;
	case 1:
		if (line >= state->irq_first_line)
		{
			value = REG_DISPCNT;
			REG_DISPCNT = (value & 0xFFF8) | 2;
			state->irq_phase++;
			goto again;
		}
		if (line >= state->irq_second_line)
		{
			value = REG_DISPCNT;
			REG_DISPCNT = value & 0xFFF8;
			value = 3;
			state->irq_phase = value;
			goto again;
		}
		break;
	case 0:
		if (line <= 158)
		{
			value = 1;
			state->irq_phase = value;
			goto again;
		}
		break;
	}
}

void DisplayTransition_UpdateFrame(void)
{
	struct DisplayTransitionFrame *state = *Ram_DisplayWork;
	s32							   value;
	s32							   blend;
	u32							   i;

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			volatile u16 *dma0;
			state->duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
			dma0 = REG_DMA0;
			dma0[5] &= 0xc5ff;
			dma0[5] &= 0x7fff;
			(void)dma0[5];
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			state->step++;
			state->value = state->start + Iwram_SignedDivide(delta * state->step, state->duration);
		}
	}

	value = state->value - 1;
	state->dither_toggle ^= 1;
	blend = 0;
	if (value & 32)
		blend = 15;
	value = (value & 31) * 2;
	for (i = 0; i <= 1; i++)
	{
		u8	entry = DisplayTransition_DitherTable[value & 63];
		u8 *p = &state->palette_nibbles[entry >> 1];
		if (entry & 1)
			*p = (*p & 0x0f) | (blend << 4);
		else
			*p = (*p & 0xf0) | blend;
		value++;
	}

	IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->palette_nibbles, 0x84000008);
}

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
	struct DisplayTransitionState *work = gDisplayTransition;
	volatile u32				   fill = 0xf000f000;
	Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
	if (color != -1)
	{
		u32	 pattern = 0;
		s32	 cnt;
		u32 *tile;
		for (cnt = 7; cnt >= 0; --cnt)
			pattern = (pattern << 4) | color;
		tile = work->solid_tile;
		for (cnt = 7; cnt >= 0; --cnt)
			*tile++ = pattern;
		Dma_Set(work->solid_tile, BG_CHAR_BLOCK(0), 0x84000008, REG_DMA3);
	}
}

void DisplayTransition_InitializeState(s32 value)
{
	struct DisplayTransitionState *state;
	volatile u32				   zero;

	state = Runtime_AllocateBlock(0x1f, sizeof(*state));
	zero = 0;
	Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
	DisplayTransition_FillTilemapAndSolidTile(0);
	state->mode = value;
	state->value = 0;
	Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
	WaitFrames(0x78);
}

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
	s32 index;
	s32 first;
	s32 second;

	if (frames > 0)
	{
		index = 0x53F;
		do
		{
			first = *source;
			second = *target;
			*step = Iwram_SignedDivide(second - first, frames);
			index--;
			source++;
			target++;
			step++;
		} while (index >= 0);
	}
}

void BattlePalette_UpdateBlend(void)
{
	struct BattleEffectBuffers *work = gPaletteBufferWork;
	u16						   *add = work->delta;
	s32							i;

	if (GameFlag_TestFar(0x152) != 0)
	{
		return;
	}
	if (work->duration == 0)
	{
		return;
	}
	if (++work->step < work->duration)
	{
		u16 *sum = work->current;
		for (i = 0; i < 0x540; i++)
		{
			*sum++ += *add++;
		}
	}
	else
	{
		Iwram_CopyWords(work->current, work->target, sizeof(work->current));
		work->duration = 0;
	}

	{
		u16 *packed = work->packed[1 ^ work->page];
		s32	 blue = 0x7c00;
		u16 *current;

		i = 0x1c0;
		current = work->current;
		for (; i != 0; i--)
		{
			*packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0)
						| (((s16)current[2] >> 10) & 0x1f);
			current += 3;
		}
	}

	work->page ^= 1;
	{
		u16 *packed = work->packed[work->page];

		IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed, 0x84000070);
		IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + 0xe0), 0x84000070);
	}
}

//--------------------------------------------------------------------------
// bufcolor.c
// エナジー演出：パレットの色変え (目標の色を作り、少しずつ近づける)
//--------------------------------------------------------------------------

#define HEAP_SLOT_PALBUF 32 // 色のバッファ (gPaletteBufferWork)

#define COLOR_MAX 448 // BG 224 色 + OBJ 224 色
#define COLOR_HALF 224

// 5 ビットの値を、バッファの 10 ビット上げた値に写す表
extern u16 BattlePalette_ChannelCurve[];
extern u16 BattlePalette_ChannelCurveLight[];
extern u16 BattlePalette_ChannelCurveContrast[];

//--------------------------------------------------------------------------
// 元の色 src から、色変えの目標 dst を作る
// dst は一色に B, G, R の三つを、5 ビットを 10 ビット上げた値で持つ
// mode : 0x8000 未満 = その色一色、0x8000 = 背景色一色、
//        PALETTE_TRANSFORM_* = 色の変換、それ以外 = 作ってある表のアドレス
// half : 0 = すべて、1 = BG の 224 色、2 = OBJ の 224 色
//--------------------------------------------------------------------------
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
	u32 i;
	u32 cnt = COLOR_MAX;
	s32 r;
	s32 b;
	u32 c;
	s32 g;

	if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
		mode = *(u16 *)BG_PLTT;
	if (half == 1)
	{
		cnt = COLOR_HALF;
	}
	else if (half == 2)
	{
		dst += COLOR_HALF * 3;
		src += COLOR_HALF;
		cnt = COLOR_HALF;
	}

	// 一色：最初の一色を書き、DMA で後ろへ広げる
	if (mode < PALETTE_TRANSFORM_FROM_SOURCE)
	{
		*dst++ = mode & 0x7c00;
		*dst++ = (mode & 0x03e0) << 5;
		*dst++ = (mode & 0x001f) << 10;
		Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | DMA_ENABLE32, REG_DMA3);
	}
	else if (mode < PALETTE_TRANSFORM_FLAGS)
	{
		switch (mode)
		{
		case PALETTE_TRANSFORM_GREY: // 明るさ (2R + 4G + B) / 7 の灰色
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = ((c << 11) & 0xf800);
				g = ((c << 7) & 0x1f000);
				b = (c & 0x7c00);
				c = Iwram_SignedDivide(r + g + b, 7);
				*dst++ = c;
				*dst++ = c;
				*dst++ = c;
			}
			break;
		case PALETTE_TRANSFORM_SEPIA: // 明るさから R を強めた色、8-28 に収める
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
				r = c * 4 + 5;
				b = c * 3 + 5;
				g = b;
				if (r < 8)
					r = 8;
				if (g < 8)
					g = 8;
				if (b < 8)
					b = 8;
				if (r > 28)
					r = 28;
				if (g > 28)
					g = 28;
				if (b > 28)
					b = 28;
				*dst++ = BattlePalette_ChannelCurveLight[b];
				*dst++ = BattlePalette_ChannelCurveLight[g];
				*dst++ = BattlePalette_ChannelCurveLight[r];
			}
			break;
		case PALETTE_TRANSFORM_WARM: // R を半分 +10、G を 2/3 +8、B を -7 (黒はそのまま)
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				if ((r | g | b) != 0)
				{
					r -= (u32)r >> 1;
					g -= g / 3;
					r += 10;
					g += 8;
					b -= 7;
					r = BattleFx_ClampRgb555Channel(r);
					g = BattleFx_ClampRgb555Channel(g);
					b = BattleFx_ClampRgb555Channel(b);
				}
				*dst++ = BattlePalette_ChannelCurveContrast[b];
				*dst++ = BattlePalette_ChannelCurveLight[g];
				*dst++ = BattlePalette_ChannelCurve[r];
			}
			break;
		case PALETTE_TRANSFORM_MUTED: // 範囲を狭め、G と B を +2
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				if (r < 10)
					r = 10;
				if (g < 16)
					g = 16;
				if (b < 16)
					b = 16;
				if (r > 28)
					r = 28;
				if (g > 24)
					g = 24;
				if (b > 26)
					b = 26;
				g += 2;
				b += 2;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				*dst++ = BattlePalette_ChannelCurve[b];
				*dst++ = BattlePalette_ChannelCurve[g];
				*dst++ = BattlePalette_ChannelCurve[r];
			}
			break;
		case PALETTE_TRANSFORM_PALE: // 半分にして平均を足す
			for (i = 0; i < cnt; i++)
			{
				s32 mean;

				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				mean = BattleFx_ClampRgb555Channel((r + g + b) / 3);
				r = (r >> 1) + mean;
				g = (g >> 1) + mean;
				b = (b >> 1) + mean;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				dst[0] = BattlePalette_ChannelCurveContrast[b];
				dst[1] = BattlePalette_ChannelCurveContrast[g];
				dst[2] = BattlePalette_ChannelCurveContrast[r];
				dst += 3;
			}
			break;
		case PALETTE_TRANSFORM_RED: // G と B の 1/8 を R へ、G と B は 2/3
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				r += ((g >> 3) + (b >> 3));
				r = BattleFx_ClampRgb555Channel(r);
				g -= g / 3;
				b -= b / 3;
				dst[0] = BattlePalette_ChannelCurve[b];
				dst[1] = BattlePalette_ChannelCurve[g];
				dst[2] = BattlePalette_ChannelCurveLight[r];
				dst += 3;
			}
			break;
		case PALETTE_TRANSFORM_WARM2: // R を半分 +6、G を 2/3 +4、B を -6
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				r = c & 31;
				g = (c >> 5) & 31;
				b = (c >> 10) & 31;
				r -= (u32)r >> 1;
				g -= g / 3;
				r += 6;
				g += 4;
				b -= 6;
				r = BattleFx_ClampRgb555Channel(r);
				g = BattleFx_ClampRgb555Channel(g);
				b = BattleFx_ClampRgb555Channel(b);
				dst[0] = BattlePalette_ChannelCurveContrast[b];
				dst[1] = BattlePalette_ChannelCurveLight[g];
				dst[2] = BattlePalette_ChannelCurve[r];
				dst += 3;
			}
			break;
		default: // そのまま
			for (i = 0; i < cnt; i++)
			{
				c = *src++;
				dst[i * 3 + 0] = c & 0x7c00;
				dst[i * 3 + 1] = (c & 0x03e0) << 5;
				dst[i * 3 + 2] = (c & 0x001f) << 10;
			}
			break;
		}
	}
	else if (mode & PALETTE_TRANSFORM_TINT)
	{
		// 明るさ (2R + 4G + B) に、mode の色をかける
		u32 tr;
		u32 tg;
		u32 tb;
		s32 r;
		s32 g;

		tr = mode & 31;
		tg = (mode >> 5) & 31;
		tb = (mode >> 10) & 31;
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			r = ((c << 11) & 0xf800);
			g = ((c << 7) & 0x1f000);
			b = (c & 0x7c00);
			c = Iwram_SignedDivide(r + g + b, 96);
			r = c * tr;
			g = c * tg;
			b = c * tb;
			r = BattleFx_ClampRgb555Component(r);
			g = BattleFx_ClampRgb555Component(g);
			b = BattleFx_ClampRgb555Component(b);
			dst[0] = b;
			dst[1] = g;
			dst[2] = r;
			dst += 3;
		}
	}
	else if (mode & PALETTE_TRANSFORM_BLEND)
	{
		// 明るさ R + G + B を、mode の色の比で配り直す
		u32 tr;
		u32 tg;
		u32 tb;
		s32 nr;
		s32 ng;
		s32 nb;

		tr = mode & 31;
		tg = (mode >> 5) & 31;
		tb = (mode >> 10) & 31;
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			r = c & 31;
			c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tr + tg + tb);
			nr = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
			ng = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
			nb = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
			nr = BattleFx_ClampRgb555Channel((u32)nr >> 16);
			ng = BattleFx_ClampRgb555Channel((u32)ng >> 16);
			nb = BattleFx_ClampRgb555Channel((u32)nb >> 16);
			*dst++ = BattlePalette_ChannelCurve[nb];
			*dst++ = BattlePalette_ChannelCurve[ng];
			*dst++ = BattlePalette_ChannelCurve[nr];
		}
	}
	else if (mode & PALETTE_TRANSFORM_SPLIT)
	{
		for (i = 0; i < cnt; i++)
		{
			c = *src++;
			dst[i * 3 + 0] = c & 0x7c00;
			dst[i * 3 + 1] = (c & 0x03e0) << 5;
			dst[i * 3 + 2] = (c & 0x001f) << 10;
		}
	}
	else
	{
		// 作ってある表を写す
		if (half == 2)
			mode += COLOR_HALF * 3 * sizeof(u16);
		Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	}
}

//--------------------------------------------------------------------------
// 色のバッファを用意し、今の BG と OBJ のパレットを写す
//--------------------------------------------------------------------------
void BattleEffect_InitializeBuffers(void)
{
	volatile s32				zero;
	struct BattleEffectBuffers *buf;

	buf = Runtime_AllocateBlock(HEAP_SLOT_PALBUF, sizeof(*buf));
	zero = 0;
	Dma_Set((const void *)&zero, buf,
		DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(*buf) >> 2), REG_DMA3);
	Dma_Set(
		(void *)BG_PLTT, &buf->palette[0], DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
	Dma_Set((void *)OBJ_PLTT, &buf->palette[COLOR_HALF],
		DMA_ENABLE32 | DMA_32BIT | (COLOR_HALF / 2), REG_DMA3);
	BattleFx_BuildBuffer(PALETTE_TRANSFORM_PLAIN, buf->palette, buf->target, 0);
	Scheduler_AddOrUpdateCallback((void (*)(void))BattlePalette_UpdateBlend, 0xc8f);
}

//--------------------------------------------------------------------------
// 色変えをやめ、バッファを返す
//--------------------------------------------------------------------------
void Runtime_ScheduleCallbackAndReleaseBlock32B(void)
{
	Scheduler_RemoveCallback(&BattlePalette_UpdateBlend);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_PALBUF);
}

//--------------------------------------------------------------------------
// 目標の色を作る
//--------------------------------------------------------------------------
void BattleFx_ApplyColorToTargetBuffer(s32 mode, s32 half)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		BattleFx_BuildBuffer(mode, buf->palette, buf->target, half);
	}
}

//--------------------------------------------------------------------------
// 今の色を作る
//--------------------------------------------------------------------------
void BattleFx_ApplyColorToSourceBuffer(s32 mode, s32 half)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		BattleFx_BuildBuffer(mode, buf->palette, buf->current, half);
	}
}

//--------------------------------------------------------------------------
// 元の背景色を変える
//--------------------------------------------------------------------------
void BattleFx_SetPrimaryBufferValue(u32 col)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		buf->palette[0] = col;
	}
}

//--------------------------------------------------------------------------
// 今の色から目標の色へ、frames フレームで変え始める
//--------------------------------------------------------------------------
void BattleFx_StartBufferInterpolation(s32 frames)
{
	struct BattleEffectBuffers *buf = gPaletteBufferWork;

	if (buf != NULL)
	{
		buf->duration = frames;
		buf->step = 0;
		BattleFx_InterpolateBuffers(buf->current, buf->target, buf->delta, frames);
	}
}

//--------------------------------------------------------------------------
// 5 ビットの色の値を 0-31 に収める
//--------------------------------------------------------------------------
s32 BattleFx_ClampRgb555Channel(s32 value)
{
	if (value > 31)
		return 31;
	if (value < 0)
		value = 0;
	return value;
}

//--------------------------------------------------------------------------
// 10 ビット上げた色の値を 31 << 10 までに収める
//--------------------------------------------------------------------------
s32 BattleFx_ClampRgb555Component(s32 value)
{
	if (value > 31744)
		value = 31744;
	return value;
}

struct FadeActor
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[2];
	s16 floor;
	u8	unknown_18[10];
	u8	layer;
};

struct FadeWork
{
	struct AnimationSpritePart marks[2];
	struct FadeActor		  *actor;
};

struct FadeView
{
	u8	unknown_00[228];
	s16 camera[4];
};

void Object_EffectSpawnCallback(void)
{
	struct FadeWork			   *work = WORK_SLOT(36);
	s16						   *camera = ((struct FadeView *)WORK_SLOT(8))->camera;
	s32							camera_x = camera[1];
	s32							camera_z = camera[3];
	struct AnimationSpritePart *mark = work->marks;
	struct FadeActor		   *actor = work->actor;
	s32							x;
	s32							z;
	s32							floor;
	s32							layer;
	u32							tile;
	s32							height;
	s32							other;
	s32							row;
	/* FAKEMATCH: needed to match: nothing reads or writes these four bytes */
	s16 unused[2];

	if (actor == 0)
		return;
	x = actor->x;
	z = actor->z;
	floor = actor->floor;
	layer = actor->layer;
	tile = gVramBlockCache[94].offset >> 5;
	x -= 0x80000;
	height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
	other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
	if (other > height)
		height = other;
	if (height > 0 && height > floor)
	{
		((u32 *)mark)[1] = 0x40000800;
		((u32 *)mark)[2] = 0x400;
		mark->priority = 0;
		mark->tile = tile;
		mark->object_mode = 1;
		mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
		row = (z >> 16) & 0xf0;
		mark->y = row - camera_z - height + 16;
		Runtime_PushSlotEntry(mark, 0);
	}
	x += 0x100000;
	height = Map_GetTerrainHeightFar(layer, x, z + 0x100000) >> 16;
	mark = &work->marks[1];
	other = (Map_GetTerrainHeightFar(layer, x, z + 0x200000) >> 16) - 16;
	if (other > height)
		height = other;
	if (height > 0 && height > floor)
	{
		((u32 *)mark)[1] = 0x40000800;
		((u32 *)mark)[2] = 0;
		mark->priority = 0;
		mark->tile = tile;
		mark->object_mode = 1;
		mark->x = ((u32)(x >> 16) & 0xfff0) - camera_x;
		row = (z >> 16) & 0xf0;
		mark->y = row - camera_z - height + 16;
		Runtime_PushSlotEntry(mark, 0);
	}
}

void BattleFx_StartFadeOverlay(void *object)
{
	u32			*work;
	u8			*tiles;
	volatile u32 fill;

	work = Runtime_AllocateBlock(36, 28);
	tiles = Runtime_AllocateHeapBlock(14, 0x400);
	fill = 0x11111111;
	Dma_Set((const void *)&fill, tiles, 0x85000080, REG_DMA3);
	VramBlock_LoadCached(94, 0x200, tiles);
	Runtime_ReleaseHeapBlock(14);
	Scheduler_AddOrUpdateCallback(Object_EffectSpawnCallback, 0xc80);
	Io_Write16(0x3f9e, &REG_BLDCNT);
	Io_Write16(16, &REG_BLDALPHA);
	Io_Write16(31, &REG_BLDY);
	fill = 0;
	Dma_Set((const void *)&fill, work, 0x85000007, REG_DMA3);
	if (object == NULL)
		object = ObjectTable_Get(gGameState.selected_actor);
	work[6] = (u32)object;
}

void Object_EnableEffectSpawnCallback(void)
{
	Scheduler_EnableCallbacks(Object_EffectSpawnCallback);
}

void Object_DisableEffectSpawnCallback(void)
{
	Scheduler_DisableCallbacks(Object_EffectSpawnCallback);
}

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
	struct BattleEffectEntry *entry = BattleFx_DefinitionTable;
	u32						  index = 0;

	if (entry->id != id)
	{
		do
		{
			index += 1;
			entry++;
			if (index > 0x81)
				break;
		} while (entry->id != id);
	}
	return entry;
}

s32 BattleFx_GetAnimationValue(s32 id)
{
	struct BattleRenderObject *object = ObjectTable_Get(id);

	if (object->kind != 1 || object->animation == NULL || object->animation->value_28 == NULL)
	{
		return 0;
	}
	return *object->animation->value_28;
}

s32 BattleFx_GetResourceId(u32 id)
{
	u8 value;

	if (gGameState.speech == 0 || (value = BattleFx_FindDefinition(id)->value) == 0xFF)
	{
		return 0;
	}
	return value + 0x100;
}

s32 BattleFx_GetFlags(s32 id)
{
	return BattleFx_FindDefinition(BattleFx_GetAnimationValue(id))->flags;
}

void Battle_UpdateModeFromShoulderButtons(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	if (*(volatile u8 *)&gDebugMode != 0)
	{
		if (gKeyState & KEY_L)
		{
			runtime->mode_1cc = 0;
		}
		if (gKeyState & KEY_R)
		{
			runtime->mode_1cc = -1;
		}
	}
}

void Battle_WaitMode0(s32 frames)
{
	if (((struct BattleRuntime *)gEventWork)->mode_1cc == 0 && frames != 0)
	{
		WaitFrames(frames);
	}
}

void Battle_InitializeRenderObject(void)
{
	struct BattleRenderObject *object;

	object = ObjectTable_Get(gGameState.selected_actor);
	object->unknown_30 = 0x10000;
	object->unknown_34 = 0x8000;
	object->unknown_38 = 0x80000000;
	object->unknown_40 = 0x80000000;
	object->unknown_24 = 0;
	object->unknown_2c = 0;
	if (gGameState.movement_mode == 1)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 0xC);
		return;
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void Battle_Reset(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	UiTimedNotice_CloseIfActiveFar();
	Battle_InitializeRenderObject();
	if (runtime->unknown_cb6 != 0)
	{
		Battle_ResetEffectCounter();
	}
	{
		s32 zero = 0;
		runtime->unknown_cc2 = zero;
		runtime->unknown_cc4 = zero;
		runtime->unknown_1c8 = 0x10;
		runtime->mode_1cc = zero;
		runtime->unknown_1da = 0xFFFF;
		runtime->unknown_1dc = -1;
		runtime->unknown_1de = -1;
		Scheduler_AddOrUpdateCallback((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
		GameFlag_ClearBitFar(0x132);
		runtime->object_id = gGameState.selected_actor;
		runtime->unknown_1f8 = zero;
	}
}

void BattleFx_FinishAction(void)
{
	Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
	Object_AttachWorkTargetToObject(gGameState.selected_actor, 1);
	GameFlag_RefreshLureCapFar();
}

void Event_NoOpHookA(void)
{
}

void Event_NoOpHook(void)
{
}

void Event_RunObjectHookAndWait(struct ScenePlacement *table)
{
	Event_NoOpHook();
	ObjectTable_ResetForObject(table);
	WaitFrames(1);
	ObjectTable_Get(gGameState.selected_actor);
}

void Event_CallWithLastActiveObjectId(struct ScenePlacement *table)
{
	Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

void Event_SetSceneEvents(s32 value)
{
	struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

	work->events = (const struct SceneEvent *)value;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
	UnitGetFar(event_id);
	Party_AddActiveOwnerFar(event_id);
	if (value != 0)
	{
		/* FAKEMATCH: Party_ShowJoinedMessageFar is called as s32 (*)(s32, s32); as void (*)(s32), its prototype, Event_PrepareObjectAndApplyValue does not compile */
		((s32 (*)(s32, s32))Party_ShowJoinedMessageFar)(event_id, value);
	}
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
	Party_AddActiveOwnerFar(first);
	Party_AddActiveOwnerFar(second);
	Party_ShowPairJoinedMessageFar(first, second);
}

s32 Event_ValidatePackedId(u32 packed_id)
{
	s32 group = (packed_id >> 10) & 0xf;
	s32 entry = packed_id & 0x3ff;

	if (group > 7)
		return -1;
	if (GameFlag_TestFar(group) == 0)
		return -2;
	if (Equipment_HasValueFar(group, entry) == 0)
		return -3;
	return 0;
}

void Event_ClearInvalidPackedValues(void)
{
	if (Event_ValidatePackedId(gGameState.first_shortcut) != 0)
		gGameState.first_shortcut = 0;
	if (Event_ValidatePackedId(gGameState.second_shortcut) != 0)
		gGameState.second_shortcut = 0;
}

void Party_RemoveOwnerRestored(s32 owner)
{
	struct BtlUnit *vitals;
	s32				i;
	s32				count;
	s32				standing;

	Party_RemoveActiveOwnerFar(owner);
	Event_ClearInvalidPackedValues();
	vitals = UnitGetFar(owner);
	Unit_Recover(vitals);
	vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
	if (vitals->hp_gauge == 0 && vitals->hp != 0)
		vitals->hp_gauge = 1;
	vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
	if (vitals->ep_gauge == 0 && vitals->ep != 0)
		vitals->ep_gauge = 1;
	vitals->doku = 0;
	standing = 0;
	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
	{
		vitals = UnitGetFar(gGameState.active_owners[i]);
		if (vitals->hp != 0)
			standing++;
	}
	if (standing == 0)
	{
		vitals = UnitGetFar(gGameState.selected_actor);
		vitals->hp = 1;
		vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
		if (vitals->hp_gauge == 0 && vitals->hp != 0)
			vitals->hp_gauge = 1;
		vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
		if (vitals->ep_gauge == 0 && vitals->ep != 0)
			vitals->ep_gauge = 1;
	}
}

s32 Party_CheckMemberValueTotal(s32 id)
{
	s32 count;
	s32 sum = 0;
	s32 i;

	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
		sum += Inventory_CountItemFar(gGameState.active_owners[i], id);

	if (sum >= count * 30)
	{
		UiWork_PushValueSlotFar(id, 2);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
		UiWork_PushValueSlotFar(id, 2);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
		return -1;
	}
	return 0;
}

s32 PartyInventory_GiveItem(s32 item, s32 unused)
{
	struct EventRuntime *work;
	s32					 saved;
	s32					 owner;
	s32					 result;
	s32					 count;
	s32					 i;
	s32					 member;
	s32					 slot;

	work = (struct EventRuntime *)gEventWork;
	saved = work->message;
	owner = PartyInventory_AddFar(item);
	if (owner == -1)
	{
		ItemGift_ShowGotItem(item, 1);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
	retry:
		UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
		result = Shop_PickUnitItemFar(&member, &slot);
		if (result == -1)
		{
			if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE)
			{
				UiWork_PushValueSlotFar(item, 2);
				UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
				goto retry;
			}
			UiWork_PushValueSlotFar(item, 2);
			UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
			result = Object_CallSpawnRoutineAtOrigin(1);
			UiWork_FinalizePendingCoreFar();
			if (result != 0)
				goto retry;
			Item_AdjustCounterFar(item, 1);
			UiWork_PushValueSlotFar(item, 2);
			UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
			work->message = saved;
			return owner;
		}
		else
		{
			UnitGetFar(member);
			{
				/* FAKEMATCH: the ROM loads the slot argument before the member. Only the English build, without the thirty-item check below, needs the copy; the direct call matches the rest. */
				s32 s = slot;

				count = Shop_GetSelectionState(member, s);
			}
#if !defined(TBS_EDITION_EN)
			if (Inventory_CountItemFar(member, item) > 29)
			{
				UiWork_PushValueSlotFar(member, 1);
				UiWork_PushValueSlotFar(item, 2);
				UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
				goto retry;
			}
#endif
			for (i = 0; i < count; i++)
				Inventory_DiscardFar(member, slot);
			owner = PartyInventory_AddFar(item);
			AudioCommand_PlayFar(83);
			if (owner == gGameState.selected_actor)
			{
				ItemGift_ShowGotItem(item, 3);
			}
			else
			{
				UiWork_PushValueSlotFar(item, 2);
				UiWork_PushValueSlotFar(owner, 1);
				UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
			}
			work->message = saved;
			return owner;
		}
	}
	else
	{
		AudioCommand_PlayFar(83);
		ItemGift_ShowGotItem(item, 3);
		if (owner != gGameState.selected_actor)
		{
			UiWork_PushValueSlotFar(item, 2);
			UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
			UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 1);
#else
			UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 3);
#endif
		}
		work->message = saved;
	}
	return owner;
}

//--------------------------------------------------------------------------
// try_add_and_return_owner.c
// 持ち物：アイテムを持たせて、持ち主を返す
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// アイテムを持たせる
// 持てればキャラクター番号を、持ち物がいっぱいなら -1 を返す
// item   : アイテム番号
// unused : 使わない
// owner  : 持たせるキャラクター番号
//--------------------------------------------------------------------------
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner)
{
	if (Inventory_AddItemFar(owner, item) >= 0)
		return owner;
	return -1;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Inventory_NoOpCallback(void)
{
}

struct EventPromptWork
{
	struct EventRuntime event;
	u8					unknown_200[0xac2];
	s16					resource_base;
	s16					width;
};

void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
	struct ObjectRuntime *obj = ObjectTable_Get(object_id);

	if (obj != 0 && obj->animation_kind == 1)
	{
		s32 cnt = 0;
		u8 *p = &((struct AnimationObject *)obj->animation)->last_no;

		while (cnt <= 89)
		{
			WaitFrames(1);
			if (value != *p)
			{
				break;
			}
			cnt++;
		}
	}
}

s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom)
{
	struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
	s32						sprite =
		((struct ScenePlacement *)BattleAction_FindDescriptor(work->event.speaker))->sprite;
	struct UiWindow *window = (struct UiWindow *)work->event.message_window;
	struct UiWindow *side = (struct UiWindow *)work->event.side_window;
	s32				 use_bottom = 1;
	s32				 answer;

	while (gKeyState != 0)
		WaitFrames(1);

	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);

	WaitFrames(3);

	if (force_bottom == 0)
	{
		s32 bottom = window->y + window->height;

		if (side != 0)
		{
			s32 side_bottom = side->y + side->height;
			if (bottom < side_bottom)
				bottom = side_bottom;
		}

		if (bottom > 15)
			use_bottom = 0;
	}

	answer = Menu_RunConfirmSelectionFar(use_bottom, work->resource_base, work->width, 0);
	if (answer != 0)
	{
		Object_SetModeById(actor, 4);
		UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
		UiWork_FinalizePendingCoreFar();
		Object_WaitUntilChildValueDiffers(actor, 4);
	}
	else
	{
		Object_SetModeById(actor, 3);
		UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
		UiWork_FinalizePendingCoreFar();
		Object_WaitUntilChildValueDiffers(actor, 3);
	}

	return answer;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
	return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}

//--------------------------------------------------------------------------
// draw_draw_quantity_pair_with_cue.c
// メッセージ：わざを覚えさせて「覚えた」と出す
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// わざを覚えさせ、効果音とメッセージを出して、送られるまで待つ
// unit : キャラクター番号（メッセージの名前 1）
// abil : わざの番号（メッセージのわざ 4）
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 abil)
{
	// 引数なしの宣言だが、r0・r1 の unit・abil がそのまま渡る
	/* FAKEMATCH: OwnerAction_AddFar is called as void (*)(void); as s32 (*)(s32, s32), its prototype, UiText_DrawQuantityPairWithCue does not compile */
	((void (*)(void))OwnerAction_AddFar)();
	AudioCommand_PlayFar(83);
	UiWork_PushValueSlotFar(unit, 1);
	UiWork_PushValueSlotFar(abil, 4);
	UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

void Event_StartScreenTransition(void)
{
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

	DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
	runtime->status_1c6 = 1;
}

void Event_FinishScreenTransition(void)
{
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

	DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
	runtime->status_1c6 = 0;
}

void Event_WaitTransitionFrames(void)
{
	WaitFrames(((struct EventRuntime *)gEventWork)->value_1c8);
}

void Event_SetDestination(s32 first, s32 second)
{
	((struct EventRuntime *)gEventWork)->value_170 = 999;
	gGameState.scene = first;
	gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	state->value_170 = 999;
	gGameState.saved_scene = first;
	gGameState.saved_entrance = second;
}

void Event_SetExitRequest(u16 value)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	state->value_170 = value;
}

//--------------------------------------------------------------------------
// result_request.c
// バトル効果：イベントから決まった敵との戦闘を用意する
//--------------------------------------------------------------------------

#define CHOJO 98	  // 頂上の嵐の背景で戦うエンカウント表
#define TOBANAI 0x800 // ジンを跳ねさせない
#define JIN_0 300	  // ジン 0 番の pending_djinn_event

//--------------------------------------------------------------------------
// エンカウント表 zone の slot 番目の敵グループとの戦闘を用意する
// ワールドマップ（mode 3）では、選択中の者の足もとの地形も引く
//--------------------------------------------------------------------------
void BattleFx_SetWeightedResult(s32 zone_no, s32 slot_no)
{
	register s32				  zone = zone_no;
	register s32				  slot = slot_no;
	register struct EventRuntime *ev = (struct EventRuntime *)gEventWork;

	ev->value_17c = BattleFx_GetWeightedResult(zone, slot); // 敵グループ
	if (zone == CHOJO && slot == 0)
		gGameState.special = (s32)&ResourceId_VinasuChojoStormBackdrop;
	if (ev->mode_19e == 3)
		BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.selected_actor) + 8); // 位置
	BattleFx_SelectBattleCue(zone, slot);
}

//--------------------------------------------------------------------------
// ジン no 番との戦闘を用意する
// flag の下 8 ビットはジンのオブジェクトで、TOBANAI がなければ跳ねさせる
// no は属性ごとに 20 ずつ（Djinn_ResolvePendingEvent）
//--------------------------------------------------------------------------
void BattleFx_SetPhaseRequest(s32 flag, s32 no)
{
	struct EventRuntime *ev;
	struct GameState	*gs;
	s32					 skip;

	ev = (struct EventRuntime *)gEventWork;
	skip = flag & TOBANAI;
	flag &= 0xff;

	if (skip == 0)
		EventActor_RunHopAndLeapSequence(flag);

	gs = &gGameState;
	gs->pending_djinn_event = (no + JIN_0) | skip;
	ev->value_17c = BattleFx_GetPhaseResult(no); // ジンの敵グループ
	if (ev->mode_19e == 3)
	{
		void *obj = ObjectTable_Get(gs->selected_actor);

		BattleFx_LookupResult((u8 *)obj + 8); // 位置
	}
	((s32 (*)())BattleFx_SelectBattleCue)(0, 0); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
}

void Party_SetFields1ceAnd1d0(s32 first, s32 second)
{
	gGameState.next_scene = first;
	gGameState.next_entrance = second;
}

void Event_SetDefeatDestination(s32 first, s32 second)
{
	gGameState.defeat_scene = first;
	gGameState.defeat_entrance = second;
}

void Event_RequestDestination(u16 first, u16 second)
{
	struct EventRuntime *work = (struct EventRuntime *)gEventWork;
	gGameState.scene = first;
	gGameState.entrance = second;
	work->value_170 = 999;
}

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
	((struct FieldWork *)gEventWork)->queued_sound = sound_id;
	if ((s16)sound_id == -1)
	{
		sound_id = 0x121;
	}
	AudioCommand_PlayFar(0x12a);
	AudioCommand_PlayFar(sound_id);
}

void BattleFx_PlayQueuedSound(void)
{
	s16 sound_id = ((struct FieldWork *)gEventWork)->queued_sound;

	if (sound_id != -1)
		AudioCommand_PlayFar(sound_id);
}

struct FieldActor *Object_GetById(s32 object_id)
{
	struct FieldActor *object = ObjectTable_Get(object_id);

	if (object == NULL)
		return NULL;
	return object;
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->acceleration = acceleration;
		object->speed_limit = speed_limit;
	}
}

void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		s32 value = 1;
		value |= object->action_flags;
		object->action_flags = value;
		ObjectMotion_SetActionCallback(object, action);
	}
}

void ObjectMotion_EnableActionAndResetMotion(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);
	u8					  enabled;

	if (object != NULL)
	{
		enabled = 1;
		object->action_flags = enabled | object->action_flags;
		ObjectDispatch_InitFromTable6Far(object);
	}
}

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->linked_object = (struct ObjectRuntime *)Object_GetById(linked_object_id);
		ObjectMotion_SetActionCallback(object, (s32)ObjectMotion_LinkedActionScript);
	}
}

void Object_RefreshSelectorById(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_WaitForValue16Far(object);
}

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		s32 value = 1;
		value |= object->action_flags;
		object->action_flags = value;
		ObjectMotion_SetActionCallback(object, action);
		ObjectDispatch_WaitForValue16Far(object);
	}
}

void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
	}
}

void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
		Script_WaitForEventTimeoutFar(object);
	}
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
	}
}

void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset)
{
	struct ObjectRuntime *object;
	s16					  current_angle;
	s32					  snapped_angle;
	s16					  angle_remainder;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		current_angle = ((union FieldCoordinate *)&object->x)->part.pixel;
		snapped_angle = current_angle;
		if (current_angle < 0)
		{
			snapped_angle += 15;
		}
		snapped_angle >>= 4;
		snapped_angle *= 16;
		angle_remainder = current_angle - snapped_angle;
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(
			object, object->x + ((8 - angle_remainder) << 16), object->y, object->z);
		Script_WaitForEventTimeoutFar(object);
		ObjectMotion_SetActionVariant(object_id, action);
		Object_SetMoveTargetFar(object, object->x, object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(
			object, object->x + (x_offset << 16), object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(
			object, object->x + (x_offset << 16), object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
	if (object != NULL)
	{
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
	struct ObjectRuntime *object = ObjectTable_Get(arg0);

	if (object != NULL)
	{
		struct ObjectRuntime *other;

		ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
		other = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
		if (other != NULL)
			ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
		object->movement_state = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)object, (const s32 *)&ObjectMotion_LaunchScript);
		object->action = arg3;
	}
}

void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object;
	s32					  terrain_height;
	s32					  tile_x;
	s32					  tile_z;
	s32					  terrain_id;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		ObjectDispatch_InitFromTable6Far(object);
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->target_y = ACTOR_NO_TARGET;
		object->target_x = ACTOR_NO_TARGET;
		object->x = x;
		object->z = z;
		if (1 & object->flags)
		{
			terrain_id = object->terrain_id;
			tile_x = x;
			if (tile_x < 0)
			{
				tile_x += 0xFFFF;
			}
			tile_x >>= 0x10;
			tile_z = z;
			if (tile_z < 0)
			{
				tile_z += 0xFFFF;
			}
			tile_z >>= 0x10;
			terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z) << 0x10;
			object->y = (object->y - object->terrain_height) + terrain_height;
			object->terrain_height = terrain_height;
		}
	}
}

void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
	struct ObjectRuntime *object;
	s32					  terrain_height;
	s32					  tile_x;
	s32					  tile_z;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		ObjectDispatch_InitFromTable6Far(object);
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->target_y = ACTOR_NO_TARGET;
		object->target_x = ACTOR_NO_TARGET;
		object->x = x;
		object->y = y;
		object->z = z;
		if (1 & object->flags)
		{
			s32 terrain_id = object->terrain_id;
			tile_x = x;
			if (tile_x < 0)
			{
				tile_x += 0xFFFF;
			}
			tile_x >>= 0x10;
			tile_z = z;
			if (tile_z < 0)
			{
				tile_z += 0xFFFF;
			}
			terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
			object->y = (object->y - object->terrain_height) + terrain_height;
			object->terrain_height = terrain_height;
		}
	}
}

void Object_SetModeById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_ApplyArgumentToChildrenFar(object, action);
}

void Object_SetActionById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, action);
}

void ObjectMotion_WaitForAnimationChange(u32 object_id)
{
	struct ObjectRuntime   *object;
	struct AnimationObject *sprite;
	volatile s32			saved;
	s32						i;

	object = ObjectTable_Get(object_id);
	if (object != NULL && object->animation_kind == 1)
	{
		sprite = object->animation;
		saved = sprite->last_no;
		for (i = 0; i <= 89; i++)
		{
			WaitFrames(1);
			if (saved != sprite->last_no)
			{
				break;
			}
		}
	}
}

void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action)
{
	Object_SetModeById(object_id, action);
	ObjectMotion_WaitForAnimationChange(object_id);
}

void ObjectMotion_NoOp(void)
{
}

void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id)
{
	struct ObjectRuntime *object;
	u8					 *object_flags;
	s32					  updated_flags;
	s32					  vertical_velocity;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		object_flags = &object->flags;
		updated_flags = *object_flags | 2;
		vertical_velocity = speed << 16;
		*object_flags = updated_flags;
		object->velocity_y = vertical_velocity;
		if (speed > 5)
		{
			AudioCommand_PlayFar(0x99);
		}
		else
		{
			AudioCommand_PlayFar(0x98);
		}
		Battle_WaitMode0(event_id);
	}
}

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
	struct ObjectRuntime *object;

	object = ObjectTable_Get(object_id);
	if (object != NULL && variant > 0)
	{
		if (variant > 3)
		{
			variant = 3;
		}
		ObjectDispatch_InitializeFar((struct DispatchObject *)object,
			(const s32 *)(ObjectMotion_VariantScripts + ((3 - variant) << 7)));
	}
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
	ObjectMotion_SetVariantCallback(object_id, variant);
	Object_RefreshSelectorById(object_id);
}

void BattleFx_UpdateParticleLinearMotion(struct FieldActor *particle)
{
	s32 velocity_x;
	s32 x;
	s32 velocity_z;
	s32 z;
	s32 velocity_y;
	s32 vz;
	velocity_x = particle->speed;
	x = particle->x.fixed + velocity_x;
	particle->x.fixed = x;
	particle->target_x = x;
	velocity_z = particle->acceleration;
	z = particle->z.fixed + velocity_z;
	particle->z.fixed = z;
	particle->target_z = z;
	velocity_y = particle->y.fixed + 0x400;
	particle->y.fixed = velocity_y;
	particle->target_y = velocity_y;
	particle->speed = (velocity_x - velocity_x / 0x12);

	vz = velocity_z;
	if (velocity_z < 0)
	{
		vz += 0xF;
	}
	particle->acceleration = (velocity_z - (vz >> 4));
}

void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 optional)
{
	struct FieldActor  *object;
	struct FieldSprite *child;
	s32					value;

	object = (struct FieldActor *)Object_CreateFar(
		222, source->x.fixed, source->y.fixed, source->z.fixed);
	if (object != 0)
	{
		child = object->sprite;
		switch (Random16() & 1)
		{
		case 1:
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptA);
			break;
		default:
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptB);
			break;
		}

		if (optional != 0)
			ObjectGroup_SetChildValue(object, optional);

		object->motion_flags = 0;
		value = Random16() % 10 + 5;
		object->acceleration = -0x1999 * value;
		value = Random16() % 15 - 7;
		value <<= 1;
		object->speed = 0x1999 * value;
		object->timer = 0;
		object->update = BattleFx_UpdateParticleLinearMotion;
		child->flags = 0;
		child->priority = source->sprite->priority;
	}
}

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
	s32					  next_y;
	s32					  base_z;
	struct ObjectRuntime *object;
	u8					 *object_flags;

	object = (struct ObjectRuntime *)Object_GetById(sequence_arg);
	base_z = object->z;
	if (object != 0)
	{
		AudioCommand_PlayFar(0x121);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, mode_or_frame);
		WaitFrames(10);
		object_flags = &object->flags;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		{
			u8 flags = 2;
			flags |= *object_flags;
			*object_flags = flags;
		}
		object->velocity_y = 0x40000;
		Object_SetMoveTargetFar(object, object->x, object->y, base_z + 0xC0000);
		WaitFrames(6);
		AudioCommand_PlayFar(0xD9);
		mode_or_frame = 0;
		ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
		*object_flags = 0;
		do
		{
			next_y = object->y + 0xFFFE0000;
			object->y = next_y;
			object->target_y = next_y;
			WaitFrames(1);
			if ((optional_action != -1) && (mode_or_frame & 1))
			{
				BattleFx_SpawnBurstParticle((struct FieldActor *)object, optional_action);
			}
			mode_or_frame++;
		} while ((u32)mode_or_frame <= 0xD);
		*object_flags = 3;
		object->velocity_y = 0x30000;
		Object_SetMoveTargetFar(object, object->x, object->y, base_z + 0x100000);
		Script_WaitForEventTimeoutFar(object);
		mode_or_frame = 0;
		if (object->y > object->terrain_height)
		{
		wait_for_target_y:
			WaitFrames(1);
			mode_or_frame++;
			if ((u32)mode_or_frame <= 0xB3)
			{
				if (object->y > object->terrain_height)
				{
					goto wait_for_target_y;
				}
			}
		}
		WaitFrames(2);
		BattleFx_PlayQueuedSound();
	}
}

void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
	struct ObjectRuntime *first;
	struct ObjectRuntime *second;

	first = ObjectTable_Get(first_id);
	second = ObjectTable_Get(second_id);
	if (first != 0 && second != 0)
	{
		first->angle = ArcTan2(second->z - first->z, second->x - first->x);
		Battle_WaitMode0(wait);
	}
}

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
	void *first = ObjectTable_Get(first_id);
	void *second = ObjectTable_Get(second_id);

	if (first != NULL && second != NULL)
	{
		FacingObject_TurnPairToFaceEachOther(first, second);
		Battle_WaitMode0(wait);
	}
}

void FacingObject_TurnPairToFaceEachOther(
	struct FacingObject *raw_first, struct FacingObject *raw_second)
{
	struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
	struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
	s32					  toward;
	s32					  away;
	s32					  frame;
	s32					  turning;
	s32					  delta;

	if (first == NULL || second == NULL)
		return;
	toward = (u16)ArcTan2(second->z - first->z, second->x - first->x);
	away = toward + 0x8000;
	for (frame = 0; frame < 60; frame++)
	{
		turning = 2;
		delta = (s16)(toward - first->angle);
		if (delta != 0)
		{
			if (delta > 0x1000)
				delta = 0x1000;
			if (delta < -0x1000)
				delta = -0x1000;
			first->angle += delta;
		}
		else
		{
			turning = 1;
		}
		delta = (s16)(away - second->angle);
		if (delta != 0)
		{
			if (delta > 0x1000)
				delta = 0x1000;
			if (delta < -0x1000)
				delta = -0x1000;
			second->angle += delta;
		}
		else
		{
			turning--;
		}
		if (turning == 0)
			break;
		WaitFrames(1);
	}
}

void ObjectTable_DestroyById(s32 index)
{
	void				   *object = ObjectTable_Get(index);
	struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;

	if (object != 0)
	{
		ObjectDispatch_ReleaseFar(object);
		table->slots[index] = NULL;
	}
}

void ObjectTable_DestroyNoOp(void)
{
}

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
	s32					   flags;
	struct DispatchObject *object;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		flags = 0x100 & value;
		if (flags != 0)
		{
			((struct ScriptMotionObject *)object)->hook =
				(void (*)(struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
			return;
		}
		((struct ScriptMotionObject *)object)->hook = NULL;
		ObjectGroup_SetChildValue((struct FieldActor *)object, value);
	}
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
	if ((object->kind & 0xf) == 1)
	{
		u8						child_value;
		struct AnimationObject *container;
		u8						child_count;

		child_value = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
		container = object->target.child;
		child_count = container->count;
		if (child_count != 0)
		{
			struct AnimationEntry **entries = container->entries;
			s32						remaining = child_count;
			do
			{
				struct AnimationEntry *entry = *entries++;
				if (entry != 0 && entry->script != 0)
				{
					entry->param = child_value;
				}
				remaining--;
			} while (remaining != 0);
		}
		container->dirty = 1;
	}
}

void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 value)
{
	struct DispatchObject *object = (struct DispatchObject *)actor;

	if ((object->kind & 0xf) == 1)
	{
		struct AnimationObject *container = object->target.child;
		u8						raw_count = container->count;

		if (raw_count != 0)
		{
			struct AnimationEntry **entry = container->entries;
			u32						count = raw_count;
			do
			{
				struct AnimationEntry *item = *entry++;
				if (item != NULL && item->script != 0)
				{
					item->param = value;
				}
				count--;
			} while (count != 0);
		}
		container->dirty = 1;
	}
}

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
	struct ObjectRuntime *first = ObjectTable_Get(object_id);
	struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

	if (first != NULL && second != NULL)
	{
		first->linked_object = second;
		if (!(target_id & 0x10000))
		{
			first->action = 40;
			first->acceleration = second->acceleration * 2;
			first->speed_limit = second->speed_limit;
			first->unknown_59 = 0;
		}
		ObjectDispatch_InitializeFar((struct DispatchObject *)first, callback);
	}
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
	s32 delta = 0;

	if (object != NULL)
	{
		s32 target_angle = (u16)object->action;
		s32 current_angle = object->angle;
		delta = (s16)(target_angle - current_angle);
		if (delta != 0)
		{
			if (delta > 4096)
				delta = 2048;
			if (delta < -4096)
				delta = -2048;
			object->angle = current_angle + delta;
		}
	}
	return delta;
}

void Object_ResetTargetAndSetMode1(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->target_x = ACTOR_NO_TARGET;
		object->target_y = ACTOR_NO_TARGET;
		object->target_z = ACTOR_NO_TARGET;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->action = angle;
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)object, (const s32 *)ObjectMotion_StepAngleScript);
		Battle_WaitMode0(wait);
	}
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL && (object->animation_kind & 0xF) == 1)
	{
		struct FieldSprite *sprite = object->animation;

		sprite->priority = priority;
		sprite->second_priority = priority;
		object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
	void *p;
	u8	  flags;
	u32	  shape;

	p = Object_GetById(source_id);
	p = ((struct ObjectRuntime *)p)->animation;
	flags = ((struct FieldSprite *)p)->vram_block;
	shape = ((struct FieldSprite *)p)->tile;

	p = Object_GetById(target_id);
	p = ((struct ObjectRuntime *)p)->animation;
	((struct FieldSprite *)p)->vram_block = flags;
	((struct FieldSprite *)p)->tile = shape;
}

void ObjectVisual_ReservedNoOp(void)
{
}

void Event_SetMessageId(s32 value)
{
	((struct EventRuntime *)gEventWork)->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
	struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
	s32					  result = -1;
	struct ObjectRuntime *entry = state->actors[key & 0x0fff];

	if (entry != 0 && entry->animation_kind == 1)
		result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
	return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
	struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
	s32					  result = -1;
	s32					  index = 8;
	struct ObjectRuntime *object = state->actors[index];

	if (object != 0 && object->animation_kind == 1
		&& ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value)
	{
		result = index;
	}
	else
	{
	next:
		index++;
		if (index <= 65)
		{
			object = state->actors[index];
			if (object == 0 || object->animation_kind != 1
				|| ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
				goto next;
			result = index;
		}
	}
	return result;
}

struct UiWindow *UiText_OpenMessageAtObject(s32 arg, s32 unused)
{
	struct UiRenderWork	 *win;
	struct EventRuntime	 *work;
	s32					  speaker;
	s32					  extra;
	s32					  tail;
	s32					  margin;
	s32					  height;
	s32					  width;
	s32					  top;
	s32					  left;
	s32					  pos[3];
	struct UiWindow		 *handle;
	s32					  side;
	s32					  flags;
	s32					  message;
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  ret;
	s32					  column;
#if !EDITION_INTERNATIONAL
	s32 none;
#endif

	win = (struct UiRenderWork *)gWindowWork[0];
	work = ((struct WorkPointers *)gWindowWork)->event;
	handle = 0;
	side = 0;
	speaker = ObjectTable_ReadActiveValue(arg);
	extra = 0;
	tail = 0;
	margin = 4;
	flags = arg & 0xf000;
	message = work->message;
	arg &= 0xfff;
	x = 0;
	object = ObjectTable_Get(arg);
	work->speaker = arg;
	y = 0;
	ret = 0;
	if (work->message_busy == 0)
	{
		if (object != 0)
		{
			if (work->mode_19e == 3)
			{
				Render_ProjectPoint(&object->x, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				ret = 1;
				y -= 2;
			}
			else
			{
				if (Object_GetScreenPosition(arg, pos) != -1)
					ret = 1;
				else
					ret = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}
		else if (arg <= 7)
		{
			speaker = arg;
			object = ObjectTable_Get(gGameState.selected_actor);
			if (work->mode_19e == 3)
			{
				Render_ProjectPoint(&object->x, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				ret = 1;
			}
			else
			{
				if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1)
					ret = 1;
				else
					ret = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}
		if (!ret)
		{
			left = 15;
			top = 10;
		}
		else
		{
			left = 0;
			top = 0;
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			left = x - width / 2;
			if (flags & 0x4000)
				top = y - height - 1;
			else if (!(flags & 0x8000) && y <= 8)
				top = y - height - 1;
			else
				top = y + 4;
		}
		if (win->mode != 0)
			margin = 5;
		column = x;
		if (flags & 0x1000)
		{
			column = x - margin - 2;
			if (column < 0)
				column = 0;
		}
		else if (flags & 0x2000)
		{
			column += 2;
			if (column + margin > 29)
				column = 29 - margin;
		}
		else if (column <= 15)
		{
			column = column - margin - 2;
			if (column < 0)
				column = x + 2;
		}
		else
		{
			column += 2;
			if (column + margin > 29)
				column = x - margin - 2;
		}
#if EDITION_INTERNATIONAL
		ret = Localization_LookupEntryIdFar(speaker);
		if (ret != -1)
		{
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = -1;
			if (top > y)
				tail = top - 5;
			else
				tail = top + height;
			if (tail < 0)
				tail = top + height;
			else if (tail + 5 > 19)
				tail = top - 5;
			if (top < tail)
			{
				s32 lines = height;

				UiText_GetResourceDimensionsFar(
					message, &left, &top, (u32 *)&width, (u32 *)&height);
				message = -1;
				extra = lines - height + 1;
			}
		}
		else if (top < y)
		{
			s32 lines = height;

			UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			extra = lines - height + 1;
			message = ret;
		}
#else
		none = Localization_LookupEntryIdFar(speaker);
		ret = -1;
		if (none != ret)
		{
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = ret;
			if (top > y)
				tail = top - 5;
			else
				tail = top + height;
			if (tail < 0)
				tail = top + height;
			else if (tail + 5 > 19)
				tail = top - 5;
		}
		if (top < y)
		{
			s32 lines = height;

			UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = -1;
			extra = lines - height + 1;
		}
#endif
		if (column < 0)
			column = 0;
		else if (column + margin > 29)
			column = 29 - margin;
		if (win->mode != 0)
		{
			WaitFrames(8);
			if (extra != 0)
				handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
			else
				handle = UiText_OpenMessageWindowFar(message, left, top, 2);
		}
		else
		{
			s32 icon = BattleFx_GetResourceId(speaker);

			if (extra != 0)
				handle =
					UiText_OpenMessageWindowFar(message, left, top + extra - 1, (icon << 16) | 17);
			else
				handle = UiText_OpenMessageWindowFar(message, left, top, (icon << 16) | 1);
		}
		if (win->mode != 0)
			side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
		else
			side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
		while (!UiWork_IsCompleteFar())
			WaitFrames(1);
	}
	work->message_window = (s32)handle;
	work->side_window = side;
	work->message++;
	return handle;
}

void BattleEv_RunWait(s32 action, s32 flag)
{
	s32					 masked_action;
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;
	struct UiWindow		*window = UiText_OpenMessageAtObject(action, flag);
	s32					 message_id;
	u32					 frames;

	frames = 0;
	WaitFrames(1);
	message_id = ObjectTable_ReadActiveValue(action);
	if (action <= 7)
	{
		masked_action = action & 0x0fff;

		if (BattleAction_FindDescriptor(masked_action) == 0)
		{
			message_id = masked_action;
		}
	}
	UiWork_FinalizeEntityMatchingLocalizedIdFar(message_id);

	if (runtime->message_busy == 0)
	{
		while (UiWork_IsIdleFar(window) == 0)
		{
			WaitFrames(1);
			frames++;
			if (frames > 600
				|| ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) && (gKeysHeld & KEY_L)
					&& (gKeysHeld & KEY_A)))
			{
				UiWork_FinalizePendingCoreFar();
			}
		}
	}

	WaitFrames(1);
}

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
	BattleEv_RunWait(first, second);
	Battle_WaitMode0(value);
}

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
	s32					 result;
	struct EventRuntime *runtime;
	struct GameState	*save;

	UiText_OpenMessageAtObject(object_id, action_id);
	save = &gGameState;
	result = Inventory_PromptAndSetObjectMode(save->selected_actor, 0);
	if (result == 0)
	{
		BattleEv_RunWait(object_id, action_id);
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
	}
	else
	{
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
		BattleEv_RunWait(object_id, action_id);
	}
	return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}

void Event_ShowSpeakerMessage(s32 speaker)
{
	// 位置は未初期化のまま読まれる (original bug: the position is never set)
	struct
	{
		s32 x, y;
	} pos;
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;

	speaker &= 0xfff;
	ObjectTable_Get(speaker);
	state->speaker = speaker;
	if (state->message_busy == 0)
	{
		s32 px = pos.x;
		s32 py = pos.y;
		s32 win;

		if (py > 119)
			py += 32;
		else
			py -= 32;
		px = px < 8 ? 8 : px;
		if (px > 312)
			px = 312;
		py = py < 20 ? 20 : py;
		if (py > 220)
			py = 220;
		win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
		state->message_window = win;
		while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
			WaitFrames(1);
	}
	state->message++;
}

void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y)
{
	Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	s32					 py = y;
	s32					 px = x;
	s32					 min_x = 8;
	s32					 min_y = 20;
	s32					 ret;

	if (py > 119)
		py += 32;
	else
		py -= 32;

	if (x < min_x)
		px = min_x;
	if (px > 312)
		px = 312;
	if (py < min_y)
		py = min_y;
	if (py > 220)
		py = 220;

	ret = UiText_OpenMessageWindowFar(state->message, px, py, 1);
	while (UiWork_IsIdleFar((struct UiWindow *)ret) == 0)
		WaitFrames(1);
	state->message++;
}

//--------------------------------------------------------------------------
// table_run_if_active.c
// フィールド：話し手の表示の後しまつ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// アクターに絵があれば、その絵番号を話し手としてウィンドウの表示を終える
// id : アクター番号
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id)
{
	s32 no;

	no = ObjectTable_ReadActiveValue(id);
	if (no != -1)
		UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}

void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 first_x, s32 first_y, s32 first_arg,
	s32 first_extra, s32 second, s32 second_x, s32 second_y, s32 second_arg, s32 second_extra)
{
	struct EventRuntime *rt = (struct EventRuntime *)gEventWork;
	s32					 id0 = ObjectTable_ReadActiveValue(first);
	s32					 id1 = ObjectTable_ReadActiveValue(second);
	s32					 h0;
	s32					 h1;

	h0 = UiText_OpenMessageWindowFar(
		rt->message++, first_x, first_y, BattleFx_GetResourceId(id0) << 16);
	UiWindow_CreateWithSideObjectFar(id0, 0, first_arg, first_extra);

	h1 = UiText_OpenMessageWindowFar(
		rt->message++, second_x, second_y, BattleFx_GetResourceId(id1) << 16);
	UiWindow_CreateWithSideObjectFar(id1, 0, second_arg, second_extra);

	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);

	WaitFrames(1);
	while ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
		WaitFrames(1);

	WaitFrames(1);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id0);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id1);
	UiWork_FinalizePendingCoreFar();
	WaitFrames(1);

	while (UiWork_IsIdleFar((struct UiWindow *)h0) == 0)
		WaitFrames(1);
	while (UiWork_IsIdleFar((struct UiWindow *)h1) == 0)
		WaitFrames(1);

	WaitFrames(1);
}

void Ui_SetRenderResultFromObject(s32 arg0)
{
	struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
	u8					 value;
	s32					 ret;

	if (arg0 == 0x80000000)
	{
		*(s16 *)&state->result[0] = (value = 0);
		*(s16 *)&state->result[1] = value;
	}
	else
	{
		ret = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(arg0));
		value = Ui_RenderResultValues[gGameState.unknown_207[5]];
		*(s16 *)&state->result[0] = ret;
		*(s16 *)&state->result[1] = value;
	}
}

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
	struct ObjectRuntime *obj;
	struct ObjectRuntime *target;
	struct EventWork	 *work;
	struct MapScrollWork *p;

	obj = ObjectTable_Get(id);
	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	target = (struct ObjectRuntime *)work->view_center;
	p = gMapWork[0];
	if (obj != 0)
	{
		p->origin = &target->x;
		ObjectDispatch_InitFromTable4WithArgumentFar(target, obj);
		if (flag == 0)
		{
			target->x = obj->x;
			target->y = obj->y;
			target->z = obj->z;
			WaitFrames(1);
			if (((struct EventRuntime *)work)->mode_19e != 3)
			{
				Map_ApplyWorkOriginAndSpanFar();
			}
		}
	}
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
	struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
	((struct ObjectRuntime *)owner->view_center)->speed_limit = first;
	((struct ObjectRuntime *)owner->view_center)->acceleration = second;
}

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
	s32					  should_use_setter;
	struct EventWork	 *runtime_block;
	s32					  minimum_x;
	s32					  minimum_z;
	s32					  maximum_x;
	s32					  object_z_offset;
	s32					  maximum_z;
	s32					  position_x;
	s32					  position_z;
	s32					  position_y;
	struct MapScrollWork *camera_state;
	struct ObjectRuntime *object;

	position_x = requested_x;
	position_y = requested_y;
	should_use_setter = use_setter;
	position_z = requested_z;
	runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
	object = (struct ObjectRuntime *)runtime_block->view_center;
	camera_state = gMapWork[0];
	minimum_x = camera_state->min_x + 0x780000;
	object_z_offset = object->y;
	minimum_z = camera_state->min_y + object_z_offset + 0x600000;
	maximum_x = camera_state->max_x + 0xFF880000;
	maximum_z = camera_state->max_y + object_z_offset + 0xFFC00000;
	camera_state->origin = &object->x;
	ObjectDispatch_InitFromTable6Far(object);
	if (position_x == -1)
	{
		position_x = object->x;
	}
	if (position_y == -1)
	{
		position_y = object->y;
	}
	if (position_z == -1)
	{
		position_z = object->z;
	}
	if (position_x < minimum_x)
	{
		position_x = minimum_x;
	}
	if (position_z < minimum_z)
	{
		position_z = minimum_z;
	}
	if (position_x > maximum_x)
	{
		position_x = maximum_x;
	}
	if (position_z > maximum_z)
	{
		position_z = maximum_z;
	}
	if (should_use_setter == 0)
	{
		object->x = position_x;
		object->y = position_y;
		object->z = position_z;
		WaitFrames(1U);
		if (((struct EventRuntime *)runtime_block)->mode_19e != 3)
		{
			Map_ApplyWorkOriginAndSpanFar();
		}
	}
	else
	{
		Object_SetMoveTargetFar(object, position_x, position_y, position_z);
	}
}

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1)
{
	struct ObjectRuntime *obj;

	obj = ObjectTable_Get(arg0);
	Runtime_AllocateBlock(0x1B, 0xCCC);
	if (obj != NULL)
	{
		Motion_CamBounds(obj->x, -1, obj->z, arg1);
	}
}

void BattleFx_CommitObjectPositionAndWait(void)
{
	Script_WaitForEventTimeoutFar(
		((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center);
	Battle_WaitMode0(2);
}

struct FieldActor *Battle_GetWorkObject1e0(void)
{
	return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
}

void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target, s32 keep_current_position)
{
	struct ObjectRuntime *object;

	object = (struct ObjectRuntime *)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))
				 ->view_center;
	if (target != NULL)
	{
		ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
		object->linked_object = target;
		if (keep_current_position == 0)
		{
			object->x = target->x;
			object->y = target->y;
			object->z = target->z;
		}
	}
}

//--------------------------------------------------------------------------
// mapfx.c
// マップの表示範囲とズームの移り変わり、ほかのオブジェクトについて回るオブジェクト
//--------------------------------------------------------------------------

// スプライトの OAM 属性2 の上位バイト (bit 2〜3 が表示優先度)
#define ATTR2_HI(spr) (((u8 *)&((struct AnimationObject *)(spr))->part[0])[9])

//--------------------------------------------------------------------------
// マップの表示範囲を決める
//--------------------------------------------------------------------------
void Map_SetViewBounds(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
	struct MapScrollWork *work = gMapWork[0];

	work->min_x = min_x;
	work->min_y = min_y;
	work->max_x = max_x;
	work->max_y = max_y;
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : ズームを前の値から目標へ 1 段すすめる
// 画面中心のオブジェクトが止まっているあいだは待つ
//--------------------------------------------------------------------------
s32 BattleFx_StepRatioTransition(void)
{
	struct PerspectiveWork *work;
	s16					   *frames;
	s32					   *from;
	s16					   *step;
	s32						zoom;
	s32						delta;

	work = gMapWork[0];
	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->view_center->paused
		!= 0)
		return;
	frames = (s16 *)&work->zoom_transition_frames;
	if (*frames == 0)
		return;
	from = &work->previous_zoom;
	delta = work->zoom - *from;
	step = &work->zoom_transition_step;
	(*step)++;
	zoom = *from + __divsi3(delta * *step, *frames);
	work->distance = Iwram_MulQ16(work->far_plane, zoom);
	gScanlinePitch = work->pitch + 1;
	if (*step == *frames)
	{
		*frames = 0;
		Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
	}
}

//--------------------------------------------------------------------------
// ズームの移り変わりを始める (イベントの mode が 3 のときだけ)
// zoom   : 倍率 (16.16 固定小数)
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames)
{
	struct PerspectiveWork *work = gMapWork[0];
	s32						inv;

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode != 3)
		return;
	{
		inv = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
	}
	work->previous_zoom = work->zoom;
	work->zoom = inv;
	work->zoom_transition_frames = frames;
	work->zoom_transition_step = 0;
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition, 0xc94);
}

//--------------------------------------------------------------------------
// ズームの移り変わりが終わるまで待つ (イベントの mode が 3 のとき、最長 300 フレーム)
//--------------------------------------------------------------------------
void Event_WaitForZoomTransition(void)
{
	s32						frames;
	struct PerspectiveWork *work = gMapWork[0];

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode == 3)
	{
		frames = 0;
		if (*(s16 *)&work->zoom_transition_frames != 0)
		{
			do
			{
				WaitFrames(1);
				frames++;
			} while (frames < 300 && *(s16 *)&work->zoom_transition_frames != 0);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 付いていく相手の位置に置き、y には絵の box_x と 8 ドットを足す
//--------------------------------------------------------------------------
s32 BattleFx_CopyLinkedObjectPosition(void *self)
{
	struct FieldActor *obj = self;
	struct FieldActor *link;

	link = obj->linked_object;
	if (link != NULL)
	{
		obj->motion_flags = 0;
		obj->x.fixed = link->x.fixed;
		obj->y.fixed = link->y.fixed
					   + ((s8)Resource_GetMetadataRecordFar(
							  (s16)BattleAction_FindDescriptor(*(s16 *)&obj->work)->sprite)
							   ->box_x
						   << 16)
					   + (8 << 16);
		obj->terrain_height = link->terrain_height;
		obj->z.fixed = link->z.fixed;
	}
	return 0;
}

//--------------------------------------------------------------------------
// id のオブジェクトについて回るオブジェクト (種類 21) を出し、wait フレーム待つ
// mode : 下位 4 ビットはオブジェクトの mode、下位 8 ビットが 6 なら音 110、
//        0x100 で表示優先度 1 (なければ相手と同じ優先度)
//--------------------------------------------------------------------------
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait)
{
	struct FieldActor *target;

	if ((mode & 0xff) == 6)
	{
		AudioCommand_PlayFar(110);
	}

	target = ObjectTable_Get(id);
	if (target != 0)
	{
		struct FieldActor *obj =
			Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_LinkedObjectScript);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, mode & 15);
			obj->motion_flags = 0;
			obj->timer = 0;
			obj->work = id;
			obj->update = (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
			obj->sprite->flags = 0;
			obj->linked_object = target;

			if ((mode & 0x100) != 0)
			{
				s32 mask = 13;
				u8	attr = ATTR2_HI(obj->sprite);

				mask = -mask;
				mask &= attr;
				mask |= 4;
				ATTR2_HI(obj->sprite) = mask;
			}
			else
			{
				s32 pri = 12;
				u8	src = ATTR2_HI(target->sprite);
				u8	dst;
				s32 mask = 13;

				pri &= src;
				dst = ATTR2_HI(obj->sprite);
				mask = -mask;
				mask &= dst;
				mask |= pri;
				ATTR2_HI(obj->sprite) = mask;
			}
		}
		Battle_WaitMode0(wait);
	}
}

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags)
{
	struct FieldActor  *object = ObjectTable_Get(id);
	struct FieldActor  *child;
	struct FieldSprite *visual;
	s32					mode;

	child = 0;
	visual = 0;

	if (object == 0)
		return;

	if ((flags & 3) != 0)
	{
		if ((flags & 3) == 2 || object->linked_object == 0)
		{
			child = Object_CreateFar(209, object->x.fixed, object->y.fixed, object->z.fixed);
		}
	}
	else
	{
		child = object->linked_object;
		if (child == 0)
			return;
		ObjectDispatch_ReleaseFar(child);
		object->linked_object = 0;
		return;
	}

	if (child == 0)
		return;

	mode = flags & 3;
	switch (mode)
	{
	case 1:
		ObjectDispatch_ApplyArgumentToChildrenFar(child, 1);
		object->linked_object = child;
		child->timer = 1;
		break;
	case 2:
		ObjectDispatch_ApplyArgumentToChildrenFar(child, 2);
		ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)ObjectMotion_WaitScript);
		child->timer = 1;
		break;
	}

	child->work = id;
	child->motion_flags = 0;
	child->update = BattleFx_CopyLinkedObjectPosition;
	visual = child->sprite;
	visual->flags = 0;
	child->linked_object = object;

	if (flags & 0x100)
	{
		s32 mask = 13;
		u8	visual_flags = ((u8 *)visual)[9];

		mask = -mask;
		mask &= visual_flags;
		mask |= 4;
		((u8 *)visual)[9] = mask;
	}
	else
	{
		s32 copied_flags = 12;
		u8	source_flags = ((u8 *)object->sprite)[9];
		u8	destination_flags;
		s32 clear_mask = 13;

		copied_flags &= source_flags;
		destination_flags = ((u8 *)visual)[9];
		clear_mask = -clear_mask;
		clear_mask &= destination_flags;
		clear_mask |= copied_flags;
		((u8 *)visual)[9] = clear_mask;
	}
}

s32 Object_ClearChildAndCollisionFlags(void *obj)
{
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	((struct ScriptObjectRuntime *)obj)->flags_59 = 0;
	return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object)
{
	s32					  step;
	struct ObjectRuntime *target;
	s32					  deltaX;
	s32					  deltaY;
	s32					  cellX;
	s32					  cellY;
	s32					  newX;
	s32					  distance;

	target = object->linked_object;
	if (target != 0)
	{
		deltaX = target->x - object->x;
		if (deltaX < 0)
			deltaX += 0xffff;
		cellX = deltaX >> 16;
		deltaY = target->z - object->z;
		if (deltaY < 0)
			deltaY += 0xffff;
		cellY = deltaY >> 16;
		distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
		step = object->action;
		if (distance >= step)
		{
			newX = object->x + (cellX << 20) / step;
			Object_SetMoveTargetFar(
				object, newX, object->y, object->z + __divsi3(cellY << 20, step));
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		}
		else
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		}
	}
	return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object)
{
	s16					  turn_step;
	u16					  current_angle;
	s32					  target_angle;
	struct ObjectRuntime *target;

	target = object->linked_object;
	if (target != NULL)
	{
		object->action_flags &= 0xfe;
		target_angle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
		current_angle = object->angle;
		turn_step = target_angle - current_angle;
		if (turn_step != 0)
		{
			if (turn_step > 0x1000)
				turn_step = 0x1000;
			if (turn_step < -0x1000)
				turn_step = -0x1000;
			object->angle = (current_angle + turn_step);
		}
	}
	return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
	switch ((kind - 1))
	{
	case 0:
		kind = (s32)ObjectMotion_ActionKind1Script;
		break;
	case 1:
		kind = (s32)ObjectMotion_ActionKind2Script;
		break;
	case 2:
		kind = (s32)ObjectMotion_ActionKind3Script;
		break;
	case 3:
		kind = (s32)ObjectMotion_ActionKind4Script;
		break;
	case 4:
		kind = (s32)ObjectMotion_MoveTowardTargetScript;
		break;
	case 5:
		object->linked_object = Object_GetById(gGameState.selected_actor);
		kind = (s32)ObjectMotion_TurnTowardLinkedScript;
		break;
	case 6:
		kind = (s32)ObjectMotion_ResetActionScript;
		break;
	default:
		break;
	}
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, kind);
}

struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id)
{
	struct ObjectRuntime *entry;
	struct ObjectRuntime *found;
	s32					  cnt;
	s32					  best;
	s32					  dy;
	s32					  dx;
	s32					  dz;
	s32					  dist;
	s32					  angle;
	s32					  turn;

	found = NULL;
	best = 40;
	entry = gObjectSlots;
	for (cnt = 0; cnt < 64; cnt++, entry++)
	{
		if (entry->script == NULL)
			continue;
		if (entry == self)
			continue;
		if (entry->animation_kind != 1)
			continue;
		dy = entry->y - self->y;
		if (dy >= 0)
		{
			if (dy > 0x2fffff)
				continue;
		}
		else
		{
			if (self->y - entry->y > 0x2fffff)
				continue;
		}
		dx = (entry->x - self->x) / 0x10000;
		dz = (entry->z - self->z) / 0x10000;
		dist = Iwram_Sqrt(dx * dx + dz * dz);
		if (dist >= best)
			continue;
		angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
		if (dist > 23)
		{
			turn = (s16)(angle - self->angle);
			if (turn < -0x2fff)
				continue;
			if (turn > 0x2fff)
				continue;
		}
		found = entry;
		best = dist;
	}
	if (found == NULL)
		return NULL;
	if (((struct AnimationObject *)found->animation)->entries[0]->anim_id != id)
		return NULL;
	return found;
}

#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_ABOVE ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

s32 Field_TryJumpForward(void)
{
	struct ObjectRuntime *leader = Object_GetById(gGameState.selected_actor);
	s32					  result = -1;
	s32					  angle = (leader->angle + 0x2000) & 0xc000;
	u8					 *flags = &leader->flags;
	u8					  saved = *flags;
	struct FieldStepWork *work = gEventWork;
	s32					  child_mode = 1;
	struct FieldPosition  position;
	struct FieldPosition *pos;
	struct ObjectRuntime *target;

	pos = &position;
again:
	pos->x = (leader->x & 0xfff00000) + 0x80000;
	pos->y = leader->y;
	pos->z = (leader->z & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x100000, angle, pos);
	if (Map_CheckObjectStepFar(leader, pos) == 1)
		return -1;
	pos->x = (leader->x & 0xfff00000) + 0x80000;
	pos->y = leader->y;
	pos->z = (leader->z & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x200000, angle, pos);
	if (Map_CheckObjectStepFar(leader, pos) != 0)
		goto done;
	if (leader->animation_kind == 1)
		child_mode = ((struct FieldSprite *)leader->animation)->flags;
	Battle_Reset();
	ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
	WaitFrames(6);
	AudioCommand_PlayFar(152);
	ObjectDispatch_ApplyArgumentToChildrenFar(leader, 7);
	leader->speed_limit = 0x30000;
	leader->acceleration = 0x20000;
	leader->velocity_y = 0x40000;
	*flags &= 0x7e;
	ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode & 0xfe);
	ObjectMotion_SetPositionAndCommit(gGameState.selected_actor,
		((union FieldCoordinate *)&pos->x)->part.pixel,
		((union FieldCoordinate *)&pos->z)->part.pixel);
	ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
	ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode);
	if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
		|| (target = Object_FindNearestFacingTarget(leader, 205)) != NULL)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(target, 7);
		leader->y -= 0x10000;
		leader->terrain_height -= 0x10000;
		WaitFrames(2);
		leader->y -= 0x10000;
		leader->terrain_height -= 0x10000;
		WaitFrames(10);
		leader->y += 0x10000;
		leader->terrain_height += 0x10000;
		WaitFrames(4);
		leader->y += 0x10000;
		leader->terrain_height += 0x10000;
	}
	else
	{
		WaitFrames(6);
	}
	*flags = saved;
	BattleFx_FinishAction();
	if (work != NULL)
		work->encounter_steps += Iwram_MulQ16(work->encounter_rate, 0x200000);
	if (GetMapCellCollisionFar(leader->terrain_id, pos->x, pos->z) == 0xf9)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
		WaitFrames(6);
		goto again;
	}
	result = 0;
done:
	return result;
}

s32 FieldEffect_UpdateGridPlacement(void)
{
	struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
	s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
	s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
	s32 result;

	Battle_Reset();

	if (gGameState.movement_mode == 0)
	{
		s32 index = grid_x / 16 + (grid_z / 16) * 128;

		if (TILE_CELLS[index].collision_code == TILE_CELLS_TARGET[index].collision_code)
		{
			/* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
			s32					  vector[6];
			struct FieldPosition *position = (struct FieldPosition *)vector;

			position->x = object->x;
			position->y = object->y - 0x100000;
			position->z = object->z;
			result = CheckMapPositionCellOccupiedFar((const s32 *)position);
			if (result != 0)
				goto failure;

			ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
			object->speed_limit = 0x10000;
			ObjectMotion_ArmCallback(gGameState.selected_actor, 0xc000, 0);
			Object_RefreshSelectorById(gGameState.selected_actor);
			object->action_flags = 1;
			object->flags = 0;
			ObjectDispatch_SetSingleChildFlagsFar(object, 0);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 13);
			Object_SetMoveTargetFar(
				object, grid_x << 16, object->y - 0x100000, (grid_z << 16) + 0x100000);
			ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
			gGameState.movement_mode = 1;
		}
		else
		{
			goto failure;
		}
	}
	else
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 10);
		object->flags = 3;
		object->velocity_y = 0x40000;
		object->terrain_height = object->y;
		ObjectDispatch_SetSingleChildFlagsFar(object, 1);
		Battle_WaitMode0(6);
		gGameState.movement_mode = 0;
		object->action_flags = 1;
		object->angle = 0xc000;
	}

	BattleFx_FinishAction();
	return 0;

failure:
	BattleFx_FinishAction();
	return -1;
}

s32 battle_owner_69(void)
{
	struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
	s16					  child_mode = 1;
	s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
	s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
	s32 result;

	Battle_Reset();

	if (object->animation_kind == 1)
		child_mode = ((struct FieldSprite *)object->animation)->flags;

	if (gGameState.movement_mode == 0)
	{
		s32 index = grid_x / 16 + (grid_z / 16) * 128;

		if (TILE_CELLS[index].collision_code == TILE_CELLS_ABOVE[index].collision_code)
		{
			/* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
			s32					  vector[6];
			struct FieldPosition *position = (struct FieldPosition *)vector;

			position->x = object->x;
			position->y = object->y;
			position->z = object->z;
			result = CheckMapPositionCellOccupiedFar((const s32 *)position);
			if (result != 0)
				goto failure;

			object->action_flags = 0;
			ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
			WaitFrames(4);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
			object->velocity_y = 0x40000;
			WaitFrames(4);
			object->flags = 0;
			child_mode &= 0xfe;
			ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
			object->speed_limit = 0x10000;
			object->velocity_y = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
			WaitFrames(4);
			gGameState.movement_mode = 1;
			object->action_flags = 1;
			WaitFrames(8);
		}
		else
		{
			goto failure;
		}
	}
	else
	{
		object->flags = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 11);
		Object_SetMoveTargetFar(
			object, grid_x << 16, object->y + 0x80000, (grid_z << 16) - 0x100000);
		ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
		object->flags = 3;
		child_mode |= 1;
		object->terrain_height = object->y;
		ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
		Battle_WaitMode0(4);
		gGameState.movement_mode = 0;
		object->action_flags = 1;
	}

	BattleFx_FinishAction();
	return 0;

failure:
	BattleFx_FinishAction();
	return -1;
}

s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);
	s32					 *camera;
	s32					  camera_x;
	s32					  camera_z;
	s32					  x;
	s32					  z;

	if (object == 0)
		return -1;
	/* FAKEMATCH: needed to match: the adjacent coordinate-word read avoids four bytes added by separate named bases. */
	camera = &((struct MapScrollWork *)gMapWork[0])->view_x;
	camera_x = camera[0] & 0xffff0000;
	camera_z = camera[1] & 0xffff0000;
	x = object->x - camera_x;
	z = object->z - camera_z - object->y;
	*position++ = x / 0x10000;
	*position = z / 0x10000;
	if ((object->animation_kind & 15) == 1)
		*position -= (s8)Resource_GetMetadataRecordFar(
			((struct AnimationObject *)object->animation)->entries[0]->anim_id)
						 ->box_x;
	return 0;
}

void ObjectEffect_ReservedNoOp941DC(void)
{
}

s32 Scene_FadeColorFromWhite(s32 unused)
{
	struct EventRuntime *work = gEventWork;
	s32					 i;
	s32					 c;

	AudioCommand_PlayFar(gGameState.scene_cue);
	AudioCommand_PlayFar(288);
	AudioCommand_PlayFar(147);
	if (work->mode_19e == 3)
	{
		Io_Write16(0x7fff, (u16 *)0x050001e6);
		DisplayTransition_Finish(0x401, 16);
		work->status_1c6 = 0;
		WaitFrames(16);
		for (i = 0; i < 16; i++)
		{
			c = 30 - i * 2;
			Io_Write16((c << 10) | (c << 5) | c, (u16 *)0x050001e6);
			WaitFrames(1);
		}
	}
	else
	{
		Io_Write16(0x7fff, &PLTT_BACKDROP);
		DisplayTransition_Finish(0x207, 16);
		work->status_1c6 = 0;
		WaitFrames(16);
		for (i = 0; i < 16; i++)
		{
			c = 30 - i * 2;
			Io_Write16((c << 10) | (c << 5) | c, &PLTT_BACKDROP);
			WaitFrames(1);
		}
	}
}

void ObjectEffect_PrepareContextEffect(s32 value)
{
	u32						zero;
	u8						kind;
	struct ObjectRuntime   *object;
	struct AnimationObject *context;
	struct AnimationEntry  *effect;

	object = ObjectTable_Get(gGameState.selected_actor);
	context = object->animation;
	effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
	zero = 0;
	kind = 15;

	context->flags = zero;
	effect->param = kind;
	object->x = (object->x & 0xFFF00000) + 0x80000;
	object->z = (object->z & 0xFFF00000) + 0x100000;
	object->velocity_x = zero;
	object->velocity_z = zero;
	object->target_x = ACTOR_NO_TARGET;
	object->target_z = ACTOR_NO_TARGET;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, value);
	WaitFrames(18);
}

void ObjectEffect_BeginContextEffect26(void)
{
	ObjectEffect_PrepareContextEffect(0x1A);
	GameFlag_SetBitFar(0x120);
}

void ObjectEffect_BeginContextEffect25(void)
{
	ObjectEffect_PrepareContextEffect(0x19);
	GameFlag_SetBitFar(0x121);
}

void ObjectEffect_EndContextEffect(s32 arg0)
{
	s32						zero;
	s32						mask;
	struct ObjectRuntime   *obj = ObjectTable_Get(gGameState.selected_actor);
	struct AnimationObject *ctx = obj->animation;
	struct AnimationEntry  *eff = (struct AnimationEntry *)ResourceMetadata_RegisterFar(ctx, 27);

	zero = 0;
	mask = 0xfff00000;
	ctx->flags = zero;
	eff->param = 15;
	obj->x = (obj->x & mask) + 0x80000;
	obj->z &= mask;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, arg0);
	WaitFrames(30);
	ctx->count = 1;
	ResourceMetadata_ClearRecordFar(ctx->entries[1]);
	ctx->entries[1] = (void *)zero;
	ctx->flags = 1;
	obj->acceleration = 0x10000;
	obj->speed_limit = 0x10000;
	Object_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
	Script_WaitForEventTimeoutFar(obj);
}

s32 ObjectEffect_RunPendingFlagEvent(void)
{
	s32 result = 0;
	s32 flag = 0x120;

	if (GameFlag_TestFar(flag) != 0)
	{
		ObjectEffect_EndContextEffect(24);
		GameFlag_ClearBitFar(flag);
		result = 1;
	}
	else
	{
		flag = 0x121;
		if (GameFlag_TestFar(flag) != 0)
		{
			ObjectEffect_EndContextEffect(23);
			GameFlag_ClearBitFar(flag);
			result = 2;
		}
		else
		{
			flag = 0x122;
			if (GameFlag_TestFar(flag) != 0)
			{
				s32					  id;
				struct ObjectRuntime *obj;

				GameFlag_ClearBitFar(flag);
				id = gGameState.selected_actor;
				obj = ObjectTable_Get(id);
				obj->y += 0x00a00000;
				Motion_CamBounds(-1, -1, -1, 0);
				while (obj->y + obj->velocity_y > obj->terrain_height)
				{
					WaitFrames(1);
				}
				AudioCommand_PlayFar(159);
				obj->y = obj->terrain_height;
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 22);
				Battle_WaitMode0(15);
				Object_AttachWorkTargetToObject(id, 1);
				result = 3;
			}
		}
	}
	return result;
}

void DisplayScroll_ArmHBlankDma(void)
{
	volatile u16			 *dma;
	struct DisplayScrollWork *state;
	u32						 *source;
	u16						  status;
	u32						  control;
	volatile u32			 *destination;

	state = gHBlankScrollWork;
	source = (u32 *)state->rows[state->page];

	dma = REG_DMA0;
	destination = (volatile u32 *)0x04000014;
	status = dma[5];
	control = status & (u16)~DMA_START_HBLANK_REPEAT;
	dma[5] = control;
	status = dma[5];
	control = status & (u16)~DMA_ENABLE;
	dma[5] = control;
	(void)dma[5];

	*destination = *source++;
	*destination = *source++;
	*destination = *source++;

	control = 0xa6600003;
	Dma_Set(source, destination, control, (volatile u32 *)dma);
}

void DisplayScroll_BuildAndSwapHBlankPage(void)
{
	struct DisplayScrollWork *work = gHBlankScrollWork;
	s32						  x3 = (s16)gBgScroll[3].x;
	s32						  y3 = (s16)gBgScroll[3].y;
	s32						  x2 = (s16)gBgScroll[2].x;
	s32						  y2 = (s16)gBgScroll[2].y;
	s32						  x1 = (s16)gBgScroll[1].x;
	s32						  y1 = (s16)gBgScroll[1].y;
	u16						 *row;
	s32						  step;
	s32						  phase;
	s32						  amp;
	s32						  i;
	u16						  d;

	row = &work->rows[work->page ^ 1][0][0].x;
	step = work->step_x;
	phase = (work->frame + (u16)y3) * work->freq_x;
	if (step == 0)
	{
		for (i = 0; i != 160; i++)
		{
			row[i * 6] = x3;
			row[i * 6 + 2] = x2;
			row[i * 6 + 4] = x1;
		}
	}
	else
	{
		u16 a = x3;
		u16 b = x2;
		u16 c = x1;

		amp = work->amp_x;
		for (i = 0; i != 160; i++)
		{
			d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
			*row = a + d;
			row += 2;
			*row = b + d;
			row += 2;
			*row = c + d;
			row += 2;
			phase += step;
		}
	}
	row = &work->rows[work->page ^ 1][0][0].y;
	step = work->step_y;
	phase = (work->frame + (u16)y3) * work->freq_y;
	if (step == 0)
	{
		for (i = 0; i != 160; i++)
		{
			row[i * 6] = y3;
			row[i * 6 + 2] = y2;
			row[i * 6 + 4] = y1;
		}
	}
	else
	{
		u16 a = y3;
		u16 b = y2;
		u16 c = y1;

		amp = work->amp_y;
		for (i = 0; i != 160; i++)
		{
			d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
			*row = a + d;
			row += 2;
			*row = b + d;
			row += 2;
			*row = c + d;
			row += 2;
			phase += step;
		}
	}
	work->frame++;
	work->page ^= 1;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../../graphics/display/scroll_initialize_hblank_dma.c"

void DisplayScroll_DisarmHBlankDma(void)
{
	volatile u16 *dma0;

	Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
	Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
	dma0 = REG_DMA0;
	dma0[5] = 0xC5FF & dma0[5];
	dma0[5] = 0x7FFF & dma0[5];
	(void)dma0[5];
}

struct GroundParticle
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 blend_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[2];
	s32 pos_x;
	s32 pos_y;
	s32 pos_z;
	s32 delay;
	u16 timer;
	u8	unknown_1e[2];
};

struct GroundParticleWork
{
	s32					  entry;
	s32					  tile_base;
	struct GroundParticle particles[32];
	s32					  unknown_408;
	s32					  stopped;
};

void FieldFx_UpdateGroundParticles(void)
{
	struct MapScrollWork	  *view = gMapWork[0];
	struct GroundParticleWork *work = gMapWork[21];
	s32						  *camera = &view->view_x;
	s32						   camera_x = camera[0];
	s32						   camera_z = camera[1];
	struct GroundParticle	  *particle = work->particles;
	u32						   i;

	for (i = 0; i < 32; i++, particle++)
	{
		s16 *frame;
		s32	 x;
		s32	 y;

		if (--particle->timer == 0xffff)
			continue;
		if (GameFlag_TestFar(0x166))
			particle->timer++;
		frame = &FieldFx_GroundParticleFrames[5 * particle->timer];
		x = (particle->pos_x - camera_x) / 0x10000 + *frame++;
		y = (particle->pos_z - particle->pos_y - camera_z) / 0x10000 + *frame++;
		if ((x >= -16 && x <= 239) && y >= -32 && y <= 159)
		{
			particle->priority = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = work->tile_base + *(u16 *)frame;
			frame++;
			particle->shape = *(u8 *)frame;
			particle->size = *(u8 *)(frame + 1);
			Runtime_PushSlotEntry(particle, 240);
		}
		if (particle->timer == 0)
		{
			s32 *leader = view->origin;

			x = leader[0] + (Random16() << 8) - 0x800000;
			y = leader[2] + (Random16() << 8) - 0x800000;
			particle->pos_x = x;
			particle->pos_z = y;
			particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
			particle->timer = 16;
		}
	}
}

void BattleFx_UpdateStormFlash(void)
{
	struct FieldBlendWork	   *work = gFieldBlendWork[0];
	struct BattleEffectBuffers *target = gFieldBlendWork[2];
	s16						   *timer = &work->timer;

	if (*timer < 0)
		return;
	if (GameFlag_TestFar(358))
		work->timer = 128;
	switch ((*timer)--)
	{
	case 0:
		if (work->enabled != 0)
		{
			u32 a = Random16();
			u32 b = Random16();
			work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
			if (work->loud != 0)
				AudioCommand_PlayFar(172);
			else
				AudioCommand_PlayFar(171);
		}
	case 5:
	case 10:
		BattleFx_ApplyColorToTargetBuffer(work, 1);
		Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
		target->duration = 12;
		target->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		BattleFx_ApplyColorToTargetBuffer(work->to, 1);
		target->duration = 1;
		target->step = 0;
		break;
	}
}

void FieldFx_StartGroundParticles(void)
{
	struct GroundParticleWork *work;
	s32						  *leader;
	struct GroundParticle	  *particle;
	u32						  *words;
	volatile u32			   fill;
	u32						   i;

	/* FAKEMATCH: needed to match: reusing the cursor preserves its register */
	work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
	leader = ((struct MapScrollWork *)gMapWork[0])->origin;
	BattleFx_SetQueuedSoundAndPlay(170);
	words = 0;
	particle = work->particles;
	fill = (u32)words;
	Dma_Set(
		(const void *)&fill, work, 0x85000000 | (sizeof(struct GroundParticleWork) / 4), REG_DMA3);
	words = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_GroundParticleTiles, words);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x300, words);
	Runtime_ReleaseHeapBlock(14);
	for (i = 0; i < 32; i++)
	{
		s32 x;
		s32 z;

		words = (u32 *)particle;
		*words++ = 0;
		*words++ = 0x40000400;
		*words = 0xd400;
		x = leader[0];
		z = leader[2];
		particle->pos_x = x;
		particle->pos_z = z;
		particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
		particle->timer = (i & 15) + 1;
		particle++;
	}
	Io_Write16(0x3f00, &REG_BLDCNT);
	Io_Write16(0x1008, &REG_BLDALPHA);
	Io_Write16(0, &REG_BLDY);
	Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

struct SparkleFrame
{
	s16 dy;
	u16 tile;
};

extern const struct SparkleFrame FieldFx_SparkleFrames[];

union DustWord
{
	u32	  value;
	void *link;
};

void FieldMotes_Update(void)
{
	struct GroundParticleWork *work = WORK_SLOT(29);
	struct MapScrollWork	  *map = WORK_SLOT(8);
	u32						   spawned = 0;
	u32						   i;
	struct GroundParticle	  *mote = work->particles;

	for (i = 0; i < 32; i++, mote++)
	{
		s32 x;
		s32 y;

		if (--mote->timer != 0xffff)
		{
			s32 *camera = &map->view_x;
			s32	 camera_x = camera[0];
			s32	 camera_z = camera[1];
			u16	 age = mote->timer;
			u32	 a;
			u32	 b;

			if (GameFlag_TestFar(0x166))
			{
				mote->timer++;
				mote->delay--;
			}
			a = Random16();
			b = Random16();
			x = ((mote->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
			y = ((mote->pos_z - mote->pos_y - camera_z) >> 16) - age;
			if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159)
			{
				if (mote->timer < 60)
				{
					mote->tile = work->tile_base + 16;
					mote->delay += 3;
				}
				else if (mote->timer < 90)
				{
					mote->tile = work->tile_base + 8;
					mote->delay++;
				}
				else
				{
					mote->tile = work->tile_base;
				}
				if ((gFrameCount >> 3) & 1)
					mote->tile += 4;
				mote->x = x;
				mote->y = y - (mote->delay >> 2);
				mote->shape = 0;
				mote->size = 1;
				Runtime_PushSlotEntry(mote, 240);
			}
			else
			{
				mote->timer = 0;
			}
		}
		if (spawned < 8 && mote->timer == 0)
		{
			s32 *leader = map->origin;

			x = leader[0] + (Random16() << 8) - 0x800000;
			y = leader[2] + (Random16() << 8) - 0x800000;
			mote->pos_z = y;
			mote->pos_x = x;
			mote->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
			mote->timer = 120;
			mote->delay = 0;
			spawned++;
		}
	}
}

void FieldMotes_Start(void)
{
	struct GroundParticleWork *work;
	struct GroundParticle	  *mote;
	u8						  *tiles;
	u32						   i;
	u32						   size;
	volatile u32			   zero;

	size = 0x410;
	work = Runtime_AllocateBlock(29, size);
	mote = work->particles;
	/* FAKEMATCH: needed to match: reusing the allocation size for the cleared word keeps it in r1. */
	size = 0;
	zero = size;
	Dma_Set((void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
	tiles = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_MoteTiles, tiles);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x300, tiles);
	Runtime_ReleaseHeapBlock(14);
	for (i = 0; i < 32; i++)
	{
		s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
		/* FAKEMATCH: needed to match: reusing the integer field cursor for the height x argument keeps the stores in r1. */
		u32 p = (u32)mote;
		s32 z;
		*(s32 *)p = 0;
		p += sizeof(s32);
		*(s32 *)p = 0x40000400;
		p += sizeof(s32);
		*(s32 *)p = 0xd400;
		p = pos[0];
		z = pos[2];
		mote->pos_x = (s32)p;
		mote->pos_z = z;
		mote->pos_y = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
		mote->timer = (i & 15) + 1;
		mote++;
	}
	Io_Write16(0x3f00, &REG_BLDCNT);
	Io_Write16(0x1008, &REG_BLDALPHA);
	Io_Write16(0, &REG_BLDY);
	Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

void FieldEffect_UpdateSparkles(void)
{
	/* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip stores together. The plain volatile read differs by 3: it splits the two bit-field stores. */
	struct GroundParticleWork *work;
	u32						   spawned;
	u32						   i;
	struct MapScrollWork	  *map;
	s32						  *camera;
	s32						   spawn_x;
	s32						   spawn_z;
	struct GroundParticle	  *sparkle;
	const u16				  *frame;
	s32						  *leader;
	s32						   camera_x;
	s32						   camera_z;
	s32						   sx;
	s32						   sy;
	s32						   delay;
	s32						   x;
	u32						   a;
	u32						   b;
	u32						   frames;

	work = WORK_SLOT(29);
	spawned = 0;
	map = WORK_SLOT(8);
	camera = &map->view_x;
	spawn_x = 0;
	spawn_z = 0;
	i = 0;
	sparkle = work->particles;
	delay = 0;
	sx = 0;
	sy = 0;
	do
	{
		if (sparkle->timer != 0)
		{
			camera_x = camera[0];
			camera_z = camera[1];
			if (GameFlag_TestFar(0x166) != 0)
				sparkle->timer++;
			frame = (const u16 *)&FieldFx_SparkleFrames[sparkle->timer >> 1];
			a = Random16();
			b = Random16();
			x = ((sparkle->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
			sx = x - 1;
			sy = (sparkle->pos_z - sparkle->pos_y - camera_z) / 0x10000 + *(const s16 *)frame;
			frame++;
			if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159)
			{
				sparkle->tile = work->tile_base + *frame;
				sparkle->x = sx;
				sparkle->shape = 0;
				sparkle->size = 1;
				frames = *(unsigned long *)&gFrameCount;
				sparkle->y = sy;
				sparkle->affine_index = ((frames >> 1) & 1) << 3;
				Runtime_PushSlotEntry(sparkle, 240);
			}
			sparkle->timer--;
		}
		if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0)
		{
			if (delay != 0)
			{
				sparkle->pos_x = spawn_x;
				sparkle->pos_z = spawn_z;
				sparkle->pos_y = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
				sparkle->timer = 62 - delay;
				sparkle->delay = 0;
				spawned++;
				delay += 4;
			}
			else if ((Random16() & 255) == 0)
			{
				leader = map->origin;
				spawn_x = leader[0] + (Random16() << 8) - 0x800000;
				spawn_z = leader[2] + (Random16() << 8) - 0x800000;
				sparkle->pos_x = spawn_x;
				sparkle->pos_z = spawn_z;
				sparkle->pos_y = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
				sparkle->timer = 30;
				sparkle->delay = delay;
				spawned++;
				delay = 4;
			}
		}
		i++;
		sparkle++;
	} while (i <= 31);
}

void FieldEffect_InitSparkles(void)
{
	u32						   size = 0x410;
	struct GroundParticleWork *work = Runtime_AllocateBlock(29, size);
	struct GroundParticle	  *p = work->particles;
	u8						  *buf;
	u32						   i;
	s32						   clear;
	volatile u32			   zero;

	/* FAKEMATCH: needed to match: reusing the allocation size for the fill word keeps zero in r1; a separate helper value differs by four instructions. */
	size = 0;
	zero = size;
	Dma_Set(&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
	buf = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_SparkleTiles, buf);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x200, buf);
	Runtime_ReleaseHeapBlock(14);
	i = 0;
	clear = 0;
loop:
	{
		struct MapScrollWork *view = gMapWork[0];
		s32					 *leader;
		/* FAKEMATCH: needed to match: an integer field cursor keeps the view load order; reusing it for the height x argument keeps the leader in r2. Separate temporaries differ by 14 instructions. */
		u32 attr;
		s32 z;

		attr = (u32)p;
		*(u32 *)attr = clear;
		attr += sizeof(union DustWord);
		leader = view->origin;
		*(u32 *)attr = 0x40000400;
		attr += sizeof(union DustWord);
		*(u32 *)attr = 0xd400;
		attr = leader[0];
		z = leader[2];
		p->pos_x = clear;
		p->pos_z = clear;
		p->pos_y = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
		p->timer = (i & 15) + 1;
	}
	i++;
	p++;
	if (i < 32)
		goto loop;
	Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

void BattleFx_StartTwelveFrameBlend(void)
{
	struct FieldBlendWork	   *work;
	struct BattleEffectBuffers *buffers;
	volatile u32				zero;

	work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	buffers = gPaletteBufferWork;
	zero = 0;
	Dma_Set(
		(const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
	((s32 (*)())BattleFx_BuildBuffer)(0x10003, buffers, work, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	((s32 (*)())BattleFx_BuildBuffer)(0x10005, buffers, work->to, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
	((s32 (*)())BattleFx_BuildBuffer)((s32)work, 0, buffers->target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	work->timer = 600;
	work->enabled = 1;
	Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 0x7FFF;
	state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 12;
	state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
	struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	state->timer = 128;
	state->enabled = 1;
}

void BattleFx_StartBufferBlend(s32 from, s32 to)
{
	struct FieldBlendWork	   *work;
	struct BattleEffectBuffers *buffers;
	volatile u32				zero;
	s16						   *target;

	work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
	buffers = gPaletteBufferWork;
	zero = 0;
	Dma_Set(
		(const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
	((s32 (*)())BattleFx_BuildBuffer)(from, buffers, work, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	target = work->to;
	((s32 (*)())BattleFx_BuildBuffer)(to, buffers, target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
	((s32 (*)())BattleFx_BuildBuffer)((s32)work, 0, buffers->target, 1); /* FAKEMATCH: called as an undeclared function (int result, no prototype) */
	work->timer = 120;
	work->enabled = 0;
	Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

//--------------------------------------------------------------------------

// motion_move_halfway_toward_target.c

// オブジェクト：つながった相手への追従

//--------------------------------------------------------------------------

// ほかのモジュールの関数 (共通ヘッダーにまだ宣言が無いもの)

// このファイルの関数 (定義より前で使うもの)

//--------------------------------------------------------------------------
// つながったオブジェクトとの距離を、x・y・z とも半分つめる
// obj : 動かすオブジェクト
//--------------------------------------------------------------------------
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *obj)
{
	struct ObjectRuntime *tgt = obj->linked_object;

	obj->x += (tgt->x - obj->x) / 2;
	obj->y += (tgt->y - obj->y) / 2;
	obj->z += (tgt->z - obj->z) / 2;
}

//--------------------------------------------------------------------------
// hop.c
// 跳ねて分身を引く演出、ジン加入の演出、シーンのオブジェクトの再配置
//--------------------------------------------------------------------------

struct ActorDescriptor
{
	s16 action;
	s16 scene; // 配置先のシーン番号 - 252
	u8	pad4[4];
	s32 x; // 配置座標 (16.16 固定小数)
	s32 y;
	s32 z;
};

#define BUNSHIN_MAX 8 // 分身の数

//--------------------------------------------------------------------------
// 後ろへ 3 回跳ね、分身を引いて前へ跳ぶ
// id : 跳ねるオブジェクト
//--------------------------------------------------------------------------
void EventActor_RunHopAndLeapSequence(s32 id)
{
	struct FieldActor *actor;
	struct FieldActor *copy;
	struct FieldActor *prev;
	void			  *res;
	s32				   dir;
	s32				   back;
	s32				   dist;
	s32				   kind;
	s32				   value;
	s32				   i;
	struct FieldActor *copies[BUNSHIN_MAX];

	// 選択中のオブジェクトの向きを 90 度単位に丸める
	actor = Object_GetById(id);
	dir = (Object_GetById(gGameState.selected_actor)->facing + 0x2000) & 0xc000;
	Battle_Reset();
	WaitFrames(10);
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(id, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(id, 1);
	back = dir + 0x8000;
	WaitFrames(20);
	ObjectMotion_ArmCallback(id, back, 0);
	WaitFrames(10);
	actor->sprite->priority = 0;
	actor->facing = back;
	Object_GetById(id)->steering_flags &= ~1;
	actor->motion_flags = 2;

	// 向いた方の逆へ 16 ドットずつ 3 回跳ねる
	dist = 16 << 16;
	Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)actor, dist, dir);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 4, 0);
	Script_WaitForEventTimeoutFar(actor);
	WaitFrames(20);

	// 同じスプライトの分身を作り、ひとつ前を追いかけるようにつなぐ
	kind = ((struct AnimationObject *)actor->sprite)->entries[0]->anim_id;
	value = 9;
	if (kind == 90)
		value = 2;
	if (kind == 92)
		value = 10;
	if (kind == 91)
		value = 9;
	res = NULL;
	prev = actor;
	for (i = 0; i < BUNSHIN_MAX; i++)
	{
		copy = (struct FieldActor *)Object_CreateFar(
			kind, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		copies[i] = copy;
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->motion_flags = 0;
			copy->priority_flags = 2;
			copy->steering_flags |= 1;
			copy->update = (void *)ObjectMotion_MoveHalfwayTowardTarget;
			copy->facing = actor->facing;
			copy->sprite->priority = 0;
			Animation_ApplyChildValuesFar(copy, value);
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
			ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
			res = Object_ReplaceResourceEntry(copy->sprite, res);
			copy->linked_object = prev;
			prev = copy;
		}
	}

	// 向いた方へ 64 ドット跳び、動きを止める
	Motion_SetTargetPositionFromMagnitudeAngle(
		(struct ObjectRuntime *)actor, 64 << 16, dir + 0x8000);
	AudioCommand_PlayFar(136);
	ObjectMotion_Launch(id, 12, 0);
	WaitFrames(24);
	actor->motion_flags = 0;
	actor->velocity_x = 0;
	actor->velocity_z = 0;
	actor->velocity_y = 0;
	actor->target_x = ACTOR_NO_TARGET;
	actor->target_z = ACTOR_NO_TARGET;
	actor->target_y = ACTOR_NO_TARGET;
	ObjectDispatch_ApplyArgumentToChildrenFar(actor, 0);
	actor->sprite->priority = 2;
	BattleFx_FinishAction();
}

//--------------------------------------------------------------------------
// ジンを仲間に加え、属性ごとの演出のあとに加入メッセージを出す
// slot  : 演出の対象 (-1 で演出なし)
// elm   : ジンの属性
// djinn : 属性内のジン番号
//--------------------------------------------------------------------------
void BattleFx_RunPageEffectForSlot(s32 slot, s32 elm, void *djinn)
{
	struct FieldWork *work;
	s32				  owner;

	// 持ち主を決める (フラグ 366 のときは 0 番にし、入手フラグだけ立てる)
	work = (struct FieldWork *)gEventWork;
	if (GameFlag_TestFar(366) != 0)
	{
		owner = 0;
		GameFlag_SetBitFar((u8 *)djinn + elm * 20 + 48); // ジン入手フラグ
	}
	else
	{
		owner = Djinn_AddToLeastLoadedOwnerFar(elm, djinn);
	}

	if (owner >= 0)
	{
		Battle_Reset();
		Battle_SetObjectFlag5bWhenMode3();
		if (slot != -1)
		{
			if (work->mode == 3)
				BattleEffect_ClearOutOfBoundsObjects();

			if (elm == 0) // ヴィーナス
				BattleFx_RunVenusDjinnCapture(slot);
			else if (elm == 1) // マーキュリー
				BattleFx_RunMercuryDjinnCapture(slot);
			else if (elm == 2)
				BattleEffect_RunPhasedRadialParticleSequence(slot);
			else if (elm == 3)
				BattleFx_FinishHeavyImpact(slot);

			work->actors[slot] = 0;
			if (work->mode == 3)
				BattleEffect_ClearAllObjects();
		}

		Djinn_ShowJoinedMessageFar(owner, elm, djinn);
		Battle_ClearObjectFlag5bWhenMode3();
		BattleFx_FinishAction();
	}
}

//--------------------------------------------------------------------------
// 保留中のジンイベントのシーン (300〜380) に属するオブジェクトを置きなおす
// 0x800 が立っていれば、選択中のオブジェクトから z を 32 ドット引いた位置へ
//--------------------------------------------------------------------------
void FieldObject_PlaceSceneActors(void)
{
	s32 flags = gGameState.pending_djinn_event & 0xf000;
	u32 scene = gGameState.pending_djinn_event & 0xfff;
	s32 id;

	if (GameFlag_TestFar(0x109) != 0 && flags == 0)
	{
		flags = scene & 0x800;
		scene &= 0x7ff;
		if (scene - 300 <= 80 && gGameState.scene_change_reason > 0)
		{
			for (id = 8; id <= 65; id++)
			{
				struct ActorDescriptor *desc =
					(struct ActorDescriptor *)BattleAction_FindDescriptor(id);
				struct ObjectRuntime *obj;

				if (desc == NULL || desc->scene - 48 != scene - 300)
					continue;
				obj = ObjectTable_Get(id);
				if (obj == NULL)
					continue;
				if (flags == 0)
				{
					obj->terrain_height = flags;
					obj->flags = 3;
					Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
				}
				else
				{
					struct ObjectRuntime *sel = ObjectTable_Get(gGameState.selected_actor);

					Object_SetPositionAndResetMotionFar(obj, sel->x, sel->y, sel->z - (32 << 16));
				}
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
			}
		}
	}
}

struct MapActor
{
	s16 id;
	s16 event;
};

void Djinn_ResolvePendingEvent(s32 capture)
{
	struct GameState *st = &gGameState;
	s16				 *pending = &st->pending_djinn_event;
	s32				  kind = *pending & 0xf000;
	s32				  flag = *pending & 0xfff;

	if (capture == 0)
	{
		if (kind == 0)
		{
			flag &= 0x7ff;
			if (flag < 300 || flag > 380)
				return;
			if (st->scene_change_reason > 0 && st->scene_change_reason != 999)
				return;
			GameFlag_SetBitFar(flag - 172);
			*pending = kind;
		}
		else if (kind == 0x1000)
		{
			if (st->scene_change_reason == 1)
				GameFlag_SetBitFar(flag);
			*pending = capture;
		}
		return;
	}
	if (kind == 0)
	{
		flag &= 0x7ff;
		if (flag >= 300 && flag <= 380)
		{
			flag &= 0x7ff;
			if (st->scene_change_reason > 0)
			{
				s32 djinn = flag - 300;
				s32 element = djinn / 20;
				s32 index = djinn % 20;
				s32 actor;

				for (actor = 8; actor <= 65; actor++)
				{
					struct MapActor *found = (struct MapActor *)BattleAction_FindDescriptor(actor);
					if (found != 0 && found->event - 48 == flag - 300)
					{
						WaitFrames(40);
						BattleFx_RunPageEffectForSlot(actor, element, (void *)index);
						break;
					}
				}
			}
		}
	}
	gGameState.pending_djinn_event = 0;
}

// 演出スロット 24 個を動かす
static __inline__ void UpdateAllEffectSlots(void)
{
	struct EffectSlot *slot;
	s32				   n;

	slot = ((struct BattleFxScene *)gEffectWork)->slots;
	n = 23;
	do
	{
		n -= 1;
		EffectSlot_Update(slot);
		slot++;
	} while (n >= 0);
}

void BattleFx_UpdateAllSlots(void)
{
	UpdateAllEffectSlots();
}

void BattleFx_InitializeSlots(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateHeapBlock(56, sizeof(struct BattleFxScene));
	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (sizeof(struct BattleFxScene) / 4), REG_DMA3);
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllSlots, 3200);
}

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectSlot	 *slot;
	s32					  i;

	Scheduler_RemoveCallback(BattleFx_UpdateAllSlots);
	for (i = 0; i < 24; i++)
	{
		slot = &scene->slots[i];
		if (slot->active != 0)
			BattleFx_ClearOwnedSlot(slot);
	}
	Runtime_ReleaseHeapBlock(56);
	WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(struct MotionObject *object)
{
	object->angle += 0x2000;
}

void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor)
{
	struct MotionObject *target;
	struct EffectVector	 pos;

	target = (struct MotionObject *)Object_GetById(gGameState.selected_actor);
	if (actor->state == 0)
	{
		actor->orbit_radius += 1;
		actor->orbit_angle += 1;
		if ((s16)actor->age == 60)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 1)
	{
		actor->orbit_angle += 1;
		if ((s16)actor->age == 40)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 2)
	{
		actor->orbit_angle += 1;
		pos.x = target->x;
		pos.y = target->y + 0x140000;
		pos.z = target->z;
		Camera_WorldToScreen(&pos);
		actor->origin_x += (pos.x - actor->origin_x) / 8;
		actor->origin_z += (pos.z - actor->origin_z) / 8;
		if ((s16)actor->age == 40)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 3)
	{
		actor->orbit_radius -= 1;
		actor->orbit_angle += 1;
		if ((s16)actor->age == 60)
		{
			actor->age = 0;
			actor->state += 1;
		}
	}
	else if (actor->state == 4)
	{
		BattleFx_ClearOwnedSlot(actor);
	}
	pos.x = actor->origin_x;
	pos.z = actor->origin_z;
	Vector_AddPolarOffset(actor->orbit_radius << 16, actor->orbit_angle << 11, &pos);
	actor->x = pos.x;
	actor->z = pos.z;
}

//--------------------------------------------------------------------------
// heavy_impact_finish.c
// フィールド効果：ジンが仲間になる演出 (属性3)
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ジンが跳ねて光の粒になり、操作キャラへ集まる
// jin_id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_FinishHeavyImpact(s32 jin_id)
{
	struct EffectVector	  pos;
	struct MotionObject	 *jin;
	struct BattleFxScene *scene;
	struct EffectSlot	 *slot;
	s32					  scale;
	s32					  cnt;

	jin = (struct MotionObject *)Object_GetById(jin_id);
	if (jin == NULL)
		return;
	BattleFx_InitializeSlots();
	scene = gEffectWork;
	Shop_RestoreSceneTilesFar(0x20118c);
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(jin_id, 1);
	AudioCommand_PlayFar(174);
	Motion_SetVarCbAndRefresh(jin_id, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(jin_id, 1);
	WaitFrames(20);

	// 回転してから高く跳ねる
	AudioCommand_PlayFar(140);
	jin->update = BattleFx_AdvanceSpinAngle;
	WaitFrames(40);
	AudioCommand_PlayFar(153);
	ObjectMotion_Launch(jin_id, 12, 22);

	// ジンの画面位置から光の粒を 24 個出す
	pos.x = jin->x;
	pos.y = jin->y;
	pos.z = jin->z;
	Camera_WorldToScreen(&pos);
	ObjectDispatch_ReleaseFar(jin);
	AudioCommand_PlayFar(164);
	slot = scene->slots;
	cnt = 23;
	do
	{
		EffectSlot_Initialize(slot, 284, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_RunAngledApproachPhases); // 回ってから操作キャラへ
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
		scale = Random16() / 3 + 0x10000; // 1.0〜1.33 倍
		slot->scale_y = scale;
		slot->scale_x = scale;
		cnt--;
		WaitFrames(1);
		slot++;
	} while (cnt >= 0);

	// 操作キャラを 0x4000 (90°) の向きへ回し、動作 28 にする
	WaitFrames(60);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(40);
	AudioCommand_PlayFar(164);
	WaitFrames(100);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor の型ではバイトが変わる)
struct DescendingOrbitObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[80];
	u16 timer;
	s16 angle;
};

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (lifetime) を使う
struct CaptureObject
{
	u8				   unknown_00[8];
	s32				   x;
	s32				   y;
	s32				   z;
	s32				   base_y;
	s32				   scale_x;
	s32				   scale_y;
	u8				   unknown_20[8];
	s32				   gravity;
	u8				   unknown_2c[4];
	s32				   speed;
	u8				   unknown_34[33];
	u8				   mode;
	u8				   unknown_56[5];
	u8				   visible;
	u8				   unknown_5c[2];
	u16				   lifetime;
	u8				   unknown_60[4];
	u16				   timer;
	u16				   angle;
	struct FieldActor *linked_object;
	void			  *callback;
};

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
	u32 *table = BattleFx_PulseScales;
	u32	 index = ((u32)gFrameTick >> 2) & 1;
	u32	 value = index[table];
	((struct MotionObject *)object)->scale_x = value;
	((struct MotionObject *)object)->scale_y = value;
}

void BattleFx_AdvanceAngleWithRamp(void *obj)
{
	u32 step;

	step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
	((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + step + 0x1000;
	if (step < 0x1000U)
	{
		FIELD_AT_OFFSET(obj, s16 *, 0x64) = FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1;
	}
}

// 毎フレームの更新 : 45 度ずつ回しながら拡大率に step を足し、0x3000 を下回ったら消す
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                                 \
	void name(void *obj)                                                                      \
	{                                                                                         \
		s32 scale;                                                                            \
                                                                                              \
		scale = ((struct MotionObject *)obj)->scale_x + step;                                 \
		((struct MotionObject *)obj)->scale_y = ((struct MotionObject *)obj)->scale_y + step; \
		((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + 0x2000;   \
		((struct MotionObject *)obj)->scale_x = scale;                                        \
		if (scale < 0x3000)                                                                   \
		{                                                                                     \
			ObjectDispatch_ReleaseFar(obj);                                                   \
		}                                                                                     \
	}

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroySlow, 0xFFFFFE40)

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect)
{
	struct Object	   *owner;
	struct EffectVector position;
	s32					state;

	owner = (struct Object *)Object_GetById(gGameState.selected_actor);
	state = effect->state;

	if (state == 0)
	{
		effect->x = effect->origin_x;
		effect->z = effect->origin_z;
		position.x = effect->x;
		position.z = effect->z;
		Vector_AddPolarOffset(0x780000,
			((Random16() * 3 << 11) >> 16) - ((Random16() * 3 << 11) >> 16) + 0xc000, &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->acceleration = 0x50000;
		effect->max_speed = 0x50000;
		effect->flag42 = state;
		effect->state++;

		((struct AnimationObject *)effect->object)->part[0].priority =
			owner->sprite->part[0].priority;
		effect->flags = 0;
		effect->age = 0;
		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(134);
	}
	else if (state == 1)
	{
		if ((s16)effect->age == 3)
		{
			((struct AnimationObject *)effect->object)->part[0].priority = 0;
			effect->flags = 4;
		}
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
		{
			effect->origin_x = effect->x;
			effect->origin_z = effect->z;
			((struct AnimationObject *)effect->object)->part[0].priority = 0;
			effect->flags = 4;
			effect->render = 0;
			effect->state++;
			effect->callback_delay = 40;
		}
	}
	else if (state == 3)
	{
		effect->render = 1;
		effect->x = effect->origin_x;
		effect->z = effect->origin_z;
		position.x = owner->pos.x;
		position.y = owner->pos.y + 0x140000;
		position.z = owner->pos.z;
		Camera_WorldToScreen(&position);
		Vector_AddPolarOffset(0x40000, Random16(), &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->state++;
		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(145);
	}
	else if (state == 4)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 5)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg)
{
	s8				   *object_pointer;
	struct EffectVector position;
	s32					value;
	s32					remaining;
	struct FieldActor  *source_object;
	void			   *effect_slots;
	struct FieldActor  *target_object;
	void			   *slot;
	s8				   *slot_state;

	source_object = Object_GetById(arg);
	target_object = Object_GetById(gGameState.selected_actor);
	if (source_object == NULL)
		return;

	BattleFx_InitializeSlots();
	effect_slots = gEffectWork;
	Shop_RestoreSceneTilesFar(0x201090);
	WaitFrames(30);
	ObjectMotion_ArmCallback(arg, 0x4000, 0);
	WaitFrames(20);
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(arg, 1);
	AudioCommand_PlayFar(174);
	Motion_SetVarCbAndRefresh(arg, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(arg, 1);
	WaitFrames(20);
	AudioCommand_PlayFar(140);
	source_object->update = (void *)BattleFx_AdvanceAngleWithRamp;
	source_object->timer = 0;
	WaitFrames(80);
	source_object->update = (void *)BattleFx_ShrinkObjectAndDestroySlow;
	ObjectDispatch_ApplyArgumentToChildrenFar(source_object, 3);

	position.x = source_object->x.fixed;
	position.y = source_object->y.fixed;
	position.z = source_object->z.fixed;
	Camera_WorldToScreen(&position);

	slot = (u8 *)effect_slots + 88;
	remaining = 23;
	do
	{
		EffectSlot_Initialize(slot, 284, position.x, position.z);
		EffectSlot_SetCallback(slot, (void *)BattleEffect_UpdatePhasedRadialParticle);
		EffectSlot_SetObjectMode(slot, 7);
		object_pointer = slot;
		value = *(s32 *)object_pointer;
		ObjectGroup_SetChildValueUnlessFifteenFar((void *)value, 10);
		value = Random16() / 3 + 0x10000;
		((struct EffectSlot *)slot)->scale_y = value;
		((struct EffectSlot *)slot)->scale_x = value;
		remaining--;
		WaitFrames(1);
		slot = (u8 *)slot + 72;
	} while (remaining >= 0);

	WaitFrames(60);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	{
		s32 next_state;

		WaitFrames(20);
		next_state = 2;
		slot_state = effect_slots;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(60);
	target_object->update = (void *)BattleFx_SetObjectAlternatingWords;
	WaitFrames(100);

	{
		s32 next_state;

		slot_state = effect_slots;
		next_state = 5;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(10);
	slot = NULL;
	target_object->update = slot;
	target_object->scale_x = 0x10000;
	target_object->scale_y = 0x10000;
	WaitFrames(30);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

DEFINE_SHRINK_AND_DESTROY(BattleFx_ShrinkObjectAndDestroyFast, 0xFFFFFC00)

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg)
{
	struct EffectVector			  local;
	struct DescendingOrbitObject *other;
	s32							  raw;
	s16							  value;
	s32							  y;

	other = (struct DescendingOrbitObject *)Object_GetById(gGameState.selected_actor);
	raw = arg->timer - 1;
	arg->timer = raw;
	value = arg->timer;
	local.x = other->x;
	local.z = other->z;
	Vector_AddPolarOffset(value * 0x6666, (value << 11) + arg->angle, &local);
	arg->x = local.x;
	arg->z = local.z;
	y = arg->y + 0xFFFF0000;
	arg->y = y;
	if (y < other->y + 0x140000)
		ObjectDispatch_ReleaseFar(arg);
}

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect)
{
	struct EffectVector	  position;
	struct ObjectRuntime *source;
	s32					  state;
	u32					  random;

	source = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
	state = effect->state;

	if (state == 0)
	{
		position.x = source->x;
		position.y = source->y;
		position.z = source->z;

		random = Random16() * 10 + 0xa0000;
		Vector_AddPolarOffset(random, Random16(), &position);
		Camera_WorldToScreen(&position);

		effect->origin_x = position.x;
		effect->origin_z = position.z;
		effect->x = position.x;
		effect->z = position.z;
		position.x = effect->x;
		position.z = effect->z;

		Vector_AddPolarOffset(0x780000, 0xc000, &position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->acceleration = 0x10000;
		effect->max_speed = 0x50000;
		effect->flag42 = state;
		effect->state++;

		if ((gFrameTick & 1) != 0)
			AudioCommand_PlayFar(0x90);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			effect->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleFx_RunVenusDjinnCapture(s32 arg)
{
	struct CaptureObject *leader;
	struct CaptureObject *djinni;
	void				 *effect_slots;
	u8					 *slot;
	s8					 *slot_state;
	s32					  remaining;
	s32					  scale;
	s32					  position[3];

	leader = (struct CaptureObject *)Object_GetById(gGameState.selected_actor);
	djinni = (struct CaptureObject *)Object_GetById(arg);
	if (djinni == NULL)
		return;

	BattleFx_InitializeSlots();
	effect_slots = gEffectWork;
	Shop_RestoreSceneTilesFar(0x201204);
	WaitFrames(30);
	djinni->visible = 0;
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(arg, 4, 15);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(arg, 4, 15);
	WaitFrames(30);
	djinni->callback = (void *)BattleFx_AdvanceSpinAngle;
	AudioCommand_PlayFar(153);
	ObjectMotion_Launch(arg, 8, 22);
	AudioCommand_PlayFar(140);
	Runtime_SetWorkTripleIfNonNegativeFar(0x14ccc, 0x14ccc, 0x10000);
	djinni->callback = (void *)BattleFx_ShrinkObjectAndDestroyFast;
	ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 3);
	WaitFrames(90);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(30);
	Runtime_SetWorkTripleIfNonNegativeFar(0x19999, 0x19999, 0x10000);

	position[0] = djinni->x;
	position[1] = djinni->y;
	position[2] = djinni->z;
	Camera_WorldToScreen((struct EffectVector *)position);

	slot = (u8 *)effect_slots + 88;
	remaining = 23;
	do
	{
		EffectSlot_Initialize((struct EffectSlot *)slot, 284, position[0], position[2]);
		EffectSlot_SetCallback((struct EffectSlot *)slot, (void *)BattleFx_UpdateRadialSpread);
		EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 11);
		((struct EffectSlot *)slot)->scale_x = 0x8000;
		((struct EffectSlot *)slot)->scale_y = Random16() + 0x18000;
		remaining--;
		WaitFrames(1);
		slot += 72;
	} while (remaining >= 0);

	WaitFrames(140);
	{
		s32 next_state;

		next_state = 2;
		slot_state = effect_slots;
		slot_state += 152;
		remaining = 23;
		do
		{
			if (slot_state[5] != 0)
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	WaitFrames(20);
	Runtime_SetWorkTripleIfNonNegativeFar(1, 1, 1);
	WaitFrames(30);

	for (remaining = 0; remaining <= 23; remaining++)
	{
		position[0] = leader->x;
		position[1] = leader->y + 0x780000;
		position[2] = leader->z;
		djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1], position[2]);
		if (djinni != NULL)
		{
			scale = Random16() / 3 + 0x10000;
			djinni->scale_y = scale;
			djinni->scale_x = scale;
			djinni->timer = 100;
			djinni->angle = (remaining << 16) / 24;
			djinni->callback = (void *)BattleFx_UpdateDescendingOrbitObject;
			djinni->mode = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(djinni, 7);
			Animation_ApplyChildValuesFar(djinni, 11);
		}
	}

	WaitFrames(100);
	AudioCommand_PlayFar(288);
	WaitFrames(1);
	AudioCommand_PlayFar(151);
	position[0] = leader->x;
	position[1] = leader->y + 0x120000;
	position[2] = leader->z;
	for (remaining = 0; remaining <= 7; remaining++)
	{
		djinni = (struct CaptureObject *)Object_Spawn(284, position[0], position[1], position[2]);
		if (djinni == NULL)
			break;
		djinni->scale_y = 0x9999;
		djinni->scale_x = 0x9999;
		djinni->mode = 2;
		djinni->gravity = 0x50000;
		djinni->base_y = djinni->y;
		djinni->speed = Random16() + 0x16666;
		Motion_SetTargetPositionFromMagnitudeAngle(
			(struct ObjectRuntime *)djinni, 0x200000, Random16());
		Animation_ApplyChildValuesFar(djinni, 11);
		djinni->lifetime = 8;
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)djinni, BattleFx_CommonParticleScript);
	}

	WaitFrames(15);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect)
{
	struct ObjectRuntime *source;
	struct EffectVector	  position;
	s8					 *state_pointer;
	s32					  state;

	source = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
	state_pointer = &effect->state;
	state = *state_pointer;

	if (state == 0)
	{
		position.x = source->x;
		position.y = source->y + Random16() * 5 + 0xf0000;
		position.z = source->z;
		Camera_WorldToScreen(&position);
		Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->x = position.x;
		effect->z = position.z - 0x640000;
		effect->acceleration = 0x30000;
		effect->max_speed = Random16() * 3 + 0x30000;
		effect->scale_x = 0x10000;
		effect->scale_y = 0x10000;
		effect->flag42 = state;
		effect->stop_at_target = 1;
		(*state_pointer)++;
	}
	else if ((u8)(effect->state - 1) <= 1)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
		{
			position.x = effect->x;
			position.z = effect->z;
			Vector_AddPolarOffset(0xc0000, Random16(), &position);
			effect->target_x = position.x;
			effect->target_z = position.z;
			effect->stop_at_target = 0;
			effect->speed = 0x10000;
			effect->acceleration = 0;
			effect->max_speed = Random16() + 0x23333;
			effect->scale_x = 0x8000;
			effect->scale_y = 0x8000;
			AudioCommand_PlayFar(143);

			if (*state_pointer == 1)
				(*state_pointer)--;
			else
				(*state_pointer)++;
			effect->callback_delay = 6;
		}
	}
	else if (state == 3)
	{
		BattleFx_ClearOwnedSlot(effect);
	}
}

void BattleFx_HalveDistanceToTarget(struct FieldActor *object)
{
	struct FieldActor *target = object->linked_object;

	object->x.fixed += (target->x.fixed - object->x.fixed) / 2;
	object->y.fixed += (target->y.fixed - object->y.fixed) / 2;
	object->z.fixed += (target->z.fixed - object->z.fixed) / 2;
}

//--------------------------------------------------------------------------
// run_mercury_djinn_capture.c
// フィールド演出：水のジンが仲間になるとき、跳ねて残像を引いて昇り、粒を降らせる
//--------------------------------------------------------------------------

#define ZANZOU_MAX 8 // 残像の数
#define TSUBU_MAX 24 // 降らせる粒の数 (scene->slots)

//--------------------------------------------------------------------------
// 画面を青く染め、ジンを 3 回跳ねさせてから 8 つの残像を引いて 120 ドット
// 昇らせて消す。selected_actor を mode 28 にしたあと、そのまわりに粒を
// 1 フレームに 1 つずつ 24 個降らせ、最後に色を戻す
// id : ジンのオブジェクト番号
//--------------------------------------------------------------------------
void BattleFx_RunMercuryDjinnCapture(s32 id)
{
	struct FieldActor  *jin;
	struct FieldActor  *copy;
	struct FieldActor  *previous;
	struct FieldSprite *resource;
	void			   *scene;
	struct FieldActor **cursor;
	struct FieldActor **list;
	u8				   *slot;
	s8				   *slot_state;
	s32					kind;
	s32					remaining;
	s32					resource_id;
	struct FieldActor  *copies[ZANZOU_MAX];
	struct EffectVector position;

	jin = Object_GetById(id);
	if (jin == NULL)
		return;

	// 画面の色を青 (R4 G4 B16) で染める
	BattleFx_InitializeSlots();
	scene = gEffectWork;
	Shop_RestoreSceneTilesFar(PALETTE_TRANSFORM_TINT | (16 << 10) | (4 << 5) | 4);
	WaitFrames(30);
	jin->paused = 0;
	AudioCommand_PlayFar(173);
	Motion_SetVarCbAndRefresh(id, 1);
	AudioCommand_PlayFar(175);
	Motion_SetVarCbAndRefresh(id, 1);
	WaitFrames(20);

	// 3・5・7 ドット/フレームで 3 回跳ねる
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 3, 14);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 5, 16);
	AudioCommand_PlayFar(152);
	ObjectMotion_Launch(id, 7, 18);
	WaitFrames(20);

	// 残像：同じ絵を 8 つ作り、それぞれ一つ前を半分ずつ追いかけさせる
	kind = ((struct AnimationObject *)jin->sprite)->entries[0]->anim_id;
	resource = NULL;
	previous = jin;
	cursor = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
	{
		copy =
			(struct FieldActor *)Object_CreateFar(kind, jin->x.fixed, jin->y.fixed, jin->z.fixed);
		*cursor++ = copy;
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->motion_flags = 0;
			copy->priority_flags = 2;
			copy->steering_flags |= 1;
			copy->update = (void *)BattleFx_HalveDistanceToTarget;
			copy->facing = jin->facing;
			Animation_ApplyChildValuesFar(copy, 9);
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 0);
			ObjectDispatch_SetSingleChildFlagsFar(copy, 0);
			resource = Object_ReplaceResourceEntry(copy->sprite, resource);
			copy->linked_object = previous;
			previous = copy;
		}
	}

	// 8 ドットずつ 15 フレーム昇らせ、ジンと残像を消す
	resource_id = resource->vram_block;
	AudioCommand_PlayFar(153);
	jin->motion_flags = 0;
	for (remaining = 14; remaining >= 0; remaining--)
	{
		jin->y.fixed += 8 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ReleaseFar(jin);
	list = copies;
	for (remaining = ZANZOU_MAX - 1; remaining >= 0; remaining--)
		ObjectDispatch_ReleaseFar(*list++);
	if (resource_id != 96)
		Resource_ResetEntry(resource_id);
	WaitFrames(10);
	ObjectMotion_ArmCallback(gGameState.selected_actor, 0x4000, 0);
	WaitFrames(20);
	ObjectDispatch_ApplyArgumentToChildrenFar(Object_GetById(gGameState.selected_actor), 28);
	WaitFrames(20);

	// 粒 (種類 240) を 1 フレームに 1 つずつ出す。動きは BattleFx_UpdateRandomTargetParticle
	position.x = jin->x.fixed;
	position.y = jin->y.fixed;
	position.z = jin->z.fixed;
	Camera_WorldToScreen(&position);

	slot = (u8 *)scene + 88; // scene->slots[0]
	remaining = TSUBU_MAX - 1;
	do
	{
		EffectSlot_Initialize((struct EffectSlot *)slot, 240, position.x, position.z);
		EffectSlot_SetCallback((struct EffectSlot *)slot, BattleFx_UpdateRandomTargetParticle);
		EffectSlot_SetObjectMode((struct EffectSlot *)slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(*(void **)slot, 9);
		remaining--;
		WaitFrames(1);
		slot += 72; // sizeof(struct EffectSlot)
	} while (remaining >= 0);

	// 120 フレーム後、動いている粒 (active) の state を 2 にする
	WaitFrames(120);
	{
		s32 next_state;

		next_state = 2;
		slot_state = scene;
		slot_state += 152; // scene->slots[0].state
		remaining = TSUBU_MAX - 1;
		do
		{
			if (slot_state[5] != 0) // active
				slot_state[0] = next_state;
			remaining--;
			slot_state += 72;
		} while (remaining >= 0);
	}

	// 色を戻す
	WaitFrames(50);
	Shop_InitEffectFar();
	BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

//--------------------------------------------------------------------------

// dispatch_type_handler.c

// フィールド：演出処理の呼び分け

//--------------------------------------------------------------------------

// 物体や座標は関数ごとの構造体で渡すので、引数は void * で受ける
