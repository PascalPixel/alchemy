#include "types.h"

extern u8 *Func_08077008(s32 group);
extern s32 Func_08077028(s32 group, s32 value);
extern u8 *Func_08077018(s32 member);
extern void Func_080772b0(s32 group, s32 index);
extern void Func_08077050(s32 group, s32 index);

/* Select a usable member for group two, then finalize every id-65 member. */
void Func_02000fc8(void)
{
    u8 *state = Func_08077008(2);
    s32 attempts = 0;

    for (;;) {
        s32 result;
        s32 selected = -1;
        s32 i;

        attempts++;
        if (attempts > 1000)
            *(u16 *)(state + 244) = 0;
        result = Func_08077028(2, 65);
        if (result == -1) {
            for (i = 0; i < 15; i++) {
                u8 *member = Func_08077018(*(u16 *)(state + 216 + i * 2));
                if (member[2] == 1) {
                    selected = i;
                    break;
                }
            }
            if (selected == -1) {
                for (i = 0; i < 15; i++) {
                    u8 *member = Func_08077018(*(u16 *)(state + 216 + i * 2));
                    if ((*(u16 *)(member + 2) & 0x08ff) == 0 && member[12] == 1) {
                        selected = i;
                        break;
                    }
                }
            }
            if (selected != -1)
                Func_080772b0(2, selected);
            continue;
        }

        for (i = 0; i < 15; i++) {
            if (*(u16 *)(state + 216 + i * 2) == 65)
                Func_08077050(2, i);
        }
        return;
    }
}
