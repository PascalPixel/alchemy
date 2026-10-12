/* Draft, differ 11 of 43: the native stores the cursor to its stack slot before the first key read
 * (str r3, [sp]; ldrb r2, [r3]) and in the loop steps it past the key before reading the key
 * (adds r3, #5; ldrb r2, [r2]; str r3, [sp]); this form drops that first store and reads before
 * it steps. A for loop is the same; a reader inline (s8 Stream_ReadS8(u8 **)) sign-extends twice
 * (differ 37). */
#include "types.h"

extern u8 Data_0202e008[];

u32 Stream_ReadU16(u8 **cursor);

s32 Func_080ca9cc(s32 id, s32 *position)
{
    u8 *cursor = Data_0202e008;
    s8 key = *cursor++;

    while (key != -1) {
        if (key == id) {
            position[0] = Stream_ReadU16(&cursor) << 16;
            position[2] = Stream_ReadU16(&cursor) << 16;
            return 0;
        }
        cursor += 4;
        key = *cursor++;
    }
    return -1;
}
