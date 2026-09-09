#include "types.h"

#define SceneData_SelectOverlayDataBySelector Func_02000030
#define SceneData_GetTable9F14 Func_02000074
#define FieldScene_RunScene37fSequenceA Func_02000154
#define SceneDialogue_RunFlag81aMessageBranch Func_02000200
#define FieldScene_RunOpeningAuxiliarySequence Func_02000258
#define FieldScene_RunFlag821Dialogue Func_020003bc

extern u8 Value_00000013;
extern u8 Value_00000010;
extern u8 Data_02009d04[];
extern u8 Data_02009d64[];
extern u8 Data_02009cd4[];
extern u8 Data_00000010[];
extern u8 Data_00000013[];
extern u8 Data_02009fc0[];
extern u8 Data_02009fd8[];
extern u8 Data_0200a050[];
extern u8 Data_0200a1b8[];
extern u8 Data_0200a2a8[];
extern s16 Data_02000240_t[][1];
extern void Func_02001db0(void);  /* Func_0808a018 veneer. */
extern s32 Func_02001d96();       /* GameFlag_IsSet veneer, first site. */
extern void Func_02001d9a();      /* UiText_DrawMessage veneer, first site. */
extern void Func_02001da4();      /* UiText_DrawMessage veneer, second site. */
extern s32 Func_02001db2();       /* GameFlag_IsSet veneer, second site. */
extern void Func_02001df0(void);  /* Func_0808a020 veneer. */

void Func_02001c70();
void Func_02001c9e();
void Func_02001cb4();
void Func_02001cca();
void Func_02001cd0();
void Func_02001ce6();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d80();
void Func_02001d8c();
void Func_02001da2();
void Func_02001da6();
void Func_02001dc2();
void Func_02001dce();
void Func_02001e12();
void Func_02001e2c();
void Func_02001e7e();
void Func_02001e8c();
void Func_02001e98();
void Func_02001dde();
s32 Func_02001df0_a();
void Func_02001df2();
s32 Func_02001dfc();
void Func_02001e26();
void Func_02001e28();
void Func_02001e30();
void Func_02001e34();
void Func_02001e40();
void Func_02001e44();
void Func_02001e48();
void Func_02001e56();
void Func_02001e7c();
void Func_02001e9e();
void Func_02001eaa();
void Func_02001ecc();
void Func_02001ef4();
void Func_02001f0e();
void Func_02001f22();
void Func_02001f24();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f4e();
void Func_02001f50();
void Func_02001f74();
void Func_02001f7e();
void Func_02001f7e_a();
void Func_02001f84();
void Func_02001f90();
void Func_02001f9c();
void Func_02001fa6();
void Func_02001fb2();
void Func_02001f6c();
s32 Func_02001f52();
void Func_02001f56();
s32 Func_02001f66();
void Func_02001f6e();
void Func_02001f82();
void Func_02001fb6();

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Resource 37f owner at 0x020003bc: 76 bytes of code plus a five-word literal
 * pool at 0x0200040c-0x0200041f.  The interworking return pops the saved
 * return address into r0, so the owner returns void.  All seven calls go
 * through import veneers, and GameFlag_IsSet is used only as a predicate.  r5
 * holds the workspace pointer, loaded before the message call and read after
 * it; that is why the owner saves r5.
 */

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/*
 * These wrappers pass their constants straight into the argument registers.
 * A direct call precomputes a costly constant into a pseudo that is then
 * shared with later uses in the block.  A value-returning call also sets r0
 * last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_02000258(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

s32 SceneData_SelectOverlayDataBySelector(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}

/*
 * Return the table at 0x02009f14.  The eight-byte owner includes its one
 * pool word.
 */
u8 *SceneData_GetTable9F14(void)
{
    return (u8 *)0x02009f14;
}

