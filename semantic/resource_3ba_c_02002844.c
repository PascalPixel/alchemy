#include "types.h"

/*
 * Signed transition-step owner at 0x02002844, complete through its return,
 * alignment and four-word pool at 0x02002900-0x0200290f.  This is the
 * independently address-mapped resource_3ba homolog of resource_3bc:3574;
 * both sign paths and all 21 calls remain explicit in machine order.
 */












extern void Func_02006644(s32 cue);
extern void Func_02006600(void);
extern void Func_02006614(void);
extern void Func_02006512(s32 frames);
extern void Func_02006678(s32 cue);
extern void Func_020049aa(s32 mode);
extern void Func_02004f36(s32 style, s32 variant);
extern void Func_02006532(s32 frames);
extern void Func_0200653c(s32 frames);
extern void Func_020066a4(s32 cue);
extern void Func_020049d6(s32 mode);
extern void Func_02004f62(s32 style, s32 variant);
extern void Func_0200655a(s32 frames);
extern void Func_0200662c(s32 actor, s32 cue, s32 frames);
extern void Func_020063dc(s32 frames);
extern s32 Func_020066d8(void);
extern void Func_020066da(s32 cue);
extern void Func_02006580(s32 frames);
extern void Func_020066e6(s32 cue);
extern void Func_020066aa(void);
extern void Func_020066b6(void);
void Func_02002844(s32 step)
{
    s32 actor;
    s32 magnitude = step < 0 ? -step : step;

    Func_02006644(0xf7);
    *(s16 *)(0x0200c628 + 26) = (s16)(step * 60);
    Func_02006600();
    Func_02006614();
    *(s16 *)(0x0200be76 + 26) = (s16)(magnitude * 60);

    if (step < 0) {
        Func_02006512(30);
        Func_02006678(0x56);
        Func_020049aa(8);
        Func_02004f36(3, 1);
        Func_02006532(magnitude * 60 + 60);
        actor = 0;
    } else {
        Func_0200653c(30);
        Func_020066a4(step + 0x5a);
        Func_020049d6(4);
        Func_02004f62(3, 0);
        Func_0200655a(step * 60 + 60);
        actor = 8;
    }

    goto check_transition;
wait_transition:
    Func_0200662c(actor, 0x105, 0);
    Func_020063dc(1);
check_transition:
    if (Func_020066d8() != 0)
        goto wait_transition;

    Func_020066da(0x13);
    Func_02006580(30);
    Func_020066e6(0x121);
    Func_020066aa();
    Func_020066b6();
}
