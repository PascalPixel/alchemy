#include "types.h"

extern u8 Data_02000240[];












/* Move actors 54 and the selected actor together for sixty ticks. */
extern u8 * Func_02007b5a(s32 actor);
extern u8 * Func_02007b62(s32 actor);
extern void Func_02007b48(void);
extern void Func_02007c72(s32 x, s32 y, s32 z, s32 enabled);
extern void Func_02007d28(s32 cue);
extern void Func_02007ad0(u8 *actor, s32 mode);
extern void Func_02007a46(s32 frames);
extern void Func_02007d0e(void);
extern void Func_02007d1a(void);
extern void Func_02007bb6(void);
extern void Func_02007b6e(s32 flag);
extern void Func_02007ce6(const void *script, s32 mode);
void Func_0200384c(void)
{
    u8 *selected;
    u8 *actor54;
    s32 ticks;

    selected = Func_02007b5a(*(s32 *)(Data_02000240 + 500));
    actor54 = Func_02007b62(54);
    Func_02007b48();
    Func_02007c72(-1, -1, -1, 0);
    Func_02007d28(219);
    Func_02007ad0(selected, 0);

    actor54[85] = 0;
    *(s32 *)(selected + 40) = 0;
    selected[85] = 0;
    selected[97] = 1;
    actor54[97] = 1;
    for (ticks = 60; ticks != 0; ticks--) {
        Func_02007a46(1);
        *(s32 *)(actor54 + 40) += 0x3333;
        *(s32 *)(selected + 40) += 0x3333;
    }

    Func_02007d0e();
    Func_02007d1a();
    Func_02007bb6();
    Func_02007b6e(0x122);
    Func_02007ce6((const void *)2, 27);
}
