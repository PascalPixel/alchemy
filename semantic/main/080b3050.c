#include "effect_0809b11c.h"
#include "layout_guard.h"
#include "types.h"

typedef struct DisplayObject_080b3050 {
    u8 padding_00[5];
    u8 status_05;
} DisplayObject_080b3050;

typedef struct Runtime_080b3050 {
    u8 padding_000[0x114];
    void *preview_objects_114[8];
    s16 source_x_134[8];
    s16 source_z_144[8];
    u8 padding_154[0x22c];
    DisplayObject_080b3050 *display_380;
    u8 padding_384[0x26];
    s8 sound_index_3aa;
    s8 active_selection_3ab;
    u8 padding_3ac[4];
    struct EffectSlot effects_3b0[24];
} Runtime_080b3050;

LAYOUT_OFFSET_GUARD(
    DisplayObject080b3050_Status,
    DisplayObject_080b3050,
    status_05,
    5);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_PreviewObjects,
    Runtime_080b3050,
    preview_objects_114,
    0x114);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_SourceX,
    Runtime_080b3050,
    source_x_134,
    0x134);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_SourceZ,
    Runtime_080b3050,
    source_z_144,
    0x144);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_Display,
    Runtime_080b3050,
    display_380,
    0x380);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_SoundIndex,
    Runtime_080b3050,
    sound_index_3aa,
    0x3aa);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_ActiveSelection,
    Runtime_080b3050,
    active_selection_3ab,
    0x3ab);
LAYOUT_OFFSET_GUARD(
    Runtime080b3050_Effects,
    Runtime_080b3050,
    effects_3b0,
    0x3b0);

extern Runtime_080b3050 *Data_03001f2c;
extern s8 Data_080b4ab2[];

void Func_080030f8(s32);
s32 Func_080041d8(void (*)(void), u16);
s32 Func_08004278(void (*)(void));
u32 Func_08004458(void);
void Func_08009248(void *, u32);
void Func_08009280(void *, s32);
void Func_0808a518(struct EffectSlot *, s32);
void Func_0808a520(struct EffectSlot *, EffectCallback);
void Func_0808a528(struct EffectSlot *, s32, s32, s32);
void Func_0808a530(struct EffectSlot *);
void Func_080b04c4(void);
void Func_080b0840(s32);
void Func_080b0894(void);
void Func_080b2f4c(struct EffectSlot *);
void Func_080b2ffc(void);
void Func_080f9010(s32);

/*
 * Run the selected preview's effect burst, let the registered callbacks drive
 * every slot to completion, then restore the menu display state.
 */
void Func_080b3050(s32 selection)
{
    Runtime_080b3050 *runtime = Data_03001f2c;
    DisplayObject_080b3050 *display = runtime->display_380;
    u8 saved_display_status = display->status_05;
    s32 x;
    s32 z;
    s32 index;

    runtime->active_selection_3ab = -1;
    display->status_05 = 13;
    Func_080f9010(Data_080b4ab2[runtime->sound_index_3aa]);
    Func_080b0840(0x202108);
    Func_08009280(runtime->preview_objects_114[selection], 0);
    Func_080030f8(20);
    Func_080041d8(Func_080b2ffc, 0x0c80);

    x = (s32)((u32)(s32)runtime->source_x_134[selection] << 16);
    z = (s32)((u32)(
        (u32)(s32)runtime->source_z_144[selection] << 16) - 0x0c0000);

    for (index = 0; index < 18; index++) {
        struct EffectSlot *effect = &runtime->effects_3b0[index];
        u32 variant;

        Func_0808a528(effect, 0x11c, x, z);
        Func_0808a520(effect, Func_080b2f4c);
        Func_0808a518(effect, 7);
        variant = (u32)(Func_08004458() * 7) >> 16;
        Func_08009248(effect->object, variant);
        effect->scale_x = 0xb333;
        effect->scale_y = 0xb333;
        Func_080030f8(3);
        if (index == 5)
            runtime->active_selection_3ab = selection;
    }

    Func_080b04c4();
    for (index = 0; index < 24; index++) {
        struct EffectSlot *effect = &runtime->effects_3b0[index];

        if (effect->active != 0)
            effect->state = 2;
    }

    Func_080030f8(20);
    Func_080f9010(0x7e);
    runtime->active_selection_3ab = -1;
    Func_08009248(runtime->preview_objects_114[selection], 0);
    Func_080030f8(20);

    for (index = 0; index < 24; index++) {
        struct EffectSlot *effect = &runtime->effects_3b0[index];

        if (effect->active != 0)
            Func_0808a530(effect);
    }

    Func_08004278(Func_080b2ffc);
    Func_08009280(runtime->preview_objects_114[selection], 16);
    Func_080b0894();
    Func_080030f8(30);
    display->status_05 = saved_display_status;
}
