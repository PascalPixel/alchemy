#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_0808a498(struct FieldSprite *sprite, s32 previous);
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

extern struct Vec3 Data_02009dc0;
extern s32 Data_02009dcc;

/* Every ten frames up to forty, ring the centre with six orbiting sparks; the frame counter wraps after 120. */
void KorimaKi_SpawnOrbitSparks(void)
{
    struct FieldActor *spark;
    s32 previous;
    u32 i;

    previous = 0;
    switch (Data_02009dcc) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i <= 5; i++) {
            spark = Engine_ObjectCreate(0x11d, Data_02009dc0.x, Data_02009dc0.y, Data_02009dc0.z);
            if (spark != 0) {
                previous = Main_0808a498(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 1;
                Engine_ActorSetSpriteFlags(spark, 0);
                Engine_ObjectSetAnimation(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = (i * 60 << 16) / 360;
                spark->target_x = Data_02009dc0.x;
                spark->target_y = Data_02009dc0.y;
                spark->target_z = Data_02009dc0.z;
                spark->speed = 0x19999;
                spark->update = PaletteScene_AdvanceOrbit;
            }
        }
        break;
    }
    if (++Data_02009dcc > 120) {
        Data_02009dcc = 0;
    }
}
