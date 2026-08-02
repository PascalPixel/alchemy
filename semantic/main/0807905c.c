#include "layout_guard.h"
#include "types.h"

enum { LEVEL_UP_STAT_COUNT_0807905C = 6 };

typedef struct LevelUpResult_0807905c {
    s16 level;
    s16 sentinel;
    u16 gains[LEVEL_UP_STAT_COUNT_0807905C];
} LevelUpResult_0807905c;

typedef struct OwnerProgress_0807905c {
    u8 padding000[0x0f];
    u8 level;
    u16 stat0;
    u16 stat1;
    u8 padding014[4];
    u16 stat2;
    u16 stat3;
    u16 stat4;
    u8 stat5;
    s8 stats_dirty;
    s8 field20;
    s8 field21;
    u8 padding022[0x102];
    u32 experience_target;
    u8 template_id;
    u8 growth_profile;
} OwnerProgress_0807905c;

typedef struct GrowthRecord_0807905c {
    u8 padding000[0x50];
    s16 stat0[6];
    s16 stat1[6];
    u16 stat2[6];
    u16 stat3[6];
    u16 stat4[6];
    u8 stat5[6];
    u8 padding092[0x22];
} GrowthRecord_0807905c;

typedef struct LevelUpWork_0807905c {
    s32 growth_profile;
    s32 old_level;
    GrowthRecord_0807905c *growth;
    u8 padding0c[0x20];
} LevelUpWork_0807905c;

LAYOUT_OFFSET_GUARD(
    OwnerProgress0807905c_Level,
    OwnerProgress_0807905c,
    level,
    0x0f);
LAYOUT_OFFSET_GUARD(
    OwnerProgress0807905c_Stat2,
    OwnerProgress_0807905c,
    stat2,
    0x18);
LAYOUT_OFFSET_GUARD(
    OwnerProgress0807905c_ExperienceTarget,
    OwnerProgress_0807905c,
    experience_target,
    0x124);
LAYOUT_OFFSET_GUARD(
    OwnerProgress0807905c_GrowthProfile,
    OwnerProgress_0807905c,
    growth_profile,
    0x129);
LAYOUT_OFFSET_GUARD(
    GrowthRecord0807905c_Stat0,
    GrowthRecord_0807905c,
    stat0,
    0x50);
LAYOUT_OFFSET_GUARD(
    GrowthRecord0807905c_Stat5,
    GrowthRecord_0807905c,
    stat5,
    0x8c);
LAYOUT_SIZE_GUARD(
    GrowthRecord0807905c_Size,
    GrowthRecord_0807905c,
    0xb4);
LAYOUT_SIZE_GUARD(
    LevelUpWork0807905c_Size,
    LevelUpWork_0807905c,
    0x2c);

s32 Func_080022ec(s32, s32);
s32 Func_080022f4(s32, s32);
void Func_08002df0(void *);
u32 Func_08004458(void);
LevelUpWork_0807905c *Func_08004970(s32);
OwnerProgress_0807905c *Func_08077394(s32);
void Func_08077428(s32);
void Func_08078bf0(s32);
GrowthRecord_0807905c *Func_08078ed8(s32);
u32 Func_08079008(s32, s32);

static u16 RandomGrowth_0807905c(s32 current, s32 next)
{
    s32 random_twentieth = (u32)(Func_08004458() * 0x14) >> 16;

    return Func_080022f4(next - current + random_twentieth, 0x14);
}

/* Advance one owner by one level and return the six generated stat gains. */
LevelUpResult_0807905c *Func_0807905c(
    s32 owner_index,
    LevelUpResult_0807905c *result)
{
    OwnerProgress_0807905c *owner = Func_08077394(owner_index);
    LevelUpWork_0807905c *work = Func_08004970(sizeof(*work));
    s32 stat;

    work->growth_profile = owner->growth_profile;
    work->old_level = owner->level;
    result->level = owner->level;
    result->sentinel = -1;
    for (stat = 0; stat < LEVEL_UP_STAT_COUNT_0807905C; stat++)
        result->gains[stat] = 0;

    if (owner->level <= 98) {
        u32 experience_target;
        s16 tier;

        owner->level++;
        result->level = owner->level;
        experience_target = Func_08079008(owner_index, owner->level);
        if (experience_target != (u32)-1 &&
            owner->experience_target < experience_target) {
            owner->experience_target = experience_target;
        }

        work->growth = Func_08078ed8(owner_index);
        if (owner->level == 1) {
            result->gains[0] += work->growth->stat0[0];
            result->gains[1] += work->growth->stat1[0];
            result->gains[2] += work->growth->stat2[0];
            result->gains[3] += work->growth->stat3[0];
            result->gains[4] += work->growth->stat4[0];
            result->gains[5] += work->growth->stat5[0];
        }

        tier = Func_080022ec((s16)owner->level, 0x14);
        if (tier < 0)
            tier = 0;
        else if (tier > 4)
            tier = 4;

        result->gains[0] += RandomGrowth_0807905c(
            work->growth->stat0[tier],
            work->growth->stat0[tier + 1]);
        result->gains[1] += RandomGrowth_0807905c(
            work->growth->stat1[tier],
            work->growth->stat1[tier + 1]);
        result->gains[2] += RandomGrowth_0807905c(
            work->growth->stat2[tier],
            work->growth->stat2[tier + 1]);
        result->gains[3] += RandomGrowth_0807905c(
            work->growth->stat3[tier],
            work->growth->stat3[tier + 1]);
        result->gains[4] += RandomGrowth_0807905c(
            work->growth->stat4[tier],
            work->growth->stat4[tier + 1]);
        result->gains[5] += RandomGrowth_0807905c(
            work->growth->stat5[tier],
            work->growth->stat5[tier + 1]);

        owner->stat0 += result->gains[0];
        owner->stat1 += result->gains[1];
        owner->stat2 += result->gains[2];
        owner->stat3 += result->gains[3];
        owner->stat4 += result->gains[4];
        owner->stat5 += result->gains[5];
        owner->stats_dirty = 1;
        owner->field20 = 0;
        owner->field21 = 0;
        Func_08078bf0(owner_index);
        Func_08077428(owner_index);
    }

    Func_08002df0(work);
    return result;
}
