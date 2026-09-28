/* Item menu: after resetting the category, place the flagged entries of
   the five category sprites in a column 16 pixels apart, starting at 88.

   FAKEMATCH: the x store goes through its own pointer, which is what keeps
   the flag index as a counter over the flags base (loop.c otherwise turns
   flags[index] into a walking pointer). */
#include "TYPES.H"
extern u8 Data_03001f2c[];
#include "GLOBAL_CELLS.H"

void ItemMenu_ResetCategory(void);
void UiIcon_PrepareObject(void *);

void ItemMenu_ApplyFlags(const u8 *flags)
{
    u8 *base;
    void **slot;
    void *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = *(u8 **)((u32)&Data_03001f2c);
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
            UiIcon_PrepareObject(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}
