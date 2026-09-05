#include "types.h"
#include "fixed_math.h"

#define BattleFormation_SelectLevelMatchedCandidate Func_080c1afc

u32 Random16(void);

struct BattleUnitLevel {
    u8 reserved_00[0x0f];
    u8 level;
};

struct FormationCandidate {
    s16 record_id;
    s16 score;
};

struct BattleUnitLevel *Runtime_GetObject(s32 unit_id);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
s32 Func_080b6a60(u16 *out_units);
s32 Party_ComputeEligibleMemberAverage(s32 record_id);
void *Func_08077198(s32 id);
s32 Func_080770d0(s32 id);
s32 Func_080770e0(s32 id);
void Func_08002df0(void *ptr);
extern u16 Data_080c73f8[];

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin)
{
    s32 match_count = 0;
    struct FormationCandidate *pool =
        (struct FormationCandidate *)Runtime_BumpAllocateAlternatePool(128);
    u16 party[8];
    s32 level_total = 0;
    s32 unit_count = Func_080b6a60(party);
    s32 i;
    s32 j;
    s32 chance;
    s32 result;

    if (unit_count > 0) {
        u16 *p = party;

        i = unit_count;
        do {
            u8 level = Runtime_GetObject((s32)*p)->level;

            i--;
            p++;
            level_total += level;
        } while (i != 0);
    }

    chance = FixedPoint_Ratio(level_total, unit_count);
    chance += (s8)Func_080770e0(1016);
    if (chance <= 0)
        chance = 1;
    if (chance > 99)
        chance = 99;

    for (i = 0; i <= 31; i++)
        pool[i].score = -1;

    for (i = 0; (u32)i <= 19; i++) {
        Func_08077198(Data_080c73f8[i]);
        Func_080770d0(Data_080c73f8[i] + 1536);
    }

    for (i = 0; (u32)i <= 379; i++) {
        s32 score = Party_ComputeEligibleMemberAverage(i);

        if (score >= 0 && score <= chance + 3) {
            s32 min_index = -1;
            s32 min_value;

            for (j = 0, min_value = 999; j <= 31; j++) {
                if (pool[j].score < min_value) {
                    min_value = pool[j].score;
                    min_index = j;
                }
            }

            if (min_index >= 0) {
                pool[min_index].score = (s16)score;
                pool[min_index].record_id = (s16)i;
                match_count++;
            }
        }
    }

    if (match_count > 32)
        match_count = 32;

    if (match_count != 0) {
        struct FormationCandidate *chosen =
            &pool[(match_count * Random16()) >> 16];

        result = chosen->record_id;
        *out_margin = chance - chosen->score;
    } else {
        *out_margin = match_count;
        result = 1;
    }

    Func_08002df0(pool);
    return result;
}
