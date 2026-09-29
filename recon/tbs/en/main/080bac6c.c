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
   still reduced.
   2026-09-29 (alchemy permute scorer): the draft scored 575; a local
   enemies pointer inside the enemy loop scores 445 (3 register-only,
   5 operand, 2 reordered, 2 deleted) and matches the frame (push r5, r6).
   The enemy scan is still reduced (base + 102 walked by 2) where the ROM
   keeps i and rebuilds (i * 2 + 100) from base + 2 every pass. A goto
   enemy loop gives exactly the ROM's unreduced indexing, but every goto,
   while and do/while spelling tried (with break, goto or a label after the
   party scan) peels or unrotates the party scan (660 to 2515). About
   160,000 searched candidates found nothing below 445. */
/* 2026-09-29 (Mercury): the lists and action queue are now BattleSession
   fields (BATTLE_WORK.H); the enemy scan indexes 50 entries past a base
   two bytes into the work, as Battle_ResolveTargetAction's summon insert
   does. Still 445: the ROM keeps the enemy scan unreduced. */
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
    s16 *slots;

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
    slots = work->enemy_units - 50;
    for (i = 0; ; ) {
        unit = slots[i + 50];
        if (unit == actor) {
            slots[i + 50] = 0xfe;
            goto removed;
        }
        i++;
        if (unit == 0xff)
            return;
    }
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->actions[j].unit_id == actor)
            work->actions[j].unit_id = 0xff;
    }
}
