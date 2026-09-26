/* Not-yet-C, complete 150-byte entry allocator; trailing 2-byte pad separate.
 * The reference retains the chosen record in r0 on return. Pointer return
 * and 40-byte record recover that contract. Unsigned copy counters and a
 * walking source reproduce both bls loops. Typed window ownership leaves
 * 148 bytes, 25 aligned halfword edits: owner/value/y saves differ, as do
 * field-store order and the two mid-function literal pools. Three structural
 * hypotheses stopped, no adoption or byte credit. */
#include "TYPES.H"

struct Slot {
    s32 tag;
    s32 padding04_field04;
    u8 padding[0];
};

extern void *Data_03001e8c;
extern u8 Data_00000000[];

struct Entry {
    void *owner;
    s16 field04;
    s16 field06;
    s16 field08[4];
    s16 field10;
    s16 field12;
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    s16 field1c;
    s16 field1e;
    s16 field20;
    s16 field22;
    s16 field24;
    s16 field26;
};

struct Entry *Func_080165d8(void *owner, s32 arg1, s32 x, s32 y, u16 *copy_source, s32 arg5)
{
    struct Entry *base;
    struct Entry *entry;
    struct Entry *found;
    u32 i;

    base = (struct Entry *)((u8 *)Data_03001e8c + 1568);
    entry = base;
    found = 0;
    i = 0;
    while (i != 3) {
        if (entry->owner == 0) {
            found = entry;
            break;
        }
        i++;
        entry++;
    }

    if (found != 0) {
        found->field1e = (s16)(x << 8);
        found->field04 = (s16)(x << 8);
        found->field06 = (s16)(y << 8);
        found->field12 = (s16)arg1;
        found->field16 = 15;
        found->field1a = 10;
        found->owner = owner;
        found->field14 = 0;
        found->field18 = 0;
        found->field20 = 0;
        found->field24 = (s16)arg5;
        if (copy_source != 0) {
            for (i = 0; i < 4; i++)
                found->field08[i] = *copy_source++;
        } else {
            for (i = 0; i < 4; i++)
                found->field08[i] = (u16)(u32)Data_00000000;
        }
        found->field10 = 0;
    }
    return found;
}
