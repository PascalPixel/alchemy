#include "types.h"

struct EffectState_08096fb0 {
    u8 unknown_000[0x1C];
    u16 action_id;
    s16 action_type;
    u8 state_20;
    u8 variant;
    u8 state_22;
    u8 state_23;
    u8 unknown_024[0x21];
    u8 special_scene;
    s16 resource_handle;
    u8 unknown_048[2];
    u16 remaining_slots;
    s32 saved_position[3];
    u8 unknown_058[0x6C4];
    u8 message_state;
    u8 unknown_71D[3];
};

struct BattleState_08096fb0 {
    u8 unknown_000[0xCC0];
    s16 transition_state;
    u8 unknown_CC2[4];
    s8 effect_mode;
};

struct ViewState_08096fb0 {
    s32 unknown_00;
    s32 position[3];
};

struct SharedState_08096fb0 {
    u8 unknown_000[0x1DA];
    s16 scene_id;
    u8 unknown_1DC[0x18];
    s32 selected_object;
};

struct ActionInfo_08096fb0 {
    u8 unknown_00[12];
    u8 type;
};

struct Dma3_08096fb0 {
    const void *source;
    void *destination;
    u32 control;
};

extern struct BattleState_08096fb0 *Data_03001ebc;
extern struct ViewState_08096fb0 *Data_03001e70;
extern struct EffectState_08096fb0 *Data_03001f30;
extern struct SharedState_08096fb0 Data_02000240;

struct EffectState_08096fb0 *Func_080048b0(s32, s32);
s32 Func_08003fa4(s32, s32, const void *);
s32 Func_08004080(void);
s32 Func_080041d8(void (*)(void), s32);
struct ActionInfo_08096fb0 *Func_08077080(s32);
s32 Func_08096c24(void);
void Func_08096f8c(void);
void Func_080970f8(s32, s32);

/*
 * Select the active effect record. Zero mode allocates and initializes a
 * fresh record; nonzero mode only replaces the action metadata.
 */
void Func_08096fb0(s32 action_id, s32 variant)
{
    struct BattleState_08096fb0 *battle = Data_03001ebc;
    struct EffectState_08096fb0 *effect;
    s32 mode;

    mode = battle->effect_mode;
    if (mode == 0) {
        volatile struct Dma3_08096fb0 *dma =
            (volatile struct Dma3_08096fb0 *)0x040000D4;
        s32 zero;

        effect = Func_080048b0(0x38, 0x720);
        zero = 0;
        dma->source = &zero;
        dma->destination = effect;
        dma->control = 0x850001C8;
    } else {
        effect = Data_03001f30;
    }

    effect->action_id = (u16)action_id;
    effect->action_type = Func_08077080(action_id)->type;

    mode = battle->effect_mode;
    if (mode == 0) {
        struct ViewState_08096fb0 *view = Data_03001e70;
        s16 resource_handle;
        s16 scene_id;

        effect->remaining_slots = (u16)(0x200 - Func_08096c24());
        effect->state_20 = 1;
        effect->variant = (u8)variant;
        effect->state_22 = 1;
        effect->state_23 = 1;
        effect->message_state = 1;
        effect->saved_position[0] = view->position[0];
        effect->saved_position[1] = view->position[1];
        effect->saved_position[2] = view->position[2];

        scene_id = Data_02000240.scene_id;
        if (scene_id == 0x35 || scene_id == 0x37) {
            effect->special_scene = 1;
        }

        Func_080970f8(Data_02000240.selected_object, -1);
        if (effect->action_type != 8) {
            battle->transition_state = (s16)mode;
        }

        resource_handle = (s16)Func_08004080();
        effect->resource_handle = resource_handle;
        Func_08003fa4(resource_handle, 0x100, (const void *)0x0809C410);
        Func_080041d8(Func_08096f8c, 0xC80);
    }
}
