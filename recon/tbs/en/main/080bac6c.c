/* Draft, not exact (2026-09-24): candidate=120 reference=124 differing_halfwords=37.
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
void Summon_ReleaseCharge(s32 actor);

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
        if (work->party[i] == 0xff)
            break;
    }
    for (i = 0;; i++) {
        if (work->enemies[i] == actor) {
            work->enemies[i] = 0xfe;
            goto removed;
        }
        if (work->enemies[i] == 0xff)
            return;
    }

removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->targets[j].actor == actor)
            work->targets[j].actor = 0xff;
    }
}
