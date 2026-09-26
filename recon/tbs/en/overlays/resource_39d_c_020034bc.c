/* NONMATCHING: 364 bytes, candidate 362, 55 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * register choices (lamp copies through r4 where the reference uses r0 and
 * r1; the random mask result lands in r0, the reference keeps it in the
 * mask's r3 and loads the pool zero into r0 then r9), and the second pool:
 * the reference zero is a narrow pool constant that is ordered first and
 * dumps the pool after the priority mask; a (u16) zero orders it first but
 * lands the pool 8 bytes late, a (u8) zero lands it early. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define FrameCounter (*(u32 *)0x03001e40)

extern u8 Value_00000000;

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
    u8 *work;
    s32 height;
    s32 scale;
    s32 tick;
    s32 zero;

    lamp = Engine_ActorGet(8);
    work = *(u8 **)0x03001e70;
    height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    work += 0xe8;
    if (*(s16 *)(work + 2) <= 129) {
        if (FrameCounter & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            actor = Engine_ActorGet(8);
            scale = 0x10000;
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
    spark->unknown_64 = Engine_RandomNext() & 0xffff000;
    zero = (u32)&Value_00000000;
    spark->unknown_66 = tick;
    *(struct FieldActor **)spark->unknown_68 = lamp;
    spark->update = (void (*)(union FieldObject *))0x0200b461;
    spark->speed = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    *((u8 *)sprite + 38) = zero;
    sprite->priority = lamp->sprite->priority;
}
