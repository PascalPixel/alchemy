#include "types.h"

#define SceneActor_PassRaisedPointOfActorZero Func_02000dc8
#define SceneActor_PassActorZeroOffsetPoint Func_02000df4
#define SceneActor_CopyActor8PositionWhenAtRow10 Func_02000fe8
#define SceneActor_ApplyPointLeftOfActorZero Func_02001348
#define SceneActor_PassPointTwoRightOfActorZero Func_020013b4
#define SceneActor_LandOnHighestPlatform Func_02001c2c
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

/*
 * resource_3c4: land an actor on the highest platform sharing its tile.
 */
typedef struct Slot_02001c2c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_02001c2c;

struct Actor *Func_02003edc(s32);
void Func_02001aba(s32 *);
struct Actor *Func_02003f08(s32);
void Func_02001ae4(s32 *);
Record *Func_020040fc(s32);
Record *Func_0200410a(s32);
Record *Func_02004118(s32);
Record *Func_02004126(s32);
struct Actor *Func_0200445c(s32);
void Func_02002038(s32 *);
struct Actor *Func_020044c8(s32);
void Func_020020a6(s32 *);
Slot_02001c2c *Func_02004d56();
Slot_02001c2c *Func_02004d5e();
Slot_02001c2c *Func_02004d6e();
Slot_02001c2c *Func_02004d76();
Slot_02001c2c *Func_02004d86();
Slot_02001c2c *Func_02004d98();
Slot_02001c2c *Func_02004da8();
void Func_02004dcc();
Slot_02001c2c *Func_02004dca();
Slot_02001c2c *Func_02004dd2();
Slot_02001c2c *Func_02004dda();
void Func_02004d4e();
void Func_02004e14();
void Func_02004e92();
void Func_02002ecc();
void Func_02004dde();

/*
 * Each site names the call word it holds, so the same slot accessor appears
 * here under several symbols. The declarations are old-style because the call
 * sites vary in arity.
 */

void SceneActor_PassRaisedPointOfActorZero(void)
{
    s32 pos[3];
    struct Actor *p = Func_02003edc(0);

    pos[0] = p->f08;
    pos[1] = p->f0c;
    pos[2] = p->f10 + 0x200000;
    Func_02001aba(pos);
}

void SceneActor_PassActorZeroOffsetPoint(void)
{
    s32 pos[3];
    struct Actor *actor = Func_02003f08(0);

    pos[0] = actor->f08;
    pos[1] = actor->f0c;
    pos[2] = actor->f10 + 0xFFE00000;
    Func_02001ae4(pos);
}

s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
    Record *ref = Func_020040fc(0);

    if (ref->w12 > (s32)0xffd00000
        && (Func_0200410a(8)->w16 >> 20) == 10) {
        record->w8 = Func_02004118(8)->w8;
        record->w12 = (s32)0xffe00000;
        record->w16 = Func_02004126(8)->w16;
    } else {
        record->w8 = 0;
        record->w12 = 0;
        record->w16 = 0;
    }
    return 0;
}

void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor *actor = Func_0200445c(0);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    Func_02002038(point);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor *actor = Func_020044c8(0);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    Func_020020a6(pos);
}

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Func_02004d56(slot)->x >> 20) != (Func_02004d5e(subject)->x >> 20)) continue;
        if ((Func_02004d6e(slot)->z >> 20) != (Func_02004d76(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > Func_02004d86(slot)->y + 0x100000) continue;

        best = Func_02004d98(slot)->y + 0x100000;
        *(u16 *)((u8 *)Func_02004da8(subject) + 100) = (u16)slot;
    }

    Func_02004dcc(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Func_02004dca(subject);
        Slot_02001c2c *from = Func_02004dd2(subject);
        s32 z = Func_02004dda(subject)->z;

        Func_02004d4e(target, from->x, best, z);
    }

    Func_02004e14(subject);
    Func_02004e92(188);
    Func_02002ecc(subject);
    Func_02004dde(30);
}
