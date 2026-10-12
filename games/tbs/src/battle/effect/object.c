#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
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

struct ActionDescriptor
{
	u16 sprite;
	s16 condition;
	s32 behavior;
	s32 x;
	s32 y;
	s32 z;
	u16 facing;
	u8	talk_facing;
	u8	flags;
};

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

// シーンのイベント表の 1 件（struct SceneEvent と同じ 12 バイト）
struct EffectDescriptor
{
	s32 flags; // 下位 4bit が種類（SceneEventKind、7 は止めた相手と話す）
	u8	value; // 相手（オブジェクト番号など）
	u8	unused_05;
	s16 condition; // GameFlag_IsConditionActive の条件
	s32 result;	   // 0x10000 未満はメッセージ、それ以上は呼ぶ関数
};

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

#define OBJ_LAST 66 // 調べるオブジェクト番号の最後

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

struct ActionDescriptor;

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

//--------------------------------------------------------------------------
// kind に応じた演出の処理を呼ぶ
// kind : 0 対象を指定せずに実行、1 指定の対象 (second_object_id) に実行、2 終わり、3 種類ごとの処理
//--------------------------------------------------------------------------
void Field_DispatchTypeHandler(s32 kind)
{
	if (kind == 0)
		BattleFx_Run();
	else if (kind == 1)
		BattleFx_DispatchRequestKind();
	else if (kind == 2)
		BattleFx_ClearChildValueOnMismatch();
	else if (kind == 3)
		FieldEvent_RunTypeHandler();
}

//--------------------------------------------------------------------------
// effect.c
// フィールド効果：種類ごとの振り分けと、効果のイベント処理
//--------------------------------------------------------------------------

#define HEAP_SLOT_FX 56 // エナジー演出のワーク (gWorkSlot[56] は gEffectWork)

void _call_via_r3(s32, s32);

//--------------------------------------------------------------------------
// 効果の種類 (animation) ごとに本体を走らせる
//--------------------------------------------------------------------------
void BattleFx_Run(void)
{
	struct BattleFxScene *scene;
	struct BattleRuntime *runtime;
	s32					  kind;
	s32					  target_id;
	s32					  obj_id;

	scene = *(gWorkSlot + HEAP_SLOT_FX);
	runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	kind = scene->animation;
	target_id = scene->second_object_id;

	switch (kind)
	{
	case 1:
		BattleFx_RunItemBreakSequence();
		return;
	case 7:
		RunBattleEffect07();
		return;
	case 11:
		RunBattleEffect11();
		return;
	case 4:
		RunBattleEffect04();
		return;
	case 5:
		RunBattleEffect05();
		return;
	case 14:
		RunBattleEffect14();
		return;
	case 6:
		BattleFx_RunOrbitingParticles();
		return;
	case 3:
		RunBattleEffect03();
		return;
	case 12:
		BattleFx_RunBurstParticles();
		return;
	case 13:
		RunBattleEffect13();
		return;
	case 9: // 前に止めた対象を戻し、近くの対象を止める
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}

		obj_id = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, kind);
		obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
		if (BattleFx_FindDescriptorWithOverride(obj_id) != 0)
		{
			BattleFx_SetupObjectPair(gGameState.selected_actor, obj_id);
			BattleFx_MarkChildAndRunFallbackTransition(obj_id);
			BattleEffect_PauseObject(obj_id);
			gGameState.paused_object = obj_id;
		}
		else
		{
			BattleEffect_RunFallbackObjectTransition();
		}
		return;
	case 2: // 波と色の効果 (種類 8 の効果が動いていれば先に止める)
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		return;
	case 8:
		RunBattleEffect08();
		return;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		return;
	case 15:
		BattleFx_RunEffect15();
		return;
	case 16:
		RunBattleEffect16();
		return;
	}
}

//--------------------------------------------------------------------------
// 対象が決まっているときの振り分け
//--------------------------------------------------------------------------
void BattleFx_DispatchRequestKind(void)
{
	struct BattleFxScene *scene = *(gWorkSlot + HEAP_SLOT_FX);
	struct BattleRuntime *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
	s32					  kind = scene->animation;
	s32					  target_id = scene->second_object_id;

	scene->enabled = 0;
	switch (kind)
	{
	case 2:
		if (*(s16 *)&runtime->unknown_cb8[0] != 0)
			ResetSceneTransitionEffect();
		if (gGameState.paused_object != scene->second_object_id)
			((struct ObjectRuntime *)scene->child)->movement_state = 1;
		RunSceneTransitionEffect(scene->first_object_id, target_id);
		break;
	case 1: // 方向キーで対象を押す
		BattleFx_SteerPushedTarget(target_id);
		break;
	case 7:
		BattleEffect_RunTargetedItemBreak(target_id);
		break;
	case 11:
		BattleFx_SteerLiftedTarget(target_id);
		break;
	case 4:
		BattleFx_CallEffect04(target_id);
		break;
	case 5:
		BattleFx_CallEffect05(target_id);
		break;
	case 6:
		BattleFx_StartOrbitingParticles(target_id);
		break;
	case 12:
		BattleFx_RunBurstParticleMainObject(target_id);
		break;
	case 9:
		if (gGameState.paused_object != -1)
		{
			BattleFx_ResumeObject(gGameState.paused_object);
			gGameState.paused_object = -1;
		}
		BattleEffect_PauseObject(target_id);
		gGameState.paused_object = target_id;
		BattleFx_MarkChildAndRunFallbackTransition(target_id);
		break;
	case 3:
		BattleFx_CallEffect03AndStop(target_id);
		break;
	case 14:
		BattleFx_CallEffect14(target_id);
		break;
	case 13:
		BattleFx_RunEffect13Hook(target_id);
		break;
	case 8:
		RunBattleEffect08();
		break;
	case 10:
		BattleFx_RunFlashingCallbackSequence();
		break;
	case 15:
		BattleFx_RunEffect15();
		break;
	case 16:
		RunBattleEffect16();
		break;
	}
}

//--------------------------------------------------------------------------
// 種類 2 の後始末：効果を終え、止めていない対象の movement_state を戻す
//--------------------------------------------------------------------------
void BattleFx_ClearChildValueOnMismatch(void)
{
	struct BattleFxScene *scene = gEffectWork;

	if (scene->animation == 2)
	{
		BattleFx_FinishSceneAndReleaseHeapBlock();
		if (gGameState.paused_object != scene->second_object_id)
		{
			((struct ObjectRuntime *)scene->child)->movement_state = 0;
		}
	}
}

//--------------------------------------------------------------------------
// 種類 8, 10, 16 の追加処理
//--------------------------------------------------------------------------
void FieldEvent_RunTypeHandler(void)
{
	u32 kind;

	kind = ((struct BattleFxScene *)gEffectWork)->animation;
	switch (kind)
	{
	case 8:
		ResetSceneTransitionEffect();
		return;
	case 10:
		MapEvent_RunTileTriggerSequence();
		return;
	case 16:
		FieldEvent_ShowStatusMessage();
		return;
	}
}

//--------------------------------------------------------------------------
// 効果に結びついたイベントを実行する
// event  : オフセット 8 の値が 0x10000 未満ならメッセージ番号、それ以上なら処理の番地
// param  : 処理へ渡す値 (使った者など)
// target : 対象
// フラグ 322 が立っていれば「何も起こらない」を出す
//--------------------------------------------------------------------------
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target)
{
	s32 script;

	if (event != NULL)
	{
		script = ((struct EffectDescriptor *)event)->result;
		if (script != 0)
		{
			if (script < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(((struct EffectDescriptor *)event)->result);
				BattleEv_RunWait(target, 0);
				BattleFx_FinishAction();
			}
			else
			{
				_call_via_r3(param, target);
			}
		}
		if (GameFlag_TestFar(322) != 0)
		{
			Battle_Reset();
			UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
			BattleFx_FinishAction();
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 各パーツのパレットをフレーム数 % 6 にする (毎フレーム呼ぶと色が巡る)
//--------------------------------------------------------------------------
void ObjectGroup_ApplyRandomChildValues(void *owner)
{
	s32			  kind;
	void		 *anim;
	s32			  initial_count;
	s32			  cnt;
	void		**entry;
	void		 *part;
	volatile s32 *frame_count;
	s32			  value;

	kind = ((struct ObjectRuntime *)owner)->animation_kind;
	if (kind == 1)
	{
		anim = ((struct ObjectRuntime *)owner)->animation;
		if (anim != 0 && (((struct AnimationObject *)anim)->display_flags & kind) == 0)
		{
			initial_count = ((struct AnimationObject *)anim)->count;
			if (initial_count != 0)
			{
				frame_count = (volatile s32 *)&gFrameCount;
				entry = (void **)((struct AnimationObject *)anim)->entries;
				cnt = initial_count;
				do
				{
					part = *entry++;
					value = (u32)*frame_count % 6;
					cnt--;
					((struct AnimationEntry *)part)->param = value;
				} while (cnt != 0);
			}
			((struct AnimationObject *)anim)->dirty = 1;
		}
	}
}

//--------------------------------------------------------------------------
// オブジェクトを向き angle へ magnitude だけずらした位置に置く
//--------------------------------------------------------------------------
void Motion_SetTargetPositionFromMagnitudeAngle(
	struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
	s32 pos[3];

	if (object != 0)
	{
		pos[0] = object->x;
		pos[1] = object->y;
		pos[2] = object->z;
		Vector_AddPolarOffset(magnitude, angle, pos);
		Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
	}
}

// 以下では物体を関数ごとの構造体で渡す

s32 ResourceTable_CountFreeBlocks(void)
{
	u8 *marker = ResourceBlockOwners;
	s32 free_count = 0;
	s32 remaining = 0x200;

	do
	{
		if (*marker++ == 0xff)
		{
			free_count++;
		}
		remaining--;
	} while (remaining != 0);
	return free_count;
}

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
	void			   *ret;
	struct FieldSprite *obj;

	obj = src;
	ret = NULL;
	if (obj != NULL)
	{
		if (alt == NULL)
		{
			obj->unknown_1d_0 = 1;
		}
		else
		{
			Resource_ResetEntry(obj->vram_block);
			obj->vram_block = ((struct FieldSprite *)alt)->vram_block;
			obj->unknown_1d_0 = 1;
			obj = alt;
		}
		ret = obj;
	}
	return ret;
}

//--------------------------------------------------------------------------
// arcupdat.c
// エナジー演出：演出用の物体、ほかの物体の一括設定、ふくらむ弧の動き
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 演出用の物体を出す
// アニメのない物体は消して NULL。高さは使う者に合わせ、優先度は 0
//--------------------------------------------------------------------------
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
	struct BattleFxScene *work = gEffectWork;
	struct ObjectRuntime *obj;

	obj = Object_CreateFar(kind, x, y, z);
	if (obj != NULL)
	{
		if (obj->animation_kind == 0)
		{
			ObjectDispatch_ReleaseFar(obj);
			return NULL;
		}
		obj->terrain_height = ((struct ObjectRuntime *)work->main_object)->terrain_height;
		obj->flags = 4;
		obj->unknown_23 = 4;
		((struct AnimationObject *)obj->animation)->part[0].priority = 0;
		ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	}
	return obj;
}

//--------------------------------------------------------------------------
// 主人公と except 以外のすべての物体に、動きの状態と値を設定する
//--------------------------------------------------------------------------
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state, s32 value)
{
	s16					 *player;
	struct ObjectRuntime *obj;
	s32					  id;

	id = 0;
	player = (s16 *)gPlayerObjectId;
	do
	{
		obj = ObjectTable_Get(id);
		if (id != *player && obj != NULL && obj != except)
		{
			obj->movement_state = state;
			ObjectDispatch_ApplyValueToChildrenFar(obj, value);
		}
		id++;
	} while (id <= OBJ_LAST);
}

//--------------------------------------------------------------------------
// 弧 A：32 フレームでふくらんでしぼみ、毎フレーム 1 ドット上がる
// z は物体の 9 ドット先から、小さいほど 5 ドットまで離れる
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = ++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

//--------------------------------------------------------------------------
// 弧 B：A の上下を反転したもの。z は物体の 16 ドット先から、小さいほど 5 ドットまで寄る
//--------------------------------------------------------------------------
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj)
{
	struct FieldActor *link;
	s32				   v;

	link = obj->linked_object;
	v = ++obj->timer;
	if (v > 31)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		return;
	}
	v = Trig_Sin(v << 10);
	obj->scale_x = v;
	obj->scale_y = -v;
	obj->x.fixed = link->x.fixed;
	obj->y.fixed += 0x10000;
	obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}

//--------------------------------------------------------------------------
// actsetup.c
// エナジー演出：ワークの準備、使う者と対象の記録、ふくらむ弧の物体
//--------------------------------------------------------------------------

// スプライト：OAM の三つの属性と管理情報
struct ArcSprite
{
	u8					   pad00[4];
	u16					   y : 8;
	u16					   affine : 1;
	u16					   double_size : 1;
	u16					   mode : 2;
	u16					   mosaic : 1;
	u16					   color : 1; // 1 = 256 色
	u16					   shape : 2;
	u16					   x : 9;
	u16					   unused : 3;
	u16					   hflip : 1;
	u16					   vflip : 1;
	u16					   size : 2;
	u16					   tile : 10;
	u16					   priority : 2;
	u16					   palette : 4;
	u8					   pad0a[18];
	u8					   slot; // VRAM ブロック
	u8					   active : 1;
	u8					   display_flags : 7;
	u8					   pad1e[8];
	u8					   flags;
	u8					   pad27;
	struct AnimationEntry *child;
};

//--------------------------------------------------------------------------
// 物体の位置に二つの弧を出す (B は上下を反転)
// 弧は 32 フレームでふくらんでしぼみ、物体についていく
//--------------------------------------------------------------------------
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *source = scene->main_object;
	struct FieldActor	 *objs[2];
	struct FieldActor	 *obj;
	struct ArcSprite	 *spr;
	s32					  i;

	for (i = 0; i <= 1; i++)
	{
		obj = (struct FieldActor *)Object_CreateFar(
			26, linked->x.fixed, linked->y.fixed, linked->z.fixed);
		objs[i] = obj;
		if (obj == 0)
			continue;
		obj->terrain_height = linked->terrain_height;
		spr = obj->sprite;
		obj->motion_flags = 0;
		obj->timer = 0;
		obj->linked_object = linked;
		obj->scale_x = obj->scale_y = 0x1999; // 0.1 倍
		if (spr == 0)
			continue;
		// 演出のタイルを使う 16 色の横長スプライト
		AnimationObjects_SelectAnimationFar((struct AnimationObject *)spr, 0);
		spr->flags = 0;
		Resource_ResetEntry(spr->slot);
		spr->slot = scene->tile_slot;
		spr->active = 1;
		spr->tile = (gVramBlockCache[spr->slot].offset >> 5) & 0x3ff;
		spr->color = 0;
		spr->shape = 1;
		spr->size = 2;
		spr->child->frame = 0;
	}
	// B の弧は最前面、A の弧は使う者と同じ優先度
	objs[0]->update = BattleFx_UpdateScaledArcObjectB;
	objs[0]->sprite->priority = 0;
	objs[1]->update = BattleFx_UpdateScaledArcObjectA;
	objs[1]->sprite->priority = source->sprite->priority;
	objs[1]->priority_flags = 2;
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 7 と 0 を切り替え、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_FlickerObjectAndTick(s32 obj)
{
	if ((gFrameCount & 2) != 0)
	{
		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 7);
	}
	else
	{
		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, 0);
	}
	if ((gFrameCount & 15) == 0)
	{
		BattleFx_SpawnScaledArcObjects(obj);
	}
}

//--------------------------------------------------------------------------
// 2 フレームごとに色 0-5 を順に回し、16 フレームごとに弧を出す
//--------------------------------------------------------------------------
void BattleFx_CycleObjectValueByCounter(s32 obj)
{
	if ((gFrameCount & 1) != 0)
	{
		s32 col = (gFrameCount >> 1) % 6;

		Animation_ApplyChildValuesFar((struct DispatchObject *)obj, col);
	}
	if ((gFrameCount & 15) == 0)
	{
		BattleFx_SpawnScaledArcObjects(obj);
	}
}

//--------------------------------------------------------------------------
// 演出スロット 24 個を動かす
//--------------------------------------------------------------------------
void BattleFx_UpdateAllEffectSlots(void)
{
	UpdateAllEffectSlots();
}

//--------------------------------------------------------------------------
// エナジー演出のワークを用意し、わざの演出を準備する
// 演出中にもう一度呼ばれたら、わざだけを入れ替える
//--------------------------------------------------------------------------
void BattleFx_LoadActionEffectResources(s32 act_no, s32 mode)
{
	struct FieldWork	 *ev = *(gWorkSlot + HEAP_SLOT_EVENT);
	struct MapScrollWork *map = *(gWorkSlot + 8);
	struct BattleFxScene *work;
	s32					  running;

	running = ev->resolving_action;
	if (running == 0)
	{
		volatile u32 fill;

		work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof *work);
		fill = running;
		Dma_Set((const void *)&fill, work,
			DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof *work / 4), REG_DMA3);
	}
	else
	{
		work = *(gWorkSlot + HEAP_SLOT_FX);
	}
	work->action = act_no;
	work->animation = BtlActGet(act_no)->type_0c;
	running = ev->resolving_action;
	if (running != 0)
		return;
	work->free_blocks = 512 - ResourceTable_CountFreeBlocks();
	work->mode = mode;
	work->visible = 1;
	work->enabled = 1;
	work->active = 1;
	work->message_mode = 1;
	// マップのゆれの大きさと減り方を取っておき、演出の後で戻す
	work->saved_x = map->shake_x;
	work->saved_y = map->shake_y;
	work->saved_z = map->shake_decay;
	if (gGameState.map == (s32)&SceneId_MakyuriIriguchi)
		work->lit = 1;
	if (gGameState.map == (s32)&SceneId_MakyuriHeya2)
		work->lit = 1;
	BattleFx_SetupObjectPair(gGameState.selected_actor, -1);
	if (work->animation != 8)
		ev->unknown_cc0 = running;
	work->tile_slot = Resource_FindFreeEntry();
	VramBlock_LoadCached((s16)work->tile_slot, 256, BattleFx_SparkTiles);
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}

//--------------------------------------------------------------------------
// 使う者と対象の物体を記録する
// 対象がなければ、使う者の向きに 16 ドット先を演出の位置にする
//--------------------------------------------------------------------------
void BattleFx_SetupObjectPair(s32 user_id, s32 tgt_id)
{
	struct ScriptMotionObject *user;
	struct ScriptMotionObject *tgt;
	s32						   dir;
	struct BattleFxScene	  *work;

	work = gEffectWork;
	work->first_object_id = user_id;
	user = ObjectTable_Get((s16)user_id);
	work->second_object_id = tgt_id;
	work->main_object = user;
	tgt = ObjectTable_Get((s16)tgt_id);
	dir = (user->facing + 0x2000) & 0xc000; // 四方向にまるめる
	work->child = tgt;
	work->angle = dir;
	if (tgt != NULL)
	{
		// 対象の動きと色を、あとで戻すために取っておく
		work->saved_callback = (void *)tgt->hook;
		work->saved_script = (const s32 *)tgt->script;
		{
			struct AnimationObject *anim = ((struct ObjectRuntime *)tgt)->animation;
			u8						pal = anim->entries[0]->param;

			work->saved_palette = pal;
		}
		work->x = tgt->x;
		work->z = tgt->z;
		work->y = tgt->y;
		return;
	}
	work->x = user->x;
	work->z = user->z;
	work->y = user->y;
	Vector_AddPolarOffset(0x100000, dir, &work->x);
}

