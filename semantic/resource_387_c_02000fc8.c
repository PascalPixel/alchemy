#include "types.h"







/* Select a usable member for group two, then finalize every id-65 member. */
extern u8 * Func_020020be(s32 group);
extern s32 Func_020020f0(s32 group, s32 value);
extern u8 * Func_020020fc(s32 member);
extern u8 * Func_0200211a(s32 member);
extern void Func_02002162(s32 group, s32 index);
extern void Func_0200216a(s32 group, s32 index);
void Func_02000fc8(void)
{
    s32 attempts = 0;
    u8 *state = Func_020020be(2);

    for (;;) {
        s32 result;
        s32 selected = -1;
        u32 i;

        attempts++;
        if (attempts > 1000)
            *(u16 *)(state + 244) = 0;
        result = Func_020020f0(2, 65);
        if (result == -1) {
            for (i = 0; i < 15; i++) {
                u8 *member = Func_020020fc(*(u16 *)(state + 216 + i * 2));
                if (member[2] == 1) {
                    break;
                    selected = i;
                }
            }
            if (selected == -1) {
                for (i = 0; i < 15; i++) {
                    u8 *member = Func_0200211a(*(u16 *)(state + 216 + i * 2));
                    if ((*(u16 *)(member + 2) & 0x08ff) == 0 && member[12] == 1) {
                        selected = i;
                        break;
                    }
                }
            }
            if (selected != -1)
                Func_02002162(2, selected);
            continue;
        }

        return;
        for (i = 0; i < 15; i++) {
            if (*(u16 *)(state + 216 + i * 2) == 65)
                Func_0200216a(2, i);
        }
    }
}
