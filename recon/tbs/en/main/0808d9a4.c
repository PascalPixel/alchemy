#include "TYPES.H"

/* NONMATCHING: 1044-byte complete extent. The old draft mistook the
 * _call_via_r3 callback for a two-argument service and omitted the 0x976
 * condition-failure message; both are corrected from the listing.
 * Baseline: 389 normalized instructions versus 396. Correct callback and
 * message flow gives 392; holding the message kind from descriptor lookup
 * gives 383. A typed flags/condition/action union gives identical code.
 * Topology still differs: base-plus-500 loads fold to absolute addresses,
 * the action-table index folds into its pool address, and saved pointer
 * and condition registers differ. No byte-match claim or adoption. */

extern u8 Data_00000970[];
extern u8 Data_00001000[];
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][2];
void Func_080030f8();
void Func_08009080();
void Func_080090d0();
s32 Func_08015040();
void Func_08015120();
void Func_08015128();
void Func_08015138();
s32 Func_08077030();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_08077230();
s32 Func_0808b05c();
void Func_0808b320();
void Func_0808c2dc();
s32 Func_0808d428();
s32 Func_0808d48c();
void Func_0808ec50();
void Func_0808ec8c();
void Func_0808ece0();
void Func_0808ed1c();
s32 Func_0808ed4c();
void Func_0808ed78();
s32 Func_0808ef70();
void Func_0808f0c8();
void Func_0808f0d8();
void Func_0809163c();
void Func_08091660();
void Func_080916b0();
void Func_08091750();
void Func_0809202c();
void Func_080f9010();

struct ActionDescriptor {
    s32 flags;
    u16 value_flags;
    s16 condition;
    union {
        u32 raw;
        u16 message;
        void (*callback)(s32 object_id);
    } action;
};

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

