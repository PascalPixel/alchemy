/* NONMATCHING: 744-byte candidate, 356 differing halfwords, 228 edits.
 * Whole owner 752 bytes including six owned pool words.
 * Canonical draft for resource_380:0200449c and resource_381:0200301c;
 * own-ROM sibling check proves equivalent flow and per-instance bindings.
 * Both remain not-yet-c. Consolidate the two draft units into one instanced
 * unit only after an exact score (the registry requires exact instances).
 *
 * 2026-09-26 baseline: 744 bytes, 356 differing halfwords, 228 edits.
 * Read the complete normalized diff: frame 68 matches, but work/current
 * entry/index and the derived entry+8 pointer have different lifetimes.
 * Reference keeps angles, scale, speed, timer and position snapshots on
 * stack; hold survives in fp and the third random sample survives in r8.
 * The second divided random sample also gates BOTH y-coordinate stores.
 *
 * H1: separate advancing record cursor from indexed animation state.
 * Result 772 bytes, 366 differing halfwords, 239 edits; frame grows to 76.
 * Cursor spills as predicted, but +16 animation and +8 position induction
 * pointers remain separate and an unwanted index*4 induction appears.
 * This is a negative structural result, not a register-spelling target.
 * H2: restore indexed baseline and express the three IWRAM unsigned divides
 * as ordinary C arithmetic, bound through __udivsi3. Own division entry
 * identifies 0x030003f0 as IwramUnsignedDivide. The entire 744-byte candidate
 * is identical to baseline, so helper-call versus division is not the cause.
 * Initializer 380:0200478c confirms ten 40-byte entries, count at +0x190,
 * scale/negative speed at +28/+32, timer=3 at +36, callback priority 0xc80.
 * Legacy 2026-09-24 pointer/index/loop-test/operand-order sweeps exhausted
 * 226..249 edits; do not repeat those without new ownership evidence. */
#include "TYPES.H"

void *Engine_AllocateBlock(s32 id, s32 size);
s32 Engine_RandomNext(void);
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);

struct SparkObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[24];
    s32 draw_x;
    s32 draw_y;
    s32 draw_z;
};

struct Spark {
    struct SparkObject *obj;
    s32 x;
    s32 y;
    s32 z;
    s32 angle_x;
    s32 angle_y;
    s32 angle_z;
    s32 scale;
    s32 scale_speed;
    u8 timer;
    u8 hold;
};

struct SparkWork {
    struct Spark spark[10];
    u16 count;
};

extern u8 gSparkJitter[][3];
extern u8 gSparkFrequency[][3];
extern s8 gSparkDirection[][3];
extern s32 gSparkMaxScale[];
extern s32 gSparkScaleStep[];

void Effect_UpdateSparkRing(void)
{
    struct SparkWork *work;
    struct Spark *spark;
    struct SparkObject *obj;
    s32 i;
    s32 ax;
    s32 ay;
    s32 az;
    s32 scale;
    s32 speed;
    s32 x;
    s32 y;
    s32 z;
    u8 timer;
    u8 hold;
    u32 rx;
    u32 ry;
    u32 rz;
    s32 dx;
    s32 dy;
    s32 dz;

    work = Engine_AllocateBlock(33, 0x194);
    for (i = 0; i != work->count; i++) {
        spark = &work->spark[i];
        obj = spark->obj;
        ax = spark->angle_x;
        ay = spark->angle_y;
        az = spark->angle_z;
        scale = spark->scale;
        speed = spark->scale_speed;
        x = spark->x;
        y = spark->y;
        z = spark->z;
        hold = spark->hold;
        timer = spark->timer - 1;
        if (timer == 0) {
            timer = 3;
            if (hold == 0) {
                scale += speed;
                if (scale >= gSparkMaxScale[i]) {
                    speed = -gSparkScaleStep[i];
                } else if (scale <= 0x1999) {
                    scale = 0x1999;
                    speed = gSparkScaleStep[i];
                    x = obj->x;
                    y = obj->y;
                    z = obj->z;
                    obj->x = 0;
                    obj->y = 0;
                    obj->z = 0;
                    hold = 24;
                }
                obj->scale_x = scale;
                obj->scale_y = scale;
            }
            rx = (u32)(gSparkJitter[i][0] * Engine_RandomNext()) >> 16;
            ry = (u32)(gSparkJitter[i][1] * Engine_RandomNext()) >> 16;
            rz = (u32)(gSparkJitter[i][2] * Engine_RandomNext()) >> 16;
            if (rx != 0)
                dx = (rx << 16) / 1000;
            else
                dx = 0;
            if (ry != 0)
                dy = (ry << 16) / 1000;
            else
                dy = 0;
            if (rz != 0)
                dz = (rz << 16) / 1000;
            else
                dz = 0;
            if (gSparkDirection[i][0] == 1) {
                ax += dx;
            } else {
                ax -= dx;
                if (gSparkDirection[i][0] != -1)
                    ax = 0;
            }
            if (gSparkDirection[i][1] == 1) {
                ay += dy;
            } else {
                ay -= dy;
                if (gSparkDirection[i][1] != -1)
                    ay = 0;
            }
            if (gSparkDirection[i][2] == 1) {
                az += dz;
            } else {
                az -= dz;
                if (gSparkDirection[i][2] != -1)
                    az = 0;
            }
            rx = Engine_MathSin(ax * gSparkFrequency[i][0]) << 1;
            ry = Engine_MathSin(ay * gSparkFrequency[i][1]) << 1;
            rz = Engine_MathCos(az * gSparkFrequency[i][2]) << 1;
            if (hold != 0) {
                x += rx;
                hold--;
                y += ry;
                z += rz;
                if (hold == 0) {
                    obj->x = x;
                    obj->draw_x = x;
                    if (dy != 0) {
                        obj->y = y;
                        obj->draw_y = y;
                    }
                    obj->z = z;
                    obj->draw_z = z;
                }
            } else {
                obj->x += rx;
                obj->draw_x = obj->x;
                if (dy != 0) {
                    obj->y += ry;
                    obj->draw_y = obj->y;
                }
                obj->z += rz;
                obj->draw_z = obj->z;
            }
        }
        spark->angle_x = ax;
        spark->angle_y = ay;
        spark->angle_z = az;
        spark->scale = scale;
        spark->scale_speed = speed;
        spark->hold = hold;
        spark->x = x;
        spark->y = y;
        spark->z = z;
        spark->timer = timer;
    }
}
