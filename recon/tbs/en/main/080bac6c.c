/* Draft, not exact (2026-09-26): whole [080bac6c, 080bace8), 124 bytes.
   H1: independently scope the enemy scan counter and use an explicit
   backedge; audit Summon_ReleaseCharge against its s32 definition.
   Candidate 134/124 bytes, 58 differing halfwords, 32 aligned edits.
   The extra saved register is gone and the enemy scan is no longer
   strength-reduced, but the first scan is peeled and its vacant store
   moved after the enemy scan.  Counter/base r0/r1 are also reversed.
   H2: a phase-local u16 vacant marker changes the party store to an
   immediate, not the required early pool load.  128/124 bytes,
   55 differing halfwords, 29 aligned edits; the party scan remains
   peeled and the enemy branch still has an extra exit jump.
   H3: nest the explicit enemy scan at the party-sentinel exit, rejecting
   H2's immediate marker and retaining the original halfword stores.
   Candidate 116/124 bytes, 56 differing halfwords, 38 aligned edits;
   topology differs.  The party store stays in its loop but both scans
   now share a hoisted marker in r4, adding a saved register and merging
   the two reference pools.  Three hypotheses used; stop here.  All
   variants are committed; the unsigned target-clear tail still matches.
   No source registration or credited bytes.  Earlier baseline follows.
   Draft (2026-09-24): candidate=120 reference=124 differing_halfwords=37.
   Battle: take an actor out of the party and enemy rosters and clear it
   from the target records. The party loop and the unsigned target loop
   match. Open: the reference enemy loop is neither strength-reduced nor
   hoisted (address (work + 2) + (i * 2 + 100), 0xfe from the pool per
   store, i++ before the 0xff test); a goto loop gives that loop but then
   the party loop loses its layout. */
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

void Func_080bac6c(s32 actor)
{
    struct BattleRoster *work;
    s32 i;
    u32 j;

    work = gBattleWork;
    Owner_GetStateFar(actor)->in_battle = 0;

    for (i = 0;; i++) {
        if (work->party[i] == actor) {
            work->party[i] = 0xfe;
            goto removed;
        }
        if (work->party[i] == 0xff) {
            s32 enemy;
            s32 value;

            enemy = 0;
scan_enemy:
            value = work->enemies[enemy];
            if (value == actor) {
                work->enemies[enemy] = 0xfe;
                goto removed;
            }
            enemy++;
            if (value != 0xff)
                goto scan_enemy;
            return;
        }
    }

removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->targets[j].actor == actor)
            work->targets[j].actor = 0xff;
    }
}