//--------------------------------------------------------------------------
// 使う者の動きを止めて、色を戻す
//--------------------------------------------------------------------------
void EffectRuntime_StopCurrentObject(void)
{
	struct BattleFxScene	  *work = gEffectWork;
	struct ScriptMotionObject *user = work->main_object;

	user->hook = NULL;
	Animation_ApplyChildValuesFar(user, 0);
	WaitFrames(1);
}

struct BattleEffectPosition
{
	u8	reserved_00[4];
	s32 x;
	s32 y;
	s32 z;
};

#define BATTLE_OBJECT_SLOTS ((struct ScriptMotionObject *)gObjectSlots)

void BattleEffect_CleanupSceneObjects(void)
{
	struct BattleFxScene	   **scene_cell;
	struct BattleFxScene		*scene;
	struct FieldWork			*runtime;
	struct BattleEffectPosition *position;
	struct EffectSlot			*scene_object;
	s32							 remaining;

	scene_cell = (struct BattleFxScene **)&gEffectWork;
	scene = *scene_cell;
	runtime = *(struct FieldWork **)((u8 *)scene_cell - 116);
	position = *(struct BattleEffectPosition **)((u8 *)scene_cell - 192);
	for (remaining = 0; remaining < 24; remaining++)
	{
		scene_object = &scene->slots[remaining];
		if (scene_object->active != 0)
			BattleFx_ClearOwnedSlot(scene_object);
	}

	if (runtime->resolving_action == 0)
	{
		s32 waited = 0;
		void (*first_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectB;
		void (*second_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectA;
		s32 active;

		do
		{
			struct ScriptMotionObject *slot = BATTLE_OBJECT_SLOTS;

			active = 0;
			remaining = 0;
			while (remaining <= 63)
			{
				void (*update)(void) = (void (*)(void))slot->hook;

				if (update == first_active_update || update == second_active_update)
				{
					active = 1;
					break;
				}
				remaining++;
				slot++;
			}

			if (active != 0)
			{
				waited++;
				WaitFrames(1);
			}
		} while (active != 0 && waited <= 29);

		runtime->unknown_cc7 = 0;
		Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
		Resource_ResetEntry((s16)scene->tile_slot);
		position->x = scene->saved_x;
		position->y = scene->saved_y;
		position->z = scene->saved_z;
		if (scene->animation != 8)
			runtime->unknown_cc0 = 1;
		BattleFx_PlayQueuedSound();
		Runtime_ReleaseHeapBlock(0x38);
	}
}

void EventObject_Initialize(void)
{
	s32					  event_value;
	s32					  event_index;
	struct FieldActor	 *event_object;
	struct BattleFxScene *event_state;
	void				 *render_state;

	event_state = *(void **)Ram_EffectWork;
	render_state = *(void **)Ram_EventWork;
	event_object = event_state->main_object;
	event_value = (s16)event_state->action;
	event_index = event_value;
	ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x14);
	event_object->target_x = event_object->x.fixed;
	event_object->target_y = event_object->y.fixed;
	event_object->target_z = event_object->z.fixed;
	event_object->velocity_x = 0;
	event_object->velocity_y = 0;
	event_object->velocity_z = 0;
	if (event_state->visible != 0)
	{
		AudioCommand_PlayFar(212);
		event_object->update = (void *)BattleFx_FlickerObjectAndTick;
	}
	if (event_state->active != 0)
	{
		ObjectGroup_SetActionForOthers(event_object, 1, 0);
		UiWork_PushValueSlotFar(event_index, 4);
		if ((s8)event_state->mode != 0)
		{
			UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce, event_state->message_mode);
		}
		else
		{
			UiText_ShowPositionedMessageAndWaitFar(MsgAbilityAnnounce, event_state->message_mode);
		}
		ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
	}
	if (GameFlag_TestFar(0x140) != 0)
	{
		if (event_state->visible != 0)
		{
			event_object->update = (void *)BattleFx_CycleObjectValueByCounter;
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x15);
	}
	else
	{
		EffectRuntime_StopCurrentObject();
	}
	((struct FieldWork *)render_state)->unknown_cc7 = 1;
}

void BattleEffect_InitializeSharedScene(void)
{
	u8						   *scene;
	struct BattleEffectBuffers *work;
	s32							no;

	scene = *(gWorkSlot + HEAP_SLOT_EVENT);
	work = *(gWorkSlot + 32);
	Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
	if (((struct FieldWork *)scene)->effect_active == 0)
		Dma_Set(work->target, scene + 0x236, 0x84000150, REG_DMA3);
	Dma_Set(work->target, work->current, 0x84000000 | (sizeof work->current / 4), REG_DMA3);

	no = gFrameCount & 7;
	if (GameFlag_TestFar(0x148))
		no = 0;
	if (GameFlag_TestFar(0x149))
		no = 1;
	if (GameFlag_TestFar(0x14a))
		no = 2;
	if (GameFlag_TestFar(0x14b))
		no = 3;
	if (GameFlag_TestFar(0x14c))
		no = 4;
	if (GameFlag_TestFar(0x14d))
		no = 5;
	if (GameFlag_TestFar(0x14e))
		no = 6;
	if (GameFlag_TestFar(0x14f))
		no = 7;
	BattleFx_ApplyColorToTargetBuffer(BattleFx_TargetColors[no], 1);
	BattleFx_StartBufferInterpolation(8);
}

void BattleFx_PrepareBufferInterpolation(void)
{
	s32 value;
	u8 *state;

	state = (u8 *)gEventWork;
	value = (s32)(state + 0x236);
	BattleFx_ApplyColorToTargetBuffer(value, 2);
	if (((struct FieldWork *)state)->effect_active != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	}
	else
	{
		BattleFx_ApplyColorToTargetBuffer(value, 1);
	}
	BattleFx_StartBufferInterpolation(8);
}

void Camera_WorldToScreen(struct EffectVector *position)
{
	struct EventRuntime **data = (struct EventRuntime **)&gEventWork;

	if ((*data)->mode_19e == 3)
	{
		struct EffectVector result;

		Render_ProjectPoint(position, &result);
		position->x = result.x << 16;
		position->z = result.y << 16;
		position->y = 0;
	}
	else
	{
		struct MapScrollWork *state = *(struct MapScrollWork **)((u8 *)data - 76);
		s32					  x = state->view_x & 0xffff0000;
		s32					  z = state->view_y & 0xffff0000;

		position->x -= x;
		position->z -= position->y;
		position->z -= z;
		position->y = 0;
	}
}

// 以下では座標を関数ごとの構造体で渡す

// RunSceneTransitionEffect が 0x298 バイト確保して初期化する
struct WaveFxWork
{
	u16 lines[2][162]; // ラインごとのずれ (2 面を交互に書く)
	u16 phase;		   // 波の位相
	u8	page;		   // 書き込む面
	s8	red;		   // 色相から求めた色 (赤・緑・青、各 4〜11)
	s8	green;
	s8	blue;
	u16 hue;	   // 色相 (0〜359 度)
	u16 source_id; // 使い手
	u16 target_id; // 対象
	u8	delay;	   // 開始までの待ち
	u8	counter;   // 粒を出す周期 (0〜60)
};

void RunSceneTransitionEffect(s32 x, s32 y)
{
	u32				   __umodsi3(u32 numerator, u32 denominator);
	struct WaveFxWork *work;
	volatile u32	   zero;

	work = Runtime_AllocateBlock(22, 0x298);
	BattleEffect_InitializeSharedScene();
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x850000a6, REG_DMA3);
	work->hue = __umodsi3(gFrameCount << 1, 360);
	BattleFx_AdvanceHueCycle();
	BattleFx_ApplyColorToTargetBuffer(
		(work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
	BattleFx_StartBufferInterpolation(8);
	work->source_id = x;
	work->target_id = y;
	work->delay = 8;
	Ui_FillBank15PaletteGrey();
	Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene, 0xc80);
}

//--------------------------------------------------------------------------
// finish_scene_and_release_heap_block.c
// フィールド効果：ラインごとの波と色の変化、対象から使い手へ流れる粒
//--------------------------------------------------------------------------

struct WaveFxSprite
{
	u32 unknown_00[2];
	u8	unknown_08;
	u32 unknown_09_0 : 2;
	u32 mode : 2;
	u32 unknown_09_4 : 4;
	u8	unknown_0a[0x1c];
	u8	palette;
};

//--------------------------------------------------------------------------
// 効果を止めて使い手の色を戻し、作業領域を返す
//--------------------------------------------------------------------------
void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
	void *work;

	work = *(void **)((u32)&gBattleBgFxWork);
	Ui_SetBank15PaletteAndClearRenderMode();
	Scheduler_RemoveCallback(BattleFx_UpdateWaveScene);
	Animation_ApplyChildPalette(Object_GetById(((struct WaveFxWork *)work)->source_id), 1);
	BattleFx_PrepareBufferInterpolation();
	Runtime_ReleaseHeapBlock(22);
}

//--------------------------------------------------------------------------
// 毎フレームの処理 (スケジューラから呼ばれる)
//--------------------------------------------------------------------------
void BattleFx_UpdateWaveScene(void)
{
	struct WaveFxWork		 *work = gBattleBgFxWork;
	u16						 *line;
	u32						  i;
	struct FieldActor		 *src;
	struct FieldActor		 *dst;
	struct FieldActor		 *obj;
	struct WaveFxSprite		 *sprite;
	struct EffectVector		  from;
	struct EffectVector		  to;
	struct AnimationMetadata *meta;

	if (work->delay != 0)
	{
		work->delay--;
		return;
	}

	// 160 ライン分のずれを ±4 ドットの正弦波で作り、面を切り替える
	line = work->lines[work->page ^ 1];
	for (i = 0; i < 160; i++)
		*line++ = Trig_Sin(__udivsi3((work->phase + i * 8) << 16, 160)) >> 14;
	work->phase += 4;
	work->page ^= 1;
	if (work->page != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(
			(work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_AdvanceHueCycle();
	}
	Animation_ApplyChildPalette(Object_GetById(work->source_id), 0);

	// 周期の 0, 8, 16 で、対象から使い手へ向かう粒を出す
	if (work->counter == 0 || work->counter == 8 || work->counter == 16)
	{
		src = Object_GetById(work->source_id);
		dst = Object_GetById(work->target_id);
		if (src != 0 && dst != 0)
		{
			// メタデータの box_x を高さに使い、そこから 2 ドット下
			from.x = src->x.fixed;
			meta = Resource_GetMetadataRecordFar(
				(s16)BattleAction_FindDescriptor(work->source_id)->sprite);
			from.y = src->y.fixed + ((s8)meta->box_x << 16) - (2 << 16);
			from.z = src->z.fixed;
			to.x = dst->x.fixed;
			meta = Resource_GetMetadataRecordFar(
				(s16)BattleAction_FindDescriptor(work->target_id)->sprite);
			to.y = dst->y.fixed + ((s8)meta->box_x << 16) - (2 << 16);
			to.z = dst->z.fixed;
			obj = Object_CreateFar(281, to.x, to.y, to.z);
			if (obj != 0)
			{
				sprite = obj->sprite;
				obj->motion_flags = 0;
				obj->speed = 0xa3d7; // 0.64 倍
				obj->acceleration = 0xa3d7;
				obj->facing = ArcTan2(from.z - to.z, from.x - to.x);
				obj->update = BattleFx_SetCallbackWhenTargetUnset;
				sprite->palette = 0;
				sprite->mode = 1;
				Object_SetMoveTargetFar(obj, from.x, from.y, from.z);
			}
		}
	}

	if (work->counter == 0)
		AudioCommand_PlayFar(130);
	if (++work->counter > 60)
		work->counter = 0;
}

//--------------------------------------------------------------------------
// hue.c
// 背景の走査線スクロール DMA、色相の巡回、BG パレット 15 の切り替え
//--------------------------------------------------------------------------

#define GREY28 0x739c // 灰色 (赤・緑・青とも 28)

//--------------------------------------------------------------------------
// BG0 の横スクロールを走査線ごとに書き換える HBlank DMA を張りなおす
//--------------------------------------------------------------------------
void BattleFx_ArmBg0HBlankDma(void)
{
	struct WaveFxWork *work = gBattleBgFxWork;
	if (!work->delay)
	{
		u16			 *line = work->lines[work->page];
		volatile u16 *dma = REG_DMA0;
		dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
		dma[5] &= 0x7fff; // 止める
		(void)dma[5];
		// HBlank ごとに 16 ビットを 1 回 (リピート、転送先リロード)
		Dma_Set(line, REG_BG0HOFS, 0xa2600001, (volatile u32 *)dma);
	}
}

//--------------------------------------------------------------------------
// 色相を 4 度すすめ、その色を求める
//--------------------------------------------------------------------------
void BattleFx_AdvanceHueCycle(void)
{
	struct WaveFxWork *work = gBattleBgFxWork;
	s32				   r = 0;
	s32				   g = 0;
	s32				   b = 0;
	s32				   ofs;
	u8				  *pr;
	u8				  *pg;
	u8				  *pb;

	BattleFx_ComputeHueChannels(work->hue << 16, &r, &g, &b);

	// 0〜31 を 4〜11 に縮める (ofs は rgb の位置から 1 ずつ進む)
	ofs = 0x28b;
	pr = (u8 *)work + ofs;
	*pr = (r >> 18) + 4;
	ofs += 1;
	pg = (u8 *)work + ofs;
	*pg = (g >> 18) + 4;
	ofs += 1;
	pb = (u8 *)work + ofs;
	*pb = (b >> 18) + 4;

	work->hue += 4;

	*pr &= 0x1f;
	*pg &= 0x1f;
	*pb &= 0x1f;

	if (work->hue >= 360)
	{
		work->hue = 0;
	}
}

//--------------------------------------------------------------------------
// 色相から赤・緑・青を求める (各 0〜31、16.16 固定小数)
// hue : 色相 (度、16.16 固定小数)
//--------------------------------------------------------------------------
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b)
{
	*r = BattleFx_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
	*g = BattleFx_HueChannelRamp(hue, 0, 31 << 16);
	*b = BattleFx_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

//--------------------------------------------------------------------------
// 色相に対する色の強さ
// 0〜60 度で上がり、60〜180 度は high、180〜240 度で下がり、ほかは low
//--------------------------------------------------------------------------
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high)
{
	angle = Fixed_Remainder(angle, 360 << 16);
	if (angle < 60 << 16)
		return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, angle));
	if (angle >= 60 << 16 && angle < 180 << 16)
		return high;
	if (angle >= 180 << 16 && angle < 240 << 16)
		return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, (240 << 16) - angle));
	return low;
}

//--------------------------------------------------------------------------
// 16.16 固定小数の剰余 (divisor が 0 なら 0)
//--------------------------------------------------------------------------
s32 Fixed_Remainder(s32 value, s32 divisor)
{
	s32 quot;

	if (divisor == 0)
		return 0;
	if (divisor & 0xf0000000)
		divisor = -divisor;
	quot = Iwram_RatioMulQ14(divisor, value);
	return value - Iwram_MulQ16(quot & 0xffff0000, divisor); // 商の整数部 × divisor を引く
}

//--------------------------------------------------------------------------
// 移動の目標がなければ、目標なしのスクリプトを始める
//--------------------------------------------------------------------------
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *obj)
{
	s32 ty;
	s32 tx;

	tx = obj->target_x;
	if (tx == 0x80000000) // 目標なし
	{
		ty = obj->target_y;
		if ((ty == tx) && (obj->target_z == ty))
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_UntargetedObjectScript);
		}
	}
}

//--------------------------------------------------------------------------
// BG パレット 15 の色 1, 3〜12 を灰色にし、走査線スクロールを始める
//--------------------------------------------------------------------------
void Ui_FillBank15PaletteGrey(void)
{
	volatile u16 *p;

	((struct UiRenderWork *)gWindowWork[0])->mode = 1;
	p = &BG_PLTT_COLOR(15, 1);
	*p = GREY28;
	p += 2;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	p += 1;
	*p = GREY28;
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg0HBlankDma, 0x480);
}

//--------------------------------------------------------------------------
// 走査線スクロールを止め、BG パレット 15 を戻す
//--------------------------------------------------------------------------
void Ui_SetBank15PaletteAndClearRenderMode(void)
{
	void *work;

	work = gWindowWork[0];
	Scheduler_RemoveCallback(BattleFx_ArmBg0HBlankDma);
	BG_PLTT_COLOR(15, 1) = 0x7fff;	// 白
	BG_PLTT_COLOR(15, 3) = 0;		// 黒
	BG_PLTT_COLOR(15, 11) = 0x294a; // 灰 (10)
	BG_PLTT_COLOR(15, 12) = 0x5294; // 灰 (20)
	PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	((struct UiRenderWork *)work)->mode = 0;
}

//--------------------------------------------------------------------------
// pattern.c
// バトル効果：十字キーの入力を向きに変える
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 押している十字キーの向き（0x10000 で 1 周）を返す
// 右 0、下 0x4000、左 0x8000、上 0xc000
// 何も押していないとき、斜めのときは 0xffff
//--------------------------------------------------------------------------
s32 BattleFx_GetCycledTableWord(u32 unused)
{
	return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15]; // 右・左・上・下の 4 ビット
}

//--------------------------------------------------------------------------
// fragment.c
// 演出：アイテムがこわれる演出の本体の動き
//--------------------------------------------------------------------------

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を読むための見え方
struct ItemBreakFragmentObject
{
	u8	reserved_00[72];
	s32 gravity;
	u8	reserved_4c[9];
	u8	motion_flags;
	u8	reserved_56[8];
	u16 unknown_5e;
};

