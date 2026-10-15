#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "edition.h"
#include "layout_guard.h"

enum
{
    BATTLE_ACTION_SIZE = 0x10,
    BATTLE_UNIT_SIZE = 0x14c,
    OWNER_ACTION_ID_MASK = 0x3fff
};

struct OwnerActionSlot
{
    u16 encodedAction;
    u16 unk02;
};

struct BattleElementStats
{
    s16 power;
    s16 res;
};

struct BattleAct
{
    u8  targetMode;
    u8  targetFlag;
    u8  damageClass;
    u8  effect;
    u8  unk04[4];
    u8  range;
    u8  ppCost;
    u16 power;
    u8  type0c;
    u8  unk0d[3];
};

enum BattleActionUse
{
    ACTION_USABLE_IN_FIELD = 0x40,
    ACTION_USABLE_IN_BATTLE = 0x80
};

struct BattleUnit
{
    u8                        name[0x0f];
    u8                        level;
    s16                       baseHp;
    s16                       basePp;
    s16                       hpGauge;
    s16                       ppGauge;
    u16                       baseAttack;
    u16                       baseDefense;
    u16                       baseAgility;
    u8                        baseLuck;
    u8                        baseActionCount;
    u8                        baseHpRegen;
    u8                        basePpRegen;
    u8                        unk022[2];
    struct BattleElementStats baseElement[4];
    s16                       maxHp;
    s16                       maxPp;
    s16                       hp;
    s16                       pp;
    u16                       atk;
    u16                       def;
    u16                       agi;
    u8                        luck;
    u8                        actionEntryCount;
    u8                        hpRegen;
    u8                        ppRegen;
    u8                        unk046[2];
    struct BattleElementStats element[4];
    struct OwnerActionSlot    actionSlots[32];
    u16                       inventory[15];
    u8                        unk0f6[2];
    u32                       djinnAvailable[4];
    u32                       djinnActive[4];
    u8                        djinnOwnedCounts[4];
    u8                        djinnActiveCounts[4];
    u32                       rewards;
    u32                       experience;
    u8                        classId;
    u8                        classIndex;
    u8                        side;
#if GAME_TLA
    u8 guardLv;
#else
    s8 guardLv;
#endif
    s8  elementLevels[4];
    s8  restraint;
    s8  poison;
    u8  atkCnt;
    s8  atkLv;
    u8  defCnt;
    s8  defLv;
    u8  resCnt;
    s8  resLv;
    u8  delusion;
    s8  confusion;
    u8  charm;
    u8  stun;
    u8  sleep;
    u8  psySeal;
    u8  refrain;
    s8  reflect;
    u8  haunt;
    u8  grimReaper;
    u8  unk142[2];
    u8  ready;
    u8  immobile;
    u8  agiCnt;
    s8  agiLv;
    s8  challenge;
    u8  unk149;
    u16 enemyId;
};

LAYOUT_SIZE_GUARD(BattleAction_Size, struct BattleAct, BATTLE_ACTION_SIZE);
LAYOUT_OFFSET_GUARD(BattleAction_Effect, struct BattleAct, effect, 0x03);
LAYOUT_OFFSET_GUARD(BattleAction_Power, struct BattleAct, power, 0x0a);
LAYOUT_OFFSET_GUARD(BattleAction_Type, struct BattleAct, type0c, 0x0c);

LAYOUT_SIZE_GUARD(OwnerActionSlot_Size, struct OwnerActionSlot, 4);
LAYOUT_SIZE_GUARD(BattleElementStats_Size, struct BattleElementStats, 4);
LAYOUT_SIZE_GUARD(BattleUnit_Size, struct BattleUnit, BATTLE_UNIT_SIZE);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseHp, struct BattleUnit, baseHp, 0x10);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseAttack, struct BattleUnit, baseAttack, 0x18);
LAYOUT_OFFSET_GUARD(BattleUnit_Actions, struct BattleUnit, actionSlots, 0x58);
LAYOUT_OFFSET_GUARD(BattleUnit_AvailableDjinn, struct BattleUnit, djinnAvailable, 0xf8);
LAYOUT_OFFSET_GUARD(BattleUnit_ActiveDjinn, struct BattleUnit, djinnActive, 0x108);
LAYOUT_OFFSET_GUARD(BattleUnit_Rewards, struct BattleUnit, rewards, 0x120);
LAYOUT_OFFSET_GUARD(BattleUnit_Hp, struct BattleUnit, hp, 0x038);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseElements, struct BattleUnit, baseElement, 0x024);
LAYOUT_OFFSET_GUARD(BattleUnit_Level, struct BattleUnit, level, 0x00f);
LAYOUT_OFFSET_GUARD(BattleUnit_Elements, struct BattleUnit, element, 0x048);
LAYOUT_OFFSET_GUARD(BattleUnit_ReadyPose, struct BattleUnit, ready, 0x144);
LAYOUT_OFFSET_GUARD(BattleUnit_Inventory, struct BattleUnit, inventory, 0x0d8);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnOwnedCounts, struct BattleUnit, djinnOwnedCounts, 0x118);
LAYOUT_OFFSET_GUARD(BattleUnit_DjinnActiveCounts, struct BattleUnit, djinnActiveCounts, 0x11c);
LAYOUT_OFFSET_GUARD(BattleUnit_GuardLevel, struct BattleUnit, guardLv, 0x12b);
LAYOUT_OFFSET_GUARD(BattleUnit_Status13a, struct BattleUnit, charm, 0x13a);
LAYOUT_OFFSET_GUARD(BattleUnit_Status140, struct BattleUnit, haunt, 0x140);

LAYOUT_OFFSET_GUARD(BattleUnit_EnemyId, struct BattleUnit, enemyId, 0x14a);

// ゲージの割合 (0x4000 で満タン) を 0〜0x4000 に収める
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

// HP と PP のゲージを計算し直す（残っていれば最低 1）
// A statement macro, written do { } while (0): every site matches through
// that shape. Written out at each site, or as an inline function, the
// gauge loads are scheduled across the store before them.
#define Unit_UpdateGauges(unit)                                             \
    do                                                                      \
    {                                                                       \
        (unit)->hpGauge = Vitals_Clamp(((unit)->hp << 14) / (unit)->maxHp); \
        if ((unit)->hpGauge == 0 && (unit)->hp != 0)                        \
            (unit)->hpGauge = 1;                                            \
        (unit)->ppGauge = Vitals_Clamp(((unit)->pp << 14) / (unit)->maxPp); \
        if ((unit)->ppGauge == 0 && (unit)->pp != 0)                        \
            (unit)->ppGauge = 1;                                            \
    } while (0)

#endif
