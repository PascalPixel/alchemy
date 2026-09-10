#include "types.h"

#define SceneState_ApplyArgMode0AndReturnZero Func_02000cc0
#define SceneState_SwapSlotPairByRank Func_02000f10
#define SceneState_RunRect73x38Step Func_02001318
#define SceneState_ApplyTwoRectsAndRunThree Func_020013e0
#define SceneActor_MirrorFlag201IntoSlot14 Func_02001458
#define SceneState_ApplyFlag970 Func_02001500
#define SceneState_RunUnlessActorZeroAt30_52 Func_02001530
#define SceneState_SetValue202ThenCall Func_02001924
#define SceneActor_ConfigureSlot21AndSetFlag201 Func_02001938
#define SceneState_SetSlot17And18Selectors Func_02001f70
#define SceneState_SetValue268bInScene Func_02002330
struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

void Func_02003d72(s32, s32);
struct Slot02000f10 *Func_02004022(s32);
struct Slot02000f10 *Func_0200402a(s32);
void Func_02003f68(s32);
void Func_02004412(void);
void Func_020043ce(s32, s32, s32, s32, s32, s32);
void Func_020013fa(void);
void Func_020036da(void);
void Func_0200443a(void);
void Func_020044da(void);
void Func_02004496(s32, s32, s32, s32, s32, s32);
void Func_020044a8(s32, s32, s32, s32, s32, s32);
void Func_020014d4(void);
void Func_02003894(void);
void Func_02004514(void);
void Func_0200453c(s32);
s32 Func_0200453a(s32);
u8 *Func_0200457c(s32);
u8 *Func_02004588(s32);
u8 *Func_02004598(s32);
u8 *Func_020045a4(s32);
void Func_020045e4(s32);
struct Actor *Func_02004642(s32);
void Func_0200233e(void);
void Func_02004a06(s32);
void Func_02003072(void);
void Func_02004a84(s32, s32);
void Func_02004aa4(s32, s32);
void Func_02004a94(s32, s32);
u8 *Func_02004a62(s32);
void Func_02004a42(s32);
void Func_02005068(void);
Slot_02001f70 *Func_02005086(s32 slot);
Slot_02001f70 *Func_020050a2(s32 slot);
void Func_02005064(s32 selector);
void Func_02005074(s32 selector);
void Func_02005080(s32 selector);
void Func_02005090(s32 selector);
void Func_020044dc(void);
void Func_020050b0(void);
void Func_02005428(void);
void Func_02005480(s32, s32);
void Func_02005410(s32, s32);
void Func_02005444(void);

/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */

s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
    Func_02003d72(no, 0);
    return 0;
}

void SceneState_SwapSlotPairByRank(s32 first, s32 second)
{
    struct Slot02000f10 *a = Func_02004022(first);
    struct Slot02000f10 *b = Func_0200402a(second);

    if (a->rank <= b->rank) {
        s32 t;

        t = a->x;    a->x    = b->x;    b->x    = t;
        t = a->y;    a->y    = b->y;    b->y    = t;
        t = a->rank; a->rank = b->rank; b->rank = t;
        Func_02003f68(1);
    }
}

void SceneState_RunRect73x38Step(void)
{
    Func_02004412();
    {
        s32 width = 9;
        s32 height = 38;

        Func_020043ce(73, 38, 5, 5, width, height);
    }
    Func_020013fa();
    Func_020036da();
    Func_0200443a();
}

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Func_020044da();
    Func_02004496(89, 49, 3, 2, lead, 49);
    Func_020044a8(89, 51, 8, 5, lead, 51);
    Func_020014d4();
    Func_02003894();
    Func_02004514();
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 *
 * Every call site bls its own veneer, so the callees are named at the decoded
 * per-site addresses.
 */
void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    Func_0200453c(0x200);
    if (Func_0200453a(0x201) != 0) {
        Func_0200457c(14)[98] = 0;
        Func_02004588(14)[89] &= (u8)0xf7;
    } else {
        Func_02004598(14)[98] = 1;
        flags = Func_020045a4(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void SceneState_ApplyFlag970(void)
{
    Func_020045e4(0x970);
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct Actor *actor = Func_02004642(0);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        Func_0200233e();
    }
}

void SceneState_SetValue202ThenCall(void)
{
    Func_02004a06(0x202);
    Func_02003072();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Func_02004a84(21, 1);
    Func_02004aa4(21, 0);
    Func_02004a94(21, 2);
    flags = Func_02004a62(21) + 35;
    *flags &= 0xFD;
    Func_02004a42(0x201);
}

void SceneState_SetSlot17And18Selectors(void)
{
    Func_02005068();

    if ((Func_02005086(17)->w8 >> 20) == 45) {
        Func_02005064(0x974);
    } else {
        Func_02005074(0x974);
    }

    if ((Func_020050a2(18)->w8 >> 20) == 46) {
        Func_02005080(0x975);
    } else {
        Func_02005090(0x975);
    }

    Func_020044dc();
    Func_020050b0();
}

void SceneState_SetValue268bInScene(void)
{
    Func_02005428();
    Func_02005480(0, 1);
    Func_02005410(0x268B, 1);
    Func_02005444();
}
