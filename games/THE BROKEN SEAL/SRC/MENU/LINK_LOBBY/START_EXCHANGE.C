#include "TYPES.H"

extern u8 Value_00000001;

extern void Func_020027a8(void);
extern void Func_020027b6(s32);
extern s32 Func_020028fe(s32, s32);

s32 LinkLobby_StartExchange(void)
{
    Func_020027a8();
    Func_020027b6(2);
    /* FAKEMATCH: the do/while loads the linked value before the 1. */
    do {
        return Func_020028fe((s32)&Value_00000001, 1);
    } while (0);
}
