#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "INVENTORY.H"
#include "BATTLE_WORK.H"
#include "BATTLE_TYPES.H"

/* Counts one defeated enemy toward the battle spoils: its coins and
   experience (randomly raised in proportion to the enemy's level when the
   party earned them), the formation slot it came from, and a chance at its
   item drop, which replaces the least valuable drop so far. */

struct EnemyRecord {
    u8 unknown_00[0x4c];
    u16 coins;                      /* 0x4c */
    s16 item;                       /* 0x4e */
    s16 item_chance;                /* 0x50 */
    u16 experience;                 /* 0x52 */
};

struct EnemyRecord *Owner_GetRecordFar(s32);
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
u32 Random16(void);
u32 BattleRandom16Far(void);
s32 Item_EncodeBankedId(s32);

static __inline__ s32 BattleEnemy_CalculateCoinReward(
    struct BattleUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 6) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(
    struct BattleUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 4) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
    struct BattleUnit *unit;
    struct EnemyRecord *rec;
    struct BattleSession *formation;
    struct BattleSpoils *spoils;
    s32 i;
    s32 chance;
    s32 slot;
    s32 lowest;
    s32 lowest_slot;
    s32 value;

    unit = (struct BattleUnit *)Owner_GetStateFar(unit_id);
    formation = gBattleWork;
    spoils = &formation->spoils;
    if ((u32)unit_id < 8)
        return -1;
    if (unit->class_index != 0)
        return -2;
    slot = 0;
    i = 0;
    if (formation->enemy_classes[0] != unit->class_id) {
        do {
            if (++i > 5)
                break;
        } while (formation->enemy_classes[i] != unit->class_id);
    }
    if (i != 6)
        slot = i;
    if (formation->defeat_state != 2) {
        if (slot < formation->first_defeated)
            formation->first_defeated = slot;
        if (spoils->defeated)
            formation->defeat_state = 1;
    }
    spoils->defeated++;
    if (GameFlag_TestFar(0x173))
        return 0;
    GameFlag_SetBitFar(unit->class_id + 0x600);
    rec = Owner_GetRecordFar(unit->class_id);
    if (earned != 0) {
        if (rec->coins != 0)
            spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
        if (rec->experience != 0)
            spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
    } else {
        spoils->coins += rec->coins;
        spoils->experience += rec->experience;
    }
    if (rec->item == 0 || rec->item_chance == 0)
        return 0;
    i = 0;
    if (spoils->items[0] != rec->item) {
        do {
            if (++i > 3)
                break;
        } while (spoils->items[i] != rec->item);
    }
    if (i != 4)
        return 0;
    chance = rec->item_chance;
    if (earned != 0)
        chance -= 2;
    if (chance < 0)
        chance = 0;
    if ((0x20000 >> chance) > (s32)(BattleRandom16Far() & 0xffff)) {
        lowest_slot = -1;
        lowest = 0x40000000;
        for (i = 0; i <= 3; i++) {
            value = Item_EncodeBankedId(spoils->items[i]);
            if (value < lowest) {
                lowest = value;
                lowest_slot = i;
            }
        }
        if (Item_EncodeBankedId(rec->item) > lowest)
            spoils->items[lowest_slot] = rec->item;
    }
    return 0;
}

/* Hands out the battle spoils: experience to every living party member,
   with the level-up, newly learned Psynergy and stat-gain messages each
   level brings, then the coins, then the most valuable dropped item that
   still fits in someone's bag. */

struct SpoilsUnit {
    u8 unk_000[15];
    u8 level;
    u8 unk_010[0x48];
    struct { u16 id; u16 unk_02; } psynergy[32];
    u8 unk_0d8[0x4c];
    s32 experience;
    u8 unk_128;
    u8 class_name;
    u8 unk_12a[0x22];
};

void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiWork_ClearValueNameTablesFar(void);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattlePresentation_WaitForAdvance(void);
s32 BattleParty_ListLivingUnits(s32 side, u16 *units);
struct SpoilsUnit *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
s32 Func_080770b8(s32 unit_id, s16 *gains);
void Audio_PlayCue(s32 cue);
void Party_AdjustSixDigitCounterAFar(s32 amount);
s32 Item_EncodeBankedId(s32 item);
s32 PartyInventory_AddFar(s32 item);

