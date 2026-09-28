/* Draft, not exact (2026-09-28): 124 of 124 bytes, 28 differing halfwords
   (was 116 bytes / 52). Both scans as ordinary for loops, the enemy scan
   nested in the party scan's end-of-list branch, give the reference's
   size and the rotated party loop. Remaining: the reference strength-
   reduces the party scan (offset from 88) but not the enemy scan (index
   shifted each pass, 0xfe reloaded from the pool); here it is the other
   way round. A goto enemy loop keeps it unreduced but breaks the party
   loop's rotation; do/while and sequential forms regress (54-64). */
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

    work = gBattleWork;
    Owner_GetStateFar(actor)->in_battle = 0;
    for (i = 0; ; i++) {
        if (work->party[i] == actor) {
            work->party[i] = 0xfe;
            goto removed;
        }
        if (work->party[i] == 0xff) {
            for (i = 0; ; i++) {
                if (work->enemies[i] == actor) {
                    work->enemies[i] = 0xfe;
                    goto removed;
                }
                if (work->enemies[i] == 0xff)
                    return;
            }
        }
    }
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->targets[j].actor == actor)
            work->targets[j].actor = 0xff;
    }
}
