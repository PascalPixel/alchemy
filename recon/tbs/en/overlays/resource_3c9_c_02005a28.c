/* NONMATCHING: 360 bytes, candidate 358, 58 differing halfwords (2026-09-25).
 * VinasuChojo_SpawnRisingSparks, meant for FIELD/VINASU_CHOJO/RISING_SPARKS.C
 * as a single-overlay unit binding Engine_* and Main_* at their import veneers
 * (runtime = listing offset + 0x8000). Remaining: the null test on the source
 * actor reloads it through r3 where the reference uses r1 and reuses that copy
 * for the x load; the RandomNext mask and the pooled zero (Value_00000000,
 * held in r9 for the sprite flags) schedule after the unknown_64 store instead
 * of before it, which moves the mid-function literal pool from after the
 * priority mask to after the MathSin call. Cross-jumped scale tails and the
 * scroll word >> 16 read already match. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct FieldView {
    u8 unknown_000[0xe8];
    s32 scroll_y;
};

extern struct FieldView *Data_03001e70;
extern u8 Data_0200e734[];
extern u8 Value_00000000;

void Scene_RunScene3c9(union FieldObject *object);

void VinasuChojo_SpawnRisingSparks(void)
{
    struct FieldActor *source = Engine_ActorGet(23);
    struct FieldView *view = Data_03001e70;
    s32 offset = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    struct FieldActor *spark;
    u8 zero;
    struct FieldSprite *sprite;
    u32 phase;

    if (view->scroll_y >> 16 <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(23, 0x1300000, 0xa40000);
            Engine_ActorGet(23)->scale_x = 0x10000;
            Engine_ActorGet(23)->scale_y = 0x10000;
        } else {
            Engine_ActorSetPosition(23, 0x1300000, 0xab0000);
            Engine_ActorGet(23)->scale_x = 0x14ccc;
            Engine_ActorGet(23)->scale_y = 0x14ccc;
        }
    } else {
        Engine_ActorSetPosition(23, 0, 0);
    }
    if (source != 0) {
        phase = gFrameCount & 15;
        if (phase == 0) {
            spark = Engine_ObjectCreate(284, source->x.fixed + 0x80000, source->y.fixed + offset + 0x80000, source->z.fixed);
            offset = Engine_MathDivide(offset, 0x60000);
            offset <<= 16;
            if (spark != 0) {
                sprite = spark->sprite;
                Engine_ObjectSetScript(spark, Data_0200e734);
                Engine_ObjectSetPalette(spark, 5);
                spark->motion_flags = phase;
                spark->unknown_64 = Engine_RandomNext() & 0x0ffff000;
                zero = (u8)(u32)&Value_00000000;
                spark->unknown_66 = phase;
                *(struct FieldActor **)spark->unknown_68 = source;
                spark->update = Scene_RunScene3c9;
                spark->speed = (Engine_MathSin((offset & 0xfffff) >> 4) * 24) >> 16;
                sprite->flags = zero;
                sprite->priority = source->sprite->priority;
            }
        }
    }
}
