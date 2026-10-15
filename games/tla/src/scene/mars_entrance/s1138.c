#include "room.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 Data_0200503c[];
extern const u8 Data_0200547c[];
extern const u8 Data_020054cc[];
extern const u8 Data_02005528[];
extern const u8 Data_02005578[];
extern const u8 Data_020055d4[];
extern const u8 Data_02005644[];
extern const u8 Data_0200610c[];
extern const u8 Data_02006118[];
extern const u8 Data_0200613c[];
extern const u8 Data_02006250[];
extern const u8 Data_020063b8[];
extern const u8 Data_020064fc[];
extern const u8 Data_02006628[];
extern const u8 Data_02006778[];
extern s16 Data_02006868;
extern s16 Data_020068a0;
extern u16 Data_020068a2;
extern u8 gMapCellBuffer[];

void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Func_02000d18(void);
void Func_02000fe0(u8 *cell);
void Func_020012e4(struct ObjectRuntime *object);
void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

void Func_02001138(void)
{
    struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->map_work;
    struct PartyState *party = &gPartyState;
    struct ObjectRuntime *object;
    s32 x;
    s32 z;
    u8 *cell;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
    do {
    } while (0);
    object = Object_GetById(party->current_owner);
    x = object->x >> 20;
    z = object->z >> 20;
    cell = gMapCellBuffer;
    if (map != NULL)
        cell = map->layers[object->terrain_id & 3].cells;
    cell += (x + (z << 7)) << 2;
    if (cell[3] & 64)
        Func_02000fe0(cell + 0x200);
}

const u8 *Func_0200119c(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom1)
        return Data_02006118;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom2)
        return Data_0200613c;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom3)
        return Data_02006250;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom4)
        return Data_020063b8;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom5)
        return Data_020064fc;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom6)
        return Data_02006628;
    if (gPartyState.scene == (s32)&SceneId_Ov6aaRoom7)
        return Data_02006778;
    return Data_0200610c;
}
