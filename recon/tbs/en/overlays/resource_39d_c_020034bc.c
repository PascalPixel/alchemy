/* NONMATCHING: 364 bytes, candidate 356, 118 differing halfwords
 * (2026-09-24). Single-overlay unit binding Engine_* at their import veneers.
 * Remaining: hand-written first pass, 72 edits, 8 bytes short: register
 * choices around the spark setup (the reference takes the divided height
 * through r8 twice and interleaves the pool zero into r9 between the spin and
 * word stores), and the sprite-priority tail sits across the second
 * mid-function pool differently. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

#define FrameCounter (*(u32 *)0x03001e40)

extern u8 Value_00000000;

void Local_020034bc(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    struct FieldActor *spark;
    struct FieldSprite *sprite;
    u8 *work;
    s32 height;
    s32 scale;
    s32 tick;
    u8 zero;

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
        Engine_ActorSetPosition(8, 0x80000, 0x80000);
    }
    if (lamp == NULL)
        return;
    tick = FrameCounter & 15;
    if (tick != 0)
        return;
    spark = Engine_ObjectCreate(0x11c, lamp->x.fixed + 0x80000, lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = Engine_MathDivide(height, 0x60000) << 16;
    if (spark == NULL)
        return;
    sprite = spark->sprite;
    Engine_ObjectSetScript(spark, (const s32 *)0x0200bc54);
    Engine_ObjectSetPalette(spark, 3);
    spark->motion_flags = tick;
    spark->unknown_64 = Engine_RandomNext() & 0xffff000;
    zero = (u8)(u32)&Value_00000000;
    spark->unknown_66 = tick;
    *(struct FieldActor **)spark->unknown_68 = lamp;
    spark->update = (void (*)(union FieldObject *))0x0200b461;
    spark->speed = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    sprite->flags = zero;
    sprite->priority = lamp->sprite->priority;
}
