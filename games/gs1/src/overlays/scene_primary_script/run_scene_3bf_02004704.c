#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002411[];
extern u8 Data_03001ebc[];
void Func_02009d48();
void Func_02009d5a();
void Func_02009d6c();
void Func_02009d8a();
void Func_02009d9e();
void Func_02009e04();
void Func_02009e22();
void Func_02009e26();
void Func_02009e34();
void Func_02009e46();
void Func_02009e4c();
void Func_02009e64();
void Func_02009e76();
void Func_02009e7c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_02004704(void)
{
    u32 i;
    s32 record;
    s32 base5_2411;

    Call3(Func_02009e26, 21, 0x101, 30);
    Call3(Func_02009e22, 21, 0xd000, 0);
    Func_02009d48(50);
    Call3(Func_02009e34, 21, 0xb000, 0);
    Func_02009d5a(50);
    Call3(Func_02009e46, 21, 0x5000, 0);
    Func_02009d6c(50);
    base5_2411 = (s32)Data_00002411;
    Func_02009e34(base5_2411);
    Func_02009e4c(21, 0);
    Func_02009e04(21, 4);
    Func_02009d8a(60);
    Call3(Func_02009e76, 21, 0xb000, 0);
    Func_02009d9e(40);
    Func_02009e64((base5_2411 + 1));
    Func_02009e7c(21, 0);
}
