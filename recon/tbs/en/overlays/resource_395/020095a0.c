/* Draft of resource_395 0x020095a0..0x0200972c (396 bytes with pool),
 * KorimaKi_SpawnOrbitSparks; the listing keeps the rows. Remaining
 * difference: the compiler reaches the overlay's unsigned divide veneer by
 * its helper name, which the overlay can only give the veneer through a
 * link-time alias or a runtime-routine label; called by the veneer's own name
 * the divide sets r0 before r1 (4 bytes differ at +0x124). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);
void PaletteScene_AdvanceOrbit(union FieldObject *object);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct Vec3 gKorimaKiSparkOrigin;
extern s32 gKorimaKiSparkCount;

/* Every ten frames up to forty, ring the centre with six orbiting sparks; the frame counter wraps after 120. */
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
            spark = Engine_ObjectCreate(0x11d, gKorimaKiSparkOrigin.x, gKorimaKiSparkOrigin.y, gKorimaKiSparkOrigin.z);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 1;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = (i * 60 << 16) / 360;
                spark->target_x = gKorimaKiSparkOrigin.x;
                spark->target_y = gKorimaKiSparkOrigin.y;
                spark->target_z = gKorimaKiSparkOrigin.z;
                spark->speed = 0x19999;
                spark->update = PaletteScene_AdvanceOrbit;
            }
        }
        break;
    }
    if (++gKorimaKiSparkCount > 120) {
        gKorimaKiSparkCount = 0;
    }
}
