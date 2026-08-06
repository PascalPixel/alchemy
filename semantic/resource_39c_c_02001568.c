#include "types.h"

/* resource_39c owner at 0x02001568, 120 bytes: when story flag 0x256 is set,
 * consume it, raise actor 0, snapshot the raised y field, wait five frames,
 * issue one six-argument presentation, play cue 217, attach descriptor
 * 0x0200e03a, and close the sequence. Prologue/frame through the sole return at
 * 0x020015ce-0x020015d4, then alignment and two pool words through 0x020015df. */










extern s32 Func_02006ff4();
extern void Func_0200702c();
extern void Func_02007012();
extern u8 * Func_02007058();
extern u8 * Func_02007068();
extern u8 * Func_02007070();
extern void Func_02007052();
extern void Func_02006fbc();
extern void Func_02007222();
extern void Func_02006fc4();
extern void Func_02007088();
void Func_02001568(void)
{
    u8 *actor;
    u8 *source;

    if (Func_02006ff4(0x256) == 0)
        return;

    Func_0200702c();
    Func_02007012(0x256);

    actor = Func_02007058(0);
    *(s32 *)(actor + 12) += 0x20000;

    *(s32 *)(actor + 60) = *(s32 *)(source + 12);
    actor = Func_02007068(0);
    source = Func_02007070(0);

    Func_02007052(5);
    Func_02006fbc(7, 2, 5, 11, 1, 1);
    Func_02007222(217);
    Func_02006fc4((void *)0x0200e03a, 9, 7);
    Func_02007088();
}
