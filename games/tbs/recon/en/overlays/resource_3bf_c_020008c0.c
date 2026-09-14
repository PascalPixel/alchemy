/* Draft C: complete owner reviewed, but linked bytes do not yet match. */
#include "STAGED_ACTOR_PROBE.H"
#include "STAGED_ACTOR_PROBE_STATE.H"

#define FieldScene_RedrawActorFootprint Func_020008c0

extern s32 *Data_03001e70;
extern s32 Data_0200df18[];
extern s32 Data_0200df30[];
extern struct StagedActor *Func_0200d650(s32);
extern void Func_0200d5d8(s32, s32, s32, s32, s32, s32);
extern s32 Func_02008244(u32, s32, s32, u32, u32, s32);

static __inline__ void Scene_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 x, s32 z, s32 width, s32 height, s32 dst_x, s32 dst_z)
{
    func(x, z, width, height, dst_x, dst_z);
}

s32 FieldScene_RedrawActorFootprint(s32 id)
{
    s32 *work = Data_03001e70;
    struct StagedActor *actor = Func_0200d650(id);
    struct StagedActorProbe probe;
    u32 idx = 0;
    s32 offset;
    s32 width;
    s32 height;
    s32 x0;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 dst_x;
    s32 dst_z;

    if (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != Data_0200df18[idx]) {
        struct StagedActorProbe *buf = &probe;

        do {
            ++idx;
            buf->footprint_index = 7;
            if (idx > 5)
                goto draw;
        } while (*STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28 != Data_0200df18[idx]);
    }
    probe.footprint_index = idx;
draw:
    if ((u32)probe.footprint_index > 6)
        return 0;
    probe.position_x = actor->x.value;
    probe.position_y = actor->y;
    probe.position_z = actor->z.value;
    offset = probe.footprint_index * 4;
    z0 = Data_0200df30[offset + 1];
    if (z0 < 0)
        z0 = -z0;
    z1 = Data_0200df30[offset + 3];
    if (z1 < 0)
        z1 = -z1;
    height = (z0 + z1) >> 4;
    x0 = Data_0200df30[offset];
    if (x0 < 0)
        x0 = -x0;
    x1 = Data_0200df30[offset + 2];
    if (x1 < 0)
        x1 = -x1;
    width = (x0 + x1) >> 4;
    probe.position_x += Data_0200df30[offset] << 16;
    probe.position_z += Data_0200df30[offset + 1] << 16;
    probe.position_x >>= 20;
    probe.position_z >>= 20;
    dst_x = work[0x13c / 4] >> 20;
    dst_z = work[0x140 / 4] >> 20;
    Scene_Call6(Func_0200d5d8, probe.position_x, probe.position_z,
        width, height, dst_x + probe.position_x, dst_z + probe.position_z);
    Func_02008244(0, probe.position_x, probe.position_z, width, height, 255);
    Func_02008244(2, probe.position_x, probe.position_z, width, height, 255);
    return 1;
}
