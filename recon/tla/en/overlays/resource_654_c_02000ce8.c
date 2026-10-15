/* Near miss, differ 4 of 142, from the listing: the leader and map layer 1
 * rise by 0x8000 a frame for 100 frames. The loop is a goto loop: as a for,
 * while or do loop, loop.c hoists the 0x8000 the native rebuilds each pass
 * (differ 64). Left: the native loads gPartyState's address before it adds
 * the layer offset to the map (ldr r5 before adds r6), and in the loop it
 * increments the counter before it loads the layer's y. alchemy permute for
 * 3 minutes found nothing closer. */
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"
#include "call.h"

void Func_02003988(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Event_StartScreenTransition(void);

void Func_02000ce8(void)
{
    struct ShrineOfTheSeaGodMapWork *map = (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
    struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[1];
    struct ShrineOfTheSeaGodObject *leader = Object_GetById(gPartyState.currentOwner);
    u32 i;

    Func_020039d8();
    Func_02003b08(0);
    Func_02003b48(244);
    ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
    ShrineOfTheSeaGod_CamMove(172 << 17, -1, 248 << 16, 0);
    ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
    WaitFrames(1);
    ObjectMotion_SetActionVariant(gPartyState.currentOwner, 2);
    leader->motionFlags = 0;
    leader->y = -50 << 16;
    leader->height = -50 << 16;
    layer->y += -50 << 16;
    Func_02003988();
    WaitFrames(1);
    ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 515;
    Event_StartScreenTransition();
    Call3((void (*)())Func_020039b8, 0x10000, 0x10000, 0x10000);
    i = 0;
loop:
        leader->y += 0x8000;
        leader->height += 0x8000;
        layer->y += 0x8000;
        WaitFrames(2);
        i++;
    if (i <= 99)
        goto loop;
    Func_02003b48(288);
    Func_02003b48(390);
    Call3((void (*)())Func_020039b8, 0, 0, 0);
    ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
    ShrineOfTheSeaGod_CopyRect(62, 69, 62, 18, 19, 6);
    WaitFrames(1);
    Func_020039e0();
}
