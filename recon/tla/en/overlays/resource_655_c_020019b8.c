/* Near miss of overlay 655's Func_020019b8: differ 2 of 50 (one reordered row in the opening). The
 * native loads the record address first (ldr r5; movs r6, #144; movs r3, #0; sub sp, #8); the draft loads
 * it third. An empty asm or a "+l" barrier after the address puts sub sp, #8 first; a one-pass block
 * around it puts sub sp, #8 second: differ 2 each time. Func_020018a8 and Func_02001930 before it are
 * the same routine over their own tables (eight records, and a call to Func_02001804 at the end), and
 * Func_02001820's draft should take this shape: a record pointer that walks, a running flag number,
 * and the flag stepped before the index. */
#include "types.h"

extern u16 Data_02003fea[];
extern u16 Data_020041b6[];
extern u16 Data_020041b8[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003be0(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);

void Func_020019b8(void)
{
    u16 *record = Data_020041b6;
    s32 flag = 0x240;
    u32 i;

    for (i = 0; i <= 15; flag++, i++) {
        if (GameFlag_Test(0x109)) {
            if (GameFlag_Test(flag)) {
                s32 x;
                s32 y;

                record = &Data_020041b8[i * 6];
                x = *record++;
                y = *record++;
                Func_02003be0(Data_02003fea[0], Data_02003fea[1], x, y, 1, 1);
            }
        } else {
            if (*record != 0)
                GameFlag_SetBit(flag);
            record += 6;
        }
    }
}
