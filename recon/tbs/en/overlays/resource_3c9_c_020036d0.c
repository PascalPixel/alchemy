/* NONMATCHING: 238 of 244 bytes, 73 differing halfwords, 16 halfword edits
 * (2026-09-26). Full-width Value_0ffff000 restores the separate field-store
 * addresses and mask register. The plain sprite-flags zero retains r8 and
 * both pool boundaries. Remaining: the object-type shift is early, the
 * script call lacks the reference's r7-to-r0 copy, and the tail is short.
 * Three structural hypotheses tested: link constants (retained mask only;
 * a symbolic zero loses r8 and the first pool), integer object return
 * (unchanged), Object_Create inline helper (234 bytes, 28 edits). Missing
 * flag/script bindings restored from the overlay's own import and pool. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Engine_MathModulo(s32 dividend, s32 divisor);

extern const s32 VinasuChojo_SparkScript[];
extern u8 Value_0ffff000;

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
    spark = Engine_ObjectCreate(
        0x11c, source->x.fixed, ((u32)rise >> 16 << 16) + source->y.fixed - 0x1c0000,
        source->z.fixed);
    if (spark != 0) {
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, VinasuChojo_SparkScript);
        Engine_ObjectSetPalette(spark, 1);
        spark->motion_flags = 0;
        spark->unknown_64 = Engine_RandomNext() & (u32)&Value_0ffff000;
        spark->unknown_66 = 0;
        spark->update = SPARK_UPDATE;
        spark->speed = Engine_MathSin((u32)(Engine_RandomNext() * 0xffff) >> 20) * 24 >> 16;
        sprite->flags = 0;
        sprite->priority = 1;
    }
}
