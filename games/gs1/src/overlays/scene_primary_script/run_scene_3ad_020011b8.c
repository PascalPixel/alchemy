#include "types.h"

#define FieldScene_RunScene3ad_020011b8 Func_020011b8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002547[];
extern u8 Data_0000254b[];
extern u8 Data_03001ebc[];
s32 Func_020024ba();
s32 Func_02002514();
s32 Func_02002538();
s32 Func_0200254a();
void Func_0200257c();
void Func_020025b4();
s32 Func_020025ce();
s32 Func_020025d4();
s32 Func_0200263e();
void Func_020026e4();
void Func_020029f2();
void Func_02002d64();
void Func_02002d68();
s32 Func_02002d6c();
void Func_02002d7c();
void Func_02002d7e();
void Func_02002d80();
void Func_02002d8a();
s32 Func_02002d8e();
void Func_02002d94();
void Func_02002d9e();
void Func_02002e18();
void Func_02002e28();
void Func_02002e32();
void Func_02002e38();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ad_020011b8(void)
{
    s32 record;
    s32 base5_2547;
    s32 v5;
    s32 base5_254b;

    base5_2547 = (s32)Data_00002547;
    Func_02002d64(base5_2547);
    Func_02002d7c(12, 0);
    Func_02002d68(1, 0, 0);
    Func_02002d7e((base5_2547 + 1));
    Value2(Func_02002d8e, 1, 0);
    Func_02002d80(2, 0, 0);
    Func_02002d8a(3, 0, 0);
    Func_02002d94(13, 0, 0);
    Func_02002d9e(12, 0, 0);
    L_0200120c:;
    if ((Func_020024ba() << 24) != 0) {
        L_02001216:;
        if ((Func_02002538() << 24) == 0) {
            goto L_02001290;
        }
        v5 = 0;
        if ((Func_0200263e() << 24) == 0) {
            L_0200122c:;
            v5 = 1;
            L_0200122e:;
            Func_0200257c();
            record = Value2(Func_02002d6c, 0, 0);
            if (record == 0) {
                goto L_02001290;
            }
        }
        if ((Func_020025d4() << 24) != 0) {
            goto L_02001296;
        }
        if (v5 == 0) {
            goto L_02001296;
        }
        goto L_0200122e;
    } else {
        if ((Func_02002514() << 24) != 0) {
            if ((Func_0200254a() << 24) != 0) {
                goto L_02001296;
            }
            goto L_0200122c;
        } else {
            if ((Func_020025ce() << 24) != 0) {
                goto L_02001216;
            }
            base5_254b = (s32)Data_0000254b;
            Func_02002e18(base5_254b);
            Func_02002e32(2, 0);
            Func_02002e28((base5_254b + 1));
            Func_02002e38(1, 0);
            goto L_0200120c;
            L_02001290:;
            Func_020029f2();
            goto L_0200129e;
        }
    }
    L_02001296:;
    Func_020025b4();
    Func_020026e4();
    L_0200129e:;
}