//--------------------------------------------------------------------------
// こわれたアイテムの本体の update
// 向く相手がいれば、そちらへ 1 回に 0x1000（22.5 度）まで曲がる
// そして、まわりにかけらの粒をひとつ出す
// src : 本体のオブジェクト
//--------------------------------------------------------------------------
void BattleFx_UpdateItemBreakFragment(struct FieldActor *src)
{
	struct FieldActor			   *tgt;
	struct EffectVector				pos;
	struct ItemBreakFragmentObject *obj;
	s32								turn;
	s32								dist;

	tgt = src->linked_object;
	if (tgt != 0)
	{
		s32 dx = tgt->x.fixed - src->x.fixed;
		s32 dz = tgt->z.fixed - src->z.fixed;

		if (dx != 0 || dz != 0)
		{
			turn = (s16)(ArcTan2(dz, dx) - src->facing);
			if (turn > 0x1000)
				turn = 0x1000;
			if (turn < -0x1000)
				turn = -0x1000;
			src->facing += turn;
		}
		src->steering_flags = 0;
	}

	// 粒は本体から y を 8〜24 ずらし、ランダムな向きに半径 0〜48 散らした位置に出す
	pos.x = src->x.fixed;
	pos.y = src->y.fixed - (Random16() << 4) - 0x80000;
	pos.z = src->z.fixed;
	dist = Random16() * 3;
	dist <<= 4;
	Vector_AddPolarOffset(dist, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = 2;
		obj->gravity = 0x1999;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// fieldfx.c
// フィールド効果：方向キーで対象を押す処理、出現と消滅の粒、範囲の見張り
//--------------------------------------------------------------------------

#define CELL_MASK 0xfff00000 // 16 ドットのマス

//--------------------------------------------------------------------------
// 対象のそばに押すオブジェクトを出し、方向キーの向きへ対象を 16 ドット押す
// A, B, L, R で取りやめる。押せないときは 16 フレームごとにエラー音
// 押す先が使い手と重なるときは、使い手の位置しだいで押せないか、使い手も動かす
//--------------------------------------------------------------------------
void BattleFx_SteerPushedTarget(s32 target_id)
{
	struct EffectVector	  position;
	struct EffectVector	  beyond;
	struct BattleFxScene *work;
	struct FieldActor	 *user;
	struct FieldActor	 *target;
	struct FieldActor	 *pusher;
	struct FieldActor	 *hit;
	s32					  x;
	s32					  z;
	s32					  angle;
	s32					  moved;
	s32					  dir;
	s32					  keys;
	s32					  cell_x;
	s32					  cell_z;
	struct EffectVector	 *p;

	work = gEffectWork;
	user = work->main_object;
	target = work->child;
	angle = work->angle + 0x8000; // work->angle の反対向き
	moved = 0;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	user->linked_object = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)user, BattleFx_SourceHoldScript);
	pusher = BattleFx_StartItemBreak(user);
	if (pusher == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}

	// 押すオブジェクトは対象の 16 ドット上、angle の向きへ 16 ドットの位置
	pusher->linked_object = target;
	p = &position;
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	Vector_AddPolarOffset(16 << 16, angle, p);
	Object_SetMoveTargetFar(pusher, p->x, p->y, p->z);
	BattleFx_SnapScaleToFull((struct MotionObject *)pusher);
	pusher->speed = 4 << 16;
	pusher->acceleration = 0x8000;
	pusher->motion_flags = 4;
	target->update = ObjectGroup_ApplyRandomChildValues; // 押す間は色を巡らせる
	target->speed = 0x6666;
	target->acceleration = 0x3333;
	target->steering_flags = moved;
	target->terrain_id = 2;
	for (;;)
	{
		WaitFrames(1);
		keys = gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if (keys != 0)
			break;

		// 方向キーが離れていれば押すオブジェクトを元の位置へ戻す
		dir = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (dir == 0xffff)
		{
			position.x = target->x.fixed;
			position.y = target->y.fixed + (16 << 16);
			position.z = target->z.fixed;
			Vector_AddPolarOffset(16 << 16, angle, &position);
			Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 1);
			pusher->velocity_x = 0;
			pusher->velocity_y = 0;
			pusher->velocity_z = 0;
			continue;
		}

		// 押すオブジェクトを押す向きへ 2 ドット寄せ、対象の 16 ドット先と 32 ドット先を調べる
		position.x = target->x.fixed;
		position.y = target->y.fixed + (16 << 16);
		position.z = target->z.fixed;
		Vector_AddPolarOffset(16 << 16, angle, &position);
		Vector_AddPolarOffset(2 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y, position.z);
		Script_WaitForEventTimeoutFar(pusher);
		position.x = target->x.fixed;
		position.y = target->y.fixed;
		position.z = target->z.fixed;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		beyond.x = target->x.fixed;
		beyond.y = target->y.fixed;
		beyond.z = target->z.fixed;
		Vector_AddPolarOffset(32 << 16, dir, &beyond);
		if (Map_CheckObjectStepFar(target, &position) > 0)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		hit = ScriptObject_FindOverlappingEntryFar(target, p);
		if (hit == NULL)
			goto move;
		if (hit != user)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}

		// 押す先が使い手と重なる：使い手のマスが押す先なら押せない。使い手が 32 ドット先の
		// マスにいれば、そこが通れるときだけ使い手もそこへ動かす
		cell_x = user->x.fixed & CELL_MASK;
		cell_z = user->z.fixed & CELL_MASK;
		if (cell_x == (p->x & CELL_MASK) && cell_z == (p->z & CELL_MASK))
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		if (cell_x != (beyond.x & CELL_MASK) || cell_z != (beyond.z & CELL_MASK))
			goto move;
		if (Map_GetCellAttributeLowNibbleFar(user->terrain_id, beyond.x, beyond.z) != 0)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(pusher, 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		moved = 1;

		// 押す：押すオブジェクトの動作は angle と押す向きの差 (90° 単位) で選ぶ
	move:
		AudioCommand_PlayFar(175);
		x = position.x;
		z = position.z;
		ObjectDispatch_ApplyArgumentToChildrenFar(
			pusher, BattleFx_QuadrantModes[(u16)(angle - dir) >> 14]);
		WaitFrames(15);
		target->paused = 0;
		target->speed = 0x3333;
		target->acceleration = 0x3333;
		Object_SetMoveTargetFar(target, position.x, position.y, position.z);
		pusher->motion_flags = 0;
		pusher->speed = 0x3333;
		pusher->acceleration = 0x3333;
		Vector_AddPolarOffset(16 << 16, dir, &position);
		Object_SetMoveTargetFar(pusher, position.x, position.y + (16 << 16), position.z);
		if (moved == 1)
		{
			Object_GetById(work->first_object_id)->steering_flags &= 0xfe;
			user->speed = 0x3333;
			user->acceleration = 0x3333;
			Object_SetMoveTargetFar(user, beyond.x, beyond.y, beyond.z);
		}
		Script_WaitForEventTimeoutFar(target);
		target->x.fixed = x;
		target->z.fixed = z;
		target->velocity_x = 0;
		target->velocity_z = 0;
		break;
	}

	// 対象の色、スクリプト、毎フレームの処理を元に戻し、押すオブジェクトを消す
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->update = work->saved_callback;
	EffectRuntime_StopCurrentObject();
	if (moved == 1)
		(Object_GetById(work->first_object_id))->steering_flags |= 1;
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(pusher);
}

//--------------------------------------------------------------------------
// 粒の動き：元の位置から 30 ドット先へ飛び、また元の位置へ戻って消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot)
{
	struct EffectVector position;
	s8				   *state_ptr = &slot->state;
	s32					state;

next_state:
	state = *state_ptr;
	if (state == 0)
	{
		u32 angle;

		position.x = slot->origin_x;
		position.z = slot->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(30 << 16, (u16)angle, &position);
		slot->target_x = position.x;
		slot->target_z = position.z;
		slot->acceleration = 4 << 16;
		slot->max_speed = 4 << 16;
		slot->flag42 = state;
		(*state_ptr)++;
		return;
	}

	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			(*state_ptr)++;
			goto next_state;
		}
		return;
	}

	if (state == 2)
	{
		slot->target_x = slot->origin_x;
		slot->target_z = slot->origin_z;
		{
			u32 value = 0x400;

			slot->max_turn_step = value;
		}
		slot->flag42 = 1;
		(*state_ptr)++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 使い手のそばにオブジェクトを出し、30 フレーム見せてから粒にして消す
//--------------------------------------------------------------------------
void BattleFx_RunItemBreakSequence(void)
{
	s32					  work[3];
	struct BattleFxScene *scene;
	void				 *object;

	scene = gEffectWork;
	object = scene->main_object;

	/* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
	do
	{
		BattleEffect_InitializeSharedScene();
	} while (0);
	object = BattleFx_StartItemBreak(object);
	BattleFx_SnapScaleToFull(object);
	if (object != 0)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 4);
		WaitFrames(30);
	}
	BattleFx_PrepareBufferInterpolation();
	UpdateRisingParticleBurst(object);
}

//--------------------------------------------------------------------------
// object の 16 ドット上にオブジェクト 215 を出し、向いている側
// (90° 単位) へ 16 ドット動かす。周りに粒 8 個を散らす
// 戻り値：出したオブジェクト (出せなければ 0)
//--------------------------------------------------------------------------
void *BattleFx_StartItemBreak(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *parent;
	struct MotionObject *child;
	s32					 angle;
	s32					 dist;
	s32					 speed;
	s32					 i;
	u32					 spread;
	u32					 jitter;

	angle = (src->angle + 0x2000) & 0xc000; // いちばん近い 90° の向き
	parent = Object_Spawn(215, src->x, src->y + (16 << 16), src->z);
	if (parent == 0)
		return 0;
	parent->scale_y = 0x4000; // 0.25 倍から
	parent->scale_x = 0x4000;
	parent->update = BattleFx_UpdateItemBreakFragment;
	parent->speed_limit = 2 << 16;
	parent->acceleration = 2 << 16;
	parent->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(parent, 3);
	Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

	// 粒：向きの ±45° へ 8〜32 ドット、速さ 1〜2 ドット
	for (i = 7; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y + (16 << 16), src->z);
		if (child != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16() + 0x10000;
			child->acceleration = 1 << 16;
			child->speed_limit = speed;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0x51e;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24 + (8 << 16);
			jitter = Random16();
			Motion_SetTargetPositionFromMagnitudeAngle(
				child, dist, ((jitter - Random16()) >> 3) + src->angle);
		}
	}
	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	return parent;
}

//--------------------------------------------------------------------------
// 拡大率を 1/16 刻みで等倍まで上げ、位置を確定する
//--------------------------------------------------------------------------
void BattleFx_SnapScaleToFull(struct MotionObject *obj)
{
	s32 next;
	s32 scale;

	if (obj != NULL)
	{
		scale = obj->scale_x;
		if (scale <= 0xffff)
		{
			do
			{
				next = scale + 0x1000;
				scale = next;
			} while (next <= 0xffff);
			obj->scale_x = next;
			obj->scale_y = next;
		}
		Script_WaitForEventTimeoutFar(obj);
	}
}

//--------------------------------------------------------------------------
// 31 フレームかけて回りながら上り、縮んでから粒 8 個になって消える
//--------------------------------------------------------------------------
void UpdateRisingParticleBurst(void *object)
{
	struct MotionObject *src = object;
	struct MotionObject *child;
	s32					 i;
	s32					 dist;
	u32					 spread;

	AudioCommand_PlayFar(154);
	for (i = 30; i >= 0; i--)
	{
		src->y += 1 << 16;
		src->angle += 0x2000; // 45°
		src->scale_x += -0x800;
		src->scale_y += -0x800;
		WaitFrames(1);
	}
	for (i = 7; i >= 0; i--)
	{
		child = Object_Spawn(285, src->x, src->y, src->z);
		if (child != 0)
		{
			s32 speed;
			/* FAKEMATCH: needed to match: a block-local 1.0 keeps 0x10000 in one register for both uses */
			s32 unit = 0x10000;

			ObjectDispatch_InitializeFar((struct DispatchObject *)child, BattleFx_FragmentScript);
			speed = Random16();
			child->acceleration = unit;
			child->speed_limit = speed + unit;
			child->motion_flags = 2;
			child->vertical_motion_strength = 0xa3d;
			spread = Random16();
			child->velocity_y = spread - Random16();
			dist = Random16() * 24;
			dist += 8 << 16;
			Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(src);
}

//--------------------------------------------------------------------------
// 64 個のオブジェクトのうち、最初のパーツが絵 200 のものへ value を渡す
//--------------------------------------------------------------------------
void ObjectDispatch_ApplyValueToKind200Children(s32 value)
{
	struct ObjectRuntime *object = gObjectSlots;
	s32					  cnt = 63;

	do
	{
		if (object->script != NULL)
		{
			u8 kind = object->animation_kind;

			if (kind == 1)
			{
				struct AnimationObject *anim = object->animation;
				struct AnimationEntry  *entry = anim->entries[0];

				if (entry->anim_id == 200)
				{
					entry->param = value;
					anim->dirty = kind;
				}
			}
		}
		cnt--;
		object++;
	} while (cnt >= 0);
}

//--------------------------------------------------------------------------
// 効果の中心から操作キャラが 60 ドット離れるか、残り時間が尽きたら終了を頼む
// (中心と時間は RunBattleEffect08 が gEventWork の 0xcb8〜 に置く)
//--------------------------------------------------------------------------
void FieldEffect_WatchLeaderDistance(void)
{
	struct FieldWork	 *work;
	struct ObjectRuntime *actor;
	s16					 *timer;
	s32					  dx;
	s32					  dz;

	work = (struct FieldWork *)gEventWork;
	actor = ObjectTable_Get(gGameState.selected_actor);
	if (work->unknown_cc0 != 0) // 時間を減らすか
	{
		timer = &work->effect_frames;
		if (*timer != 0)
			(*timer)--;
	}

	// x は 0xd105 (約 0.82) 倍して比べる
	dx = Iwram_MulQ16(work->effect_center_x - actor->x / 0x10000, 0xd105);
	dz = work->effect_center_y - (actor->z - actor->y) / 0x10000;
	if (dx * dx + dz * dz >= 3600 || work->effect_frames == 0)
	{
		u16 *slot = (u16 *)&work->command; // psynergy_request
		u32	 request = 0x2090;

		*slot = request;
	}
}

//--------------------------------------------------------------------------
// markers.c
// 戦闘エフェクト 8 番：選択中のオブジェクトを中心に画面を切りかえ、印を出す
//--------------------------------------------------------------------------
// gEventWork から並ぶワークのポインタ
struct EffectGlobals
{
	struct FieldWork			  *scene; // イベントのワーク
	u8							   unknown_04[12];
	struct DisplayTransitionState *work; // 画面切りかえのワーク
};

//--------------------------------------------------------------------------
// 戦闘エフェクト 8 番
// 選択中のオブジェクトの位置を中心に画面切りかえを始め、
// 合うイベントがあれば動かしてから、まわりに印を出す
//--------------------------------------------------------------------------
void RunBattleEffect08(void)
{
	struct FieldWork			  *scene;
	struct DisplayTransitionState *work;
	struct FieldActor			  *target;
	s32							   arg;
	s32							   ev;
	s32							   frame;

	scene = ((struct EffectGlobals *)&gEventWork)->scene;
	ObjectDispatch_ApplyValueToKind200Children(6);
	DisplayTransition_InitializeBattleEffectState(8);
	work = ((struct EffectGlobals *)&gEventWork)->work;
	target = ObjectTable_Get(gGameState.selected_actor);
	work->centre_x = target->x.fixed;
	work->centre_y = target->z.fixed - target->y.fixed;
	BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
	BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	BattleFx_StartBufferInterpolation(1);
#if !defined(TBS_EDITION_EN)
	scene->effect_active = 0; // 英語版にはない
#endif
	WaitFrames(1);
	ev = BattleFx_FindMatchingEvent(0x50000005, 8, &arg);
	if (ev != 0)
		BattleFx_RunEventAction(ev, gGameState.selected_actor, arg);
	AudioCommand_PlayFar(131);

	// イベントのワークに中心 (ドット) と時間を入れ、近くに印を出す
	scene->effect_active = 1;
	scene->effect_center_x = work->centre_x / 0x10000;
	scene->effect_center_y = work->centre_y / 0x10000;
	scene->effect_frames = 600;
	scene->unknown_cc0 = 1;
	FieldEffect_SpawnNearbyMarkers();
	frame = 0;
	do
	{
		WaitFrames(1);
		work->value = frame;
		frame++;
	} while (frame <= 18);
	Scheduler_AddOrUpdateCallback(FieldEffect_WatchLeaderDistance, 0xc80);
}

//--------------------------------------------------------------------------
// burstobj.c
// エナジー演出：イマジンの打ち切り、演出 12 の粒、演出 7 で対象を持ち上げる
//--------------------------------------------------------------------------

#define ENA_EV_4 0x40000005 // エナジーのイベント、段階 4
#define FX_IMAJIN 8			// イマジンの演出
#define TSUBU_OBJ 284		// 粒の物体
#define FALL_OBJ 217		// 落ちる物体
#define FALL_LAST 4			// 落ちる物体は 0-4 の 5 個

struct BurstParticleVector
{
	s32 values[3]; // x, y, z
};

// 粒のスプライトの属性
struct BurstParticleVisual
{
	u8 reserved_00[5];
	u8 flags_a_low : 5;
	u8 flip : 1;
	u8 flags_a_high : 2;
	u8 reserved_06;
	u8 flags_b_low : 6;
	u8 flags_b_high : 2;
	union
	{
		struct
		{
			u16 shape : 10;
			u16 unused : 6;
		} bits;
		struct
		{
			u8 lo;
			u8 high_low : 4;
			u8 palette : 4;
		} bytes;
	} attributes;
};

struct BurstParticleVisualGroup
{
	struct BurstParticleVisual primary;
	struct BurstParticleVisual child;
};

//--------------------------------------------------------------------------
// イマジンの効果中なら打ち切る
// 見張りをやめ、色を黒から 30 フレームで戻し、イマジンの段階 4 のイベントを実行する
//--------------------------------------------------------------------------
void ResetSceneTransitionEffect(void)
{
	// gEffectWork はスロット 56。0x74 前がイベントワーク (27)、0x64 前がスロット 31
	struct BattleFxScene		 **cell = &gEffectWork;
	struct BattleFxScene		  *work = *cell;
	struct DisplayTransitionState *scene = *(struct DisplayTransitionState **)((u8 *)cell - 0x64);
	struct FieldWork			  *state = *(struct FieldWork **)((u8 *)cell - 0x74);
	s16							   zero;
	s32							   tid;
	void						  *ev;

	if (state->effect_active != 0)
	{
		AudioCommand_PlayFar(SOUND_SCENE_TRANSITION);
		Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);

		zero = 0;
		state->effect_active = zero;
		state->effect_frames = zero;
		ObjectDispatch_ApplyValueToKind200Children(0);

		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 1);
		BattleFx_StartBufferInterpolation(1);
		BattleFx_ApplyColorToSourceBuffer(0, 0);
		BattleFx_ApplyColorToTargetBuffer(PALETTE_TRANSFORM_PLAIN, 0);
		BattleFx_StartBufferInterpolation(30);
		WaitFrames(1);

		ev = BattleFx_FindMatchingEvent(ENA_EV_4, FX_IMAJIN, &tid);
		if (ev != NULL)
			BattleFx_RunEventAction(ev, gGameState.selected_actor, tid);

		if (work->child_option == 0)
		{
			scene->next_mode = 0;
			scene->duration = 1;
			scene->step = 1;
			WaitFrames(10);
		}
	}
}

//--------------------------------------------------------------------------
// 対象のある演出 12：対象のまわりに粒を出し、対象を足元の優先度にする
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticleMainObject(s32 unused)
{
	struct FieldActor *tgt;

	tgt = ((struct BattleFxScene *)gEffectWork)->child;
	if (tgt != NULL)
	{
		BattleEffect_SpawnBurstParticleField();
		ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		tgt->collision_flags = 0;
		ObjectDispatch_SetSingleChildFlagsFar(tgt, 0);
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		WaitFrames(10);
		AudioCommand_PlayFar(SOUND_RECOVERY);
		WaitFrames(40);
		BattleFx_PrepareBufferInterpolation();
	}
}

//--------------------------------------------------------------------------
// 対象のない演出 12：粒を出したあと、まわり 4-10 ドットに落ちる物体を 5 個出す
//--------------------------------------------------------------------------
void BattleFx_RunBurstParticles(void)
{
	struct BattleFxScene	   *work = gEffectWork;
	struct BurstParticleVector	pos;
	struct BurstParticleVector *p;
	s32							n;

	BattleEffect_SpawnBurstParticleField();
	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	p = &pos;
	n = FALL_LAST;
	do
	{
		struct FieldActor *obj;
		s32				   dist;

		p->values[0] = work->x;
		p->values[2] = work->z;
		dist = (Random16() * 6) + 0x40000;
		Vector_AddPolarOffset(dist, Random16(), p);
		p->values[1] = work->y;
		obj = Object_Spawn(FALL_OBJ, p->values[0], p->values[1], p->values[2]);
		if (obj != 0)
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript);
			obj->motion_flags = ACTOR_FALLS;
		}
		WaitFrames(((Random16() * 2) >> 16) + 2);
		n--;
	} while (n >= 0);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 使う者の前から粒を 24 個、2 フレームおきに出し、演出の位置のまわりへ飛ばす
