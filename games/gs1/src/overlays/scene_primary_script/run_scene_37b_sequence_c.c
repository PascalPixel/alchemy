#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_0200aa50[];
extern u8 Data_03001ebc[];
extern u16 Data_02000240_t[][1];
void Func_0200157c();
void Func_02001a82();
void Func_0200252c();
void Func_020034b4();
void Func_020034b8();
s32 Func_020034ee();
u8 *Func_02003506();
s32 Func_02003512();
s32 Func_0200351c();
void Func_0200354a();
void Func_02003568();
void Func_0200357a();
void Func_0200357c();
void Func_02003586();
void Func_0200359e();
void Func_020035ac();
void Func_020035b4();
void Func_020035ba();
s32 Func_020035be();
s32 Func_020035c8();
void Func_020035cc();
void Func_020035d6();
void Func_020035f0();
void Func_020035f8();
void Func_02003610();
void Func_02003620();
void Func_0200363c();
void Func_0200364a();
void Func_02003658();
void Func_02003662();
s32 Func_0200366c();
void Func_0200367a();
void Func_0200368e();
s32 Func_02003698();
void Func_0200369e();
void Func_020036ba();
s32 Func_020036c4();
void Func_020036ca();
void Func_020036de();
void Func_020036ee();
void Func_0200370a();
s32 Func_02003714();
void Func_0200371a();
void Func_0200372e();
void Func_02003746();
void Func_0200375a();
s32 Func_02003764();
void Func_0200376a();
void Func_0200377c();
void Func_02003796();
void Func_020037a8();
s32 Func_020037b2();
void Func_020037b8();
void Func_020037ca();
void Func_020037e6();
s32 Func_02003810();
void Func_02003834();
s32 Func_02003858();
void Func_0200386e();
s32 Func_0200387a();
void Func_0200388e();
s32 Func_02003890();
void Func_02003898();
s32 Func_0200389a();
void Func_020038aa();
s32 Func_020038ae();
void Func_020038b0();
s32 Func_020038be();
s32 Func_020038e6();
s32 Func_020038e8();
void Func_020038f8();
void Func_02003902();
s32 Func_02003910();
s32 Func_02003914();
void Func_02003920();
void Func_02003926();
void Func_020039d4();
void Func_020039e0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}


void Func_020014b8(void)
{
    u32 i;
    s32 value;
    volatile s32 *p;
    s32 buf;

    p = (volatile s32 *)Data_0200aa50;
    buf = Value2(Func_02003890, 14, 0x400);
    Call2(Func_020038aa, 0x200a56c, buf);
    value = Func_020038be(Func_020038be(), 128, buf);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Func_020038e8(Func_020038e6(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Func_02003914(Func_02003910(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Func_02003926(14);
    {
        s32 size = 0xc80;

        Func_02003920(0x2008eb1, size);
    }
}
