#include "layout_guard.h"
#include "types.h"

typedef struct EffectController_08099838 {
    u8 padding00[5];
    u8 state;
} EffectController_08099838;

typedef struct DisplayEffect_08099838 {
    u8 padding00[0x25];
    u8 enabled;
    u8 phase;
    u8 padding27;
    EffectController_08099838 *controller;
} DisplayEffect_08099838;

typedef struct SceneObject_08099838 {
    u8 padding00[0x50];
    DisplayEffect_08099838 *effect;
    u8 padding54[0x18];
    void (*callback)(void);
} SceneObject_08099838;

typedef struct Runtime_08099838 {
    u8 padding000[0x17e];
    s16 field_17e;
} Runtime_08099838;

typedef struct GlobalState_08099838 {
    u8 padding000[0x1f4];
    u32 selected_object;
    u8 padding1f8[0x54];
    u16 callback_active;
} GlobalState_08099838;

LAYOUT_OFFSET_GUARD(
    EffectController08099838_State,
    EffectController_08099838,
    state,
    5);
LAYOUT_OFFSET_GUARD(
    DisplayEffect08099838_Enabled,
    DisplayEffect_08099838,
    enabled,
    0x25);
LAYOUT_OFFSET_GUARD(
    DisplayEffect08099838_Controller,
    DisplayEffect_08099838,
    controller,
    0x28);
LAYOUT_OFFSET_GUARD(
    SceneObject08099838_Effect,
    SceneObject_08099838,
    effect,
    0x50);
LAYOUT_OFFSET_GUARD(
    SceneObject08099838_Callback,
    SceneObject_08099838,
    callback,
    0x6c);
LAYOUT_OFFSET_GUARD(
    Runtime08099838_Field17e,
    Runtime_08099838,
    field_17e,
    0x17e);
LAYOUT_OFFSET_GUARD(
    GlobalState08099838_CallbackActive,
    GlobalState_08099838,
    callback_active,
    0x24c);

extern Runtime_08099838 *Data_03001ebc;
extern GlobalState_08099838 Data_02000240;

void Func_080030f8(s32 frames);
void Func_080041d8(void (*callback)(void), s32 period);
void Func_08009080(SceneObject_08099838 *object, s32 mode);
SceneObject_08099838 *Func_0808ba1c(u32 object_id);
void Func_08099678(void);
void Func_08099738(void);
void Func_080f9010(s32 sound);

/* Blink the selected object, then start and immediately sample its watcher. */
void Func_08099838(void)
{
    Runtime_08099838 *runtime = Data_03001ebc;
    SceneObject_08099838 *object =
        Func_0808ba1c(Data_02000240.selected_object);
    DisplayEffect_08099838 *effect = object->effect;
    EffectController_08099838 *controller = effect->controller;
    void (*watcher)(void) = Func_08099678;
    s32 cycle;

    Func_080f9010(0x82);
    Func_08009080(object, 0);
    object->callback = 0;

    for (cycle = 0; cycle < 10; cycle++) {
        controller->state = 7;
        effect->enabled = 1;
        effect->phase = 2;
        Func_080030f8(2);
        effect->enabled = 1;
        effect->phase = 0;
        Func_080030f8(2);
    }

    controller->state = 0;
    effect->phase = 2;
    effect->enabled = 1;
    Func_080041d8(watcher, 0xc80);
    Data_02000240.callback_active = 1;

    /* The original reaches this indirect call through __call_via_r5. */
    watcher();
    if (runtime->field_17e == 0x2092) {
        Func_08099738();
        runtime->field_17e = 0;
    }
}
