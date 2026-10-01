/* EXACT (score 0, 2026-10-01, wave 1 slice 5) but not adopted: the enemy
 * scan is a goto loop inside a block that runs once, and that block is not a
 * steering device the rules admit here, because without it the difference is
 * more than instruction order or register choice. Pascal's call.
 *
 * What the reference shows, measured with the loop pass dump:
 * - The party scan is a for (i = 0; ; i++) whose match block stays in the
 *   loop. find_and_verify_loops moves such a block next to any BARRIER at the
 *   depth of its target; it stays only while no unconditional jump lies
 *   outside a loop. So the enemy scan's return and back jump are inside loop
 *   notes.
 * - The enemy scan is not strength-reduced and its 0xfe is not hoisted:
 *   scan_loop skipped it. A noted region that begins with an insn instead of
 *   a label is "phony", which is what j = 0 before the scan's label gives;
 *   gcse still lifts work + 2 out.
 * - The scan counts in j, the u32 the action loop uses afterwards: with a
 *   separate s32 the counter and the base trade r0 and r1 (score 30).
 * Plain spellings of the scan (for, while (1), do/while (1), nested in the
 * party scan) are reduced and hoist 0xfe (575 to 765); a bare goto loop
 * leaves its jumps at depth 0 and the party scan is peeled (1195).
 * while (1) { j = 0; again: ...; goto again; } is exact as well. The earlier
 * draft pinned the index and the mark with empty asm (445). */
#include "TYPES.H"
#include "BATTLE_WORK.H"

struct RosterOwner {
    u8 unknown_000[0x12a];
    u8 in_battle;
};

struct RosterOwner *Owner_GetStateFar(s32 owner);
s32 Summon_ReleaseCharge(s32 actor);

void BattleActor_RemoveFromLists(s32 actor)
{
    struct BattleSession *work;
    s32 i;
    u32 j;
    s32 unit;

    work = gBattleWork;
    Owner_GetStateFar(actor)->in_battle = 0;
    for (i = 0; ; i++) {
        if (work->party_units[i] == actor) {
            work->party_units[i] = 0xfe;
            goto removed;
        }
        if (work->party_units[i] == 0xff)
            break;
    }
    do {
        j = 0;
again:
        unit = work->enemy_units[j];
        if (unit == actor) {
            work->enemy_units[j] = 0xfe;
            goto removed;
        }
        j++;
        if (unit == 0xff)
            return;
        goto again;
    } while (0);
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->actions[j].unit_id == actor)
            work->actions[j].unit_id = 0xff;
    }
}
