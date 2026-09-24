#include "TYPES.H"

#define MakyuriIriguchi_OpenEntrance Func_020011b4

extern u8 Data_00000035[];
extern u8 Data_02000240[];
void Func_02003694();
void Func_0200369a();
void Func_020036d6();
void Func_02003704();
void Func_02003790();
void Func_0200379a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

void MakyuriIriguchi_OpenEntrance(void)
{
    s32 base3_2000240;

    Func_02003694();
    Func_02003704(8, 2);
    Func_0200369a(20);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x200;
    /* FAKEMATCH: 0x35 is loaded from the literal pool through a link
     * symbol, and the do/while keeps the flag store before the call. */
    Func_02003790((s32)Data_00000035, 31);
    base3_2000240 = (s32)Data_02000240;
    do {
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
        Func_0200379a(36, 1);
    } while (0);
    Func_020036d6();
}
