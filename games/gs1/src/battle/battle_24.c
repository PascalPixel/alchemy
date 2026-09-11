#include "types.h"
#include "scene.h"

/* battle/classify_entry_kind.c */
s32 Battle_ClassifyEntryKind(const u8 *entry)
{
    s32 b3;
    s32 kind;
    s32 low;
    s32 ret;
    u8 k;

    ret = 0;
    low = entry[1] & 0xF;
    if (low == 1) {
        ret = 1;
    }
    if (low == 0xB) {
        ret = 2;
    }
    b3 = ((const s8 *)entry)[3]; kind = (s32)(b3);
    k = kind;
    if (k == 3) {
        ret = 3;
    }
    if (k == 4) {
        ret = 4;
    }
    if (k == 0x40) {
        ret = 6;
    }
    if (Battle_Check(entry[3]) != 0) {
        ret = 5;
    }
    return ret;
}

/* battle/draw_party_panels_with_empty_list.c */
extern void Battle_DrawPartyPanelsWithEmptyList(void)
    __attribute__((alias("Nested_080270ac.0")));

static __inline__ s32 Scope_080270ac(void)
{
    void Nested_080270ac(void)
    {
        u16 data[2];

        data[0] = 0xff;
        Battle_Do(data);
        Battle_Apply(data, 1);
    }

    return 0;
}