// 出る位置：向き 0x4000 は 10 ドット上、0xc000 は 24 ドット上、ほかは 10 ドット上の 10 ドット前
//--------------------------------------------------------------------------
void BattleEffect_SpawnBurstParticleField(void)
{
	struct BattleFxScene *work;
	struct FieldActor	 *user;
	s32					  pos[3];
	s32					  n;

	work = gEffectWork;
	user = work->main_object;
	BattleEffect_InitializeSharedScene();

	for (n = 0; n < TSUBU_MAX; n++)
	{
		struct FieldActor		   *obj;
		struct BurstParticleVisual *visual;
		struct BurstParticleVisual *child;
		s32							dist;

		if (work->angle == 0x4000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
		}
		else if (work->angle == 0xc000)
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0x180000;
			pos[2] = user->z.fixed;
		}
		else
		{
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + 0xa0000;
			pos[2] = user->z.fixed;
			Vector_AddPolarOffset(0xa0000, work->angle, pos);
		}

		// 子のスプライトに、向きと形と色を写す
		obj = Object_Spawn(TSUBU_OBJ, pos[0], pos[1], pos[2]);
		visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
		child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
		child->flip = visual->flip;
		child->flags_a_high = visual->flags_a_high;
		child->flags_b_high = visual->flags_b_high;
		child->attributes.bits.shape = visual->attributes.bits.shape;
		child->attributes.bytes.palette = visual->attributes.bytes.palette;

		if (obj != 0)
		{
			obj->scale_y = 0xb333; // 0.7 倍
			obj->scale_x = 0xb333;
			obj->acceleration = 0x18000;
			obj->speed = 0x18000;
			obj->motion_flags = 0;
			Animation_ApplyChildValuesFar(obj, 11);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, 7);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript + 1);
			ObjectDispatch_SetSingleChildFlagsFar(obj, 1);

			// 行き先：演出の位置から 4-10 ドット
			pos[0] = work->x;
			pos[1] = work->y;
			pos[2] = work->z;
			if (work->angle == 0xc000)
				Vector_AddPolarOffset(0xe0000, work->angle, pos);
			dist = Random16() * 6 + 0x40000;
			Vector_AddPolarOffset(dist, Random16(), pos);
			Object_SetMoveTargetFar(obj, pos[0], pos[1], pos[2]);
		}
		AudioCommand_PlayFar(131);
		WaitFrames(2);
	}
	WaitFrames(8);
}

//--------------------------------------------------------------------------
// 対象のある演出 7
// 左右 32 ドットに物体を出して 14 ドット動かし、対象といっしょに 32 ドット持ち上げる
//--------------------------------------------------------------------------
void BattleEffect_RunTargetedItemBreak(s32 unused)
{
	struct BattleFxScene  *work;
	struct DispatchObject *user;
	struct FieldActor	  *tgt;
	struct MotionObject	  *pair[2];
	s32					   pos[3];
	s32					   x, y, z;
	s32					   n;
	s32					   start_y;

	work = gEffectWork;
	tgt = work->child;
	user = work->main_object;
	if (tgt == NULL)
		return;

	BattleEffect_InitializeSharedScene();
	user->argument = (s32)tgt;
	ObjectDispatch_InitializeFar(user, BattleFx_CommonParticleScript + 3);

	x = work->x;
	pos[0] = x;
	y = work->y + 0x100000; // 16 ドット上
	pos[1] = y;
	z = work->z;
	pos[2] = z;
	pair[0] = BattleFx_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
	pair[1] = BattleFx_SpawnItemBreakMode3(pos[0] - 0x200000, pos[1], pos[2], 0);

	WaitFrames(15);
	for (n = 0; n < 2; n++)
	{
		struct MotionObject *obj = pair[n];
		if (obj != NULL)
			Motion_SetTargetPositionFromMagnitudeAngle(obj, 0xe0000, obj->angle);
	}

	Script_WaitForEventTimeoutFar(pair[0]);
	tgt->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(130);
	tgt->motion_flags = 4;

	// 1/4 ドットずつ 32 ドット上げる
	start_y = tgt->y.fixed;
	if (pair[0] != NULL && pair[1] != NULL && start_y <= start_y + 0x200000)
	{
		do
		{
			pair[0]->y += 0x4000;
			pair[1]->y += 0x4000;
			tgt->y.fixed += 0x4000;
			WaitFrames(1);
		} while (tgt->y.fixed <= start_y + 0x200000);
	}

	UpdateRisingParticleBurst(pair[0]);
	UpdateRisingParticleBurst(pair[1]);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// anchor.c
// エナジー演出 7：左右の二つの物体と、飛び散る破片
//--------------------------------------------------------------------------

// 破片の数
#define HAHEN_MAX 24

//--------------------------------------------------------------------------
// 対象のない演出の本体：演出の位置の 16 ドット上、左右 32 ドットに spawn で
// 物体を出し、それぞれの向きへ 24 ドット動かしてから、破片を 24 個、
// 8-32 ドット先へ散らす
//--------------------------------------------------------------------------
#define DEFINE_ITEM_BREAK_EFFECT(name, spawn)                                          \
	void name(void)                                                                    \
	{                                                                                  \
		struct BattleFxScene  *work;                                                   \
		struct MotionObject	  *obj;                                                    \
		struct MotionObject	  *pair[2];                                                \
		struct ScatterPosition pos;                                                    \
		struct MotionObject	 **walk;                                                   \
		s32					   n;                                                      \
		s32					   dist;                                                   \
                                                                                       \
		work = gEffectWork;                                                            \
		BattleEffect_InitializeSharedScene();                                          \
		pos.x = work->x;                                                               \
		pos.y = work->y + 0x100000;                                                    \
		pos.z = work->z;                                                               \
		pair[0] = spawn(pos.x + 0x200000, pos.y, pos.z, 0x8000);                       \
		pair[1] = spawn(pos.x - 0x200000, pos.y, pos.z, 0);                            \
                                                                                       \
		WaitFrames(15);                                                                \
		walk = pair;                                                                   \
		for (n = 0; n < 2; n++)                                                        \
		{                                                                              \
			obj = *walk++;                                                             \
			if (obj != 0)                                                              \
				Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->angle); \
		}                                                                              \
                                                                                       \
		Script_WaitForEventTimeoutFar(pair[0]);                                        \
		AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);                                      \
		for (n = 0; n < HAHEN_MAX; n++)                                                \
		{                                                                              \
			pos.x = work->x;                                                           \
			pos.y = work->y + 0x100000;                                                \
			pos.z = work->z;                                                           \
			obj = Object_Spawn(285, pos.x, pos.y, pos.z);                              \
			if (obj != 0)                                                              \
			{                                                                          \
				ObjectDispatch_InitializeFar(                                          \
					(struct DispatchObject *)obj, BattleFx_FragmentScript);            \
				obj->speed_limit = Random16() + 0x20000;                               \
				obj->acceleration = 0x20000;                                           \
				obj->motion_flags = 0;                                                 \
				dist = Random16() * 24 + 0x80000;                                      \
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());     \
			}                                                                          \
		}                                                                              \
                                                                                       \
		ObjectDispatch_ReleaseFar(pair[0]);                                            \
		ObjectDispatch_ReleaseFar(pair[1]);                                            \
		BattleFx_PrepareBufferInterpolation();                                         \
	}

// 対象のない演出 7
DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect07, BattleFx_SpawnItemBreakMode3)

//--------------------------------------------------------------------------
// 物体 215 を出し、回しながら 1/4 から等倍まで 24 フレームで大きくする
// angle : 大きくなったあとの向き
//--------------------------------------------------------------------------
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_x = obj->scale_y = 0x4000;
		obj->acceleration = obj->speed_limit = 0x30000; // 3 ドット/フレーム
		((struct FieldSprite *)obj->records)->priority = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000; // 1/8 回転
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

//--------------------------------------------------------------------------
// impact.c
// 散って集まる粒の動きと、重い衝撃の破片
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 粒の動き : 原点のまわりへ散ってから、主オブジェクトのそばへ集まって消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectVector	  pos;
	s32					  state;

again:
	state = slot->state;
	if (state == 0)
	{
		// 原点から 25 ドット、でたらめな方向へ
		pos.x = slot->origin_x;
		pos.z = slot->origin_z;
		Vector_AddPolarOffset(25 << 16, (u16)Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 3 << 16;
		slot->max_speed = 3 << 16;
		slot->flag42 = 0;
		slot->state++;
		return;
	}

	if (state == 1)
	{
		// 着いたらすぐ次へ
		if (BattleFx_HasReachedTarget(slot) == 0)
		{
			slot->state++;
			goto again;
		}
		return;
	}

	if (state == 2)
	{
		// 主オブジェクトの 16 ドット上から scene->angle へ 8 ドット、
		// 画面座標にして 4 ドットの揺らぎを足す
		struct MotionObject *obj = scene->main_object;

		pos.x = obj->x;
		pos.y = obj->y + (16 << 16);
		pos.z = obj->z;
		Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->max_turn_step = 0x800;
		slot->flag42 = 1;
		slot->state++;
		return;
	}

	if (state == 3 && BattleFx_HasReachedTarget(slot) == 0)
		BattleFx_ClearOwnedSlot(slot);
}

//--------------------------------------------------------------------------
// 重い衝撃 : y を 32 ドット下げた位置に 0x11b を置き、破片を 12 個まき散らす
// src : 衝撃の位置になるオブジェクト
//--------------------------------------------------------------------------
s32 SpawnHeavyImpactEffect(struct MotionObject *src)
{
	struct EffectVector	  pos;
	struct ObjectRuntime *obj;
	struct ObjectRuntime *frag;
	s32					  i;

	AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
	pos.x = src->x;
	pos.y = src->y;
	pos.z = src->z;
	obj = Object_Spawn(0x11b, pos.x, pos.y - (32 << 16), pos.z);
	if (obj != NULL)
	{
		obj->flags = 0;
		obj->unknown_5e = 20;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}

	// 破片 : 速さ 1〜2 ドット、8〜32 ドット先のでたらめな方向へ
	for (i = 0; i < 12; i++)
	{
		frag = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
		if (frag != NULL)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)frag, BattleFx_FragmentScript);
			frag->speed_limit = Random16() + (1 << 16);
			frag->acceleration = 1 << 16;
			frag->flags = 0;
			Motion_SetTargetPositionFromMagnitudeAngle(
				frag, Random16() * 24 + (8 << 16), Random16());
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 4 番を呼ぶ
//--------------------------------------------------------------------------
void BattleFx_CallEffect04(s32 unused)
{
	RunBattleEffect04();
}

//--------------------------------------------------------------------------
// orbit.c
// フィールドのエナジー演出：4 番（押し出す弾）、6 番（渦を巻いて集まる粒）
//--------------------------------------------------------------------------

#define UE (16 << 16) // 16 ドット上

//--------------------------------------------------------------------------
// obj の 16 ドット上の位置
//--------------------------------------------------------------------------
static __inline__ void PosAbove(struct FieldActor *obj, struct EffectVector *pos)
{
	pos->x = obj->x.fixed;
	pos->y = obj->y.fixed + UE;
	pos->z = obj->z.fixed;
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e を使う
struct SparkObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[4];
	s32 scale_x;
	s32 scale_y;
	u8	unknown_20[24];
	s32 anchor_x; // ゆれの中心（FieldActor の target_x の位置）
	u8	unknown_3c[12];
	s32 gravity; // FieldActor と同じ位置
	u8	unknown_4c[9];
	u8	motion_flags; // FieldActor と同じ位置
	u8	unknown_56[8];
	u16 unknown_5e;
	u8	unknown_60[4];
	s16 phase; // ゆれの位相（128 フレームで 1 周）
	u8	unknown_66[6];
	void (*update)(struct SparkObject *);
};

//--------------------------------------------------------------------------
// エナジー 4 番 : 使う者の前に弾を出して大きくし、残像 3 つと前へ飛ばす
// 当たった物（scene->child）は 16 ドット押し出す
//--------------------------------------------------------------------------
void RunBattleEffect04(void)
{
	struct FieldActor	   *tgt;
	s32						ctx;
	struct FieldActor	   *copies[4];
	struct EffectVector		pos;
	struct FieldActor	   *obj;
	struct FieldActor	   *copy;
	struct FieldActor	   *src;
	struct BattleFxScene   *scene;
	struct EffectSlot	   *slot;
	struct AnimationObject *res;
	s32						ev;
	s32						scale;
	s32						i;
	u8						res_slot;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(130);
	slot = scene->slots;

	// 使う者の頭上から、光を 2 フレームおきに 12 個はじけさせる
	for (i = 0; i <= 11; i++)
	{
		src = scene->main_object;
		PosAbove(src, &pos);
		Camera_WorldToScreen(&pos);
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialBurst);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 9);
		slot->scale_y = 0xb333; // 0.7 倍
		slot->scale_x = 0xb333;
		WaitFrames(2);
		slot++;
	}

	// 弾：使う者の向きに 8 ドット先で、0.25 倍から等倍まで大きくする
	src = scene->main_object;
	pos.x = src->x.fixed;
	pos.y = src->y.fixed + UE;
	pos.z = src->z.fixed;
	Vector_AddPolarOffset(8 << 16, scene->angle, &pos);
	obj = Object_Spawn(0xd7, pos.x, pos.y, pos.z);
	if (obj == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	obj->scale_y = 0x4000;
	obj->scale_x = 0x4000;
	obj->facing = scene->angle;
	obj->speed = 4 << 16;
	obj->acceleration = 4 << 16;
	obj->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 5);
	Animation_ApplyChildValuesFar(obj, 3);
	scale = obj->scale_x;
	if (scale < 0x10000)
	{
		do
		{
			scale += 0x500;
			obj->scale_y = scale;
			obj->scale_x = scale;
			WaitFrames(1);
			scale = obj->scale_x;
		} while (scale <= 0xffff);
	}
	WaitFrames(3);

	// 残像 3 つ。絵の資源はひとつを分けあう
	res = NULL;
	for (i = 2; i >= 0; i--)
	{
		copy = copies[i] = Object_Spawn(0xd7, obj->x.fixed, obj->y.fixed, obj->z.fixed);
		if (copy != NULL)
		{
			copy->scale_y = 0xf000;
			copy->scale_x = 0xf000;
			copy->facing = scene->angle;
			copy->speed = 4 << 16;
			copy->acceleration = 4 << 16;
			copy->motion_flags = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(copy, 5);
			Animation_ApplyChildValuesFar(copy, 2);
			res = Object_ReplaceResourceEntry((struct AnimationObject *)copy->sprite, res);
		}
	}
	res_slot = res->slot;

	// 行き先：scene->enabled なら使う者の 56 ドット先、ほかは scene の位置
	if (scene->enabled != 0)
	{
		src = scene->main_object;
		pos.x = src->x.fixed;
		pos.y = src->y.fixed + UE;
		pos.z = src->z.fixed;
		Vector_AddPolarOffset(56 << 16, scene->angle, &pos);
	}
	else
	{
		pos.x = scene->x;
		pos.y = scene->y + UE;
		pos.z = scene->z;
	}
	Object_SetMoveTargetFar(obj, pos.x, pos.y, pos.z);
	ObjectDispatch_InitializeFar(
		(struct DispatchObject *)obj, BattleFx_BurstParticleObjectScript + 4);
	for (i = 0; i < 3; i++)
	{
		copy = copies[i];
		if (copy != NULL)
		{
			WaitFrames(3);
			Object_SetMoveTargetFar(copy, pos.x, pos.y, pos.z);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)copy, BattleFx_CommonParticleScript + 1);
		}
	}

	// 弾のスクリプトが終わるまで、60 フレームまで待つ
	i = 0;
	if (((struct ScriptMotionObject *)obj)->script != NULL)
	{
	wait_script:
		WaitFrames(1);
		i++;
		if (i <= 59 && ((struct ScriptMotionObject *)obj)->script != NULL)
			goto wait_script;
	}

	// 当たった物を、ぶつからなければ 16 ドット押し出す（child_option なら跳ねる）
	if (tgt != NULL && scene->child_mode == 0)
	{
		if (scene->child_option != 0)
			tgt->velocity_y = 8 << 16;
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed;
		pos.z = tgt->z.fixed;
		Vector_AddPolarOffset(16 << 16, scene->angle, &pos);
		if (Map_CheckObjectStepFar(tgt, &pos) == 0 && ScriptObject_CheckOverlapFar(tgt, &pos) == 0)
		{
			tgt->acceleration = 1 << 16;
			tgt->speed = 1 << 16;
			Object_SetMoveTargetFar(tgt, pos.x, pos.y, pos.z);
		}
	}

	// 応じるマップのイベントがあれば動かす
	ev = BattleFx_FindMatchingEvent(0x50000005, 4, &ctx);
	if (ev != 0)
		BattleFx_RunEventAction((void *)ev, gGameState.selected_actor, ctx);
	WaitFrames(10);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(20);
	if (res_slot != 96)
		Resource_ResetEntry(res_slot);
}

