#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The motion script that deletes a spark once it has risen. */
extern const s32 ShindenHeya_SparkEndScript[];

/*
 * Shrine room: raises a spark by its speed each frame and twinkles it
 * through full, four-fifths and three-fifths size every four frames, until
 * its timer runs out and its script deletes it. The spark's speed and
 * timer are the halfwords its spawner sets at 0x64 and 0x66.
 */
void ShindenHeya_UpdateRisingSpark(struct FieldActor *spark)
{
    s32 scale;
    s32 timer;

    spark->y.fixed += (s16)spark->unknown_64 << 12;
    spark->target_y = spark->y.fixed;
    scale = 0;
    switch ((u16)((s16)spark->unknown_66 >> 2) & 3) {
    case 0:
        scale = 0x10000;
        break;
    case 1:
    case 3:
        scale = 0xcccc;
        break;
    case 2:
        scale = 0x9999;
        break;
    }
    spark->scale_x = scale;
    spark->scale_y = scale;
    timer = spark->unknown_66 - 1;
    spark->unknown_66 = timer;
    if ((s16)timer <= 0)
        Engine_ObjectSetScript(spark, ShindenHeya_SparkEndScript);
}
