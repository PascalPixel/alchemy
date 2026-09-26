/* NONMATCHING: 238 of 244 bytes, 39 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 39 halfword edits, 238 of 244 bytes: the sprite-flags zero is now plain 0 and lands from the pool in r8 as in the reference. Left: Engine_ObjectCreate's argument scheduling (0x11c shift), the reference reusing r0 for spark in Engine_ObjectSetScript, the unknown_64/unknown_66 stores computing each address separately, and the pool that follows the flag store. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Engine_MathModulo(s32 dividend, s32 divisor);

extern const s32 VinasuChojo_SparkScript[];

/* The EWRAM routine at 0x0200b600 (Thumb). */
#define SPARK_UPDATE ((void (*)(union FieldObject *))0x0200b601)

void Func_020036d0(void)
{
    struct FieldActor *source;
    struct FieldActor *spark;
    struct FieldSprite *sprite;
    s32 rise;

    if (!GameFlag_IsSet(0x236) && Engine_MathModulo(gFrameCount, 3) != 0)
        return;
    source = Engine_ActorGet(24);
    if (GameFlag_IsSet(0x236)) {
        rise = Engine_RandomNext();
        rise <<= 8;
    } else {
        rise = Engine_RandomNext();
        rise <<= 6;
    }
    spark = Engine_ObjectCreate(0x11c, source->x.fixed, ((u32)rise >> 16 << 16) + source->y.fixed - 0x1c0000,
                                source->z.fixed);
    if (spark != 0) {
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, VinasuChojo_SparkScript);
        Engine_ObjectSetPalette(spark, 1);
        spark->motion_flags = 0;
        spark->unknown_64 = Engine_RandomNext() & 0xffff000;
        spark->unknown_66 = 0;
        spark->update = SPARK_UPDATE;
        spark->speed = Engine_MathSin((u32)(Engine_RandomNext() * 0xffff) >> 20) * 24 >> 16;
        sprite->flags = 0;
        sprite->priority = 1;
    }
}