//--------------------------------------------------------------------------
// 毎フレームの処理：8 フレームごとに 2 フレームだけアニメ 2 にして光らせる
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle)
{
	s32 frame = gFrameCount & 7;
	if (frame == 0)
	{
		Animation_ApplyChildValuesFar(particle, 2);
	}
	else if (frame == 2)
	{
		Animation_ApplyChildValuesFar(particle, 0);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：1/16 ずつ縮み、1/16 以下で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle)
{
	s32 scale;

	if (particle != NULL)
	{
		scale = particle->scale_x - 0x1000;
		particle->scale_y -= 0x1000;
		particle->scale_x = scale;
		if (scale <= 0x1000)
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)particle, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：target のまわりを左まわりに渦を巻いて寄る
// 半径は残りフレーム × 2 ドット、1 フレームに 0x800 まわる
// work は始めの向き、timer は残りフレーム。0 で縮んで消える
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle)
{
	struct EffectVector pos;
	s16					life;
	s32					cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work + (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：左まわりと同じで、右まわり
//--------------------------------------------------------------------------
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle)
{
	struct EffectVector pos;
	s16					life;
	s32					cnt;

	if (particle != NULL)
	{
		cnt = (u16)particle->timer - 1;
		particle->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = particle->target_x;
			pos.y = particle->target_y;
			pos.z = particle->target_z;
			Vector_AddPolarOffset(life << 17, (s16)particle->work - (life << 11), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else
		{
			particle->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleFade;
		}
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番をかけられた物（scene->child）から始める
// child_mode があれば、物は変えずに粒だけ出す
//--------------------------------------------------------------------------
void BattleFx_StartOrbitingParticles(s32 unused)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *tgt = scene->child;

	if (tgt != 0)
	{
		if (scene->child_mode != 0)
		{
			scene->enabled = 1;
		}
		tgt->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		BattleFx_RunOrbitingParticles();
	}
}

//--------------------------------------------------------------------------
// エナジー 6 番 : 粒 16 個が scene の位置へ渦を巻いて集まり、
// 物を 16 回光らせる。child_option がなければ物を mode 2 にする
//--------------------------------------------------------------------------
void BattleFx_RunOrbitingParticles(void)
{
	s32					  ctx;
	struct EffectVector	  pos;
	struct EffectVector	 *p;
	struct BattleFxScene *scene;
	struct FieldActor	 *tgt;
	struct FieldActor	 *particle;
	void				 *ev;
	s32					  cnt;

	scene = gEffectWork;
	tgt = scene->child;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(115);

	// 3 フレームおきに粒を出す。大きさ 0.5〜1 倍、まわる向きはでたらめ
	// 中心は scene の位置から 2〜6 ドットずらす
	p = &pos;
	cnt = 15;
	do
	{
		particle = Object_Spawn(0xe8, 0, 0, 0);
		if (particle != NULL)
		{
			u32 scale;
			s32 dist;

			scale = (Random16() >> 1) + 0x8000;
			particle->scale_y = scale;
			particle->scale_x = scale;
			if ((Random16() & 1) != 0)
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleLeft;
			else
				particle->update =
					(void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleRight;

			particle->facing = Random16();
			particle->timer = 60;
			particle->work = Random16();
			Animation_ApplyChildValuesFar(particle, 9);

			p->x = scene->x;
			p->y = scene->y;
			p->z = scene->z;
			dist = (Random16() << 2) + (2 << 16);
			Vector_AddPolarOffset(dist, Random16(), p);
			particle->target_x = p->x;
			particle->target_y = p->y;
			particle->target_z = p->z;
		}

		WaitFrames(3);
		cnt--;
	} while (cnt >= 0);

	WaitFrames(10);
	AudioCommand_PlayFar(115);
	WaitFrames(50);

	if (tgt != NULL && scene->enabled == 0)
	{
		AudioCommand_PlayFar(212);

		// 5 フレームごとに 1 フレーム光る
		cnt = 15;
		do
		{
			Animation_ApplyChildValuesFar(tgt, 7);
			WaitFrames(1);
			Animation_ApplyChildValuesFar(tgt, 0);
			WaitFrames(4);
			cnt--;
		} while (cnt >= 0);

		if (scene->child_option == 0)
		{
			AudioCommand_PlayFar(220);
			ObjectDispatch_ApplyArgumentToChildrenFar(tgt, 2);
		}

		tgt->update = (void (*)(union FieldObject *))BattleFx_UpdateOrbitingParticleMain;
		ev = BattleFx_FindMatchingEvent(0x50000005, 6, &ctx);
		if (ev != NULL)
		{
			BattleFx_RunEventAction(ev, gGameState.selected_actor, ctx);
		}
		WaitFrames(20);
	}

	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// 毎フレームの処理：anchor_x を中心に左右 4 ドット、128 フレームで 1 往復ゆれる
//--------------------------------------------------------------------------
void BattleFx_SwayParticle(struct SparkObject *obj)
{
	s16 *phase;

	phase = &obj->phase;
	obj->x = obj->anchor_x + Iwram_MulQ16(4 << 16, Trig_Sin(*phase << 9));
	(*phase)++;
	*phase = (*phase + 128) % 128;
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 10 ドット上を中心に、渦を巻いて寄る
// 半径は残りフレームと同じドット数。timer が 0 で消える
//--------------------------------------------------------------------------
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *obj)
{
	struct BattleFxScene *scene = gEffectWork;
	struct EffectVector	  pos;
	s16					  life;
	s32					  cnt;

	if (obj != NULL)
	{
		cnt = (u16)obj->timer - 1;
		obj->timer = cnt;
		life = cnt;
		if (life != 0)
		{
			pos.x = scene->x;
			pos.y = scene->y + (10 << 16);
			pos.z = scene->z;
			Vector_AddPolarOffset(life << 16, (s16)obj->work + (life << 11), &pos);
			obj->x.fixed = pos.x;
			obj->y.fixed = pos.y;
			obj->z.fixed = pos.z;
		}
		else
		{
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理：scene の位置の 16 ドット上で左右 6 ドット（64 フレーム）ゆれ、
// 3 フレームごとに、ゆれながらゆっくり落ちる粒を出す
// phase が -1 なら、その場から動かない
//--------------------------------------------------------------------------
void BattleFx_RunSparkEmitter(struct SparkObject *obj)
{
	struct BattleFxScene *scene;
	s16					 *phase;
	struct SparkObject	 *spark;
	s32					  pos[3];

	phase = &obj->phase;
	scene = gEffectWork;
	if (*phase != -1)
	{
		obj->x = scene->x + Iwram_MulQ16(6 << 16, Trig_Sin(*phase << 10));
		obj->y = scene->y + UE;
		obj->z = scene->z;
		(*phase)++;
		*phase = (*phase + 64) % 64;
	}
	if (gFrameCount % 3 == 0)
	{
		// 2 ドット上から、でたらめな向きに 0〜6 ドットずらす
		pos[0] = obj->x;
		pos[1] = obj->y + (2 << 16);
		pos[2] = obj->z;
		Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
		spark = Object_Spawn(0x11d, pos[0], pos[1], pos[2]);
		if (spark != 0)
		{
			spark->update = BattleFx_SwayParticle;
			spark->scale_x = spark->scale_y = 0x9999; // 0.6 倍
			spark->motion_flags = ACTOR_FALLS;
			spark->gravity = 458;
			spark->phase = Random16() >> 9;
			spark->anchor_x = spark->x;
			Animation_ApplyChildValuesFar(spark, 9);
			spark->unknown_5e = 72;
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)spark, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// call_effect_03_and_stop.c
// 演出 03：渦を巻いて集まる 8 個の粒と、火花を出して飛び去る 1 個
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 演出 03 を流し、いまのオブジェクトを止める
//--------------------------------------------------------------------------
void BattleFx_CallEffect03AndStop(s32 unused)
{
	RunBattleEffect03();
	EffectRuntime_StopCurrentObject();
}

//--------------------------------------------------------------------------
// 演出 03
// 8 個の粒が渦を巻いて中心へ集まり、中心に現れた 1 個が火花を出してから
// 遠くへ飛び去る
//--------------------------------------------------------------------------
void RunBattleEffect03(void)
{
	struct BattleFxScene *scn = gEffectWork;
	struct FieldActor	 *target = scn->main_object;
	struct FieldActor	 *object;
	struct FieldSprite	 *last;
	u8					  marker;
	s32					  i;

	BattleEffect_InitializeSharedScene();

	// 渦の粒：1 フレームに 1 個ずつ、1/8 周ずらして出す
	last = 0;
	for (i = 0; i < 8; i++)
	{
		object = Object_Spawn(233, target->x.fixed, target->y.fixed + 0x200000, target->z.fixed);
		if (object != 0)
		{
			object->scale_y = 0xb333; // 0.7 倍
			object->scale_x = 0xb333;
			object->update = BattleFx_UpdateShrinkingOrbitObject;
			object->timer = 120;
			object->work = i << 13;
			object->motion_flags = 4;
			last = Object_ReplaceResourceEntry(object->sprite, last);
		}
		WaitFrames(1);
	}

	marker = last->vram_block;
	AudioCommand_PlayFar(130);
	WaitFrames(110);

	// 中心の 1 個
	target = Object_Spawn(233, 0, 0, 0);
	object = target;
	if (target != 0)
	{
		target->scale_y = 0xb333;
		target->scale_x = 0xb333;
		target->x.fixed = scn->x;
		target->y.fixed = scn->y + 0x100000;
		target->z.fixed = scn->z;
		target->motion_flags = 4;
		Animation_ApplyChildValuesFar(target, 7);
	}

	AudioCommand_PlayFar(131);
	WaitFrames(12);
	if (target != 0)
	{
		// 2 フレームごとに子の値を切り替える（4 回に 1 回は 10）
		for (i = 0; i < 30; i++)
		{
			/* FAKEMATCH: needed to match: the ROM keeps object as a second register copy of the spark (r5 beside r6); this redundant reassignment stops gcse copy-propagating object = target away, and loop then drops it (differ 1 without it). */
			target = object;
			if (i & 3)
				Animation_ApplyChildValuesFar(object, 9);
			else
				Animation_ApplyChildValuesFar(object, 10);
			WaitFrames(2);
		}
	}

	// 火花
	Animation_ApplyChildValuesFar(object, 0);
	AudioCommand_PlayFar(84);
	if (object != 0)
	{
		target->update = BattleFx_RunSparkEmitter;
		target->timer = 0;
		if (scn->enabled != 0)
			WaitFrames(128);
		else
			WaitFrames(192);
	}

	// 飛び去る
	if (target != 0)
	{
		target->timer = -1;
		target->speed = 0x50000;
		target->acceleration = 0x6666;
		target->steering_flags = 0;
		Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
		Script_WaitForEventTimeoutFar(target);
		ObjectDispatch_ReleaseFar(target);
	}
	if (marker != 0x60)
		Resource_ResetEntry(marker);
	if (scn->finish_callback != 0)
		scn->finish_callback();
	BattleFx_PrepareBufferInterpolation();
}

#define CLOAK_TILE 0xfb	 // 隠れていられるタイルの属性
#define CLOAK_END 0x2092 // 隠れていられないときの psynergy_request

//--------------------------------------------------------------------------
// 毎フレームの処理 : 選択中のオブジェクトの足もとのタイルが
// CLOAK_TILE でなければ、CLOAK_END を要求する
// イベントの mode が 3 なら 32 ドット四方の gMapBlocks、
// ほかは 16 ドット四方の地形のマップを見る
//--------------------------------------------------------------------------
void CheckObjectMapTile(void)
{
	u32					  slot;
	struct EventRuntime	 *ev;
	struct ObjectRuntime *obj;
	u8					 *tile;
	s32					  x;
	s32					  y;

	slot = (u32)&gEventWork;
	ev = gEventWork;
	obj = ObjectTable_Get(gGameState.selected_actor);
	tile = (u8 *)*(struct MapState **)(slot - 76); // gMapWork[0]

	if (ev->mode_19e == 3)
	{
		u32 tx;
		u32 ty;

		x = obj->x;
		if (x < 0)
			x += 0x1fffff;
		tx = (x >> 21) & 31;

		y = obj->z;
		if (y < 0)
			y += 0x1fffff;
		ty = (y >> 21) & 31;

		tile = (u8 *)gMapBlocks + ((tx + (ty << 5)) << 2);
	}
	else
	{
		if (obj->terrain_id <= 2)
		{
			tile = (u8 *)((struct MapState *)tile)->layers[obj->terrain_id].cells;
		}
		else
			tile = (u8 *)gMapCellBuffer;

		x = obj->x;
		if (x < 0)
			x += 0xfffff;
		{
			u32 tx = x >> 20;

			y = obj->z;
			if (y < 0)
				y += 0xfffff;

			tile = (u8 *)(tile + ((tx + ((y >> 20) << 7)) << 2));
		}
	}

	if (tile[2] != CLOAK_TILE)
		((struct EventWork *)ev)->psynergy_request = CLOAK_END;
}

//--------------------------------------------------------------------------
// 姿を戻す : 確認を止め、点滅させて cloaked を消す
//--------------------------------------------------------------------------
void MapEvent_RunTileTriggerSequence(void)
{
	struct ObjectRuntime   *obj;
	struct AnimationObject *anim;
	struct AnimationEntry  *entry;
	u32						i;

	obj = ObjectTable_Get(gGameState.selected_actor);
	anim = obj->animation;
	entry = anim->entries[0];

	AudioCommand_PlayFar(154);
	Scheduler_RemoveCallback(CheckObjectMapTile);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
	}

	for (i = 0; i < 5; ++i)
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		entry->param = 0;
		anim->dirty = 1;
		WaitFrames(2);
	}

	anim->flags = 1;
	gGameState.cloaked = 0;
}

//--------------------------------------------------------------------------
// 隠れているなら、足もとの確認を始める
//--------------------------------------------------------------------------
void BattleFx_ScheduleCloakedMapCheck(void)
{
	if (gGameState.cloaked != 0)
	{
		Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
	}
}

//--------------------------------------------------------------------------
// 姿を隠す : 10 回点滅させて cloaked を立て、足もとを確かめる
// その場で隠れられなければ、すぐに姿を戻す
//--------------------------------------------------------------------------
void BattleFx_RunFlashingCallbackSequence(void)
{
	struct EventWork	   *ev = gEventWork;
	struct ObjectRuntime   *obj = ObjectTable_Get(gGameState.selected_actor);
	struct AnimationObject *anim = obj->animation;
	struct AnimationEntry  *entry = anim->entries[0];
	u32						n;
	void (*check)(void);

	AudioCommand_PlayFar(130);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	((struct ScriptMotionObject *)obj)->hook = 0;
	n = 0;
	do
	{
		entry->param = 7;
		anim->dirty = 1;
		anim->flags = 2;
		WaitFrames(2);
		anim->dirty = 1;
		anim->flags = 0;
		WaitFrames(2);
		n++;
	} while (n <= 9);
	n = 0;
	entry->param = n;
	anim->flags = 2;
	anim->dirty = 1;
	check = CheckObjectMapTile;
	Scheduler_AddOrUpdateCallback(check, 0xc80);
	gGameState.cloaked = 1;
	check();
	if (ev->psynergy_request == CLOAK_END)
	{
		MapEvent_RunTileTriggerSequence();
		ev->psynergy_request = n;
	}
}

// FieldActor と同じ物体。FieldActor に欄のない 0x5e (phase) を使う
struct Effect05Object
{
	u8					 reserved_00[8];
	s32					 x, y, z;
	s32					 altitude; // 落ちて止まる高さ
	s32					 scale_x, scale_y;
	u8					 reserved_20[8];
	s32					 velocity_y;
	u8					 reserved_2c[4];
	s32					 speed;
	u8					 reserved_34[0x1c];
	struct WaveFxSprite *sprite;
	u8					 reserved_54;
	u8					 flag;
	u8					 reserved_56[8];
	s16					 phase;
	u8					 reserved_60[0xc];
	void				*callback; // 毎フレームの処理
};

struct Effect05State
{
	s32			   angle;
	s32			   x, y, z; // 玉の行き先の足もと
	struct Object *target;
	s32			   initialized;
	u8			   reserved_18[8];
	u8			   high_arc;
	u8			   reserved_21[0x13];
	u8			   variant;
	u8			   reserved_35[0x10];
	u8			   alternate;
};

//--------------------------------------------------------------------------
// 毎フレームの処理 : 落ちる粒が altitude に着いたら、小さな粒を 3 つ散らす
//--------------------------------------------------------------------------
void BattleFx_SpawnRandomAngleTriplet(struct Effect05Object *obj)
{
	s32					   i;
	struct Effect05Object *p;
	s32					   phase = 2;
	s32					   phase2;
	s16					  *pp;

	if (obj->y <= obj->altitude)
	{
		obj->phase = phase;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
		p = NULL;
		obj->callback = p;

		// 半分の大きさで、上へ 1 ドットの速さで跳ね、32 ドット先のでたらめな方向へ
		for (i = 0; i <= 2; i++)
		{
			p = Object_Spawn(0xf0, obj->x, obj->y, obj->z);
			if (p == NULL)
			{
				break;
			}
			p->scale_y = 0x8000;
			p->scale_x = 0x8000;
			p->flag = 2;
			p->velocity_y = 1 << 16;
			p->speed = Random16() + 0x13333;
			Motion_SetTargetPositionFromMagnitudeAngle(p, 32 << 16, Random16());
			pp = &p->phase;
			phase2 = 6;
			*pp = phase2;
			ObjectDispatch_InitializeFar((struct DispatchObject *)p, BattleFx_CommonParticleScript);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 左右にゆれながらゆっくり落ち、地面に着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *obj)
{
	s32 r;

	obj->y -= 0x4ccc;
	r = Random16();
	obj->x += r - Random16();
	if (obj->y <= obj->terrain_height)
	{
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

void BattleFx_CallEffect05(s32 unused)
{
	RunBattleEffect05();
}

//--------------------------------------------------------------------------
// 0〜10 段で start から end へ
//--------------------------------------------------------------------------
static __inline__ s32 Lerp10(s32 start, s32 end, s32 step)
{
	return start + step * (end - start) / 10;
}

//--------------------------------------------------------------------------
// 光の玉を対象の 16 ドット上から行き先の 32 ドット上 (variant なら 80 ドット上) へ
// 10 フレームで運びながら大きくし、粒を降らせてから戻す
// 行き先は、まだ決まっていなければ対象から angle の向きへ 16 ドット
//--------------------------------------------------------------------------
void RunBattleEffect05(void)
{
	struct EffectVector	   pos;
	struct EffectVector	   start;
	struct EffectVector	   end;
	struct Effect05State  *work = gEffectWork;
	struct Object		  *target = work->target;
	struct Effect05Object *orb;
	struct EffectVector	  *from;
	struct EffectVector	  *to;
	s32					   i;
	s32					   steps = 11;
	s32					   count;
	s32					   y;
	struct Effect05Object *mote;

	orb = Object_Spawn(0xef, i = 0, 0, 0);
	if (orb == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	if (work->initialized == 0)
	{
		work->x = target->pos.x;
		work->z = target->pos.z;
		Vector_AddPolarOffset(16 << 16, work->angle, (struct EffectVector *)&work->x);
		work->y = Map_GetTerrainHeightFar(0, work->x, work->z);
	}
	from = &start;
	from->x = target->pos.x;
	from->y = target->pos.y + (16 << 16);
	from->z = target->pos.z;
	to = &end;
	to->x = work->x;
	y = work->y;
	to->y = y + (32 << 16);
	to->z = work->z;
	if ((s8)work->variant != 0)
		to->y = y + (80 << 16);

	// 運びながら 1/4 から等倍まで大きくする
	for (; i < steps; i++)
	{
		s32 scale;

		{
			struct EffectVector *a = &start;
			struct EffectVector *b = &end;
			s32					 e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x4000, 0x10000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
	}
	WaitFrames(10);

	if ((s8)work->alternate == 0)
	{
		// 玉のまわり 3〜8 ドットから粒を 6 フレームおきに落とす (最後は玉の真下)
		count = 10;
		if ((s8)work->high_arc == 0)
			count = 24;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			if (i == count - 1)
			{
				WaitFrames(25);
				pos.x = orb->x;
				pos.y = orb->y;
				pos.z = orb->z;
			}
			mote = Object_Spawn(0xf0, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->altitude = pos.y - (32 << 16);
				mote->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
				mote->flag = 2;
			}
			AudioCommand_PlayFar(132);
			WaitFrames(6);
		}
		WaitFrames(10);
	}
	else
	{
		// 玉のまわり 3〜8 ドットから、ゆれて落ちる粒を 6 フレームおきに出す
		if ((s8)work->high_arc != 0)
			count = 10;
		else
			count = 30;
		for (i = 0; i < count; i++)
		{
			pos.x = orb->x;
			pos.y = orb->y;
			pos.z = orb->z;
			Vector_AddPolarOffset(Random16() * 5 + (3 << 16), Random16(), &pos);
			mote = Object_Spawn(0x11c, pos.x, pos.y, pos.z);
			if (mote != 0)
			{
				mote->callback = (void *)BattleFx_UpdateDriftingFallObject;
				mote->flag = 0;
				mote->sprite->mode = 2;
				ObjectDispatch_ApplyArgumentToChildrenFar(mote, 8);
				Animation_ApplyChildValuesFar(mote, 7);
			}
			WaitFrames(6);
		}
		WaitFrames(70);
	}

	// 等倍から 1/4 まで縮めながら、対象の上へ戻す
	i = 0;
	do
	{
		s32 scale;

		{
			struct EffectVector *a = &end;
			struct EffectVector *b = &start;
			s32					 e, t;

			e = b->x;
			t = a->x;
			t += i * (e - t) / 10;
			orb->x = t;
			e = b->y;
			t = a->y;
			t += i * (e - t) / 10;
			orb->y = t;
			e = b->z;
			t = a->z;
			t += i * (e - t) / 10;
			orb->z = t;
		}
		scale = Lerp10(0x10000, 0x4000, i);
		orb->scale_x = scale;
		orb->scale_y = scale;
		WaitFrames(1);
		i++;
	} while (i < steps);
	ObjectDispatch_ReleaseFar(orb);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// particles.c
// バトル効果：持ち上げた物のまわりに落ちる粒
//--------------------------------------------------------------------------

struct FallScene
{
	u8				   reserved_00[20];
	struct FieldActor *child; // 持ち上げている物
};

LAYOUT_SIZE_GUARD(FallPos_Size, struct EffectVector, 0x0c);
LAYOUT_OFFSET_GUARD(FallScene_Child, struct FallScene, child, 0x14);

//--------------------------------------------------------------------------
// 毎フレームの処理：持ち上げている物のまわりに、落ちる粒をひとつ出す
// 高さは物から 8〜24 ドット上、横はでたらめな向きに 0〜48 ドット
//--------------------------------------------------------------------------
void BattleFx_SpawnFallingParticles(void)
{
	struct FieldActor			   *src;
	struct EffectVector				pos;
	struct ItemBreakFragmentObject *obj;
	u32								dist;

	src = ((struct FallScene *)gEffectWork)->child;
	pos.x = src->x.fixed;
	pos.y = src->y.fixed - Random16() * 16 + (24 << 16);
	pos.z = src->z.fixed;

	dist = Random16() * 3;
	Vector_AddPolarOffset(dist * 16, Random16(), &pos);

	obj = Object_Spawn(0x11d, pos.x, pos.y, pos.z);
	if (obj != 0)
	{
		obj->motion_flags = ACTOR_FALLS;
		obj->gravity = 0x1999; // 重力 0.1 ドット
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
		obj->unknown_5e = 12;
		ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_CommonParticleScript);
	}
}

//--------------------------------------------------------------------------
// lift_drop.c
// 対象を 2 つの持ち手で持ち上げ、方向キーで動かして落とす
//--------------------------------------------------------------------------

#define SHARD_MAX 20 // 破片の数

//--------------------------------------------------------------------------
// 足もとを 16 ドット上げて、pos へ動いたときにぶつかるかを調べる
//--------------------------------------------------------------------------
static __inline__ s32 ObjProbeUp(struct FieldActor *obj, struct EffectVector *pos)
{
	s32 hit;

	obj->terrain_height += 16 << 16;
	hit = Map_CheckObjectStepFar(obj, pos);
	obj->terrain_height -= 16 << 16;
	return hit;
}

//--------------------------------------------------------------------------
// 対象を左右の持ち手で 24 ドットの高さまで持ち上げ、方向キーの向きへ
// 16 ドット動かして落とす (A・B・L・R ならその場で落とす)
// 行き先がふさがっていれば、ブザーを鳴らして待つ
//--------------------------------------------------------------------------
void BattleFx_SteerLiftedTarget(s32 id)
{
	struct BattleFxScene *work;
	struct FieldActor	 *src;
	struct FieldActor	 *target;
	struct FieldActor	 *shards[SHARD_MAX];
	struct EffectVector	  pos;
	struct FieldActor	 *hands[2];
	struct FieldActor	 *obj;
	struct FieldActor	 *right;
	s32					  i;
	s32					  angle;
	s32					  dist;
	struct EffectVector	 *p;

	work = gEffectWork;
	src = work->main_object;
	target = work->child;
	if (target == NULL)
		return;
	BattleEffect_InitializeSharedScene();
	src->linked_object = target;
	ObjectDispatch_InitializeFar((struct DispatchObject *)src, BattleFx_SourceHoldScript);

	// 持ち手を左右 32 ドットに出し、対象の左右 16 ドットへ動かす
	/* FAKEMATCH: needed to match: the position is reached through this pointer everywhere but in the steering loop's first part, which names the position itself */
	p = &pos;
	p->x = work->x;
	p->y = work->y + (16 << 16);
	p->z = work->z;
	obj = BattleFx_SpawnItemBreakMode1(p->x + (32 << 16), p->y, p->z, 0x8000);
	hands[0] = obj;
	right = BattleFx_SpawnItemBreakMode1(p->x - (32 << 16), p->y, p->z, 0);
	hands[1] = right;
	if (hands[0] == NULL || right == NULL)
	{
		BattleFx_PrepareBufferInterpolation();
		return;
	}
	WaitFrames(15);
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	Object_SetMoveTargetFar(obj, p->x + (16 << 16), p->y, p->z);
	Object_SetMoveTargetFar(right, p->x - (16 << 16), p->y, p->z);
	Script_WaitForEventTimeoutFar(obj);
	Script_WaitForEventTimeoutFar(right);
	obj->x.fixed = p->x + (16 << 16);
	obj->velocity_x = 0;
	right->x.fixed = p->x - (16 << 16);
	right->velocity_x = 0;

	// 地面から 24 ドットの高さまで持ち上げる
	target->update = ObjectGroup_ApplyRandomChildValues;
	Scheduler_AddOrUpdateCallback(BattleFx_SpawnFallingParticles, 0xc80);
	AudioCommand_PlayFar(130);
	target->motion_flags = 4;
	ObjectDispatch_SetSingleChildFlagsFar(target, 0);
	if (hands[0] != NULL && hands[1] != NULL)
	{
		while (target->y.fixed - target->terrain_height <= 24 << 16)
		{
			obj->y.fixed += 0x6000;
			right->y.fixed += 0x6000;
			target->y.fixed += 0x6000;
			WaitFrames(1);
		}
	}
	hands[0]->speed = 4 << 16;
	hands[0]->acceleration = 0x8000;
	hands[1]->speed = 4 << 16;
	hands[1]->acceleration = 0x8000;
	target->speed = 0x6666;
	target->acceleration = 0x3333;
	target->steering_flags = 0;
	target->terrain_id = 2;

	//------------------------------------------------------------------
	// 方向キーで動かす
	//------------------------------------------------------------------
	for (;;)
	{
		WaitFrames(1);
		if ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;

		// 方向キーなし : 持ち手を対象の左右に戻す
		angle = (u16)BattleFx_GetCycledTableWord(gKeysHeld);
		if (angle == 0xffff)
		{
			pos.x = target->x.fixed;
			pos.y = target->y.fixed + (16 << 16);
			pos.z = target->z.fixed;
			Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
			Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 1);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 1);
			continue;
		}

		// 持ち手をその向きへ 2 ドット寄せ、16 ドット先の地面を調べる
		pos.x = target->x.fixed;
		pos.y = target->y.fixed + (16 << 16);
		pos.z = target->z.fixed;
		Vector_AddPolarOffset(2 << 16, angle, &pos);
		Object_SetMoveTargetFar(hands[0], pos.x + (16 << 16), pos.y, pos.z);
		Object_SetMoveTargetFar(hands[1], pos.x - (16 << 16), pos.y, pos.z);
		Script_WaitForEventTimeoutFar(hands[0]);
		Script_WaitForEventTimeoutFar(hands[1]);
		pos.x = target->x.fixed;
		pos.y = target->terrain_height;
		pos.z = target->z.fixed;
		Vector_AddPolarOffset(16 << 16, angle, &pos);
		i = ScriptObject_CheckOverlapFar(target, &pos);
		/* FAKEMATCH: needed to match: entering the blocked path from both tests keeps it ahead of the drop path, as the ROM lays them out */
		if (i != 0)
			goto blocked;
		if (ObjProbeUp(target, p) > 0)
		{
		blocked: // ふさがっている : 16 フレームごとにブザー
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			if ((gFrameCount & 15) == 0)
				AudioCommand_PlayFar(SOUND_MENU_ERROR);
			continue;
		}
		// 空いている : 対象をそこへ動かす
		{
			struct FieldActor *second;
			s32				   x;
			s32				   z;

			AudioCommand_PlayFar(175);
			x = p->x;
			z = p->z;
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
			ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
			WaitFrames(15);
			target->paused = i;
			target->speed = 0x3333;
			target->acceleration = 0x3333;
			Object_SetMoveTargetFar(target, p->x, p->y, p->z);
			hands[0]->speed = 0x3333;
			hands[0]->acceleration = 0x3333;
			second = hands[1];
			second->speed = 0x3333;
			second->acceleration = 0x3333;
			Object_SetMoveTargetFar(hands[0], p->x + (16 << 16), p->y, p->z);
			Object_SetMoveTargetFar(second, p->x - (16 << 16), p->y, p->z);
			Script_WaitForEventTimeoutFar(target);
			target->x.fixed = x;
			target->z.fixed = z;
			target->velocity_x = i;
			target->velocity_z = i;
			WaitFrames(10);
			break;
		}
	}

	//------------------------------------------------------------------
	// 対象の位置から破片を飛ばして持ち手を消し、対象を跳ねさせて落とす
	//------------------------------------------------------------------
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[0], 4);
	ObjectDispatch_ApplyArgumentToChildrenFar(hands[1], 4);
	Scheduler_RemoveCallback(BattleFx_SpawnFallingParticles);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	AudioCommand_PlayFar(135);
	WaitFrames(15);
	p->x = target->x.fixed;
	p->y = target->y.fixed + (16 << 16);
	p->z = target->z.fixed;
	{
		struct FieldActor **walk = shards;

		// 破片 : 速さ 2〜3 ドット、8〜32 ドット先のでたらめな方向へ
		for (i = 0; i < SHARD_MAX; i++)
		{
			obj = Object_Spawn(0x11d, p->x, p->y, p->z);
			*walk++ = obj;
			if (obj != NULL)
			{
				ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_FragmentScript);
				obj->speed = Random16() + (2 << 16);
				obj->acceleration = 2 << 16;
				obj->motion_flags = 0;
				dist = Random16() * 24 + (8 << 16);
				Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
			}
		}
	}
	AudioCommand_PlayFar(131);
	ObjectDispatch_ReleaseFar(hands[0]);
	ObjectDispatch_ReleaseFar(hands[1]);

	// 対象の見た目と動きを戻し、上へ 10 ドットの速さで跳ねさせる
	Animation_ApplyChildValuesFar(target, work->saved_palette);
	ObjectDispatch_InitializeFar((struct DispatchObject *)target, work->saved_script);
	target->update = work->saved_callback;
	target->motion_flags = 3;
	target->velocity_y = 10 << 16;
	target->bounce = 0x3333;
	target->terrain_id = 0;
	src->update = 0;
	Animation_ApplyChildValuesFar(src, 0);
	if (work->child_option != 0)
	{
		// 上がりきって落ちきるまで (それぞれ最長 90 フレーム) 待ち、輪を出す
		for (i = 0; i < 90 && target->velocity_y >= 0; i++)
			WaitFrames(1);
		WaitFrames(1);
		for (i = 0; i < 90 && target->velocity_y < 0; i++)
			WaitFrames(1);
		BattleFx_SpawnRadialParticleRing(target);
		BattleFx_PrepareBufferInterpolation();
		WaitFrames(30);
	}
	else
	{
		BattleFx_PrepareBufferInterpolation();
	}
}

DEFINE_ITEM_BREAK_EFFECT(RunBattleEffect11, BattleFx_SpawnItemBreakMode1)

struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle)
{
	struct MotionObject *obj;
	s32					 scale;

	AudioCommand_PlayFar(SOUND_ITEM_BREAK);
	obj = Object_Spawn(215, x, y, z);
	if (obj != NULL)
	{
		obj->scale_y = 0x4000;
		obj->scale_x = 0x4000;
		obj->speed_limit = 0x30000;
		obj->acceleration = 0x30000;
		obj->auto_face_motion = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
		scale = obj->scale_x;
		if (scale < 0x10000)
		{
			do
			{
				scale += 0x800;
				obj->scale_y = scale;
				obj->scale_x = scale;
				obj->angle += 0x2000;
				WaitFrames(1);
				scale = obj->scale_x;
			} while (scale <= 0xffff);
		}
		obj->angle = angle;
	}
	return obj;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../../field/common/effect/move.c"

// FieldActor と同じ物体。0x64 を符号なし、0x66 を符号つきで読む (FieldActor の型ではバイトが変わる)
struct OrbitEffect
{
	u8					unknown_00[6];
	u16					angle; // 進む向き・回る角度
	s32					x;
	s32					y;
	s32					z;
	u8					unknown_14[20];
	s32					scale;
	u8					unknown_2c[28];
	s32					speed;
	u8					unknown_4c[24];
	u16					timer;	// 経過フレーム
	s16					pause;	// 向きを大きく変える残りフレーム
	struct OrbitEffect *anchor; // 回る中心
	void (*update)(struct OrbitEffect *);
};

// ここに共通ソースが入る（ROM の並びのとおり）
#define EFFECT_SPAWN_RESIDENT
#include "../../field/common/effect/spawn.c"
#undef EFFECT_SPAWN_RESIDENT

//--------------------------------------------------------------------------
// 粒子の毎フレーム処理
// 速度で進み、x と z の速度を少しずつ落とし、拡大率と回転を変える
//--------------------------------------------------------------------------
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object)
{
	struct FieldEffect *efx = &object->effect;

	efx->x += efx->velocity_x;
	efx->y += efx->velocity_y;
	efx->z += efx->velocity_z;
	efx->velocity_x -= efx->velocity_x / 18;
	efx->velocity_z -= efx->velocity_z / 16;
	efx->scale_x += efx->scale_rate_x;
	efx->scale_y += efx->scale_rate_y;
	efx->sprite->rotation += efx->spin;
}

//--------------------------------------------------------------------------
// origin の位置から、水平に輪を描いて広がる粒子を 17 個出す
// x 方向の速度は z 方向の 1.5 倍
//--------------------------------------------------------------------------
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
	struct EffectOptions opt;
	s32					 vel[3];
	s32					 vz;
	s32					 angle;
	u32					 i;

	opt.palette = 0;
	opt.update = BattleFx_UpdateParticleMotionAndScale;
	opt.start_scale_x = 0xcccc; // 0.8 倍
	opt.start_scale_y = 0xcccc;
	i = 0;
	do
	{
		angle = i << 12; // 1/16 周ずつ
		vel[0] = Trig_Cos(angle) * 3 / 2;
		vel[1] = 0;
		vz = Trig_Sin(angle);
		vel[2] = vz;
		Effect_SpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1], vz, 0x01090001, &opt);
		i++;
	} while (i <= 16);
}

//--------------------------------------------------------------------------
// 中心を離れてさまよう（毎フレーム）
// 1 フレームに 2〜3 ドット進み、ときどき向きを大きく変える
// 101 フレームで共通の粒子スクリプトへ移る
//--------------------------------------------------------------------------
void BattleFx_WanderAroundAnchor(struct OrbitEffect *efx)
{
	s32 len;
	s32 angle;
	s32 dx;
	s32 dz;

	len = Random16() + 0x20000;
	angle = efx->angle;
	dx = Iwram_MulQ16(len, Trig_Cos(angle));
	dz = Iwram_MulQ16(len, Trig_Sin(angle));
	efx->x += dx;
	efx->z += dz;
	efx->angle += 0xfff0;
	if (efx->pause != 0)
	{
		efx->pause--;
		efx->angle += 0x800;
	}
	else if ((Random16() << 5) >> 16 == 0) // 1/32 の確率
	{
		efx->pause = ((Random16() << 4) >> 16) + 8; // 8〜23 フレーム
	}
	if (++efx->timer == 101)
		ObjectDispatch_InitializeFar((struct DispatchObject *)efx, BattleFx_CommonParticleScript);
}

//--------------------------------------------------------------------------
// 中心のまわりを半径 8 ドットで回る（毎フレーム）
// 121 フレーム回ったら、さまよう動きに切り替える
//--------------------------------------------------------------------------
void BattleFx_CircleAnchor(struct OrbitEffect *efx)
{
	struct OrbitEffect *anchor;
	s32					angle;
	s32					r;
	s32					dx;
	s32					dz;

	anchor = efx->anchor;
	r = 0x80000;
	angle = efx->angle;
	dx = Iwram_MulQ16(r, Trig_Cos(angle));
	dz = Iwram_MulQ16(r, Trig_Sin(angle));
	efx->x = anchor->x + dx;
	efx->z = anchor->z + dz;
	efx->angle += 0x800;
	if (++efx->timer == 121)
	{
		efx->update = BattleFx_WanderAroundAnchor;
		efx->timer = 0;
		efx->pause = 0;
		efx->speed = 0x1999;
		efx->scale = 0x30000;
		efx->angle = Random16();
	}
}

//--------------------------------------------------------------------------
// 毎フレーム縮め、0.5 倍を切ったら位置を戻して処理を外す
//--------------------------------------------------------------------------
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj)
{
	s32 scale;

	scale = obj->scale_x - 0x80;
	obj->scale_y = scale;
	obj->scale_x = scale;
	if (scale < 0x8000)
	{
		Object_SetPositionAndResetMotionFar(obj, 0, 0, 0);
		((struct ScriptMotionObject *)obj)->hook = 0;
	}
}

void BattleFx_CallEffect14(s32 unused)
{
	RunBattleEffect14();
}

//--------------------------------------------------------------------------
// radial.c
// フィールドのエナジー演出：14 番（運ばれる玉と、まわりを回る粒）ほか
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// エナジー 14 番 : 玉を使う者から scene の位置（4 ドット上）まで 10 フレームで運び、
// 0.25 倍から大きくする。scene->enabled がなければ、かけられた物を縮め、
// 玉のまわりに 6 フレームおきに粒を 16 個出して回らせる
//--------------------------------------------------------------------------
void RunBattleEffect14(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *src = scene->main_object;
	struct FieldActor	 *tgt = scene->child;
	struct EffectVector	  pos;
	struct EffectVector	  from;
	struct EffectVector	  to;
	struct FieldActor	 *tama;
	s32					  step;
	struct FieldActor	 *particle;
	s32					  steps = 11;

	step = 0;
	from.x = src->x.fixed;
	from.y = src->y.fixed;
	from.z = src->z.fixed;
	to.x = scene->x;
	to.y = scene->y - (4 << 16);
	to.z = scene->z;
	tama = Object_Spawn(0xda, 0, 0, 0);
	if (tama == 0)
	{
		return;
	}
	BattleEffect_InitializeSharedScene();
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 2);
	do
	{
		s32 scale;
		tama->x.fixed = from.x + step * (to.x - from.x) / 10;
		tama->y.fixed = from.y + step * (to.y - from.y) / 10;
		tama->z.fixed = from.z + step * (to.z - from.z) / 10;
		scale = 0x4000 + step * 0x10ccc / 10; // 0.25 倍から 1.3 倍へ
		tama->scale_x = scale;
		tama->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	tama->scale_x = 0x1b333; // 横 1.7 倍
	tama->scale_y = 0x14ccc; // 縦 1.3 倍
	AudioCommand_PlayFar(163);
	WaitFrames(20);
	if (scene->enabled == 0)
	{
		if (tgt != 0)
			tgt->update = BattleFx_ShrinkObjectScaleUntilHalf;

		// 粒は玉の 4 ドット上から 0.8 ドットずつ高く、3〜8 ドット離して出す
		step = 0;
		do
		{
			s32 dist;

			pos.x = tama->x.fixed;
			pos.y = tama->y.fixed + step * 0xcccc + (4 << 16);
			pos.z = tama->z.fixed;
			dist = Random16() * 5 + (3 << 16);
			Vector_AddPolarOffset(dist, Random16(), &pos);
			particle = Object_Spawn(0xf9, pos.x, pos.y, pos.z);
			if (particle != 0)
			{
				particle->update = BattleFx_CircleAnchor;
				particle->linked_object = tama;
				particle->timer = 0;
				particle->work = 0;
				particle->facing = Random16();
			}
			WaitFrames(6);
			step++;
		} while (step <= 15);
		WaitFrames(20);
		WaitFrames(120);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(tama, 1);
	WaitFrames(30);
	AudioCommand_PlayFar(136);
	WaitFrames(20);
	ObjectDispatch_ReleaseFar(tama);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// スロットの粒の動き（毎フレーム）
// state 0 : 出た所から 40〜70 ドット離れた所へ向かう
// state 1 : 着くまで待ち、着いたら state 2 へ
// state 2 : scene の位置の 8 ドット上（画面の位置）のまわり 4 ドットへ向かう
// state 3 : 着いたらスロットを空ける
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect)
{
	struct BattleFxScene *scene;
	struct EffectVector	  pos;
	s8					 *st;
	s16					  angle;
	s32					  state;

	scene = gEffectWork;
	st = &effect->state;
top:
	state = *st;
	if (state == 0)
	{
		pos.x = effect->origin_x;
		pos.z = effect->origin_z;
		angle = Random16();
		Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->acceleration = 4 << 16;
		effect->max_speed = 4 << 16;
		effect->flag42 = state;
		goto advance;
	}
	if (state == 1)
	{
		if (BattleFx_HasReachedTarget(effect) != 0)
			return;
		*st = *st + 1;
		goto top;
	}
	if (state == 2)
	{
		pos.x = scene->x;
		pos.y = scene->y + (8 << 16);
		pos.z = scene->z;
		Camera_WorldToScreen(&pos);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos);
		effect->target_x = pos.x;
		effect->target_z = pos.z;
		effect->max_turn_step = 0x1000;
		effect->flag42 = 1;
	advance:
		*st = *st + 1;
		return;
	}
	if (state == 3)
	{
		if (BattleFx_HasReachedTarget(effect) == 0)
			BattleFx_ClearOwnedSlot(effect);
		return;
	}
	else
	{
		return;
	}
}

//--------------------------------------------------------------------------
// かけられた物（scene->child）に paused を立て、
// BattleEffect_RunFallbackObjectTransition の演出を動かす
//--------------------------------------------------------------------------
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused)
{
	struct FieldActor *tgt = ((struct BattleFxScene *)gEffectWork)->child;

	tgt->paused = 1;
	BattleEffect_RunFallbackObjectTransition();
}

//--------------------------------------------------------------------------
// fallback.c
// フィールド効果：対象のない場所へ飛ばして散らす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 使い手から効果の位置へオブジェクトを 10 フレームで飛ばし、16 個の粒にして散らす
//--------------------------------------------------------------------------
void BattleEffect_RunFallbackObjectTransition(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct MotionObject	 *user = scene->main_object;
	struct MotionObject	 *obj;
	struct EffectSlot	 *slot;
	struct EffectVector	  pos;
	struct EffectVector	  from;
	struct EffectVector	  to;
	s32					  step;
	s32					  i;

	scene->y = user->y;
	obj = Object_Spawn(250, 0, 0, 0);
	step = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 0);
	if (obj == 0)
		return;

	// 使い手の 16 ドット上から、効果の位置の 8 ドット上へ
	BattleEffect_InitializeSharedScene();
	from.x = user->x;
	from.y = user->y + (16 << 16);
	from.z = user->z;
	to.x = scene->x;
	to.y = scene->y + (8 << 16);
	to.z = scene->z;
	for (;;)
	{
		s32 steps = 11;
		s32 scale;

		obj->x = from.x + (to.x - from.x) * step / 10;
		obj->y = from.y + (to.y - from.y) * step / 10;
		obj->z = from.z + (to.z - from.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step); // 0.25 倍から等倍へ
		obj->scale_x = scale;
		obj->scale_y = scale;
		WaitFrames(1);
		step++;
		if (step >= steps)
			break;
	}

	WaitFrames(5);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);
	WaitFrames(10);
	AudioCommand_PlayFar(SOUND_TRIPLE_TONE_HIGH);

	// その周り 4 ドットに粒を置き、外へ飛ばしてから効果の位置へ戻す
	slot = &scene->slots[0];
	for (i = 0; i < 16; i++)
	{
		pos.x = obj->x;
		pos.y = obj->y + (8 << 16);
		pos.z = obj->z;
		Camera_WorldToScreen(&pos.x);
		Vector_AddPolarOffset(4 << 16, Random16(), &pos.x);
		EffectSlot_Initialize(slot, 285, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialCamera);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, 7);
		slot++;
	}

	pos.x = obj->x;
	pos.y = obj->y + (8 << 16);
	pos.z = obj->z;
	WaitFrames(8);
	ObjectDispatch_ReleaseFar(obj);
	WaitFrames(4);
	WaitFrames(30);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// object_pause.c
// オブジェクトを止めて表の値をでたらめに当て続け、もとの処理に戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 止めているあいだの毎フレームの処理 : 表の 8 つからでたらめに選んで当てる
//--------------------------------------------------------------------------
void BattleEffect_SetRandomTableValueOnObject(void *obj)
{
	s8 *table = BattleFx_RandomChildValues;
	s32 r = Random16();
	Animation_ApplyChildValuesFar(obj, table[(u32)(r * 8) >> 16]);
}

//--------------------------------------------------------------------------
// id のオブジェクトを止める
// 毎フレームの処理と、動いていれば最初のアニメーションの param を
// gGameState に覚えておく
//--------------------------------------------------------------------------
void BattleEffect_PauseObject(s32 id)
{
	struct FieldActor	  *obj;
	struct AnimationEntry *entry;

	obj = ObjectTable_Get(id);
	if (obj != NULL)
	{
		gGameState.paused_update = (u32)obj->update;
		gGameState.paused_pose = 0;
		if (obj->active == 1)
		{
			entry = ((struct AnimationObject *)obj->sprite)->entries[0];
			if (entry != NULL)
			{
				gGameState.paused_pose = entry->param;
			}
		}
		obj->update = (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject;
		obj->paused = 1;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
	}
}

//--------------------------------------------------------------------------
// id のオブジェクトを、覚えておいた毎フレームの処理と param に戻す
//--------------------------------------------------------------------------
void BattleFx_ResumeObject(s32 id)
{
	struct FieldActor *obj = ObjectTable_Get(id);
	if (obj != 0)
	{
		if (obj->update == (void (*)(union FieldObject *))BattleEffect_SetRandomTableValueOnObject)
		{
			struct GameState *state = &gGameState;
			obj->update = (void (*)(union FieldObject *))state->paused_update;
			state->paused_update = 0;
			Animation_ApplyChildValuesFar(obj, state->paused_pose);
		}
		obj->paused = 0;
		ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
	}
}

//--------------------------------------------------------------------------
// 今の演出定義の flags が 0xff なら、対象なし (-1) を返す
// id : 対象オブジェクト番号
//--------------------------------------------------------------------------
s32 BattleFx_FilterObjectIdByFlags(s32 id)
{
	if (BattleFx_GetFlags(id) == 0xff)
		return -1;
	return id;
}

//--------------------------------------------------------------------------
// run_effect13_hook.c
// エナジー演出 13：オブジェクト 215 を目標まで伸ばし、押し下げて戻す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象への演出 (BattleFx_DispatchRequestKind から)
//--------------------------------------------------------------------------
void BattleFx_RunEffect13Hook(s32 unused)
{
	RunBattleEffect13();
}

//--------------------------------------------------------------------------
// オブジェクト 215 を使う者の 16 ドット上から目標へ 10 段で運びながら
// 1/4 倍から等倍にし、20 ドット押し下げて child を (-9, -9) へ移し、
// 19.5 ドット上げてから縮めて戻す
// 目標は scene->enabled なら使う者の前 32 ドット、ほかは scene の位置で、
// どちらも 32 ドット上
//--------------------------------------------------------------------------
void RunBattleEffect13(void)
{
	struct BattleFxScene	 *scene = gEffectWork;
	struct MotionObject		 *actor = scene->main_object;
	struct MotionObject		 *child = scene->child;
	struct MotionObject		 *object;
	struct FixedPointPosition spawn;
	struct FixedPointPosition origin;
	struct FixedPointPosition target;
	s32						  step;
	s32						  i;
	s32						  steps = 11;

	origin.x = actor->x;
	origin.y = actor->y + (16 << 16);
	origin.z = actor->z;
	if (scene->enabled)
	{
		target.x = actor->x;
		target.y = actor->y + (32 << 16);
		target.z = actor->z;
		Vector_AddPolarOffset(32 << 16, scene->angle, &target.x);
	}
	else
	{
		target.x = scene->x;
		target.y = scene->y + (32 << 16);
		target.z = scene->z;
	}
	spawn.x = scene->x;
	spawn.y = scene->y + (32 << 16);
	spawn.z = scene->z;
	object = Object_Spawn(215, spawn.x, spawn.y, spawn.z);
	if (object == 0)
		return;
	BattleEffect_InitializeSharedScene();
	AudioCommand_PlayFar(138);
	object->angle = actor->angle;
	object->speed_limit = 0x14ccc;
	object->motion_flags = 0;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	Animation_ApplyChildValuesFar(object, 1);

	// 目標へ伸ばす
	step = 0;
	do
	{
		s32 scale;

		object->x = origin.x + (target.x - origin.x) * step / 10;
		object->y = origin.y + (target.y - origin.y) * step / 10;
		object->z = origin.z + (target.z - origin.z) * step / 10;
		scale = BattleFx_Interpolate(0x10000, 0x4000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	WaitFrames(10);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
	WaitFrames(15);

	// 2 ドットずつ 10 フレーム押し下げる
	for (i = 0; i < 10; i++)
	{
		object->y -= 2 << 16;
		WaitFrames(1);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
	AudioCommand_PlayFar(132);
	if (child != 0)
		Object_SetPositionAndResetMotionFar(child, -(9 << 16), child->y, -(9 << 16));
	WaitFrames(20);

	// 1.5 ドットずつ 13 フレーム上げる
	for (i = 0; i < 13; i++)
	{
		object->y += 0x18000;
		WaitFrames(1);
	}
	WaitFrames(10);
	AudioCommand_PlayFar(114);

	// 使う者のところへ縮めて戻す
	step = 0;
	do
	{
		s32 scale;

		object->x = target.x + (origin.x - target.x) * step / 10;
		object->y = target.y + (origin.y - target.y) * step / 10;
		object->z = target.z + (origin.z - target.z) * step / 10;
		scale = BattleFx_Interpolate(0x4000, 0x10000, step);
		object->scale_x = scale;
		object->scale_y = scale;
		WaitFrames(1);
		step++;
	} while (step < steps);
	ObjectDispatch_ReleaseFar(object);
	BattleFx_PrepareBufferInterpolation();
}

//--------------------------------------------------------------------------
// launch.c
// 戦闘エフェクト 15 番：回って縮む主オブジェクトと、扇状に飛び出す粒
//--------------------------------------------------------------------------

#define SLOT_MAX 24 // エフェクト枠の数 (粒の数)

//--------------------------------------------------------------------------
// 毎フレームの更新 : 0x1000 + timer × 0x50 ずつ回し、timer × 0x50 が 0x1000 に
// 達するまで timer を増やして速める
//--------------------------------------------------------------------------
void BattleFx_UpdateSpinAngle(struct FieldActor *obj)
{
	u32 step;

	step = obj->timer * 0x50;
	obj->facing = obj->facing + step + 0x1000;
	if (step < 0x1000)
		obj->timer = obj->timer + 1;
}

//--------------------------------------------------------------------------
// 毎フレームの更新 : 45 度ずつ回りながら縮み、y を 1 ドットずつ増やす
// 拡大率が 0x3000 を下回ったら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateDescendingObject(struct FieldActor *obj)
{
	u32 scale;

	obj->scale_y -= 0x280;
	scale = obj->scale_x - 0x280;
	obj->facing += 0x2000;
	obj->y.fixed += 1 << 16;
	obj->scale_x = scale;
	if ((s32)scale < 0x3000)
		obj->active = 0;
}

//--------------------------------------------------------------------------
// 粒の動き : 主オブジェクトの画面位置 (z + 8 ドット) から、0xc000 の向きの ±45 度へ
// 120 ドット飛ぶ。状態 1 は着いたら飛びなおし、状態 2 は着いたら消える
//--------------------------------------------------------------------------
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot)
{
	struct EffectVector pos;
	struct FieldActor  *src;
	s32					state;
	u32					r;

	src = ((struct BattleFxScene *)gEffectWork)->main_object;
	state = slot->state;

	if (state == 0)
	{
		pos.x = src->x.fixed;
		pos.y = src->y.fixed;
		pos.z = src->z.fixed;
		Camera_WorldToScreen(&pos);

		slot->x = pos.x;
		slot->z = pos.z + (8 << 16);
		slot->origin_z = slot->z;
		slot->origin_x = slot->x;
		pos.z = slot->origin_z;
		pos.x = slot->origin_x;

		r = Random16();
		Vector_AddPolarOffset(
			120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000, &pos);

		slot->target_x = pos.x;
		slot->target_z = pos.z;
		slot->acceleration = 5 << 16;
		slot->max_speed = 5 << 16;
		slot->flag42 = state;
		slot->state++;

		if ((gFrameCount & 2) != 0)
			AudioCommand_PlayFar(246);
	}
	else if (state == 1)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			slot->state--;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTarget(slot) == 0)
			BattleFx_ClearOwnedSlot(slot);
	}
}

//--------------------------------------------------------------------------
// 戦闘エフェクト 15 番
// 主オブジェクトを回して縮めながら、粒を 1 フレームに 1 つずつ 24 個飛ばす
//--------------------------------------------------------------------------
void BattleFx_RunEffect15(void)
{
	struct BattleFxScene *scene = gEffectWork;
	struct FieldActor	 *obj = scene->main_object;
	struct EffectVector	  pos;
	struct EffectSlot	 *slot;
	u8					  last;
	u32					  i;

	Battle_Reset();
	Motion_CamBounds(-1, -1, -1, 0);
	BattleEffect_InitializeSharedScene();
	WaitFrames(10);
	ObjectMotion_ArmCallback(scene->first_object_id, 0x4000, 0);
	WaitFrames(30);
	obj->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
	AudioCommand_PlayFar(131);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 28);
	WaitFrames(40);

	// 回りはじめる
	AudioCommand_PlayFar(220);
	Animation_ApplyChildValuesFar(obj, 0);
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, 3);
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
	obj->timer = 0;
	WaitFrames(70);

	// 縮みはじめ、その画面位置から粒を飛ばす
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	obj->motion_flags = 0;
	obj->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
	obj->target_x = ACTOR_NO_TARGET;
	pos.x = obj->x.fixed;
	pos.y = obj->y.fixed;
	pos.z = obj->z.fixed;
	Camera_WorldToScreen(&pos);
	i = 0;
	slot = scene->slots;
	do
	{
		s32 scale;
		EffectSlot_Initialize(slot, 0x11c, pos.x, pos.z);
		EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
		EffectSlot_SetObjectMode(slot, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
		scale = (Random16() >> 1) + 0x13333; // 1.2〜1.7 倍
		slot->scale_y = scale;
		slot->scale_x = scale;
		i++;
		WaitFrames(1);
		slot++;
	} while (i < SLOT_MAX);
	WaitFrames(70);

	// 飛んでいる粒を、着いたら消える状態にする
	i = 0;
	last = 2;
	do
	{
		if (scene->slots[i].active != 0)
			scene->slots[i].state = last;
		i++;
	} while (i < SLOT_MAX);
	WaitFrames(40);
	BattleFx_PrepareBufferInterpolation();
	WaitFrames(10);
}

void BattleFx_UpdateDescendingParticlePositiveArc(struct FieldActor *particle)
{
	struct FieldActor *source;
	s32				   y;
	s32				   limit;
	s32				   scale;

	limit = particle->terrain_height + 0xA0000;
	source = particle->linked_object;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->terrain_height + 0x40000;

	y = particle->y.fixed;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - 0x20000;
	particle->z.fixed = source->z.fixed + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct FieldActor *particle)
{
	struct FieldActor *source;
	s32				   y;
	s32				   limit;
	s32				   scale;

	limit = particle->terrain_height + 0xA0000;
	source = particle->linked_object;
	if (gGameState.map == (s32)&SceneId_Clear)
		limit = particle->terrain_height + 0x40000;

	y = particle->y.fixed;
	if (y <= limit)
	{
		ObjectDispatch_ReleaseFar(particle);
		return;
	}

	scale = particle->scale_x + 0xC00;
	if (scale > 0x10000)
		scale = 0x10000;
	particle->scale_x = scale;
	particle->scale_y = -scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - 0x20000;
	particle->z.fixed = source->z.fixed - ((0x10000 - scale) * 5) + 0x100000;
}

struct DescendingArcSprite
{
	u8 padding00[5];
	u8 flags05_0 : 5;
	u8 flags05_5 : 1;
	u8 layer : 2;
	u8 padding06;
	u8 flags07_0 : 6;
	u8 size : 2;
	union
	{
		struct
		{
			u16 tile : 10;
			u16 flags08_10 : 6;
		} half;
		struct
		{
			u8 tile_low;
			u8 flags09_0 : 2;
			u8 variant : 2;
			u8 flags09_4 : 4;
		} byte;
	} attr;
	u8					   padding0c[16];
	u8					   resource;
	u8					   active : 1;
	u8					   flags1d_1 : 7;
	u8					   padding1e[8];
	u8					   animation;
	u8					   padding27;
	struct AnimationEntry *frame;
};

void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc)
{
	struct FieldActor		   *source = arc;
	struct BattleFxScene	   *scene = gEffectWork;
	struct FieldActor		   *leader = scene->main_object;
	struct FieldActor		   *objects[2];
	struct FieldActor		   *object;
	struct DescendingArcSprite *sprite;
	s32							i;
	u8							zero;

	for (i = 0; i <= 1; i++)
	{
		object = Object_CreateFar(26, source->x.fixed, source->y.fixed + 0x400000, source->z.fixed);
		objects[i] = object;
		if (object == NULL)
			continue;
		object->terrain_height = source->terrain_height;
		sprite = object->sprite;
		object->motion_flags = 0;
		object->timer = 0;
		zero = 0;
		object->linked_object = source;
		object->scale_y = 0x6666;
		object->scale_x = 0x6666;
		if (sprite == NULL)
			continue;
		AnimationObjects_SelectAnimationFar(sprite, 0);
		sprite->animation = zero;
		Resource_ResetEntry(sprite->resource);
		sprite->resource = scene->arc_tile_slot;
		sprite->active = 1;
		sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
		sprite->flags05_5 = 0;
		sprite->layer = 1;
		sprite->size = 2;
		sprite->frame->frame = zero;
	}
	objects[0]->update = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
	((struct DescendingArcSprite *)objects[0]->sprite)->attr.byte.variant = 0;
	objects[1]->update = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
	((struct DescendingArcSprite *)objects[1]->sprite)->attr.byte.variant =
		((struct DescendingArcSprite *)leader->sprite)->attr.byte.variant;
}

//--------------------------------------------------------------------------
// arcstate.c
// エナジー演出 16 (プルーフ)：使う者の動きと、効果が切れたときのメッセージ
//--------------------------------------------------------------------------

#define ENA_PURUFU 150 // プルーフ
#define ITEM_HANE 236  // せいなるはね

//--------------------------------------------------------------------------
// 演出の終わり：使う者の表示の flags を 5 フレームの周期で 3 と 1 に切り替える
//--------------------------------------------------------------------------
void BattleFx_UpdateEffect16State(void)
{
	struct ArcEffectObject *obj;
	struct AnimationObject *rec;
	struct AnimationEntry  *entry;
	s32						phase;

	obj = ObjectTable_Get(gGameState.selected_actor);
	rec = obj->records;
	entry = rec->entries[0];
	phase = gFrameCount % 5;
	if (phase == 0)
	{
		rec->dirty = 1;
		rec->flags = 3;
	}
	else if (phase == 2)
	{
		entry->param = 0; // 色を戻す
		rec->dirty = 1;
		rec->flags = 1;
	}
}

//--------------------------------------------------------------------------
// 演出中の使う者：5 フレームごと (ワールドマップでは 7) に降る弧の粒を出し、
// phase 1 では回る
//--------------------------------------------------------------------------
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *obj)
{
	s16 phase;
	s16 cnt;

	phase = obj->phase;
	cnt = obj->step++;

	if (gGameState.map == (s32)&SceneId_Clear)
	{
		if (__modsi3(cnt, 7) == 0)
			BattleFx_SpawnDescendingArcParticles(obj);
	}
	else if (__modsi3(cnt, 5) == 0)
	{
		BattleFx_SpawnDescendingArcParticles(obj);
	}

	if (phase == 1)
		obj->angle += 0xc00;
}

