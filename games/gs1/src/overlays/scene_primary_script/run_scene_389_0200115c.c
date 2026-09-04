#include "types.h"

#define FieldScene_RunScene389_0200115c Func_0200115c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200251a();
void Func_020025a6();
void Func_020025da();
s32 Func_020025f8();
void Func_02002618();
void Func_0200262a();
void Func_0200262e();
void Func_02002634();
void Func_0200263c();
u8 *Func_02002646();
void Func_02002648();
void Func_02002652();
void Func_02002654();
void Func_02002656();
void Func_0200265e();
void Func_02002664();
void Func_02002678();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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

void FieldScene_RunScene389_0200115c(void)
{
    u32 i;
    u8 *record;

    Call1(Func_0200251a, 0x20090c9);
    Func_020025a6();
    Call3(Func_02002652, 13, 0x100, 30);
    Func_0200262a(13, 2);
    Call3(Func_0200265e, 0, 0xa000, 0);
    Call1(Func_02002654, 0x132f);
    Func_02002664(13, 0);
    Func_02002634(13, 3);
    Func_020025da(30);
    *(u8 *)(Func_020025f8(10) + 35) &= 253;
    Call3(Func_02002618, 13, 0x20000, 0x10000);
    Call3(Func_0200263c, 13, 0x258, 216);
    Call3(Func_02002648, 13, 0x258, 248);
    Call3(Func_02002656, 13, 0x238, 0x128);
    Func_02002678(13, 0, 0);
    {
        u8 *record = Func_02002646(10);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 2);
    }
    Call1(Func_0200262e, 0x869);
    Func_02002652();
}
