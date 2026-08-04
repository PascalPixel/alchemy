#include "types.h"

extern u16 Data_080b413c[];

/* 4行分の非0バイトを指定配置へ順にコピーする。 */
void Func_080b06ec(s32 arg0, s32 arg1, u32 arg2) {
    u8 *source = (u8 *)(0x080b3d40 + ((u32)arg0 << 5));
    u8 *destination =
        (u8 *)((u32)arg1 + Data_080b413c[arg2] + 2);
    s32 count = 3;

    do {
        if (*source != 0) {
            destination[0] = *source++;
            if (*source != 0) {
                destination[1] = *source++;
                if (*source != 0) {
                    destination[30] = *source++;
                    if (*source != 0) destination[31] = *source++;
                }
            }
        }
        destination += 4;
        count--;
    } while (count >= 0);
}
