#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* battle/get_tagged_slot_value.c */
s16 Battle_GetTaggedSlotValue(s32 arg0)
{
    u8 *base = *(u8 **)ADDR_03001E74;
    s32 offset;

    if ((arg0 & 0x80) != 0) {
        offset = (arg0 & 0xF) * 2 + 0x64;
        base += 2;
    } else {
        offset = (arg0 & 0xF) * 2 + 0x58;
    }
    return *(s16 *)(base + offset);
}

/* battle/find_tagged_slot_by_value.c */
extern void *gBattleWork;

s32 Battle_FindTaggedSlotByValue(u32 value)
{
    s32 index;
    s32 tag;
    s32 offset;
    char *base;
    s16 item;

    base = gBattleWork;
    if (value <= 7) {
        tag = 0x80;
        index = 0;
        tag <<= 1;
        offset = 0x58;
first:
        item = *(s16 *)(offset + (u32)base);
        if (item == 0xff)
            return -1;
        if (item == 0xfe)
            goto next_first;
        if (item == value)
            return index | tag;
next_first:
        offset += 2;
        index++;
        goto first;
    }

    tag = 0xc0;
    index = 0;
    base += 2;
    tag <<= 1;
    offset = 0x64;
second:
    item = *(s16 *)(offset + (u32)base);
    if (item == 0xff)
        return -1;
    if (item == 0xfe)
        goto next_second;
    if (item == value)
        return index | tag;
next_second:
    offset += 2;
    index++;
    goto second;
}

/* battle/presentation/cam/shoulder_alt.c */
extern u16 Value_fffffe00;

void BattlePres_AdjustCameraByShoulderKeysAlt(void)
{
    void **slot = (void **)ADDR_03001E80;
    u8 *cam = slot[0];
    u8 *trans = slot[32];
    volatile u32 *keys = (volatile u32 *)ADDR_03001AE8;

    if ((*keys & 512) != 0) {
        *(u16 *)(cam + 54) += 512;
    }
    if ((*keys & 256) != 0) {
        *(u16 *)(cam + 54) += (u16)(u32)&Value_fffffe00;
    }
    if (*(u32 *)(trans + 20) == 0) {
        Battle_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

/* battle/runtime/reserved_no_op_b.c */
void Battle_ReservedNoOp9B2C(void)
{
}
