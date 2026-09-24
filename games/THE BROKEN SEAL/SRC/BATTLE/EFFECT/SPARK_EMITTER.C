#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

struct SparkObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[24];
    s32 anchor_x;
    u8 unknown_3c[12];
    s32 speed;
    u8 unknown_4c[9];
    u8 mode;
    u8 unknown_56[8];
    u16 unknown_5e;
    u8 unknown_60[4];
    s16 phase;
    u8 unknown_66[6];
    void (*update)(struct SparkObject *);
};

struct SparkAnchor {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
};

extern struct SparkAnchor *gEffectWork;
extern u32 gFrameCount;

u32 Math_ModU(u32 value, u32 divisor);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct SparkObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct SparkObject *object, s32 value);
void ObjectDispatch_InitializeFar(struct SparkObject *object, s32 data);
void BattleFx_SwayParticle(struct SparkObject *object);

/* Sways the emitter above its anchor and, every third frame, spawns a
   swaying spark at a random offset around it. */
void BattleFx_RunSparkEmitter(struct SparkObject *object)
{
    struct SparkAnchor *anchor;
    s16 *phase;
    struct SparkObject *spark;
    s32 position[3];

    phase = &object->phase;
    anchor = gEffectWork;
    if (*phase != -1) {
        object->x = anchor->x + Iwram_MulQ16(0x60000, Trig_Sin(*phase << 10));
        object->y = anchor->y + 0x100000;
        object->z = anchor->z;
        (*phase)++;
        *phase = (*phase + 64) % 64;
    }
    if (Math_ModU(gFrameCount, 3) == 0) {
        position[0] = object->x;
        position[1] = object->y + 0x20000;
        position[2] = object->z;
        Vector_AddPolarOffset(Random16() * 6, Random16(), position);
        spark = Object_Spawn(0x11d, position[0], position[1], position[2]);
        if (spark != 0) {
            spark->update = BattleFx_SwayParticle;
            spark->scale_x = spark->scale_y = 0x9999;
            spark->mode = 2;
            spark->speed = 458;
            spark->phase = Random16() >> 9;
            spark->anchor_x = spark->x;
            Animation_ApplyChildValuesFar(spark, 9);
            spark->unknown_5e = 72;
            ObjectDispatch_InitializeFar(spark, 0x0809f0b0);
        }
    }
}
