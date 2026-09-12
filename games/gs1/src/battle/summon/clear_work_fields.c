#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* battle/summon/clear_work_fields.c */
extern s16 gCell[];

union Word {
    s32 value;
};

void Summon_ClearWorkFields(void)
{
    u8 *base;
    union Word *words;
    s16 *slots;
    s32 index;

    base = *(u8 **)ADDR_03001E74;
    words = (union Word *)(base + 0x530);
    gCell[286] = 0;
    words[0].value = 0;
    words[1].value = 0;
    words[2].value = 0;
    slots = (s16 *)(base + 0x53C);
    for (index = 3; index >= 0; index--)
        slots[index] = 0;
}

/* battle/runtime/reserved_no_op_a.c */
void Battle_ReservedNoOp2A08(void)
{
}

/* battle/effects/common/advance_scroll_on_interval.c */
struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 gIw;
extern struct Position gIw2;

void BattleFx_AdvanceScrollOnInterval(void)
{
    u8 *base = (u8 *)gIw;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        gIw2.x += *(s32 *)(base + 0x7798);
        gIw2.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}
