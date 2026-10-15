#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_02001698[];
extern const u8 gData_02001728[];

void Func_020012c4(s32, s32);

struct TileSprite
{
    u8  unk00[18];
    u16 tile;
    u8  unk14[6];
    u8  frame;
};

s32 Func_02000038(struct ObjectRuntime *object)
{
    u16               *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
    struct TileSprite *sprite = object->animation;

    sprite->tile = *tile;
    sprite->frame = 0;
    return 1;
}

void Func_02000058(void)
{
    Func_020012c4(21, 49);
}

const u8 *Func_02000064(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov66bRoom2)
        return gData_02001728;
    return gData_02001698;
}
