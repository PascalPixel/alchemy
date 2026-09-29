/* resource_396 0x0200962c..0x020097ec ToretoHeya_SpawnSwirlSparks (448 bytes
 * with pool), formerly FIELD/TORETO_HEYA/SPAWN_SPARKS.C; the listing keeps
 * the rows. Compiles exactly. Remaining difference: its unsigned divisions
 * call the compiler's __udivsi3, a name the publication gate keeps out of
 * game source; the overlay's unsigned divider veneer has no other name. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);
void ToretoHeya_UpdateSwirlObject(union FieldObject *object);

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

extern struct Vec3 ToretoHeya_SparkOrigin;
extern s32 ToretoHeya_SparkCounter;

void ToretoHeya_SpawnSwirlSparks(void)
{
    struct FieldActor *spark;
    u32 frame;
    s32 previous;
    s32 wave;
    u32 i;
    s32 *counter;

    counter = &ToretoHeya_SparkCounter;
    frame = *counter;
    previous = 0;
    wave = Engine_MathDivide(frame, 10);
    switch (frame) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i < 6 - wave; i++) {
            spark = Engine_ObjectCreate(0x11d, ToretoHeya_SparkOrigin.x, ToretoHeya_SparkOrigin.y, ToretoHeya_SparkOrigin.z);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 0;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = ((360 / (u32)(6 - wave) * i) << 16) / 360;
                spark->target_x = ToretoHeya_SparkOrigin.x;
                spark->target_y = ToretoHeya_SparkOrigin.y;
                spark->target_z = ToretoHeya_SparkOrigin.z;
                spark->speed = 0x19999;
                spark->update = ToretoHeya_UpdateSwirlObject;
            }
        }
    case 44:
        Engine_AudioPlayCue(0x121);
        /* FAKEMATCH: the counter pointer is taken again here so it is dead
         * across the spark loop, where the loop index takes its register. */
        counter = &ToretoHeya_SparkCounter;
        break;
    }
    if (++*counter > 120) {
        *counter = 0;
    }
}
