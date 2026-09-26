/* NONMATCHING: 488 of 484 bytes, 195 differing halfwords, 105 halfword
 * edits (2026-09-25). Reusing Sparkles map-cell derivation restores the
 * reference opening loads (previously492 bytes,204 differing halfwords).
 * Remaining: tile-mask lifetime, loop/spawn stack slots and screen bounds
 * ordering. This owner renders timed frames and delayed particle bursts.
 */

#include "TYPES.H"
#include "SYSTEM.H"

s32 GameFlag_TestFar(s32 flag);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 slot);

struct Particle {
    void *next;
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 unknown_0a;
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    s32 fall;
    u16 timer;
    u16 unknown_1e;
};

struct ParticleWork {
    u32 unknown_00;
    s32 tile;
    struct Particle particles[32];
};

struct MapPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct CameraWork {
    struct MapPosition *leader;
    u8 unknown_04[0xe0];
    s32 camera_x;
    s32 camera_z;
};

extern struct ParticleWork *Data_03001ec4;
#define gParticleWork Data_03001ec4
/* FAKEMATCH: derive the adjacent map cell from the particle-work cell,
 * as in the exact Sparkles routine, to preserve the reference loads. */
#define gCamera (*(struct CameraWork **)((u8 *)&Data_03001ec4 - 84))
extern unsigned long Data_03001e40;
#define gFrameCounter Data_03001e40

void Unnamed_08094bbc(void)
{
    struct ParticleWork *work;
    struct CameraWork *camera;
    struct Particle *p;
    u32 i;
    s32 spawned;
    s32 cam_x;
    s32 cam_y;
    s16 lift;
    s32 sx;
    s32 sy;
    u32 a;
    u32 b;

    work = gParticleWork;
    camera = gCamera;
    spawned = 0;
    p = work->particles;
    for (i = 0; i < 32; i++, p++) {
        if (p->timer-- != 0) {
            cam_x = camera->camera_x;
            cam_y = camera->camera_z;
            lift = p->timer;
            if (GameFlag_TestFar(0x166)) {
                p->timer++;
                p->fall--;
            }
            a = Random16();
            b = Random16();
            sx = ((p->pos_x - cam_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
            sy = ((p->pos_z - p->pos_y - cam_y) >> 16) - (u16)lift;
            if ((u32)(sx + 15) <= 255 && sy >= -32 && sy <= 159) {
                if (p->timer < 60) {
                    p->tile = work->tile + 16;
                    p->fall += 3;
                } else if (p->timer < 90) {
                    p->tile = work->tile + 8;
                    p->fall += 1;
                } else {
                    p->tile = work->tile;
                }
                if ((gFrameCounter >> 3) & 1)
                    p->tile += 4;
                p->x = sx - 1;
                p->y = sy - (p->fall >> 2);
                p->shape = 0;
                p->size = 1;
                Runtime_PushSlotEntry(p, 240);
            } else {
                p->timer = 0;
            }
        }
        if (spawned < 8 && p->timer == 0) {
            struct MapPosition *focus = camera->leader;
            s32 x;
            s32 z;

            x = focus->x + (Random16() << 8) - 0x800000;
            z = focus->z + (Random16() << 8) - 0x800000;
            p->pos_z = z;
            p->pos_x = x;
            p->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
            p->timer = 120;
            p->fall = 0;
            spawned++;
        }
    }
}
