#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u8 *MusicTrack_SetToneLength(s32 arg0, void *arg1) {
    u32 cursor;

    cursor = (u32)FIELD_AT_OFFSET(arg1, u8 **, 0x40);
    cursor = *(u8 *)cursor;
    FIELD_AT_OFFSET(arg1, u8 *, 0x26) = cursor;
    cursor = (u32)FIELD_AT_OFFSET(arg1, u8 **, 0x40);
    cursor++;
    FIELD_AT_OFFSET(arg1, u8 **, 0x40) = (u8 *)cursor;
    return (u8 *)cursor;
}