//--------------------------------------------------------------------------
// プルーフ (せいなるはね) の効果が切れた
//--------------------------------------------------------------------------
void FieldEvent_ShowStatusMessage(void)
{
	gGameState.avoid_distance = 0;
	if (gGameState.avoid_by_item == 0)
	{
		UiWork_PushValueSlotFar(ENA_PURUFU, 4);
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgAbilityWoreOff, 1);
		return;
	}
	UiWork_PushValueSlotFar(ITEM_HANE, 2);
	UiText_ShowPositionedMessageAndWaitFar((s32)MsgItemWoreOff, 1);
}

void RunBattleEffect16(void)
{
	struct BattleFxScene   *scene;
	struct ArcEffectObject *object;
	struct AnimationObject *group;
	struct AnimationEntry  *entry;
	u32						angle;
	s32						slot;
	s32						count;

	scene = gEffectWork;
	object = scene->main_object;
	group = object->records;
	entry = group->entries[0];
	angle = object->angle;
	slot = Resource_FindFreeEntry();
	{
		s16 *tile_slot = &scene->arc_tile_slot;
		s32	 zero = 0;

		*tile_slot = slot;
		VramBlock_LoadCached((s16)slot, 0x100, BattleFx_ArcSparkTiles);
		gGameState.avoid_distance = 0x09600000;
		gGameState.avoid_by_item = GameFlag_TestFar(0x145);
		Animation_ApplyChildValuesFar(object, zero);
		object->callback = BattleFx_UpdatePairedArcSpawner;
		object->phase = zero;
		object->step = zero;
	}
	AudioCommand_PlayFar(0x8c);
	WaitFrames(15);
	object->phase = 1;
	WaitFrames(10);
	for (count = 0; count < 20; count++)
	{
		entry->param = 7;
		group->dirty = 1;
		WaitFrames(2);
		group->dirty = 1;
		entry->param = 0;
		group->flags = 1;
		WaitFrames(3);
	}
	object->callback = NULL;
	object->angle = angle;
	Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
	WaitFrames(15);
	AudioCommand_PlayFar(0xae);
	WaitFrames(55);
	Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
	if (gGameState.cloaked != 0)
		ObjectDispatch_SetSingleChildFlagsFar(object, 2);
	else
		ObjectDispatch_SetSingleChildFlagsFar(object, 1);
	Animation_ApplyChildValuesFar(object, 0);
	Resource_ResetEntry(scene->arc_tile_slot);
	UiText_ShowPositionedMessageAndWaitFar((s32)MsgMonstersAttackLess, 1);
}

