#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

#define FrameCounter (*(u32 *)0x03001e40)

extern s32 Data_0200b1f0;
extern u8 Value_00000000;

void Main_08000128(s32 radius, s32 angle, s32 *pos);

/* Sway the actor from side to side and, every third frame, throw a spark
 * from a random point around it. */
void ImiruMura_SwayAndSpark(struct FieldActor *actor)
{
    s16 *phase;
    s32 pos[3];
    struct FieldActor *spark;
    s32 radius;
    u8 zero;

    phase = (s16 *)&actor->unknown_64;
    actor->x.fixed = Data_0200b1f0 + Iwram_MulQ16(0x60000, Engine_MathSin(*phase << 10));
    (*phase)++;
    *phase = (*phase + 64) % 64;
    if (Engine_MathModulo(FrameCounter, 3) == 0) {
        pos[0] = actor->x.fixed;
        pos[1] = actor->y.fixed + 0x20000;
        pos[2] = actor->z.fixed;
        radius = Engine_RandomNext();
        Main_08000128(radius * 6, Engine_RandomNext(), pos);
        spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
        if (spark != NULL) {
            spark->sprite->priority = 0;
            Engine_ActorSetSpriteFlags(spark, 0);
            Engine_ObjectSetAnimation(spark, 1);
            spark->scale_x = 0x9999;
            spark->scale_y = 0x9999;
            spark->priority_flags = 2;
            zero = (u8)(u32)&Value_00000000;
            spark->motion_flags = zero;
            Engine_ObjectSetPalette(spark, 9);
            Engine_ObjectSetScript(spark, (const s32 *)0x0200a64c);
        }
    }
}
