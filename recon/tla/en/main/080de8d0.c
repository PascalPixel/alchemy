/* Near miss, differ 8 of 125 (2026-10-15), ported from ☀️'s BattleFx_RunSparkEmitter
 * (object 673, update Func_080de818). The amplitude 6 << 16 as a block local gives the
 * listing's r5 and the phase pointer's r8 (differ 60 without it). Left: the phase
 * pointer's copy to r8 one slot late, two scene loads one slot late, and the
 * argument 9 started after the anchor store. */
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "frame.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "fixed_math.h"
#include "fx_scene.h"
#include "ram_buffer.h"

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
    s32 gravity;
    u8 unknown_4c[9];
    u8 motion_flags;
    u8 unknown_56[8];
    u16 unknown_5e;
    u8 unknown_60[4];
    s16 phase;
    u8 unknown_66[6];
    void (*update)(struct SparkObject *);
};

struct SparkObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *pos);
void Animation_ApplyChildValuesFar(struct SparkObject *object, u32 value);
void Func_080de818(struct SparkObject *obj);

void Func_080de8d0(struct SparkObject *obj)
{
    struct BattleFxScene *scene;
    s16 *phase;
    struct SparkObject *spark;
    s32 pos[3];

    scene = Ram_HeapSlots->effect_work;
    phase = &obj->phase;
    if (*phase != -1) {
        s32 amplitude = 6 << 16;

        obj->x = scene->x + Iwram_MulQ16(amplitude, Trig_Sin(*phase << 10));
        obj->y = scene->y + (16 << 16);
        obj->z = scene->z;
        (*phase)++;
        *phase = (*phase + 64) % 64;
    }
    if (gFrameCount % 3 == 0) {
        pos[0] = obj->x;
        pos[1] = obj->y + (2 << 16);
        pos[2] = obj->z;
        Vector_AddPolarOffset(Random16() * 6, Random16(), pos);
        spark = Object_Spawn(673, pos[0], pos[1], pos[2]);
        if (spark != 0) {
            spark->update = Func_080de818;
            spark->scale_x = spark->scale_y = 0x9999;
            spark->motion_flags = 2;
            spark->gravity = 458;
            spark->phase = Random16() >> 9;
            spark->anchor_x = spark->x;
            Animation_ApplyChildValuesFar(spark, 9);
            spark->unknown_5e = 72;
            ObjectDispatch_InitializeFar((struct DispatchObject *)spark, BattleFx_CommonParticleScript);
        }
    }
}
