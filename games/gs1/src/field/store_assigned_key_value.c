#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* field/store_assigned_key_value.c */
extern u8 *gWork;
s32 GameFlag_IsSet(s32);

static __inline__ void StoreHalfword(u8 *address, s32 value)
{
    *(s16 *)address = value;
}

u32 Field_StoreAssignedKeyValue(u32 value)
{
    u32 no = value >> 14;
    u32 ret = 0x3FFF & value;
    u8 *state = gWork;

    if (GameFlag_IsSet(0x107) != 0) {
        StoreHalfword(state + 0x182, 0xFA);
    } else if (*(s16 *)(state + 0x19E) == 3) {
        if (*(volatile u32 *)ADDR_03001C94 & 0x100) {
            StoreHalfword(state + 0x182, 0xFC88);
        } else if (*(volatile u32 *)ADDR_03001C94 & 0x200) {
            StoreHalfword(state + 0x182, 0xFC87);
        }
    } else {
        switch (no) {
        case 0:
            StoreHalfword(state + 0x17E, ret);
            break;
        case 1:
            StoreHalfword(state + 0x180, ret);
            break;
        }
    }

    return ret;
}
