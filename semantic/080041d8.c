#include "types.h"

void Func_08004144(void);
extern u8 Data_00000000[];

s32 Func_080041d8(s32 arg0, s32 arg1)
{
    s32 index;
    s32 result;
    u32 saved;
    u8 *entry;
    volatile u16 *ime;

    entry = (u8 *)0x03001a20;
    result = -1;
    *(volatile u8 *)0x03001a10;
    ime = (volatile u16 *)0x04000208;
    saved = *ime;
    *ime = (u32)ime;
    index = 0;
    if (*(s32 *)entry == arg0) {
        *(s16 *)(entry + 4) = arg1;
        result = 0;
    } else {
find_existing:
        index++;
        entry += 8;
        if (index <= 19) {
            if (*(s32 *)entry == arg0) {
                *(s16 *)(entry + 4) = arg1;
                result = index;
            } else {
                goto find_existing;
            }
        }
    }

    entry = (u8 *)0x03001a20;
    if (result == -1) {
        index = 0;
        if (*(s32 *)entry == 0) {
            *(s32 *)entry = arg0;
            *(s16 *)(entry + 4) = arg1;
            *(s8 *)(entry + 6) = (s32)Data_00000000;
            result = 0;
        } else {
find_empty:
            index++;
            entry += 8;
            if (index <= 19) {
                if (*(s32 *)entry == 0) {
                    *(s32 *)entry = arg0;
                    *(s16 *)(entry + 4) = arg1;
                    *(s8 *)(entry + 6) = (s32)Data_00000000;
                    result = index;
                } else {
                    goto find_empty;
                }
            }
        }
    }
    Func_08004144();
    *(volatile u16 *)0x04000208 = saved;
    return result;
}
