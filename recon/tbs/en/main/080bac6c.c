/* Draft, not exact (2026-09-28): 120 of 124 bytes, 14 aligned edits (was
   124 bytes / 26 edits with the enemy scan nested in the party scan).
   Sequential scans: the party scan as a for loop that breaks at the end of
   its list now matches the reference exactly (rotated, strength-reduced
   offset from 88, 0xfe hoisted). The enemy scan must stay unreduced, as
   the reference indexes (battle + 2) + (i * 2 + 100) each pass and loads
   0xfe inside the loop; every for-loop spelling tried here is reduced and
   hoists 0xfe, while do/while, while and goto spellings keep it unreduced
   but stop the party scan's rotation (duplicated exit test instead of the
   entry jump). Removal outside the enemy loop keeps 0xfe unhoisted but is
   still reduced. */
#include "TYPES.H"

struct RosterTarget {
    s16 actor;
    u8 unknown_02[14];
};

struct BattleRoster {
    u8 unknown_000[0x58];
    s16 party[7];
    s16 enemies[8];
    u8 unknown_076[0x276];
    struct RosterTarget targets[20];
};

struct RosterOwner {
    u8 unknown_000[0x12a];
    u8 in_battle;
};

extern struct BattleRoster *gBattleWork;
struct RosterOwner *Owner_GetStateFar(s32 owner);
s32 Summon_ReleaseCharge(s32 actor);

/* Takes an actor out of battle: clears its in-battle flag, marks it removed
   in the party or enemy list, releases its summon charge and clears it as
   a target. */
void BattleActor_RemoveFromLists(s32 actor)
{
    struct BattleRoster *work;
    s32 i;
    u32 j;
    s32 unit;

    work = gBattleWork;
    Owner_GetStateFar(actor)->in_battle = 0;
    for (i = 0; ; i++) {
        if (work->party[i] == actor) {
            work->party[i] = 0xfe;
            goto removed;
        }
        if (work->party[i] == 0xff)
            break;
    }
    for (i = 0; ; ) {
        unit = work->enemies[i];
        if (unit == actor) {
            work->enemies[i] = 0xfe;
            goto removed;
        }
        i++;
        if (unit == 0xff)
            return;
    }
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->targets[j].actor == actor)
            work->targets[j].actor = 0xff;
    }
}
