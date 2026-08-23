#include "types.h"

extern u8 Value_00000c91;

u8 *Func_08077008(s32 actor);
void Func_08015060(s32 window);
s32 Func_08077248(s32 actor);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
u8 *Func_080152d0(u16 no, s32 kind, s32 window, s32 x, s32 y);

void Func_080b1dec(s32 window, s32 actor)
{
    u8 *state;
    s32 x;
    s32 y;
    s32 i;
    s32 offset;
    u8 *entry;

    state = Func_08077008(actor);
    x = 8;
    y = 0;
    if (window != 0) {
        Func_08015060(window);
        if (Func_08077248(actor) == 0) {
            Func_08015078((s32)&Value_00000c91, window, 8, 20);
        } else {
            i = 0;
            offset = 216;
            do {
                if (*(u16 *)((u32)offset + (u32)state) != 0) {
                    entry = Func_080152d0(
                        *(u16 *)((u32)offset + (u32)state),
                        27, window, x, y);
                    entry[15] = 252;
                }
                x += 16;
                if (i == 4) {
                    x = 8;
                    y += 16;
                }
                if (i == 9) {
                    x = 8;
                    y += 16;
                }
                i++;
                offset += 2;
            } while (i <= 14);
        }
    }
}
