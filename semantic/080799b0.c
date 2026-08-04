#include "layout_guard.h"
#include "types.h"

enum {
    SCORE_COUNT_080799B0 = 4,
    HIGHEST_RANK_080799B0 = 202,
};

typedef struct RankRequirement_080799b0 {
    s32 score_key;
    u8 minimum_tens[SCORE_COUNT_080799B0];
    u8 padding08[0x4c];
} RankRequirement_080799b0;

LAYOUT_OFFSET_GUARD(
    RankRequirement080799b0_Minimums,
    RankRequirement_080799b0,
    minimum_tens,
    4);
LAYOUT_SIZE_GUARD(
    RankRequirement080799b0_Size,
    RankRequirement_080799b0,
    0x54);

void Func_080797fc(u8, const u8 *, s32 *);
s32 Func_08079338(s32);
s32 Func_080797ec(s32, s32);

static s32 FindLargestScore_080799b0(
    const s32 *scores,
    s32 excluded)
{
    s32 largest = -1;
    s32 largest_index = -1;
    s32 index;

    for (index = 0; index < SCORE_COUNT_080799B0; index++) {
        if (index != excluded && largest < scores[index]) {
            largest = scores[index];
            largest_index = index;
        }
    }

    return largest_index;
}

static s32 MeetsRequirements_080799b0(
    const RankRequirement_080799b0 *requirement,
    const s32 *scores)
{
    s32 index;

    for (index = 0; index < SCORE_COUNT_080799B0; index++) {
        if (scores[index] < requirement->minimum_tens[index] * 10)
            return 0;
    }

    return 1;
}

/*
 * Classify a four-score result.  Types 0, 1, and 5 can return their dedicated
 * sentinel ranks; other types derive a score key from the strongest two
 * dimensions and then choose the highest table rank whose four minimums are
 * met.  Rank zero is the fallback when no record qualifies.
 */
s8 Func_080799b0(u8 type, const u8 *input)
{
    const RankRequirement_080799b0 *requirement;
    s32 scores[SCORE_COUNT_080799B0];
    s32 strongest;
    s32 second;
    s32 key_partner;
    s32 score_key;
    s32 rank;

    if (type > 7)
        return 0;

    Func_080797fc(type, input, scores);
    if (Func_08079338(0x20) != 0) {
        if (type == 0)
            return (s8)200;
        if (type == 1)
            return (s8)201;
    }
    if (type == 5)
        return (s8)202;

    strongest = FindLargestScore_080799b0(scores, -1);
    second = FindLargestScore_080799b0(scores, strongest);
    key_partner = strongest;
    if (scores[second] > 9)
        key_partner = second;
    score_key = Func_080797ec(strongest, key_partner);

    requirement = (const RankRequirement_080799b0 *)0x08088d64;
    for (rank = HIGHEST_RANK_080799B0; rank >= 0; rank--, requirement--) {
        if (requirement->score_key == score_key &&
            MeetsRequirements_080799b0(requirement, scores)) {
            return (s8)rank;
        }
    }

    return 0;
}