void EffectSlot_Update(struct EffectSlot *effect)
{
	if (effect->active != 0)
	{
		effect->age++;
		if (effect->callback_delay != 0)
			effect->callback_delay--;
		else if (effect->callback != 0)
			effect->callback(effect);
		if (effect->active != 0)
		{
			if (effect->update_motion != 0)
				EffectSlot_UpdateMotion(effect);
			if (effect->render != 0)
				BattleFx_DrawScaledObject(effect);
		}
	}
}

void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
	struct AnimationObject *sprite;
	s32						offset;
	s32						scale[2];
	s32						position[4];

	sprite = object->object;
	offset = 0;
	if (object->flags & 4)
		offset = 0x1fc0000 - object->z;
	scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
	scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
	position[0] = object->x;
	position[1] = offset;
	position[2] = object->z + offset;
	position[3] = 0;
	if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000
		&& object->z < 0xe00000)
		Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
	s32 dx;
	s32 dz;
	s32 distance;
	s32 ix;
	s32 iz;
	s32 speed;
	s16 angle;
	s32 heading;
	s32 facing;
	s16 turn;

	if (effect->target_x == EFFECT_NO_TARGET)
		return;
	dx = effect->target_x - effect->x;
	dz = effect->target_z - effect->z;
	if (effect->stop_at_target != 0)
	{
		ix = dx / 0x10000;
		iz = dz / 0x10000;
		distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
		if (distance < 0x800000)
			distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
		if (distance <= 0x80000)
		{
			EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
			return;
		}
	}
	angle = ArcTan2(dz, dx);
	if (effect->flag42 != 0)
	{
		heading = effect->heading;
		turn = angle - heading;
		if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step)
		{
			if (turn < 0)
			{
				if (-turn > effect->max_turn_step)
					turn = -effect->max_turn_step;
			}
			else if (turn > effect->max_turn_step)
			{
				turn = effect->max_turn_step;
			}
			angle = turn + heading;
		}
	}
	facing = (u16)angle;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if (speed > effect->max_speed)
		speed = effect->max_speed;
	angle = facing;
	effect->speed = speed;
	effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
	effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
	u32 value;

	if (effect->stop_at_target == 0)
	{
		return 0;
	}
	value = effect->target_x ^ 0x80000000;
	return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
	effect->target_x = EFFECT_NO_TARGET;
	effect->target_z = EFFECT_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
	AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
	effect->callback = callback;
	effect->callback_delay = 0;
	effect->age = 0;
	effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
	s32			 unit;
	s8			*object;
	volatile u32 zero;

	zero = 0;
	Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4), REG_DMA3);
	object = ResourceObject_CreateFar(kind);
	effect->object = object;
	if (object != NULL)
		object[9] &= ~0x0c;
	EffectSlot_SetPosition(effect, x, z);
	/* FAKEMATCH: needed to match: 1.0 is held in a local, set before max_speed, so the object reload is scheduled ahead of the three unit stores */
	unit = 0x10000;
	effect->max_speed = 0x20000;
	effect->acceleration = effect->scale_y = effect->scale_x = unit;
	effect->origin_x = x;
	effect->origin_z = z;
	((s8 *)effect->object)[38] = 0;
	effect->stop_at_target = 1;
	effect->flag42 = 1;
	effect->update_motion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->random_value = Random16();
	effect->flags = 4;
	EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
	volatile u32 zero;
	if (slot->object)
		ResourceObject_ReleaseFar(slot->object);
	zero = 0;
	Dma_Set(&zero, slot, 0x85000000 | (sizeof *slot / 4), REG_DMA3);
}

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork
{
	u16						   vram_block;	// 印の OBJ キャラクタの VRAM ブロック
	u16						   window_slot; // もうひとつの VRAM ブロック（終わりに返す）
	union FieldCoordinate	   x;			// カーソル（ドット）
	union FieldCoordinate	   z;
	u8						   padding0c[6];
	s16						   shown; // いま地名ウインドウに出している印（-1 はなし）
	u8						   padding14[4];
	s32						   speed; // カーソルの速さ
	s32						   window;
	struct AnimationSpritePart markers[67];
};

