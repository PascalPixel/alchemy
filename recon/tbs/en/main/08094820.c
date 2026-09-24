/* Draft, not exact (2026-09-24): candidate=392 reference=392 differing_halfwords=166.
   The body's instruction sequence matches; the difference is allocation. The
   reference keeps the shape/size mask 63 in fp across the loop (view in sl,
   work in r8, the counter in r9) and spills the new x to sp+0 around the
   second Random16; here 63 is rematerialised and the counter takes fp.
   Dust particles: 32 sprites drift around the leader, each re-seeded at a
   random spot on the ground when its 16-frame timer runs out. */
#include "TYPES.H"

struct DustParticle {
    u8 pad0[4];
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
    u8 pad0a[2];
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    u8 pad18[4];
    u16 timer;
    u8 pad1e[2];
};

struct DustWork {
    u8 pad0[4];
    s32 tile_base;
    struct DustParticle particles[32];
};

struct FieldView {
    s32 *leader;
    u8 pad4[0xe4 - 4];
    s32 camera[2];
};

extern void *Data_03001e70[];
extern s16 Data_0809ef84[];
s32 GameFlag_TestFar(s32 flag);
void Runtime_PushSlotEntry(void *entry, s32 value);
s32 Random16(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void Func_08094820(void)
{
    struct FieldView *view = Data_03001e70[0];
    struct DustWork *work = Data_03001e70[21];
    s32 *camera = view->camera;
    s32 camera_x = camera[0];
    s32 camera_z = camera[1];
    struct DustParticle *p = work->particles;
    u32 i;

    for (i = 0; i < 32; i++, p++) {
        s16 *frame;
        s32 x;
        s32 y;

        if (--p->timer == 0xffff)
            continue;
        if (GameFlag_TestFar(0x166))
            p->timer++;
        frame = &Data_0809ef84[p->timer * 5];
        x = (p->pos_x - camera_x) / 0x10000 + *frame++;
        y = (p->pos_z - p->pos_y - camera_z) / 0x10000 + *frame++;
        if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
            p->priority = 1;
            p->x = x;
            p->y = y;
            p->tile = work->tile_base + *(u16 *)frame;
            frame++;
            p->shape = *(u8 *)frame;
            p->size = *(u8 *)(frame + 1);
            Runtime_PushSlotEntry(p, 240);
        }
        if (p->timer == 0) {
            s32 *leader = view->leader;
            s32 nx = leader[0] + (Random16() << 8) - 0x800000;
            s32 nz = leader[2] + (Random16() << 8) - 0x800000;

            p->pos_x = nx;
            p->pos_z = nz;
            p->pos_y = Map_GetTerrainHeightFar(0, nx >> 16, nz >> 16) << 16;
            p->timer = 16;
        }
    }
}