extern u8 MsgAgilityRises[];
extern u8 MsgExpGained[];
extern u8 MsgLevelUp[];
extern u8 MsgAbilityMastered[];
extern u8 MsgMaxHpRises[];
extern u8 MsgMaxPpRises[];
extern u8 MsgAttackRises[];
extern u8 MsgDefenseRises[];
extern u8 MsgLuckRises[];
extern u8 MsgCoinsGained[];
extern u8 MsgItemGained[];

#define PSYNERGY_MASK 0x3fff

void Battle_AwardSpoils(void)
{
    struct BattleSpoils *spoils;
    s32 count;
    u16 *list;
    s32 i;
    s32 unit_id;
    struct SpoilsUnit *unit;
    struct SpoilsUnit *backup;
    s32 cnt;
    u32 learned;
    s32 j;
    s32 best;
    s32 best_slot;
    s32 slot;
    u16 *found;
    s32 value;
    s16 gains[8];
    u16 units[8];

    spoils = &gBattleWork->spoils;
    if (spoils->experience != 0) {
        UiWork_PushValueSlotFar(spoils->experience, 5);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgExpGained);
        BattlePresentation_WaitForAdvance();
    }
    list = units;
    count = BattleParty_ListLivingUnits(1, list);
    backup = Runtime_BumpAllocateAlternatePool(sizeof(struct SpoilsUnit));
    for (i = 0; i < count; i++) {
        unit_id = list[i];
        unit = (struct SpoilsUnit *)Owner_GetStateFar(unit_id);
        unit->experience += spoils->experience;
        while (Iwram_CopyWords(backup, unit, sizeof(struct SpoilsUnit)),
            Func_080770b8(unit_id, gains) != 0) {
            Audio_PlayCue(0x59);
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->class_name, 3);
            UiWork_PushValueSlotFar(list[i], 1);
            UiWork_PushValueSlotFar(unit->level, 5);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgLevelUp);
            BattlePresentation_WaitForAdvance();
            for (cnt = 0; cnt < 32; cnt++) {
                learned = unit->psynergy[cnt].id;
                if ((learned & PSYNERGY_MASK) && (learned >> 15)) {
                    for (j = 0; j < 32; j++) {
                        if (learned == backup->psynergy[j].id)
                            break;
                    }
                    if (j == 32) {
                        UiWork_ClearValueNameTablesFar();
                        UiWork_PushValueSlotFar(unit->class_name, 3);
                        UiWork_PushValueSlotFar(unit_id, 1);
                        UiWork_PushValueSlotFar(learned & PSYNERGY_MASK, 4);
                        Audio_PlayCue(0x9a);
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgAbilityMastered);
                        BattlePresentation_WaitForAdvance();
                    }
                }
            }
            if (gains[2] != 0) {
                UiWork_PushValueSlotFar(gains[2], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxHpRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[3] != 0) {
                UiWork_PushValueSlotFar(gains[3], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxPpRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[4] != 0) {
                UiWork_PushValueSlotFar(gains[4], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAttackRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[5] != 0) {
                UiWork_PushValueSlotFar(gains[5], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgDefenseRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[6] != 0) {
                UiWork_PushValueSlotFar(gains[6], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAgilityRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[7] != 0) {
                UiWork_PushValueSlotFar(gains[7], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgLuckRises);
                BattlePresentation_WaitForAdvance();
            }
        }
    }
    Runtime_BumpFree(backup);
    if (spoils->coins != 0) {
        UiWork_PushValueSlotFar(spoils->coins, 5);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgCoinsGained);
        Party_AdjustSixDigitCounterAFar(spoils->coins);
        BattlePresentation_WaitForAdvance();
    }
    found = &gOverflowItem;
    for (;;) {
        best_slot = -1;
        best = -1;
        for (slot = 0; slot <= 3; slot++) {
            if (spoils->items[slot] != 0) {
                value = Item_EncodeBankedId(spoils->items[slot]);
                if (value >= best) {
                    best = value;
                    best_slot = slot;
                }
            }
        }
        if (best_slot == -1)
            break;
        UiWork_PushValueSlotFar(spoils->items[best_slot], 2);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgItemGained);
        BattlePresentation_WaitForAdvance();
        if (PartyInventory_AddFar(spoils->items[best_slot]) == -1) {
            *found = spoils->items[best_slot];
            break;
        }
        spoils->items[best_slot] = 0;
    }
}
