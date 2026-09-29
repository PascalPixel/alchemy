#include "KORIMAKI.H"

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

/* Every ten frames up to forty, ring the centre with six orbiting sparks; the
 * frame counter wraps after 120. */
void KorimaKi_SpawnOrbitSparks(void)
{
    struct FieldActor *spark;
    s32 previous;
    u32 i;

    previous = 0;
    switch (gKorimaKiSparkCount) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i <= 5; i++) {
            spark = Engine_ObjectCreate(0x11d, gKorimaKiSparkOrigin[0], gKorimaKiSparkOrigin[1], gKorimaKiSparkOrigin[2]);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 1;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = (i * 60 << 16) / 360;
                spark->target_x = gKorimaKiSparkOrigin[0];
                spark->target_y = gKorimaKiSparkOrigin[1];
                spark->target_z = gKorimaKiSparkOrigin[2];
                spark->speed = 0x19999;
                spark->update = (void (*)(union FieldObject *))PaletteScene_AdvanceOrbit;
            }
        }
        break;
    }
    if (++gKorimaKiSparkCount > 120) {
        gKorimaKiSparkCount = 0;
    }
}
