#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Func_0200220c(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_02002214(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_0200227c(s32 id, s32 x, s32 z);
void Func_02002304(s32 x, s32 z, s32 a2, s32 a3);
void Func_0200232c(s32 cue);

static inline void CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each stack argument in its own register (differ 10 as direct calls). */
    Func_02002214(src_x, src_y, width, height, dest_x, dest_y);
}

// Actor id comes down: once it rests on the terrain (or after 30 frames) cue
// 188 plays, and at cell (20, 39) it sets flag 0xa02 and copies map cells in
// three steps, each with cue 167.
void Func_02000500(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    struct PartyState *party;
    s32 i;

    object->motion_flags = 3;
    Object_GetById(id)->priority_flags |= 1;
    /* FAKEMATCH: the one-pass block loads gPartyState's address before the owner's offset is built (one reordered row without it). */
    do {
        party = &gPartyState;
    } while (0);
    Object_GetById(party->current_owner)->priority_flags |= 1;
    if (object->x >> 20 == 20 && object->z >> 20 == 39)
        ObjectMotion_SetActionVariant(id, 3);
    /* FAKEMATCH: the memory barrier reads the position again for the terrain height, as the native does (differ 18 without). */
    asm("" ::: "memory");
    if (object->terrain_height != Map_GetTerrainHeight(object->terrain_id, object->x, object->z)) {
        WaitFrames(2);
        for (i = 0; i <= 29 && (object->y > object->terrain_height || object->velocity_y != 0); i++)
            WaitFrames(1);
        Func_0200232c(188);
    }
    if (object->x >> 20 == 20 && object->z >> 20 == 39) {
        GameFlag_SetBit(0xa02);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0);
        ObjectMotion_SetActionVariant(id, 3);
        Func_0200227c(id, 0, 0);
        Func_0200220c(28, 39, 1, 1, 20, 39);
        CopyCells(28, 39, 1, 1, 20, 39);
        CopyCells(92, 39, 1, 1, 84, 39);
        Battle_WaitMode0(30);
        Func_0200232c(167);
        CopyCells(92, 40, 1, 1, 84, 40);
        Battle_WaitMode0(20);
        Func_0200232c(167);
        CopyCells(92, 41, 1, 1, 84, 41);
        Battle_WaitMode0(20);
        Func_0200232c(167);
        CopyCells(93, 41, 1, 1, 85, 41);
        Func_02002304(168 << 17, 164 << 18, 2, 236);
    }
}
