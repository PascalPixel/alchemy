/* Draft, not exact: 204 differing halfwords, 492-byte candidate for the
   484-byte owner (2026-09-24). Control flow, the OAM bitfield writes, the
   spawn block and the visible/hidden split follow the reference. Still
   open: the reference hoists the 0xfffffc00 tile mask into fp for the
   whole loop and keeps the camera pointer, the spawn count and the loop
   index on the stack (sp+12, sp+8, sp+4), deriving the camera cell as
   0x03001ec4 - 84; this candidate keeps the camera in fp, reloads the mask
   per use and loads both cells from the pool. The screen x minus one is
   formed before the bounds tests in the reference.

   Rain or snow particles: 32 sprites that fall toward the ground under
   the camera, drawn while on screen, respawned around the camera focus. */

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

struct CameraWork {
    s32 *focus;
    u8 unknown_04[0xe0];
    s32 x;
    s32 y;
};

#define gParticleWork (*(struct ParticleWork **)0x03001ec4)
#define gCamera (*(struct CameraWork **)0x03001e70)
#define gFrameCounter (*(u32 *)0x03001e40)

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
            cam_x = camera->x;
            cam_y = camera->y;
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
            s32 *focus = camera->focus;
            s32 x;
            s32 z;

            x = focus[0] + (Random16() << 8) - 0x800000;
            z = focus[2] + (Random16() << 8) - 0x800000;
            p->pos_z = z;
            p->pos_x = x;
            p->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
            p->timer = 120;
            p->fall = 0;
            spawned++;
        }
    }
}
