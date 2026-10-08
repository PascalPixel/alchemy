//--------------------------------------------------------------------------
// encountr.c
// エンカウント：場所ごとの出現テーブルの選択と、敵グループの抽選
//--------------------------------------------------------------------------

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "battle_unit.h"

#define BattleFx_SetSpecialFromTable ((void (*)(void))BattleFx_SetSpecialFromTable)

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

s32 GameFlag_TestFar(s32 flag);

// ワールドマップの地形ごとの出現テーブル (BattleFx_ResultRules、-1 で終わり)
typedef struct TerrainRule
{
	s16 kind;	 // 地形の種類 (マップブロックから)
	s16 terrain; // 地形の動作 (-1 ならどれでも)
	s16 flag;	 // -1 か、立っていない間だけ有効なフラグ
	s16 zone;	 // 出現テーブル番号
} TerrainRule;

extern TerrainRule BattleFx_ResultRules[];

// 出現テーブル
struct EncounterZone
{
	u16 rate;		// 基本の出現率
	u16 level;		// 想定レベル
	u16 enemies[8]; // 敵グループ
	u8	weights[8]; // 各グループの重み
};

struct FieldPartyState
{
	u8	unknown_000[0x1c0];
	s16 scene;
	s16 entrance;
	u8	unknown_1c4[0x74];
	s32 encounter_steps;
	u8	unknown_23c[8];
	s32 encounters_off;
	u8	unknown_248[4];
	s16 no_encounters;
};

extern struct EventWork		 *gEventWork;
extern struct FieldPartyState gGameState;
extern struct EncounterZone	  Encounter_EnemyGroupTable[];
struct BtlUnit				 *UnitGetFar(s32 unit);
u32							  Random16(void);
void						  BattleFx_SelectBattleCue(s32 zone);

//--------------------------------------------------------------------------
// 今のマップと部屋に合う規則から、出現テーブル番号の並びを作る
// use_rules : 0 なら規則を見ずに、すべて 0 (出現なし) にする
//--------------------------------------------------------------------------
void BattleFx_SelectLocationRule(s32 use_rules)
{
	/* FAKEMATCH: needed to match: the shift temporaries order the zero between the two stores */
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
		u8	v = work->encounter_groups[1];
		u8 *d = &work->encounter_groups[0];
		u8	zero = 0;

		*d = v;
		work->encounter_groups[1] = zero;
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
	if (gGameState.no_encounters != 0)
		return 0;
	entry = &Encounter_EnemyGroupTable[zone];
	rate = entry->rate;
	if (rate == 0)
		return 0;
	if (GameFlag_TestFar(5) != 0)
	{
		level = (s32)UnitGetFar(5)->experience;
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
	if (level > 0 && gGameState.encounters_off != 0)
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
	BattleFx_SelectBattleCue(zone);
	return sum;
}
