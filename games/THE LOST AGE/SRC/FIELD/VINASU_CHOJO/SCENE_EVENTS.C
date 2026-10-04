#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "FIELD_EVENT.H"
#include "RAM_BUFFER.H"

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
