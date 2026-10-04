#include "TYPES.H"
#include "OWNER_STATE.H"
#include "BATTLE_TYPES.H"

/* Enemies 110 and 113 are left at one HP rather than defeated. */
s32 BattleUnit_KeepsOneHp(s32 unit_id)
{
    struct BattleUnit *unit = Owner_GetState(unit_id);

    if (unit->enemy_id == 110 || unit->enemy_id == 113)
        return 1;
    return 0;
}

/*
 * Puts UNIT into a battle line of up to six entries: into the free place
 * (254) nearest the line's divider (128), else the first free place, else
 * at the end, where 255 closes the line again. The units on each side of
 * the divider are counted on the way.
 */
void BattleParty_InsertUnitCentered(u16 *list, s32 unit)
{
    s32 counts[2] = {0, 0};
    s32 side;
    s32 slot;
    s32 divider;
    s32 i;
    u16 *entry;

    slot = -1;
    side = 0;
    divider = slot;
    i = 0;
    entry = list;
    do {
        u32 value = *entry++;

        if (value == 254) {
            if (divider >= 0 && slot >= 0) {
                s32 distance = i - divider;

                if (distance < 0)
                    distance = divider - i;
                /* FAKEMATCH: the native compares the distances in this order. */
                __asm__("");
                if (slot - divider >= 0) {
                    if (distance >= slot - divider)
                        goto next;
                } else if (distance >= divider - slot) {
                    goto next;
                }
            }
            slot = i;
            /* FAKEMATCH: the native branches to the loop step from here. */
            __asm__("");
        next:;
        } else {
            if (value == 255)
                break;
            if (value == 128) {
                side ^= 1;
                divider = i;
            }
            counts[side]++;
        }
        i++;
    } while (i <= 5);
    if (slot >= 0) {
        list[slot] = unit;
    } else {
        list[i] = unit;
        list[i + 1] = 255;
    }
}
