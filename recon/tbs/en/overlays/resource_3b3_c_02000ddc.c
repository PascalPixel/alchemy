/* NONMATCHING: 236 of 236 bytes, 5 differing halfwords (2026-09-24).
 * Hand-written: finds the actor's sprite kind in the six-entry table at
 * 0x0200ace0 (7 when absent), then fills the probe with the actor's position
 * moved by that entry's step, the step's extents and the map's scroll cell.
 * Binds Engine_ActorGet (0x0200ab1c) and the three data symbols. Remaining:
 * the z step's add; written z + step, local-alloc gives the step load r2 and
 * z r3 (the reference has them the other way round), written step + z, the
 * sum lands in the step's register. */
#include "TYPES.H"

u8 *Engine_ActorGet(s32 actor);

struct Probe {
    u32 index;
    u8 unknown_04[4];
    s32 x;
    s32 y;
    s32 z;
};

struct ProbeStep {
    s32 dx;
    s32 dz;
    s32 ex;
    s32 ez;
};

extern u8 *Data_03001e70;
extern s32 Data_0200ace0[];
extern struct ProbeStep Data_0200acf8[];

s32 Func_02000ddc(s32 id, s32 *width, s32 *depth, struct Probe *probe, s32 *left, s32 *top)
{
    u8 *map = Data_03001e70;
    u8 *actor = Engine_ActorGet(id);
    u32 i;
    s32 a;
    s32 b;

    i = 0;
    if (*(s16 *)*(s32 *)(*(s32 *)(actor + 80) + 40) != Data_0200ace0[i]) {
    miss:
        probe->index = 7;
        if (++i > 5) {
            goto check;
        }
        if (*(s16 *)*(s32 *)(*(s32 *)(actor + 80) + 40) != Data_0200ace0[i]) {
            goto miss;
        }
    }
    probe->index = i;
check:
    if (probe->index > 6) {
        return 0;
    }
    probe->x = *(s32 *)(actor + 8);
    probe->y = *(s32 *)(actor + 12);
    probe->z = *(s32 *)(actor + 16);
    a = Data_0200acf8[probe->index].dz;
    if (a < 0) {
        a = -a;
    }
    b = Data_0200acf8[probe->index].ez;
    if (b < 0) {
        b = -b;
    }
    *depth = (a + b) >> 4;
    a = Data_0200acf8[probe->index].dx;
    if (a < 0) {
        a = -a;
    }
    b = Data_0200acf8[probe->index].ex;
    if (b < 0) {
        b = -b;
    }
    *width = (a + b) >> 4;
    probe->x += Data_0200acf8[probe->index].dx << 16;
    probe->z = ((Data_0200acf8[probe->index].dz << 16) + probe->z) >> 20;
    probe->x >>= 20;
    *left = *(s32 *)(map + 0x13c) >> 20;
    *top = *(s32 *)(map + 0x140) >> 20;
    return 1;
}
