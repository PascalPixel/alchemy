#include "types.h"

extern s32 Func_020032a8(void);
extern void Func_080b0010(s32 scene);
extern void Func_0808a018(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a020(void);

/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
void Func_020034a8(void)
{
    if (Func_020032a8() != 0) {
        Func_080b0010(8);
        return;
    }

    Func_0808a018();
    Func_0808a170(0x190a);
    if (Func_080770c0(0x909) != 0)
        Func_0808a170(0x1951);
    Func_0808a180(8, 0);
    Func_0808a020();
}
