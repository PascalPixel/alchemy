#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_0808bb2c {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_0808bb2c;

typedef struct ObjectDisplay_0808bb2c {
    u8 padding00[9];
    u8 first_mode;
    u8 padding0a[0x0b];
    u8 second_mode;
} ObjectDisplay_0808bb2c;

typedef struct SceneObject_0808bb2c {
    u8 padding00[0x0c];
    s32 y;
    u8 padding10[0x40];
    ObjectDisplay_0808bb2c *display;
    u8 padding54[0x1c];
} SceneObject_0808bb2c;

typedef struct CameraTarget_0808bb2c {
    u8 padding00[0x0c];
    s32 y;
    u8 padding10[4];
    s32 target_y;
} CameraTarget_0808bb2c;

typedef struct BattleRuntime_0808bb2c {
    u8 padding000[0x1e0];
    CameraTarget_0808bb2c *camera_target;
} BattleRuntime_0808bb2c;

typedef struct ViewPosition_0808bb2c {
    s32 x;
    s32 y;
    s32 z;
} ViewPosition_0808bb2c;

typedef struct ViewState_0808bb2c {
    ViewPosition_0808bb2c *position;
} ViewState_0808bb2c;

typedef struct SavedObjects_0808bb2c {
    u8 ids[32];
    u8 snapshots[32][0x70];
    u8 animations[32];
    u8 display_modes[32];
    u8 oam_modes[32];
} SavedObjects_0808bb2c;

LAYOUT_OFFSET_GUARD(
    ObjectDisplay0808bb2c_FirstMode,
    ObjectDisplay_0808bb2c,
    first_mode,
    9);
LAYOUT_OFFSET_GUARD(
    ObjectDisplay0808bb2c_SecondMode,
    ObjectDisplay_0808bb2c,
    second_mode,
    0x15);
LAYOUT_OFFSET_GUARD(
    SceneObject0808bb2c_Y,
    SceneObject_0808bb2c,
    y,
    0x0c);
LAYOUT_OFFSET_GUARD(
    SceneObject0808bb2c_Display,
    SceneObject_0808bb2c,
    display,
    0x50);
LAYOUT_SIZE_GUARD(
    SceneObject0808bb2c_Size,
    SceneObject_0808bb2c,
    0x70);
LAYOUT_OFFSET_GUARD(
    BattleRuntime0808bb2c_CameraTarget,
    BattleRuntime_0808bb2c,
    camera_target,
    0x1e0);
LAYOUT_OFFSET_GUARD(
    SavedObjects0808bb2c_Snapshots,
    SavedObjects_0808bb2c,
    snapshots,
    0x20);
LAYOUT_OFFSET_GUARD(
    SavedObjects0808bb2c_Animations,
    SavedObjects_0808bb2c,
    animations,
    0xe20);
LAYOUT_OFFSET_GUARD(
    SavedObjects0808bb2c_DisplayModes,
    SavedObjects_0808bb2c,
    display_modes,
    0xe40);
LAYOUT_OFFSET_GUARD(
    SavedObjects0808bb2c_OamModes,
    SavedObjects_0808bb2c,
    oam_modes,
    0xe60);

extern u32 Data_02000434;
extern BattleRuntime_0808bb2c *Data_03001ebc;
extern ViewState_0808bb2c *Data_03001e70;

void Func_08009080(SceneObject_0808bb2c *object, s32 animation);
void Func_08009140(SceneObject_0808bb2c *object);
void Func_080091e0(SceneObject_0808bb2c *object, s32 mode);
SceneObject_0808bb2c *Func_0808ba1c(u32 id);

/* Restore up to 32 packed scene-object snapshots and their live displays. */
void Func_0808bb2c(void)
{
    volatile DmaChannel_0808bb2c *dma =
        (volatile DmaChannel_0808bb2c *)0x040000d4;
    SavedObjects_0808bb2c *saved =
        (SavedObjects_0808bb2c *)0x02001104;
    s32 slot;

    for (slot = 0; slot < 32 && saved->ids[slot] != 0xff; slot++) {
        u8 id = saved->ids[slot];
        SceneObject_0808bb2c *object = Func_0808ba1c(id);

        if (object != 0) {
            ObjectDisplay_0808bb2c *display = object->display;
            u8 mode = (saved->oam_modes[slot] & 3) << 2;

            dma->source = saved->snapshots[slot];
            dma->destination = object;
            dma->control = 0x8400001c;

            if (saved->animations[slot] != 0)
                Func_08009080(object, saved->animations[slot]);
            Func_080091e0(object, saved->display_modes[slot]);

            display->first_mode =
                (display->first_mode & (u8)~0x0c) | mode;
            display->second_mode =
                (display->second_mode & (u8)~0x0c) | mode;
            object->display = display;

            if (id == Data_02000434) {
                CameraTarget_0808bb2c *target =
                    Data_03001ebc->camera_target;

                target->target_y = object->y;
                target->y = object->y;
                Data_03001e70->position->y = object->y;
                Func_08009140(object);
            }
        }
    }
}
