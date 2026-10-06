#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EVENT.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"
#include "TLA_EDITION.H"

#define Object_SetPartAttribute ((void (*)(struct FieldActor*,s32))Object_SetPartAttribute)

enum {
    ENTRANCE_OPENING = 99
};


const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}

s32 VinasuChojo_Idle(void)
{
    return 0;
}

struct SetupWork {
    u8 unknown_00[0x24];
    s32 (*idle)(void);
};


void VinasuChojo_RunSetupStep(void)
{
    Func_02001c7c(140, 1);
    Func_02001c84(4, 10);
    ((struct SetupWork *)Ram_HeapSlots->effect_work)->idle = VinasuChojo_Idle;
    Func_02001c9c();
    Engine_EventWait(10);
    Field_DispatchTypeHandler(1);
    Func_02001c8c();
    Func_02001c94();
}

struct AerieEventWork {
    u8 unknown_000[0x1ac];
    s32 mode;
};


#if !EDITION_INTERNATIONAL
s32 VinasuChojo_Start(void)
{
    ((struct AerieEventWork *)Ram_HeapSlots->event_work)->mode = 0x204;
    if (gPartyState.entrance == ENTRANCE_OPENING) {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Func_02000b2c();
        Func_02000d30();
        Engine_EventEnd();
    } else {
        Object_SetPartAttribute(Engine_ActorGet(10), 6);
        Object_SetPartAttribute(Engine_ActorGet(17), 6);
        Object_SetPartAttribute(Engine_ActorGet(18), 6);
        Object_SetPartAttribute(Engine_ActorGet(19), 6);
        Object_SetPartAttribute(Engine_ActorGet(20), 6);
        Object_SetPartAttribute(Engine_ActorGet(21), 6);
        Object_SetPartAttribute(Engine_ActorGet(22), 6);
        Object_SetPartAttribute(Engine_ActorGet(23), 6);
        Object_SetPartAttribute(Engine_ActorGet(24), 6);
        Object_SetPartAttribute(Engine_ActorGet(25), 6);
    }
    return 0;
}
#endif
