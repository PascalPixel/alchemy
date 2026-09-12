#include "flash.h"

u8 *VerifyFlashCore(u8 *left, u8 *right, u16 size)
{
    u16 index = size - 1;

    while (index != 0xFFFF) {
        u8 rightByte = *right;
        u8 leftByte = *left;

        left++;
        right++;
        if (rightByte != leftByte) {
            return right - 1;
        }
        index--;
    }
    return 0;
}
