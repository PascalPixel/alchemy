#include "types.h"

#define FieldScene_RunFlagDependentActorPose Func_02004254

void Func_02007a5c();
void Func_02008b0e();
void Func_02008b50();
void Func_02008b7a();
void Func_02008bf8();
void Func_02008c1a();
void Func_02008ca8();
void Func_02008ce2();
void Func_02008d1c();
void Func_02008d20();
void Func_02008da2();
void Func_02008dca();
void Func_02008dda();
void Func_02008de8();
void Func_02008dfa();
s32 Func_02009290();
void Func_0200a6aa();
void Func_0200a726();
s32 Func_0200a77a();
void Func_0200a788();
void Func_0200a790();
void Func_0200a7b8();
void Func_0200a7d4();
void Func_0200a7e0();
void Func_0200a7ea();
void Func_0200a81a();
u8 *Func_0200a828();
void Func_0200a842();
void Func_0200a852();
void Func_0200a85c();
void Func_0200a862();
void Func_0200a872();
void Func_0200a876();
s32 Func_0200a888();
void Func_0200a892();
void Func_0200a896();
void Func_0200a8b0();
void Func_0200a8be();
s32 Func_0200a8d0();
void Func_0200a8d6();
void Func_0200a8f8();
void Func_0200a908();
void Func_0200a916();
void Func_0200a920();
void Func_0200a928();
void Func_0200a942();
void Func_0200a94c();
void Func_0200a94e();
void Func_0200a980();
void Func_0200a988();
void Func_0200a99c();
void Func_0200a9a0();
void Func_0200a9a8();
void Func_0200a9c4();
void Func_0200a9c6();
void Func_0200a9c8();
void Func_0200a9d4();
void Func_0200a9e0();
void Func_0200aa06();
void Func_0200aa0e();
void Func_0200aa10();
void Func_0200aa18();
void Func_0200aa1a();
void Func_0200aa1c();
void Func_0200aa20();
void Func_0200aa26();
void Func_0200aa2a();
void Func_0200aa3a();
void Func_0200aa4a();
void Func_0200aa4c();
void Func_0200aa58();
void Func_0200aac2();
void Func_0200aad0();
void Func_0200aad6();
void Func_0200aae4();
void Func_0200aae6();
void Func_0200ab18();
void Func_0200ab30();
void Func_0200ab38();
void Func_0200ab50();
void Func_0200aba8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 group)
{
    f(group);
}

static __inline__ s32 Value1(s32 (*f)(), s32 group)
{
    return f(group);
}

static __inline__ void Call2(void (*f)(), s32 group, s32 a1)
{
    f(group, a1);
}

static __inline__ void Call3(void (*f)(), s32 group, s32 a1, s32 a2)
{
    f(group, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 group, s32 a1, s32 a2)
{
    return f(group, a1, a2);
}

void Func_0200a896_a();
s32 Func_0200a85c_a();

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a)
{
    return f(a);
}