s32 Func_0200007c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    s32 base5_2009fd8;
    s32 lo = 11;

    if (Data_02000240_t[224][0] == (s32)Data_00000010) {
        if (Data_02000240_t[225][0] >= lo) {
            if (Data_02000240_t[225][0] > 13) {
                if (Data_02000240_t[225][0] > 16) {
                    goto L_020000ae;
                }
                return (s32)Data_0200a1b8;
            }
            return (s32)Data_0200a050;
        }
        L_020000ae:;
        base5_2009fd8 = (s32)Data_02009fd8;
        Func_02001c70(base5_2009fd8);
        return base5_2009fd8;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000013) {
            return (s32)Data_0200a2a8;
        }
    }
    return (s32)Data_02009fc0;
}

void FieldScene_RunScene37fSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Func_02001d06();
    v5 = 3;
    v6 = 2;
    Func_02001e2c(181);
    Call6(Func_02001cd0, 16, 28, 21, 3, v5, v6);
    Func_02001c9e(10);
    Call6(Func_02001ce6, 16, 30, 21, 3, v5, v6);
    Func_02001cb4(10);
    Call6(Func_02001cfc, 16, 32, 21, 3, v5, v6);
    Func_02001cca(10);
    Func_02001e12(0, 2);
    Call3(Func_02001d8c, 0, 0x9999, 0x4ccc);
    Func_02001da6(0, 120, 98);
    Func_02001dce(0, 2);
    Call3(Func_02001dc2, 0, 0, -8);
    Func_02001d80(10);
    Func_02001e8c();
    Func_02001e98();
    Func_02001e7e(2);
    Func_02001da2();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{
    extern u8 *Data_03001ebc;

    Func_02001db0();

    if (Func_02001d96(0x81a) != 0) {
        Func_02001d9a(0x1034, 1);
    } else {
        Func_02001da4(0x1031, 1);
        if (Func_02001db2(0xf01) != 0) {
            u16 *p = (u16 *)(Data_03001ebc + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Func_02001df0();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    s32 id;
    s32 v5;
    s32 v6;

    if (Value1(Func_02001df0_a, 0xf01) == 0) {
    } else {
        if (Value1(Func_02001dfc, 0x81a) != 0) {
        } else {
            Func_02001e26();
            Func_02001f3a();
            v5 = 1;
            Func_02001f50(182);
            Call6(Func_02001df2, 0, 70, 30, 42, v5, v5);
            Func_02001dde();
            Func_02001e44(40);
            id = 0x1032;
            Call2(Func_02001e30, id, 1);
            Func_02001e56(20);
            v6 = 3;
            Func_02001f84(183);
            Call6(Func_02001e28, 0, 29, 3, 1, v6, 2);
            Call6(Func_02001e40, 0, 29, 3, 2, v6, v5);
            Call6(Func_02001e48, 1, 109, 4, 81, v5, v5);
            Func_02001e34();
            Call3_02000258(Func_02001e7c, 0x10000, 0x10000, 0x10000);
            Func_02001eaa(20);
            Call3_02000258(Func_02001f7e, 0, 0x100, 0);
            Call3_02000258(Func_02001e9e, 0x20000, 0x20000, 0x10000);
            Func_02001ecc(20);
            Call3_02000258(Func_02001f90, 0, 0x4000, 40);
            Call3_02000258(Func_02001f9c, 0, 0x8000, 20);
            Func_02001fa6(0, 0, 20);
            Call3_02000258(Func_02001fb2, 0, 0x4000, 10);
            Func_02001f74(0, 4, 20);
            Func_02001f7e_a(0, 6, 40);
            Call3_02000258(Func_02001ef4, -1, -1, 0xe666);
            Func_02001f22(40);
            Call2(Func_02001f0e, id + 1, 1);
            Call1(Func_02001f24, 0x143);
            Call1(Func_02001f2a, 0x81a);
            Func_02001f4e();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    Func_02001f6c();

    if (Func_02001f52(0x821) != 0) {
        Func_02001f56(0x1034, 1);
    } else if (Func_02001f66(0xf02) != 0) {
        work = Data_03001ebc;
        Func_02001f6e(0x1031, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Func_02001f82(0x1031, 1);
    }

    Func_02001fb6();
}
