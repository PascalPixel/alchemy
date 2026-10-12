#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
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
