/* NONMATCHING: 34 differing halfwords. Calling the fill directly makes the
 * last call reload its address after the loop label, as in the ROM. The
 * ROM rebuilds the 0x100 size before advancing destination; here CSE
 * shares it in a saved register across the first call. */
#include "TYPES.H"

#define BattlePres_BuildTilemap Func_080c00d8

/* The IWRAM word fill, called through a call-via veneer. */
#define IwramFill ((void (*)(void *destination, s32 size, s32 value))0x03000168)

void BattlePres_BuildTilemap(s32 *destination)
{
    s32 entry;
    u32 index;

    IwramFill(destination, 0x100, -1);
    destination += 0x40;
    IwramFill(destination, 0x80, 0x03ff03ff);
    entry = 0x02010200;
    destination += 0x20;

    index = 0;
    do {
        index++;
        *destination++ = entry;
        entry += 0x00020002;
    } while (index <= 239);

    IwramFill(destination, 0x280, 0x03ff03ff);
}
