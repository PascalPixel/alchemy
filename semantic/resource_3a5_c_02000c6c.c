#include "types.h"

extern u32 Func_080000f8(void);
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_0808a018();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a020();

/* Choose one of three unused variants, then play the caller-selected cue. */
void Func_02000c6c(s32 cue_group)
{
    s32 variant = 0;
    s32 attempt;

    for (attempt = 0; attempt < 3; attempt++) {
        variant = (s32)(((Func_080000f8() * 3) >> 16) + 0x303);
        if (Func_080770c0(variant) == 0) {
            Func_080770c8(variant);
            break;
        }
    }

    Func_0808a018();
    Func_0808a170(cue_group * 3 + (variant - 0x303) + 0x1a10);
    Func_0808a180(variant - 0x302, 0);
    Func_0808a020();
}
