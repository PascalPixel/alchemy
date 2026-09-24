/* Item menu: after resetting the category, place the flagged entries of
   the five category sprites in a column 16 pixels apart, starting at 88.

   FAKEMATCH: the x store goes through its own pointer, which is what keeps
   the flag index as a counter over the flags base (loop.c otherwise turns
   flags[index] into a walking pointer). */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

void ItemMenu_ResetCategory(void);
void Func_080a17c4(void *);

void ItemMenu_ApplyFlags(const u8 *flags)
{
    u8 *base;
    void **slot;
    void *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = *(u8 **)ADDR_03001F2C;
    ItemMenu_ResetCategory();
    index = 0;
    slot = (void **)(base + 200);
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            *(u16 *)((u8 *)entry + 6) = kind;
            {
                u16 *x = (u16 *)((u8 *)entry + 8);

                *x = value;
            }
            *(u8 *)((u8 *)entry + 15) = 240;
            Func_080a17c4(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}
