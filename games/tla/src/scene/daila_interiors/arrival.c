#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_call.h"
#include "party_state.h"
#include "scene_ids.h"
#include "daila.h"

void Func_02000d9c(void);
void Func_02000ca4(s32 mode);

struct DailaStartActor {
    u8 unk00[0x18];
    s32 speed18;
    s32 speed1c;
    u8 unk20[3];
    u8 priorityFlags;
    u8 unk24[0x35];
    u8 collisionFlags;
};

struct DailaMapWork {
    u8 unk000[0x17];
    u8 low17 : 2;
    u8 mode17 : 2;
    u8 high17 : 4;
};

extern const u8 gData_02000e04[];

struct DailaStartActor *Object_GetById(s32 id);
void Object_SetModeById(s32 actor, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct DailaStartActor *object, s32 value);
void Func_02000c58(void);
void Func_02000cbc(struct DailaStartActor *object, s32 item);
void Func_02000dac(s32 mode);
void Func_02000dc4(const void *script);

s32 DailaInteriors_Start(void)
{
    struct DailaMapWork *map;

    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    if (gPartyState.scene == (s32)&SceneId_DailaInteriors1) {
        if (gPartyState.entrance == 7) {
            Func_02000c58();
        } else {
            if (GameFlag_Test(0x873)) {
                Object_SetModeById(24, 5);
                ObjectDispatch_SetSingleChildFlags(Object_GetById(24), 0);
                Object_SetModeById(25, 5);
                ObjectDispatch_SetSingleChildFlags(Object_GetById(25), 0);
            }
            Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
            Object_GetById(13)->priorityFlags |= 32;
            Object_GetById(19)->priorityFlags |= 32;
            Engine_ActorSetSpritePriority(19, 2);
            Object_GetById(19)->collisionFlags |= 4;
        }
    } else if (gPartyState.scene == (s32)&SceneId_DailaInteriors2) {
        map = (struct DailaMapWork *)Ram_HeapSlots->mapWork;
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 0);
        Func_02000dac(0);
        if (!GameFlag_Test(0xa20)) {
            Func_02000cbc(Object_GetById(9), 458);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
            Engine_ActorSetSpritePriority(9, 1);
            Object_GetById(9)->speed1c = 0x13333;
            Object_GetById(9)->speed18 = 0x13333;
            Object_GetById(9)->collisionFlags |= 4;
            Object_GetById(13)->collisionFlags |= 4;
        } else {
            Object_GetById(9)->collisionFlags |= 4;
            ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
        }
        if (!GameFlag_Test(0x873)) {
            Func_02000d4c(10, 0, 0);
            Func_02000d4c(11, 0, 0);
            Func_02000d4c(12, 0, 0);
            map->mode17 = 1;
        } else {
            Func_02000dc4(gData_02000e04);
        }
        Engine_EventEnd();
    }
    return 0;
}

s32 Scene_PrepareMap(void)
{
    if (gPartyState.scene == (s32)&SceneId_DailaInteriors2 && GameFlag_Test(0x873)) {
        Func_02000ca4(0);
        Daila_CopyCells(29, 33, 10, 6, 10, 12);
    }
    return 0;
}

void Func_02000c58(void)
{
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x209;
    GameFlag_ClearBit(0x12f);
}

void Func_02000c74(void)
{
    Func_02000d9c();
}
