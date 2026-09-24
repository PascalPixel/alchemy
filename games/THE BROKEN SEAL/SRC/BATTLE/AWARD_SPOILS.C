#include "TYPES.H"

/* Hands out the battle spoils: experience to every living party member,
   with the level-up, newly learned Psynergy and stat-gain messages each
   level brings, then the coins, then the most valuable dropped item that
   still fits in someone's bag. */

struct BattleSpoils {
    s32 coins;
    s32 experience;
    u8 unk_08[4];
    u16 items[4];
};

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
struct SpoilsUnit *Owner_GetStateFar(s32 unit_id);
s32 Func_080770b8(s32 unit_id, s16 *gains);
void Audio_PlayCue(s32 cue);
void Party_AdjustSixDigitCounterAFar(s32 amount);
s32 Item_EncodeBankedId(s32 item);
s32 Func_08077030(s32 item);

/* The word-copy entry the runtime publishes in IWRAM. */
typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

extern u8 Data_03001e74_a[];
extern u8 Value_000008a0[];

#define BATTLE_SPOILS ((struct BattleSpoils *)(*(u8 **)Data_03001e74_a + 0x530))
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

    spoils = BATTLE_SPOILS;
    if (spoils->experience != 0) {
        UiWork_PushValueSlotFar(spoils->experience, 5);
        UiText_ShowMessageAndWaitCoreFar(0x83a);
        BattlePresentation_WaitForAdvance();
    }
    list = units;
    count = BattleParty_ListLivingUnits(1, list);
    backup = Runtime_BumpAllocateAlternatePool(sizeof(struct SpoilsUnit));
    for (i = 0; i < count; i++) {
        unit_id = list[i];
        unit = Owner_GetStateFar(unit_id);
        unit->experience += spoils->experience;
        while (((WordCopyFn)0x03001388)(backup, unit, sizeof(struct SpoilsUnit)),
            Func_080770b8(unit_id, gains) != 0) {
            Audio_PlayCue(0x59);
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->class_name, 3);
            UiWork_PushValueSlotFar(list[i], 1);
            UiWork_PushValueSlotFar(unit->level, 5);
            UiText_ShowMessageAndWaitCoreFar(0x89a);
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
                        UiText_ShowMessageAndWaitCoreFar(0x89b);
                        BattlePresentation_WaitForAdvance();
                    }
                }
            }
            if (gains[2] != 0) {
                UiWork_PushValueSlotFar(gains[2], 5);
                UiText_ShowMessageAndWaitCoreFar(0x89c);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[3] != 0) {
                UiWork_PushValueSlotFar(gains[3], 5);
                UiText_ShowMessageAndWaitCoreFar(0x89d);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[4] != 0) {
                UiWork_PushValueSlotFar(gains[4], 5);
                UiText_ShowMessageAndWaitCoreFar(0x89e);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[5] != 0) {
                UiWork_PushValueSlotFar(gains[5], 5);
                UiText_ShowMessageAndWaitCoreFar(0x89f);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[6] != 0) {
                UiWork_PushValueSlotFar(gains[6], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)Value_000008a0);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[7] != 0) {
                UiWork_PushValueSlotFar(gains[7], 5);
                UiText_ShowMessageAndWaitCoreFar(0x8a1);
                BattlePresentation_WaitForAdvance();
            }
        }
    }
    Runtime_BumpFree(backup);
    if (spoils->coins != 0) {
        UiWork_PushValueSlotFar(spoils->coins, 5);
        UiText_ShowMessageAndWaitCoreFar(0x83b);
        Party_AdjustSixDigitCounterAFar(spoils->coins);
        BattlePresentation_WaitForAdvance();
    }
    found = (u16 *)0x0200047c;
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
        UiText_ShowMessageAndWaitCoreFar(0x83c);
        BattlePresentation_WaitForAdvance();
        if (Func_08077030(spoils->items[best_slot]) == -1) {
            *found = spoils->items[best_slot];
            break;
        }
        spoils->items[best_slot] = 0;
    }
}
