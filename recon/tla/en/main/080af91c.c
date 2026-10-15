/* Near miss, differ 2 of 285 (one reordered pair): after the band's clamp
 * the native doubles the band (lsls r0, r5, #1), copies the work pointer
 * (mov r3, r9) and only then saves the doubled band (mov r8, r0); this saves
 * it before the copy. From the sched2 dump of that block: the copy (insn
 * 1143) and the save (1146) both have priority 482, and the save wins on
 * dependents (9 against 3), so identical instructions do not give the
 * native order. Measured: The Broken Seal's division form instead of the
 * library calls (same 2); an empty barrier after the clamp (same 2); an
 * input barrier on the work pointer puts the copy ahead of the shift
 * (differ 2); with the doubled band as a second operand the shift is
 * emitted twice (differ 9); five minutes of alchemy permute, 8,200
 * candidates, none better. The unit is common's struct BtlUnit; its
 * neighbour Owner_GetLevelThreshold is C in level_hook.c. */
#include "owner_state.h"
#include "mem.h"
#include "types.h"

struct OwnerLevelState {
    u8 unknown_000[0x0f];
    u8 level;
    u16 base_hp;
    u16 base_pp;
    u8 unknown_014[4];
    u16 base_attack;
    u16 base_defense;
    u16 base_agility;
    u8 base_luck;
    u8 base_turns;
    u8 base_20;
    u8 base_21;
    u8 unknown_022[0x102];
    u32 experience;
    u8 character;
    u8 class_id;
};

struct OwnerGrowth {
    u8 unknown_00[0x50];
    s16 hp[6];
    s16 pp[6];
    u16 atk[6];
    u16 def[6];
    u16 agi[6];
    u8 luck[6];
};

struct LevelUpResult {
    s16 level;
    u16 ability;
    u16 hp;
    u16 pp;
    u16 atk;
    u16 def;
    u16 agi;
    u16 luck;
};

struct LevelUpWork {
    s32 class_id;
    s32 level;
    struct OwnerGrowth *growth;
    u8 unused_0c[0x20];
};

void MemFree(void *buffer);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
struct OwnerGrowth *Character_GetDefinition(s32 owner);
s32 Owner_RefreshClassActions(s32 owner);
void UnitCalc(s32 owner);
u32 Random16(void);
s32 __divsi3(s32 numerator, s32 denominator);
u32 __udivsi3(u32 numerator, u32 denominator);

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
    struct OwnerLevelState *st;
    struct LevelUpWork *work;
    u32 threshold;
    s16 band;
    s32 diff;
    s32 level;

    st = UnitGet(owner);
    work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
    work->class_id = st->class_id;
    level = st->level;
    work->level = level;
    res->level = level;
    res->ability = 0xffff;
    res->hp = 0;
    res->pp = 0;
    res->atk = 0;
    res->def = 0;
    res->agi = 0;
    res->luck = 0;
    if (level < 99) {
        st->level++;
        res->level = level + 1;
        threshold = Owner_GetLevelThreshold(owner, st->level);
        if (threshold != (u32)-1 && st->experience < threshold)
            st->experience = threshold;
        work->growth = Character_GetDefinition(owner);
        if (res->level == 1) {
            res->hp += work->growth->hp[0];
            res->pp += work->growth->pp[0];
            res->atk += work->growth->atk[0];
            res->def += work->growth->def[0];
            res->agi += work->growth->agi[0];
            res->luck += work->growth->luck[0];
        }
        band = __divsi3(res->level, 20);
        if (band < 0)
            band = 0;
        if (band > 4)
            band = 4;
        diff = work->growth->hp[band + 1] - work->growth->hp[band];
        res->hp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->pp[band + 1] - work->growth->pp[band];
        res->pp += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->atk[band + 1] - work->growth->atk[band];
        res->atk += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->def[band + 1] - work->growth->def[band];
        res->def += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->agi[band + 1] - work->growth->agi[band];
        res->agi += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->luck[band + 1] - work->growth->luck[band];
        res->luck += __udivsi3((Random16() * 20 >> 16) + diff, 20);
        st->base_hp += res->hp;
        st->base_pp += res->pp;
        st->base_attack += res->atk;
        st->base_defense += res->def;
        st->base_agility += res->agi;
        st->base_luck += res->luck;
        st->base_turns = 1;
        st->base_20 = 0;
        st->base_21 = 0;
        Owner_RefreshClassActions(owner);
        UnitCalc(owner);
    }
    MemFree(work);
    return res;
}
