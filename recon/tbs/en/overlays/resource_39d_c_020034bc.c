/* NONMATCHING: 364 bytes, candidate 364, 60 differing halfwords, 39 aligned
 * halfword edits (2026-09-26). The halfword zero aggregate preserves the
 * complete extent; the Value mask keeps the full-width literal and result
 * in r3. Remaining: lamp and zero reload registers, the signed height
 * access index, and a second pool four bytes later than the reference.
 * An independent random-result local and scalar zero regress the pools. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define FrameCounter (*(u32 *)0x03001e40)

extern u8 Value_00000000;
extern u8 Value_0ffff000;

struct LampWork {
    s16 unused;
    s16 height;
};

struct Half {
    u16 value;
};

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_020034bc(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    struct FieldActor *spark;
    struct FieldSprite *sprite;
    struct LampWork *work;
    s32 height;
    s32 scale;
    s32 tick;
    struct Half zero;

    lamp = Engine_ActorGet(8);
    work = (struct LampWork *)(*(u8 **)0x03001e70 + 0xe8);
    height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    if (work->height <= 129) {
        if (FrameCounter & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            actor = Engine_ActorGet(8);
            /* FAKEMATCH: keep the scale load after the actor lookup. */
            do { scale = 0x10000; } while (0);
        } else {
            Engine_ActorSetPosition(8, 0x1300000, 0x970000);
            actor = Engine_ActorGet(8);
            scale = 0x14ccc;
        }
        actor->scale_x = scale;
        Engine_ActorGet(8)->scale_y = scale;
    } else {
        Call3(Engine_ActorSetPosition, 8, 0x80000, 0x80000);
    }
    if (lamp == NULL)
        return;
    tick = FrameCounter & 15;
    if (tick != 0)
        return;
    spark = Engine_ObjectCreate(0x11c, lamp->x.fixed + 0x80000, lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = Engine_MathDivide(height, 0x60000);
    height <<= 16;
    if (spark == NULL)
        return;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, (const s32 *)0x0200bc54);
    Engine_ObjectSetPalette(spark, 3);
    spark->motion_flags = tick;
    spark->unknown_64 = (u32)&Value_0ffff000 & Engine_RandomNext();
    zero.value = (u16)(u32)&Value_00000000;
    spark->unknown_66 = tick;
    *(struct FieldActor **)spark->unknown_68 = lamp;
    spark->update = (void (*)(union FieldObject *))0x0200b461;
    spark->speed = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    *((u8 *)sprite + 38) = zero.value;
    sprite->priority = lamp->sprite->priority;
}