void FieldScene_RunFlagDependentActorPose(s32 group)
{
    s32 slot;
    u8 *record;
    s32 flag;
    s32 height;
    s32 coordinate_scale;
    s32 alternate_pose;
    s32 request;

    Func_0200a726();
    Func_02008b50(24, 0, 0);
    coordinate_scale = 0x8000;
    Func_02008b0e(0, 0x1b0, 134, coordinate_scale);
    Func_02007a5c(1);
    Func_0200a6aa(1);
    Func_0200a8be();
    Call3(Func_0200a790, 0, 0xcccc, 0x6666);
    Call3(Func_0200a7d4, 0, 0x196, 134);
    Call3(Func_0200a7e0, 0, 0x196, 152);
    Call3(Func_0200a7ea, 0, 0x1a5, 152);
    Func_0200a842(27, 1);
    Func_0200a788(20);
    Func_0200a862(27, 0, 10);
    if (Value1(Func_0200a77a, 0x300) == 0) {
    } else {
        slot = Func_02009290(group, 0);
        Func_0200a872(27, 1);
        Func_0200a7b8(20);
        Func_0200a892(27, 0, 10);
        Call1(Func_0200a8b0, 0x1ebc);
        Call1(Func_02008b7a, 0xa01b);
        Func_0200a876(0, 3);
        Func_0200a81a(0, 0x10000, coordinate_scale);
        Func_0200a85c(0, 0x1b0, 168);
        Call3(Func_0200a908, 0, 0xc000, 0);
        record = Pointer1(Func_0200a828, 0);
        if (record != 0) {
            Func_0200a896(slot, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_0200a852(slot, 0x10000, coordinate_scale);
        Call3(Func_0200a896_a, slot, 0x1c0, 168);
        Call3(Func_0200a942, slot, 0xb000, 20);
        Call3(Func_0200a94e, 27, 0x3000, 20);
        Func_0200a8d6(27, 3);
        Func_02008bf8(27);
        Call3(Func_0200a980, slot, 0x102, 60);
        Func_0200a920(27, 1);
        Func_0200a8f8(27, 3);
        Func_02008c1a(27);
        Func_0200a916(slot, 3);
        alternate_pose = 0;
        if (Value1(Func_0200a85c_a, 0x92b) != 0) {
            Call3(Func_0200a99c, 0, 0x2000, 0);
            Call3(Func_0200a9a8, 27, 0x3000, 0);
            height = 204;
            goto common_pose;
        } else {
            if (Value1(Func_0200a888, 0x92a) != 0) {
                Call3(Func_0200a928, 0, 0x1a6, 154);
                Call3(Func_0200a9d4, 0, 0x6000, 0);
                Call3(Func_0200a9e0, 27, 0x5000, 0);
                Call3(Func_0200a94c, slot, 0x19a, 204);
                alternate_pose = 1;
                Func_02008ca8(slot, 0xd000);
                goto L_020044a2;
            }
            flag = Value1(Func_0200a8d0, 0x929);
            if (flag == 0) {
                goto L_02004466;
            }
            Call3(Func_0200aa10, 0, 0x2000, 0);
            Call3(Func_0200aa1c, 27, 0x3000, 0);
            height = 172;
        }
        common_pose:;
        Call3(Func_0200a988, slot, 0x1d6, height);
        Func_02008ce2(slot, 0xb000);
        goto L_020044a2;
        L_02004466:;
        Call3(Func_0200a9a0, 0, 0x1a6, 154);
        Call3(Func_0200aa4c, 0, 0x6000, 0);
        Call3(Func_0200aa58, 27, 0x5000, 0);
        Call3(Func_0200a9c4, slot, 0x19a, 172);
        alternate_pose = 1;
        Func_02008d20(slot, 0xd000);
        L_020044a2:;
        Func_0200aa3a(27, 0, 20);
        Call1(Func_02008d1c, 0x201b);
        Func_0200aa18(0, 3);
        Func_0200aa20(27, 3);
        Call3(Func_0200a9c6, 27, 0x10000, 0x8000);
        if (alternate_pose != 0) {
            Call3(Func_0200aa0e, 27, 0x1ac, 164);
            Call3(Func_0200aa1a, 27, 0x198, 164);
        }
        Call3(Func_0200aa26, 27, 0x198, 134);
        Call3(Func_0200aa2a, 27, 0x1b8, 134);
        Func_0200a9c8(40);
        Func_02008dfa(9, 10, 0);
        goto L_02004592;
    }
    Call1(Func_0200aac2, 0x1eb7);
    request = 0xa01b;
    Call3(Func_0200aae6, 0xa01b, 0, 40);
    Call3(Func_0200ab18, 27, 0x101, 60);
    Func_02008da2(request);
    Call2(Func_0200ab30, 0, 0x102);
    Func_0200aa06(60);
    Call3(Func_0200ab38, 27, 0x103, 40);
    Func_0200aad0(27, 2);
    Func_02008dca(request);
    Call3(Func_0200ab50, 27, 0x105, 40);
    Func_02008dda(request);
    Func_0200aad6(27, 4);
    Func_02008de8(request);
    Func_0200aae4(0, 3);
    Func_0200aa4a(20);
    Func_0200aba8(4);
    L_02004592:;
}
