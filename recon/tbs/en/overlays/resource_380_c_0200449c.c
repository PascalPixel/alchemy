/* NONMATCHING: 728 bytes, candidate 744, 240 halfword edits (2026-09-24).
 * Hand-written from the disassembly; equivalent twin of resource_381:0200301c
 * (one instanced unit once it matches). Unit symbols beyond the Engine_
 * veneers: Local_030003f0 (fixed divide) and the tables Data_0200d0e4,
 * Data_0200d102, Data_0200d120, Data_0200d140, Data_0200d168 (data).
 * Control flow, calls, loads and stores follow the reference. Remaining:
 * register allocation of a loop with more live values than saved registers:
 * the reference keeps the work block, the entry pointer and the index on the
 * stack and gives sl a strength-reduced pointer to entry +8 (angles, scale,
 * and the y store), r8 the third random draw and fp the hold count; here the
 * entry pointer takes r8, the +8 pointer is spilled, hold takes sl and the
 * third draw is spilled around the divides, 16 bytes longer. Pointer
 * iteration, direct indexing, != and < loop tests and operand order moved
 * the edit count only between 226 and 249. */
#include "TYPES.H"

void *Engine_AllocateBlock(s32 id, s32 size);
s32 Engine_RandomNext(void);
s32 Local_030003f0(s32 num, s32 den);
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

extern u8 Data_0200d0e4[][3];
extern u8 Data_0200d102[][3];
extern s8 Data_0200d120[][3];
extern s32 Data_0200d140[];
extern s32 Data_0200d168[];

void Local_0200449c(void)
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
                if (scale >= Data_0200d140[i]) {
                    speed = -Data_0200d168[i];
                } else if (scale <= 0x1999) {
                    scale = 0x1999;
                    speed = Data_0200d168[i];
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
            rx = (u32)(Data_0200d0e4[i][0] * Engine_RandomNext()) >> 16;
            ry = (u32)(Data_0200d0e4[i][1] * Engine_RandomNext()) >> 16;
            rz = (u32)(Data_0200d0e4[i][2] * Engine_RandomNext()) >> 16;
            if (rx != 0)
                dx = Local_030003f0(rx << 16, 1000);
            else
                dx = 0;
            if (ry != 0)
                dy = Local_030003f0(ry << 16, 1000);
            else
                dy = 0;
            if (rz != 0)
                dz = Local_030003f0(rz << 16, 1000);
            else
                dz = 0;
            if (Data_0200d120[i][0] == 1) {
                ax += dx;
            } else {
                ax -= dx;
                if (Data_0200d120[i][0] != -1)
                    ax = 0;
            }
            if (Data_0200d120[i][1] == 1) {
                ay += dy;
            } else {
                ay -= dy;
                if (Data_0200d120[i][1] != -1)
                    ay = 0;
            }
            if (Data_0200d120[i][2] == 1) {
                az += dz;
            } else {
                az -= dz;
                if (Data_0200d120[i][2] != -1)
                    az = 0;
            }
            rx = Engine_MathSin(ax * Data_0200d102[i][0]) << 1;
            ry = Engine_MathSin(ay * Data_0200d102[i][1]) << 1;
            rz = Engine_MathCos(az * Data_0200d102[i][2]) << 1;
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