void BattleFx_SetupResourcesAndWindow(void)
{
	u32					*oam;
	volatile u32		 fill;
	u32					*pal;
	struct WorldMapWork *work;
	struct FieldActor	*object;
	s32					 attr;
	u32					 i;
	u32					 tile;
	u32					 attr2;

	pal = Runtime_BumpAllocateAlternatePool(32);
	work = (struct WorldMapWork *)gMapCellBuffer;
	oam = (u32 *)work->markers;
	object = Object_GetById(gGameState.selected_actor);
	work->vram_block = Resource_FindFreeEntry();
	fill = 0;
	Dma_Set((const void *)&fill, pal, 0x85000020, REG_DMA3);
	pal[0] = 255;
	pal[1] = 0x1ff;
	pal[2] = 272;
	pal[8] = 68;
	pal[9] = 324;
	pal[10] = 272;
	pal[16] = 119;
	pal[17] = 0x177;
	pal[18] = 272;
	pal[24] = 0xff0;
	pal[25] = 0xffff;
	pal[26] = 0x1ffff;
	pal[27] = 0x11ff0;
	pal[28] = 0x1100;
	attr = VramBlock_LoadCached(work->vram_block, 128, pal) | 0x400;
	for (i = 0; i < 66; i++)
	{
		*oam++ = 0;
		*oam++ = 0;
		*oam++ = attr;
	}
	MemFree(pal);
	if (GameFlag_TestFar(284))
	{
		work->x.fixed = 240 << 15;
		work->z.fixed = 160 << 15;
	}
	else
	{
		work->x.fixed = (((object->x.fixed - 0x10000000) >> 16) * 240 / 4096) << 16;
		work->z.fixed = ((object->z.part.pixel * 160) / 4096) << 16;
	}
	work->window_slot = Resource_LoadFixedBlockBIntoFreeSlotFar();
	tile = gVramBlockCache[work->window_slot].offset;
	work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
	work->shown = -1;
	work->speed = 0x10000;
	*oam++ = 0;
	*oam++ = 0x40000000;
	attr2 = 0x400;
	attr2 |= tile >> 5;
	*oam = attr2;
}

//--------------------------------------------------------------------------
// cleanup_resources_and_window.c
// ワールドマップ：地名の印・カーソル・地名ウインドウ
//--------------------------------------------------------------------------

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3	// 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void)
{
	struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

	Resource_ResetEntry(work->vram_block);
	Resource_ResetEntry(work->window_slot);
	UiWork_FinalizeFar(work->window, 2);
}

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void)
{
	s32							leader;
	const s32				   *place;
	s32							tile_base;
	s32							blend;
	s32							best;
	s32							best_dist;
	s32							cursor_x;
	s32							best_x;
	s32							best_y;
	s32							mes;
	s32							best_mes;
	s32							height;
	s32							width;
	s32							pos[3];
	struct WorldMapWork		   *work;
	struct FieldActor		   *obj;
	s32							cursor_y;
	s32							tile;
	s32							mode;
	s32							flag;
	s32							i;
	struct AnimationSpritePart *marker;
	s32							x;
	s32							y;
	s32							angle;

#if WORLD_MAP_CURSOR_SNAPS
	leader = gGameState.selected_actor;
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	marker = work->markers;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
#else
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	leader = gGameState.selected_actor;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
	marker = work->markers;
#endif
	best = -1;
	best_dist = WORLD_MAP_NAME_RANGE;
	blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

	//------------------------------------------------------------------
	// カーソル
	//------------------------------------------------------------------
	if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS))
	{
		// L・R：現在地
		obj = ObjectTable_Get(gGameState.selected_actor);
		if (obj == NULL)
			goto markers;
		cursor_x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
		cursor_y = obj->z.part.pixel * 160 / 4096;
	}
	else
	{
		// 十字キー：押し続けると速くなる
		angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
		if (angle != 0xffff)
		{
			pos[0] = work->x.fixed;
			pos[1] = 0;
			pos[2] = work->z.fixed;
			Vector_AddPolarOffset(work->speed, angle, pos);
			if (pos[0] < 0x100000) // x は 16〜239 ドット
				pos[0] = 0x100000;
			if (pos[0] > 0xef0000)
				pos[0] = 0xef0000;
			if (pos[2] < 0) // y は 0〜141 ドット
				pos[2] = 0;
			if (pos[2] > 0x8d0000)
				pos[2] = 0x8d0000;
			work->x.fixed = pos[0];
			work->z.fixed = pos[2];
			if (work->speed < 0x60000)
				work->speed += 0x2000;
		}
		else
		{
			work->speed = 0x10000;
		}
		cursor_x = work->x.part.pixel;
		cursor_y = work->z.part.pixel;
	}

	//------------------------------------------------------------------
	// 印：0 番は現在地、11〜65 番は WorldMap_PlaceMarkers の場所
	// 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
	//------------------------------------------------------------------
markers:
	for (i = 0; i <= 65; i++)
	{
		if (i != leader && i <= 10)
			continue;
		if (i == 0)
		{
			if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
				continue;
			tile = 0;
			flag = leader;
			mode = 0;
		}
		else
		{
			flag = *place++;
			mes = *place++;
			if (flag == 0)
				continue;
#if WORLD_MAP_CURSOR_SNAPS
			if (flag == -1)
				break;
			tile = flag >> 16;
			mode = 1;
#else
			tile = flag >> 16;
			mode = 1;
			if (flag == -1)
				break;
#endif
		}
		if (!GameFlag_TestFar(flag))
			continue;
		obj = ObjectTable_Get(i);
		if (obj == NULL)
			continue;

		// ワールドの 4096 ドット四方を 240x160 に縮める
		x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
		y = obj->z.part.pixel * 160 / 4096;
		marker->object_mode = mode;
		marker->tile = tile_base + tile;
		marker->x = x - 1;
		marker->y = y - 1;
		width = x - cursor_x;
		height = y - cursor_y;
		if (width * width + height * height < best_dist)
		{
			best_mes = mes;
			best = i;
			best_dist = width * width + height * height;
			best_x = x;
			best_y = y;
		}
		if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
			Runtime_PushSlotEntry(marker++, y);
	}
#if WORLD_MAP_CURSOR_SNAPS
	// キーを離していれば、カーソルをいちばん近い印へ寄せる
	if (gKeysHeld == 0)
	{
		if (best != -1)
		{
			if (best_dist <= 1)
			{
				work->x.fixed = best_x << 16;
				work->z.fixed = best_y << 16;
				work->speed = 0x10000;
			}
			else
			{
				if (work->speed < 0x20000)
					work->speed = 0x20000;
				angle = ArcTan2(best_y - cursor_y, best_x - cursor_x);
				pos[0] = work->x.fixed;
				pos[1] = 0;
				pos[2] = work->z.fixed;
				Vector_AddPolarOffset(work->speed, (u16)angle, pos);
				work->x.fixed = pos[0];
				work->z.fixed = pos[2];
				if (work->speed < 0x60000)
					work->speed += 0x2000;
			}
			cursor_x = work->x.part.pixel;
			cursor_y = work->z.part.pixel;
		}
		else
		{
			work->speed = 0x10000;
		}
	}
#endif

	// いちばん近い印を囲む枠（点滅）
	if (best != -1 && (gFrameCount & 15) <= 7)
	{
		marker->object_mode = 0;
		marker->tile = tile_base + 3;
		marker->x = best_x - 2;
		marker->y = best_y - 2;
		Runtime_PushSlotEntry(marker, best_y);
	}

	// カーソル
	marker = &work->markers[66];
	marker->affine_index = 0;
	marker->x = cursor_x - 17;
	marker->y = cursor_y + 1;
	Runtime_PushSlotEntry(marker, 246);

	//------------------------------------------------------------------
	// 地名ウインドウ：印が変わったときだけ書き直す
	//------------------------------------------------------------------
	if (work->shown != best)
	{
		work->shown = best;
		RenderOutput_PrepareForRedrawFar(work->window);
		if (best != -1)
		{
			if (best == 0)
				best_mes = (s32)MsgPresentLocation;
			else
				best_mes = Scene_GetAreaNameIndex(best_mes, 1) + (s32)MsgDebugEntryName;
			UiText_MeasureResourceEntriesFar(best_mes, &width, &height);
			cursor_x = best_x;
			cursor_y = best_y;
			cursor_x -= WORLD_MAP_NAME_LEFT;
			cursor_y -= 11;
			if (cursor_x + width > 240)
			{
				// 右へはみ出すなら、印の左上へ回す
				cursor_x = 232 - width;
				cursor_y = best_y - 20;
			}
			if (cursor_y < 0)
				cursor_y = 0;
			UiText_DrawResourceFar(best_mes, work->window, cursor_x, cursor_y);
		}
	}

	// 印の半透明を明滅させる
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

void Map_ShowWorldMap(s32 picture)
{
	struct MapState			*map = WORK_SLOT(8);
	struct EventWork		*field = Runtime_AllocateBlock(27, 0xccc);
	struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
	s32						 resource = (s32)&ResourceId_WorldMapPicture;
	struct MapAnimation		*layer;
	u8						 saved_flags[16];
	s32						 i;
	u8						*graphics;
	s32						 saved_status;
	s16						 blend;

	if (field->mode != 3)
		return;
	field->view_center->paused = 1;
	saved_status = field->transition_frames;
	field->transition_frames = 6;
	Event_FinishScreenTransition();
	Event_WaitTransitionFrames();
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		saved_flags[i] = layer->paused;
		layer->paused = 1;
		layer++;
	}
	menu->suspended = 1;
	WaitFrames(1);
	blend = REG_BLDALPHA;
	graphics = Resource_GetTableEntry(resource);
	Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4), REG_DMA3);
	BG_PLTT[0] = 0;
	Resource_DecodeType01(graphics + 448, gMapCellBuffer);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4),
		REG_DMA3);
	Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
	Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BattleFx_SetupResourcesAndWindow();
	Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
	if (GameFlag_TestFar(284))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
	do
	{
		WaitFrames(1);
	} while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
	Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
	BattleFx_CleanupResourcesAndWindow();
	Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	Map_LoadAreaGraphicsFar();
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		layer->paused = saved_flags[i];
		layer++;
	}
	menu->suspended = 0;
	Event_StartScreenTransition();
	Event_WaitTransitionFrames();
	field->transition_frames = saved_status;
	field->view_center->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  left;
	s32					  right;
	s32					  top;
	s32					  bottom;
	u32					  id;

	// the selected actor is looked up and the result discarded, as the ROM does
	Object_GetById(gGameState.selected_actor);
	object = (struct ObjectRuntime *)gEventWork->view_center;
	x = object->x;
	left = x - (320 << 16);
	right = x + (320 << 16);
	y = object->z;
	top = y - (600 << 16);
	bottom = y + (400 << 16);

	id = 8;
	do
	{
		object = ObjectTable_Get(id);

		if (object != 0)
		{
			s32 ox = object->x;
			s32 oy = object->z;

			if (ox < left || ox > right || oy < top || oy > bottom)
			{
				object->animation_kind = 0;
			}
			else
			{
				object->animation_kind = 1;
			}
		}
		id++;
	} while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
	struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

	if (field->mode == 3)
	{
		Scheduler_EnableUnmaskedOverlayCallbacks();
		BattleFx_UpdateObjectVisibilityBounds();
		Battle_InitializeRenderObject();
		BattleFx_ScheduleRatioTransition(0x9d89, 6);
		if (gKeysHeld & KEY_L)
		{
			do
			{
				WaitFrames(1);
			} while (gKeysHeld & KEY_L);
		}
		BattleFx_ScheduleRatioTransition(0x10000, 6);
		Scheduler_DisableOverlayCallbacks();
	}
}
