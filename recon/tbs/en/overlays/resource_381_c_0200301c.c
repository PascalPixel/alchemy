/* NONMATCHING: 752 bytes, candidate 744, 356 differing halfwords, 233
 * halfword edits (2026-09-25). SoruFunka_Func0200301c, meant for
 * FIELD/SORU_FUNKA/F_0301C.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: First complete typed spark-ring draft with separate ROM table
 * symbols; remaining differences span the loop and allocation.
 * WALL: structural-topology: spark lifetime and velocity loops */
#include "TYPES.H"

void *Engine_AllocateBlock(s32 id, s32 size);
s32 Engine_RandomNext(void);
s32 Local_020033d4(s32 num, s32 den);
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);

struct SoruRingObject {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[24];
    s32 draw_x;
    s32 draw_y;
    s32 draw_z;
};

struct SoruRingSpark {
    struct SoruRingObject *obj;
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

struct SoruRingWork {
    struct SoruRingSpark sparks[10];
    u16 count;
};

extern u8 Data_0200ba0c[][3];
extern u8 Data_0200ba2a[][3];
extern s8 Data_0200ba48[][3];
extern s32 Data_0200ba68[];
extern s32 Data_0200ba90[];

void SoruFunka_Func0200301c(void)
{
    struct SoruRingWork *work;
    struct SoruRingSpark *spark;
    struct SoruRingObject *obj;
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
            spark = &work->sparks[i];
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
                    if (scale >= Data_0200ba68[i]) {
                        speed = -Data_0200ba90[i];
                    } else if (scale <= 0x1999) {
                        scale = 0x1999;
                        speed = Data_0200ba90[i];
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

                rx = (u32)(Data_0200ba0c[i][0] * Engine_RandomNext()) >> 16;
                ry = (u32)(Data_0200ba0c[i][1] * Engine_RandomNext()) >> 16;
                rz = (u32)(Data_0200ba0c[i][2] * Engine_RandomNext()) >> 16;
                dx = rx != 0 ? Local_020033d4(rx << 16, 1000) : 0;
                dy = ry != 0 ? Local_020033d4(ry << 16, 1000) : 0;
                dz = rz != 0 ? Local_020033d4(rz << 16, 1000) : 0;

                if (Data_0200ba48[i][0] == 1)
                    ax += dx;
                else {
                    ax -= dx;
                    if (Data_0200ba48[i][0] != -1)
                        ax = 0;
                }
                if (Data_0200ba48[i][1] == 1)
                    ay += dy;
                else {
                    ay -= dy;
                    if (Data_0200ba48[i][1] != -1)
                        ay = 0;
                }
                if (Data_0200ba48[i][2] == 1)
                    az += dz;
                else {
                    az -= dz;
                    if (Data_0200ba48[i][2] != -1)
                        az = 0;
                }

                rx = Engine_MathSin(Data_0200ba2a[i][0] * ax) << 1;
                ry = Engine_MathSin(Data_0200ba2a[i][1] * ay) << 1;
                rz = Engine_MathCos(Data_0200ba2a[i][2] * az) << 1;
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