s32 Func_0808d9a4(s32 a0)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p8;
    s32 p8b;
    struct ActionDescriptor *rec;
    s32 rec3;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v2;
    s32 base2_2000240;
    s32 v10;
    s32 base5_968;
    s32 v5;
    u8 *p6;
    u8 *p5;

    p8 = a0;
    p6 = *(s32 *)((s16 *)Data_02000240 + 250);
    i = p8 - 242;
    if (i <= 5) {
        Func_08091660();
        p8b = *(u8 *)(0x0809e680 + i);
        ((void (*)())Func_08015040)((0x928 + p8b), 1);
        ((void (*)())Func_08015040)((0x948 + p8b), 1);
    } else {
        rec = (struct ActionDescriptor *)Value2(Func_0808d48c, 3, p8);
        if (rec == 0) {
        } else {
            v5 = ((rec->flags >> 4) & 31);
            p10 = rec->condition;
            if ((0x400 & rec->value_flags) == 0) {
                if (v5 == 0) {
                    goto L_0808da2c;
                }
                Func_08091660();
                Value2(Func_08015040, (v5 + 0x928), 1);
                Call1(Func_080770c8, 0x142);
            } else {
                L_0808da2c:;
                Call1(Func_080770d0, 0x142);
            }
            if ((0xf000000 & (s32)rec->action.raw) == 0) {
                if ((-0x100000 & (s32)rec->action.raw) != 0x400000) {
                    goto L_0808da9a;
                }
            } else {
                if (Value1(Func_0808d428, p10) != 0) {
                    rec->action.callback(
                        *(s32 *)((s16 *)Data_02000240 + 250));
                }
                if (Value1(Func_080770c0, 0x142) == 0) {
                    goto L_0808dd6a;
                }
                ((void (*)())Func_08015040)((v5 + 0x948), 1);
                goto L_0808dd6a;
            }
            if (Value1(Func_0808d428, p10) != 0) {
                ((void (*)())Func_08015040)(rec->action.message, 1);
            } else {
                ((void (*)())Func_08015040)(0x976, 1);
            }
            goto L_0808dd6a;
            L_0808da9a:;
            Func_080916b0();
            if (Value1(Func_0808d428, p10) == 0) {
            } else {
                v2 = 1;
                if ((0xf0000 & (s32)rec->action.raw) == 0x10000) {
                    v2 = 1;
                    if ((s32)p6 <= 7) {
                        v2 = 0;
                    }
                }
                if (v2 == 0) {
                } else {
                    if ((rec->flags & 0x1ff) == 19) {
                        Func_0808ece0(p8);
                    }
                    if ((-0x100000 & (s32)rec->action.raw) == 0x300000) {
                        if ((rec->flags & 0x1ff) == 19) {
                            Func_0808ed1c(p8);
                        }
                        rec8 = Value1(Func_0808ed4c, p8);
                        Func_0808f0d8();
                        Func_080f9010(83);
                        Func_08015120(rec->action.message, 5);
                        Value2(Func_08015040, (s32)Data_00000970, 3);
                        Call2(Func_0808c2dc, 0x3e7, 0);
                        Func_08015128(1);
                        Func_080f9010(126);
                        ((void (*)())Func_08015040)(((s32)Data_00000970 + 1), 1);
                        Func_08015138();
                        Func_08009080(rec8, 2);
                        Func_080f9010(246);
                        Func_0809163c(30);
                        ((void (*)())Func_08015040)(((s32)Data_00000970 + 2), 1);
                        Func_0808ed78(p8);
                        if (p10 == -1) {
                            goto L_0808dd50;
                        }
                        Func_080770c8(p10);
                        goto L_0808dd50;
                    }
                    v10 = p10;
                    if ((-0x100000 & (s32)rec->action.raw) == 0x500000) {
                        p5 = *(s32 *)0x03001ebc;
                        if ((rec->flags & 0x1ff) == 19) {
                            Func_0808ec8c(p8);
                        }
                        if (p10 != -1) {
                            p10b = ((s32)p10 | (s32)Data_00001000);
                            Data_02000240_t[141][0] = p10b;
                        }
                        record = Value2(Func_0808b05c, 99, rec->action.message);
                        *(u16 *)(((s32)p5 + 0x17c)) = record;
                        base2_2000240 = (s32)Data_02000240;
                        *(u8 *)((base2_2000240 + 0x22b)) = 2;
                        Func_0808b320(99, rec->action.message);
                        Func_080f9010(*(s16 *)(0x200042e));
                        goto L_0808dd3e;
                        v10 = p10b;
                    }
                    if ((-0x100000 & (s32)rec->action.raw) == 0x200000) {
                        rec7 = Func_0808ef70(*(s32 *)0x02000434, 0);
                        Func_080030f8(30);
                        if ((rec->flags & 0x1ff) == 19) {
                            Func_0808ed1c(p8);
                        }
                        Func_0808f0d8(rec7);
                        Func_080f9010(83);
                        Func_08015120(rec->action.message, 5);
                        Call2(Func_08015040, 0x969, 3);
                        Func_08077230(rec->action.message);
                        if (v10 != -1) {
                            Func_080770c8(v10);
                        }
                        Func_080090d0(rec7);
                        goto L_0808dd50;
                    }
                    rec3 = Value2(Func_0808ef70, *(s32 *)0x02000434, ((s32)rec->action.raw & 0xfff));
                    Func_080030f8(30);
                    rec8 = Func_08077030(rec->action.message);
                    v5 = 0xffff;
                    if (rec8 == -1) {
                        Func_08015120(((s32)rec->action.raw & 0xfff), 2);
                        base5_968 = 0x968;
                        v5 = (base5_968 + 4);
                        Value2(Func_08015040, base5_968, 1);
                        ((void (*)())Func_08015040)((base5_968 + 4), 1);
                        Func_0808f0c8(rec3);
                        if ((rec->flags & 0x1ff) != 19) {
                            goto L_0808dd50;
                        }
                        Func_0808ec50(p8);
                        goto L_0808dd50;
                    }
                    if ((rec->flags & 0x1ff) == 19) {
                        Func_0808ed1c(p8);
                    }
                    Func_0808f0d8(rec3);
                    Func_080f9010(83);
                    Func_08015120(((s32)rec->action.raw & v5), 2);
                    if (rec8 == *(s32 *)0x02000434) {
                        Call2(Func_08015040, 0x96a, 3);
                    } else {
                        Func_08015120(rec8, 1);
                        Call2(Func_08015040, 0x96b, 3);
                    }
                    if (v10 != -1) {
                        Func_080770c8(v10);
                    }
                    Func_080090d0(rec3);
                    goto L_0808dd50;
                }
                L_0808dd3e:;
                Call2(Func_08015040, 0x973, 1);
                goto L_0808dd50;
                v5 = 0x2000240;
            }
            ((void (*)())Func_08015040)((v5 + 0x948), 1);
            L_0808dd50:;
            Func_08091750();
            Func_0809202c();
            goto L_0808dd6a;
        }
        Call2(Func_08015040, 0x92d, 1);
        Call2(Func_08015040, 0x94d, 1);
        L_0808dd6a:;
        Call1(Func_080770d0, 0x142);
    }
    return 0;
}
