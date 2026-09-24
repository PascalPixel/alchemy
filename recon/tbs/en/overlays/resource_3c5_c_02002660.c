#include "TYPES.H"

extern s32 Data_0200afd4[];

s32 Main_080091a8();
s32 StagedActor_FindAtTile();

/* NONMATCHING: 72 of 72 bytes, 6 halfword edits (2026-09-24). The heading
 * step and the z sum swap r0 and r2 against the reference. */
s32 Func_02002660(u8 *actor)
{
    s32 pos[3];
    s32 *p = pos;
    s32 step = Data_0200afd4[*(u16 *)(actor + 6) >> 12];

    {
        s32 x = *(s32 *)(actor + 8);
        s32 z = *(s32 *)(actor + 16);

        x += -0x10000 & step;
        z += step << 16;
        p[0] = x;
        p[2] = z;
    }
    p[1] = Main_080091a8(actor[34]);
    return StagedActor_FindAtTile(p, actor);
}
