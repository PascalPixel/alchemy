#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EVENT.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"
#include "TLA_EDITION.H"

/* The opening's cast arrives at the aerie through this entrance. */
enum {
    ENTRANCE_OPENING = 99
};

extern const struct SceneEvent gSceneEvents[];

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSceneEvents;
}

/* The idle hook the setup step installs: nothing to do. */
s32 VinasuChojo_Idle(void)
{
    return 0;
}

/* The effect work in its heap slot keeps the scene's idle hook. */
struct SetupWork {
    u8 unknown_00[0x24];
    s32 (*idle)(void);
};

void Func_02001c7c(s32, s32);
void Func_02001c84(s32, s32);
void Func_02001c8c(void);
void Func_02001c94(void);
void Func_02001c9c(void);
void Field_DispatchTypeHandler(s32);

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

/* The scene's event work. */
struct AerieEventWork {
    u8 unknown_000[0x1ac];
    s32 mode;
};

void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Func_02000b2c(void);
void Func_02000d30(void);

/* Opens the aerie: through entrance 99 the opening plays out, otherwise
   the lighthouse's ten beacon actors take part attribute 6. The
   international editions also set the saved scene to 5, entrance 22, before
   the opening; their build keeps the listing until that branch matches. */
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
