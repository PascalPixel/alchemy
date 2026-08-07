#include "types.h"

extern u8 *Func_02000350(s32 *position, u8 *movingActor);
extern s32 Func_080091d8(u8 *actor, s32 *position);

static void ClearMotion_0200058c(u8 *actor)
{
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}

/*
 * Probe the tile immediately ahead of an actor.  Ordinary occupants are
 * allowed only when their type appears in this scene's six-entry pass list;
 * the terrain probe is still authoritative.  A rejected move cancels both
 * velocity pairs and restores the two inactive-axis sentinels.
 *
 * Complete owner: 0x0200058c through the return at 0x02000622, followed by
 * its packed-step, mask, and pass-list pool through 0x0200062f.
 */
s32 Func_0200058c(u8 *actor)
{
    const s32 *steps = (const s32 *)0x0200a700;
    const s32 *passableTypes = (const s32 *)0x0200a740;
    s32 position[3];
    s32 packed = steps[*(u16 *)(actor + 6) >> 12];
    u8 *occupant;
    s16 type;
    u32 i;

    occupant = Func_02000350(position, actor);
    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    position[1] = *(s32 *)(actor + 12);
    position[2] = *(s32 *)(actor + 16) + (s32)((unsigned int)packed << 16);
    if (occupant != 0) {
        type = *(s16 *)(*(u8 **)(occupant + 80) + 40);
        for (i = 0; i < 6; i++) {
            if (type == passableTypes[i])
                return 0;
        }
        ClearMotion_0200058c(actor);
    }

    position[1] = *(s32 *)(actor + 12);
    position[2] = *(s32 *)(actor + 16) + (s32)((unsigned int)packed << 16);
    position[0] = *(s32 *)(actor + 8) + (packed & (s32)0xffff0000);
    if (Func_080091d8(actor, position) > 0)
        ClearMotion_0200058c(actor);
    return 0;
}
