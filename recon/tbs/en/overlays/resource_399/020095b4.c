/*
 * 2026-10-01 (matcher 3): the pooled zero is a halfword constant. agscc
 * loads every HImode constant from the literal pool (*thumb_movhi_insn
 * lists "mn" before "I"), and local-alloc moves a set-once, used-once
 * REG_EQUIV constant to just before its use when set and use sit in
 * different basic blocks outside loop notes. So the reference's zero is an
 * HImode 0 made before the `spark != NULL` test (or before the frame test)
 * that CSE shares into motion_flags' byte store; ResetSceneTransitionEffect
 * (EFFECT38.C) gets the same shape from an s16 field store, whose
 * store_bit_field mask is that zero. Not yet found here.
 *
 * Draft of resource_399 0x020095b4 (ImiruMura_SwayAndSpark), from games/THE
 * BROKEN SEAL/SRC/FIELD/IMIRU_MURA; the range links as disassembly (section
 * .text.x020095b4 of the overlay listing).
 *
 * Remaining: the reference stores the spark's motion flags from a zero it
 * loads from the literal pool (ldrb, first pool word). Written as a plain
 * zero below, GCC reuses the register that held the frame modulo, which is
 * known to be zero inside the test, keeps it in r7 across the calls and
 * drops the pool word; a u8 or u16 zero variable, a block-scope zero and a
 * negated test give the same. The unit matched only while the zero was a
 * link-time symbol named after its own value.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"


extern s32 ImiruMura_ArcOrigin;
extern u8 ImiruMura_SparkScript[];

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *pos);

/* Sway the actor from side to side and, every third frame, throw a spark
 * from a random point around it. */
void ImiruMura_SwayAndSpark(struct FieldActor *actor)
{
    s16 *phase;
    s32 pos[3];
    struct FieldActor *spark;
    s32 radius;

    phase = (s16 *)&actor->unknown_64;
    actor->x.fixed = ImiruMura_ArcOrigin + Iwram_MulQ16(0x60000, Engine_MathSin(*phase << 10));
    (*phase)++;
    *phase = (*phase + 64) % 64;
    if (Engine_MathModulo((*(u32 *)&gFrameCount), 3) == 0) {
        pos[0] = actor->x.fixed;
        pos[1] = actor->y.fixed + 0x20000;
        pos[2] = actor->z.fixed;
        radius = Engine_RandomNext();
        Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
        spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
        if (spark != NULL) {
            spark->sprite->priority = 0;
            Engine_ActorSetSpriteFlags(spark, 0);
            Object_SetMode(spark, 1);
            spark->scale_x = 0x9999;
            spark->scale_y = 0x9999;
            spark->priority_flags = 2;
            spark->motion_flags = 0;
            ObjectGroup_SetChildValue(spark, 9);
            Engine_ObjectSetScript(spark, (const s32 *)ImiruMura_SparkScript);
        }
    }
}
