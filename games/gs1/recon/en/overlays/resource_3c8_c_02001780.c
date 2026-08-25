#include "types.h"

/* Complete scene setup, including the two global halfword state writes. */

void Func_0200662c();
void Func_0200663a();
void Func_0200668e();
void Func_020066cc();
void Func_020066e2();
void Func_0200670c();
void Func_02006720();
void Func_0200673a();
void Func_0200674c();
void Func_02006766();
void Func_02006772();

void Func_02001780(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    register s32 zero = 0;
    register s32 one = 1;

    *(s16 *)(state + 0xcba) = zero;
    *(s16 *)(state + 0xcb6) = one;
    Func_0200662c();
    Func_020066e2(0x267d);
    Func_020066cc(10, 0, 0);
    Func_0200663a(10);
    Func_0200670c(10, 0, 20);
    Func_02006720(10, 57344, 0);
    Func_0200674c(65536, 8192);
    Func_02006766(29360128, -1, 28311552, 1);
    Func_02006772();
    Func_0200673a(10, 0);
    Func_0200668e();
}
