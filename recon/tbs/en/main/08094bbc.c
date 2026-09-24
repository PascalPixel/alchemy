/* Draft, not exact (2026-09-24): candidate=488 reference=484 differing_halfwords=100.
   Storm motes: 32 OAM records after the work header (see 08094da0), each
   counted down, tiled by age, bobbed, blinked on frame bit 3 and queued at
   depth 240; up to eight dead motes respawn around the camera focus.
   Code shape matches. Open: the reference loads the work pointer through
   0x03001ec4 and derives the camera table by subs #84 (here a negative
   index load), keeps the camera in sp+12 and hoists the 0xfffffc00 tile
   mask into fp (here the camera takes fp and the mask reloads per use),
   and spills the respawn x to sp+0 across Random16. Literal addresses,
   separate externs and a pointer-plus-21 spelling were worse. */
#include "TYPES.H"
#include "SYSTEM.H"

s32 GameFlag_TestFar(s32 flag);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Runtime_PushSlotEntry(void *entry, s32 slot);

struct Mote {
    struct Mote *next;
    u16 y : 8;
    u16 affine_mode : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 colors : 1;
    u16 shape : 2;
    u16 screen_x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 pad_0a;
    s32 x;
    s32 height;
    s32 z;
    s32 rise;
    u16 phase;
    u16 pad_1e;
};

struct MoteWork {
    s32 resource;
    s32 vram;
    struct Mote motes[32];
};

struct MoteCamera {
    s32 *focus;
    u8 unknown_04[0xe0];
    s32 x;
    s32 y;
};

extern void *Data_03001ec4[];
extern u32 Data_03001e40;

void Func_08094bbc(void)
{
    struct MoteWork *work = Data_03001ec4[0];
    struct MoteCamera *camera = Data_03001ec4[-21];
    struct Mote *mote;
    u32 spawned = 0;
    u32 i = 0;

    mote = work->motes;
    do {
        if (mote->phase-- != 0) {
            s32 camera_x = camera->x;
            s32 camera_y = camera->y;
            s16 bob = mote->phase;
            u32 a;
            u32 b;
            s32 sx;
            s32 sy;

            if (GameFlag_TestFar(358)) {
                mote->phase++;
                mote->rise--;
            }
            a = Random16();
            b = Random16();
            sx = ((mote->x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
            sy = ((mote->z - mote->height - camera_y) >> 16) - (u16)bob;
            if ((u32)(sx + 16) <= 255 && sy >= -32 && sy <= 159) {
                if (mote->phase < 60) {
                    mote->tile = work->vram + 16;
                    mote->rise += 3;
                } else if (mote->phase < 90) {
                    mote->tile = work->vram + 8;
                    mote->rise += 1;
                } else {
                    mote->tile = work->vram;
                }
                if ((Data_03001e40 >> 3) & 1)
                    mote->tile += 4;
                mote->screen_x = sx;
                mote->y = sy - (mote->rise >> 2);
                mote->shape = 0;
                mote->size = 1;
                Runtime_PushSlotEntry(mote, 240);
            } else {
                mote->phase = 0;
            }
        }
        if (spawned < 8 && mote->phase == 0) {
            s32 *focus = camera->focus;
            s32 x = focus[0] + (Random16() << 8) - 0x800000;
            s32 z = focus[2] + (Random16() << 8) - 0x800000;

            mote->z = z;
            mote->x = x;
            mote->height = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
            mote->phase = 120;
            mote->rise = 0;
            spawned++;
        }
        i++;
        mote++;
    } while (i < 32);
}
