#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_0808a498(struct FieldSprite *sprite, s32 previous);
void ToretoHeya_Func020015ac(union FieldObject *object);

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

extern struct Vec3 Data_0200adc0;
extern s32 Data_0200adcc;

void ToretoHeya_Func0200162c(void)
{
    struct FieldActor *spark;
    u32 frame;
    s32 previous;
    s32 wave;
    u32 i;
    s32 *counter;

    counter = &Data_0200adcc;
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
            spark = Engine_ObjectCreate(0x11d, Data_0200adc0.x, Data_0200adc0.y, Data_0200adc0.z);
            if (spark != 0) {
                previous = Main_0808a498(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 0;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = ((360 / (u32)(6 - wave) * i) << 16) / 360;
                spark->target_x = Data_0200adc0.x;
                spark->target_y = Data_0200adc0.y;
                spark->target_z = Data_0200adc0.z;
                spark->speed = 0x19999;
                spark->update = ToretoHeya_Func020015ac;
            }
        }
    case 44:
        Engine_AudioPlayCue(0x121);
        /* FAKEMATCH: the counter pointer is taken again here so it is dead
         * across the spark loop, where the loop index takes its register. */
        counter = &Data_0200adcc;
        break;
    }
    if (++*counter > 120) {
        *counter = 0;
    }
}
