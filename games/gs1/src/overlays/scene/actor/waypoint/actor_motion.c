#include "types.h"

#define SceneActor_TryRunSlotZeroMoveStep Func_02000cd0
#define SceneActor_BobActorZeroWhenAheadClear Func_02001078
#define SceneActor_RunActorTwelveThreeWaypointMotion Func_02001328
#define SceneActor_RunActorFourteenFourWaypointMotion Func_02001880
#define SceneActor_BobActorZeroWhenTargetClear Func_02001ac8
s32 *Func_02003a98_a(s32 a);
void Func_02003a98_b(void);
s32 Func_02003a40(s32 *p, s16 *q);
void Func_02003a20(s32 *p, s32 b);
void Func_020039ee(s32 a);
void Func_02003bbc(s32 a);
void Func_02003a34(s32 *p, s32 b);
void Func_02003a8e(s32 *p, s32 b);
s32 Func_02003b14(s32 a, s32 b, s32 c);
void Func_02003a6c(s32 *p, s32 b);
void Func_02003aac(s32 *p, s32 b);
void Func_02003b04(void);
s32 *Func_02003e40(s32 a);
s32 Func_02001d82(s32 *actor);
void Func_02003e5e(void);
void Func_02003ec2_a(s32 a, s32 b);
void Func_02003ec2_b(void);
void Func_02003dc6(s32 a);
void Func_02003dd8(s32 a);
void Func_02003dee(s32 a);
u8 *Func_020040ec(s32);
void Func_020040dc(void);
void Func_020021e8(s32, s32);
void Func_020020e2(s32, s32, s32, s32);
void Func_02001e58(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_020041cc(s32, s32);
void Func_0200419e(s32, s32, s32);
void Func_02004124(s32);
void Func_02004194(s32, s32);
void Func_020041e6(s32, s32);
void Func_0200413e(s32);
void Func_02002142(s32, s32, s32, s32);
void Func_020041d0(s32, s32, s32);
void Func_0200415e(s32);
void Func_02002160(s32, s32, s32, s32);
void Func_020041ee(s32, s32, s32);
void Func_0200417c(s32);
void Func_0200217e(s32, s32, s32, s32);
void Func_0200420c(s32, s32, s32);
void Func_0200419a(s32);
void Func_02004198(s32);
void Func_02004202(s32, s32, s32);
void Func_020041be(void);
u8 *Func_02004644(s32);
void Func_02004632(void);
void Func_0200273e(s32, s32);
void Func_02002638(s32, s32, s32, s32);
void Func_0200464a(s32);
void Func_020023b4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02004728(s32, s32);
void Func_020046fa_a(s32, s32, s32);
void Func_02004680(s32);
void Func_020046f0(s32, s32);
void Func_02004742(s32, s32);
void Func_0200469c(s32);
void Func_020026a2(s32, s32, s32, s32);
void Func_02004730_a(s32, s32, s32);
void Func_020046be(s32);
void Func_020026c0(s32, s32, s32, s32);
void Func_0200474e(s32, s32, s32);
void Func_020046dc(s32);
void Func_020026de(s32, s32, s32, s32);
void Func_0200476c(s32, s32, s32);
void Func_020046fa_b(s32);
void Func_020026fc(s32, s32, s32, s32);
void Func_0200478a(s32, s32, s32);
void Func_02004718(s32);
void Func_020047d8(s32, s32);
void Func_02004782(s32, s32, s32);
void Func_02004730_b(s32);
void Func_0200472e(s32);
void Func_02004798(s32, s32, s32);
void Func_02004754(void);
u8 *Func_02004890(s32);
u8 *Func_020048a0(s32);
u8 *Func_020048b0(s32);
u8 *Func_020048ba(s32);
s32 Func_020027d8(s32 *);
void Func_020048b4(void);
void Func_02004918_a(s32, s32);
void Func_02004918_b(void);
void Func_0200481c(s32);
void Func_0200482e(s32);
void Func_02004844(s32);

/* Two veneers each serve two imports: 0x020046fa is the three-argument setter
 * at site 8 and the one-argument wait at site 21, and 0x02004730 is the
 * three-argument step at site 14 and the one-argument wait at site 27. */

/* Four sites of the record accessor, so four names. */

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */

s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Func_02003a98_a(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Func_02003a40(p, arg);

    if (r == 0) {
        s32 m;

        Func_02003a98_b();
        Func_02003a20(p, 6);
        Func_020039ee(6);
        Func_02003bbc(152);
        Func_02003a34(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Func_02003a8e(p, 0);
        Func_02003b14(0, arg[1], arg[5]);
        Func_02003a6c(p, 6);
        Func_02003aac(p, 1);
        *f = saved;
        Func_02003b04();
        return 1;
    }
    return 0;
}

void SceneActor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Func_02003e40(0);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (Func_02001d82(pos)!= 0) {
        Func_02003e5e();
        *fp = 0;
        Func_02003ec2_a(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dc6(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Func_02003dd8(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        Func_02003dee(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Func_02003ec2_b();
    }
}

void SceneActor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Func_020040ec(12);

    /* r0 still holds the record returned above. */
    Func_020040dc();

    Func_020021e8(12, 1);
    Func_020020e2(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Func_02001e58(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Func_020041cc(12, 1);
    Func_0200419e(12, 0, 0);
    Func_02004124(20);
    Func_02004194(12, 2);
    Func_020041e6(12, 258);                     /* 129 << 1 */
    Func_0200413e(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Func_02002142(12, 584, 344, 0x30000);
    Func_020041d0(0, 12, 0);
    Func_0200415e(6);

    Func_02002160(12, 632, 344, 0x30000);
    Func_020041ee(0, 12, 0);
    Func_0200417c(6);

    Func_0200217e(12, 680, 344, 0x30000);
    Func_0200420c(0, 12, 0);
    Func_0200419a(6);

    Func_02004198(0x302);
    Func_02004202(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020041be();
}

void SceneActor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Func_02004644(14);

    /* r0 still holds the record returned above. */
    Func_02004632();

    Func_0200273e(14, 1);
    Func_02002638(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Func_0200464a(10);

    Func_020023b4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004728(14, 1);
    Func_020046fa_a(14, 0, 0);
    Func_02004680(20);
    Func_020046f0(14, 2);
    Func_02004742(14, 258);                     /* 129 << 1 */
    Func_0200469c(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Func_020026a2(14, 360, 528, 0x30000);
    Func_02004730_a(0, 14, 0);
    Func_020046be(6);

    Func_020026c0(14, 328, 528, 0x30000);
    Func_0200474e(0, 14, 0);
    Func_020046dc(6);

    Func_020026de(14, 288, 528, 0x30000);
    Func_0200476c(0, 14, 0);
    Func_020046fa_b(6);

    Func_020026fc(14, 256, 528, 0x30000);
    Func_0200478a(0, 14, 0);
    Func_02004718(6);

    Func_020047d8(0, 1);
    Func_02004782(14, 0, 0);
    Func_02004730_b(30);
    Func_0200472e(0x306);
    Func_02004798(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004754();
}

void SceneActor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_02004890(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Func_020048a0(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Func_020048b0(0) + 12);
    target[2] = *(s32 *)(Func_020048ba(0) + 16);

    if (Func_020027d8(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Func_020048b4();

        *mode = 0;
        Func_02004918_a(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200481c(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_0200482e(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Func_02004844(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Func_02004918_b();
    }
}
