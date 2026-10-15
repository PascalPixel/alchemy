#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "text/msg_ids.h"

void Func_02001e00();
void Func_02001df8();
void Func_02001e70();
void Func_02001eb0();
void Func_02001f08();
void GameFlag_SetBit();
void Func_02000724();
struct FieldActor *Object_GetById();
void PartyInventory_Remove();
void UiText_ShowPositionedMessageAndWait();
u8 Func_02001ef0(s32 x, s32 z, s32 layer, s32 value);
s32 GameFlag_Test();

struct EventWorkLock {
    u8 unk000[0x15a];
    u16 locked;
};

static inline void MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep the native registers */
    Func_02001e00(a, b, c, d, e, f);
}

static inline void MapCopyAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep the native registers */
    Func_02001df8(a, b, c, d, e, f);
}

static inline void SetCollisionCode(s32 x, s32 z, s32 layer, s32 value)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants (direct: differ 12) */
    Func_02001ef0(x, z, layer, value);
}

void Func_02000054(void)
{
    MapCopy(17, 10, 1, 4, 19, 10);
    MapCopyAttr(17, 10, 1, 1, 19, 10);
    SetCollisionCode(0x1300000, 0xa00000, 0, 254);
    Func_02001e70(14, 0x1f80000, 0xa80000);
    if (GameFlag_Test(0xa00)) {
        Func_02000724(14);
        SetCollisionCode(0x1f00000, 0xa00000, 0, 254);
    }
}


void Func_020000c0(void)
{
    Func_02001e00(16, 10, 1, 4, 19, 10);
    Func_02001df8(16, 10, 1, 1, 19, 10);
    Func_02001e70(14, 0, 0);
    Func_02001df8(30, 10, 1, 1, 31, 10);
}

void Func_02000108(s32 unused, s32 id)
{
    ObjectMotion_SetActionVariant(id, 2);
}

void Func_02000114(s32 unused, s32 id)
{
    struct FieldActor *object;
    s32 i;

    object = Object_GetById(id);
    if (object->x >> 20 == 10 && object->z >> 20 == 32) {
        object->motionFlags = 0;
        for (i = 15; i >= 0; i--) {
            object->y += 0x20000;
            WaitFrames(1);
        }
        Func_02000724(id);
        GameFlag_SetBit(0xa06);
    }
}

void Func_02000164(void)
{
    Func_02001eb0(12);
}

void Func_02000170(void)
{
    Func_02001eb0(13);
}

void Func_0200017c(void)
{
    MapCopy(73, 18, 1, 4, 74, 18);
    MapCopyAttr(10, 9, 1, 1, 10, 20);
}

void Func_020001ac(void)
{
    MapCopy(103, 18, 1, 4, 104, 18);
    MapCopyAttr(40, 9, 1, 1, 40, 20);
}

void Func_020001dc(s32 item)
{
    GameFlag_SetBit(0xa07);
    Func_02001f08(134);
    Func_0200017c();
    PartyInventory_Remove(item);
}

void Func_020001fc(s32 item)
{
    GameFlag_SetBit(0xa08);
    Func_02001f08(134);
    Func_020001ac();
    PartyInventory_Remove(item);
}

void Func_0200021c(void)
{
    ((struct EventWorkLock *)Ram_HeapSlots->eventWork)->locked = 1;
    UiText_ShowPositionedMessageAndWait(MsgFieldDoorLocked, 1);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}
