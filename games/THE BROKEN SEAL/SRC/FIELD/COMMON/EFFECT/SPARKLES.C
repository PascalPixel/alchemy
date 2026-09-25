#include "TYPES.H"

/* field/common/effect/sparkles.c */

struct Sparkle {
    u8 padding00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 padding0a[2];
    s32 pos_x;
    s32 height;
    s32 pos_z;
    s32 delay;
    u16 timer;
    u8 padding1e[2];
};

struct SparkleWork {
    u8 padding00[4];
    s32 tile_base;
    struct Sparkle sparkles[32];
    u8 padding408[4];
    s32 stopped;
};

struct SparkleFrame {
    s16 dy;
    u16 tile;
};

struct MapPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct MapWork {
    struct MapPosition *leader;
    u8 padding04[0xe0];
    s32 camera_x;
    s32 camera_z;
};

/* FAKEMATCH: the map work pointer is reached 84 bytes below the sparkle work
   pointer, so both loads share one pool address. */
extern struct SparkleWork *Data_03001ec4;
#define SparkleMap (*(struct MapWork **)((u8 *)&Data_03001ec4 - 84))
/* The frame counter is an unsigned long: a type no sparkle field shares, so
   reading it does not keep the size store apart from the flip store. */
extern unsigned long Data_03001e40;
extern const struct SparkleFrame Data_0809f024[];

s32 GameFlag_TestFar(s32 flag);
u32 Random16(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 value);

/* Draw the ground sparkles that twinkle around the leader, and every so often
   start a burst of four at a random spot nearby. */
void FieldEffect_UpdateSparkles(void)
{
    struct SparkleWork *work;
    u32 spawned;
    u32 i;
    struct MapWork *map;
    s32 *camera;
    s32 spawn_x;
    s32 spawn_z;
    struct Sparkle *sparkle;
    const u16 *frame;
    struct MapPosition *leader;
    s32 camera_x;
    s32 camera_z;
    s32 sx;
    s32 sy;
    s32 delay;
    s32 x;
    u32 a;
    u32 b;
    u32 frames;

    work = Data_03001ec4;
    spawned = 0;
    map = SparkleMap;
    camera = &map->camera_x;
    spawn_x = 0;
    spawn_z = 0;
    i = 0;
    sparkle = work->sparkles;
    delay = 0;
    sx = 0;
    sy = 0;
    do {
        if (sparkle->timer != 0) {
            camera_x = camera[0];
            camera_z = camera[1];
            if (GameFlag_TestFar(0x166) != 0)
                sparkle->timer++;
            frame = (const u16 *)&Data_0809f024[sparkle->timer >> 1];
            a = Random16();
            b = Random16();
            x = ((sparkle->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
            sx = x - 1;
            sy = (sparkle->pos_z - sparkle->height - camera_z) / 0x10000 + *(const s16 *)frame;
            frame++;
            if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
                sparkle->tile = work->tile_base + *frame;
                sparkle->x = sx;
                sparkle->shape = 0;
                sparkle->size = 1;
                frames = Data_03001e40;
                sparkle->y = sy;
                sparkle->matrix = ((frames >> 1) & 1) << 3;
                Runtime_PushSlotEntry(sparkle, 240);
            }
            sparkle->timer--;
        }
        if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
            if (delay != 0) {
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 62 - delay;
                sparkle->delay = 0;
                spawned++;
                delay += 4;
            } else if ((Random16() & 255) == 0) {
                leader = map->leader;
                spawn_x = leader->x + (Random16() << 8) - 0x800000;
                spawn_z = leader->z + (Random16() << 8) - 0x800000;
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 30;
                sparkle->delay = delay;
                spawned++;
                delay = 4;
            }
        }
        i++;
        sparkle++;
    } while (i <= 31);
}
