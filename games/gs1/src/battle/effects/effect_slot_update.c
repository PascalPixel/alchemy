#include "scene.h"
#include "effect_0809b11c.h"
#include "types.h"
#include "object_lookup.h"
#include "global_cells.h"
#include "far_runtime.h"

/* battle/effects/effect_slot_update.c */
/* battle/effects/runtime/upd/update_slot.c */
void EffectSlot_Update(struct EffectSlot *effect)
{
    if (effect->active != 0) {
        effect->age++;
        if (effect->callback_delay != 0)
            effect->callback_delay--;
        else if (effect->callback != 0)
            effect->callback(effect);
        if (effect->active != 0) {
            if (effect->update_motion != 0)
                Battle_Run(effect);
            if (effect->render != 0)
                Battle_unk2_4(effect);
        }
    }
}

/* battle/effects/runtime/misc/has_reached_target.c */
u32 EffectSlot_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->flag41 == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}

/* battle/effects/runtime/misc/set_position.c */
void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->target_x = EFFECT_NO_TARGET;
    effect->target_z = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}

/* battle/effects/runtime/misc/set_object_mode.c */
void Object_InitializeMode(void *, s32);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    Object_InitializeMode(effect->object, mode);
}

/* battle/effects/runtime/misc/set_callback.c */
void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}

/* battle/effects/misc/cleanup_resources_and_window.c */
extern u8 gUnk[];

extern s32 Resource_ResetEntry(u32 index);
void UiWindow_Close(void *handle);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(*(u16 *)(gUnk + 0));
    Resource_ResetEntry(*(u16 *)(gUnk + 2));
    UiWindow_Close(*(u32 *)(gUnk + 0x1c), 2);
}

/* battle/effects/obj/update_object_visibility_bounds.c */
s32 Object_GetById(u32);

extern s32 gCell[];
extern u8 gWork[];

void BattleFx_UpdateObjectVisibilityBounds(void)
{
    s32 object;
    s32 x;
    s32 y;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    u32 id;

    object = gCell[125];
    Object_GetById(object);
    object = *(s32 *)(*(u8 **)gWork + 480);
    x = *(s32 *)(object + 8);
    left = x + 0xFEC00000;
    right = x + 0x01400000;
    y = *(s32 *)(object + 16);
    top = y + 0xFDA80000;
    bottom = y + 0x01900000;

    id = 8;
    do {
        object = ObjectTable_Get(id);

        if (object != 0) {
            s32 ox = *(s32 *)(object + 8);
            s32 oy = *(s32 *)(object + 16);

            if (ox < left || ox > right ||
                oy < top || oy > bottom) {
                *(u8 *)(object + 84) = 0;
            } else {
                *(u8 *)(object + 84) = 1;
            }
        }
        id++;
    } while (id <= 65);
}

/* battle/effects/run/run_visibility_transition.c */
void WaitFrames(s32);

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Battle_InitializeRenderObject(void);

void BattleFx_UpdateObjectVisibilityBounds(void);

void BattleFx_RunVisibilityTransition(void)
{
  if ((*(s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Battle_Check();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    Battle_Apply(0x9D89, 6);
    if ((*((volatile u32 *) ADDR_03001AE8)) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ADDR_03001AE8)) & 0x200);
    }
    Battle_Apply(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}

/* runtime/scheduler/scheduler_disable_overlay_callbacks_with_flags.c */
/* runtime/scheduler/disable_overlay_callbacks_with_flags.c */
s32 GameFlag_Clear(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Sys_Check();
    Scheduler_EnableUnmaskedOverlayCallbacks();
    GameFlag_Clear(0x166);
    return GameFlag_Clear(0x152);
}

/* runtime/scheduler/enable_overlay_callbacks_with_flags.c */
s32 GameFlag_Set(s32 flag_no);

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_Set(0x152);
    GameFlag_Set(0x166);
    Sys_Check();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}

/* ui/window_update_or_create.c */
/* ui/window/update_or_create.c */
s32 UiWindow_CreateFar(s32 first, s32 second, s32 third, s32 fourth, s32 flags);

s32 UiWindow_UpdateOrCreate(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags)
{
    s32 current;
    s32 masked;

    current = *handle;
    if (current != 0) {
        if (0x100 & flags) {
            return 0;
        }
        UiWindow_Commit(current);
        return 0;
    }
    masked = flags & 0xff;
    *handle = UiWindow_CreateFar(first, second, third, fourth, masked);
    return 1;
}

/* ui/window/close_if_open.c */

void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL) {
        UiWindow_Close(*handle);
        *handle = NULL;
    }
}

/* ui/text/draw/draw_number_right_aligned_far.c */
s32 FixedPoint_Ratio(s32 value, s32 divisor);

void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y)
{
    s32 value = number;
    s32 digits = 1;

    while (digits <= 15) {
        value = FixedPoint_Ratio(value, 10);
        if (value <= 9) {
            break;
        }
        digits++;
    }

    digits++;
    x -= digits << 3;
    Ui_SetRange(number, digits, layer, x, y);
}

/* ui/render/draw_value_pair_rows.c */
s32 UiPalette_SetColor(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

    Ui_SetMode(0x080af210, layer, 0, 40);
    Ui_SetMode2(0x080af214, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    Ui_SetMode3(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiPalette_SetColor(4);
    }
    if (val == 0) {
        UiPalette_SetColor(2);
    }
    Ui_SetMode3(val, layer, 48, 40);
    UiPalette_SetColor(15);
    Ui_SetMode(0x080af218, layer, 0, 48);
    Ui_SetMode2(0x080af214, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    Ui_SetMode3(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    Ui_SetMode3(val, layer, 88, 48);
}

/* ui/text/draw/draw_stat_comparison.c */
/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    u32 i;
    s32 tmp2;
    s32 p;
    s32 tmp;
    s32 rec;

    p = alt;
    Ui_Run(0xb1c, work, 0, 32);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(p + 60) != *(u16 *)(base + 60)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 60), 3, work, 64, 40);
        if (*(u16 *)(p + 60) > *(u16 *)(base + 60)) {
            Ui_SpawnStatDeltaMarker(work, 44, 36, 0);
        } else {
            Ui_SpawnStatDeltaMarker(work, 44, 36, 1);
        }
    }
    Ui_Run(0xb1d, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(p + 62) != *(u16 *)(base + 62)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 62), 3, work, 64, 56);
        if (*(u16 *)(p + 62) > *(u16 *)(base + 62)) {
            Ui_SpawnStatDeltaMarker(work, 44, 52, 0);
        } else {
            Ui_SpawnStatDeltaMarker(work, 44, 52, 1);
        }
    }
    UiText_DrawAt((s32)gVal, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(p + 64) != *(u16 *)(base + 64)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(p + 64), 3, work, 64, 72);
        if (*(u16 *)(p + 64) > *(u16 *)(base + 64)) {
            Ui_SpawnStatDeltaMarker(work, 44, 68, 0);
        } else {
            Ui_SpawnStatDeltaMarker(work, 44, 68, 1);
        }
    }
    tmp = *(u16 *)(base + 64);
    tmp2 = p + 64;
}

/* ui/icon/create_with_resource.c */
extern s32 Resource_FindFreeSlot(void);
s32 Resource_CopyData(s32 entry_no, s32 mode, s32 data);
s32 UiIcon_DrawWithFlags(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
extern u8 gRom[];
s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeSlot();
    if (entry_no != 0) {
        Resource_CopyData(entry_no, 0x80, gRom);
        result = UiIcon_DrawWithFlags(entry_no, 0x40000000, first, second, third);
    }
    return result;
}

/* ui/icon/create_with_resource_variant.c */
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Resource_FindFreeSlot();
  resource_mode = 0x80;
  if (slot != 0)
  {
    Resource_CopyData(slot, copy_mode = resource_mode, gRom);
    icon = UiIcon_DrawWithFlags(slot, 0x40000000, first, second, third);
  }
  return icon;
}

/* ui/icon/prepare_object.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void UiIcon_PrepareObject(void *object)
{
    if (object != NULL) {
        FIELD_AT_OFFSET(object, s8, 5) = 1;
        ((struct UiIconObject *) object)->value_16 = FIELD_AT_OFFSET(object, u16, 6);
        FIELD_AT_OFFSET(object, s8, 0x14) = FIELD_AT_OFFSET(object, u16, 8);
        FIELD_AT_OFFSET(object, s8, 0x17) = -0x3F & FIELD_AT_OFFSET(object, s8, 0x17);
        FIELD_AT_OFFSET(object, s8, 0x15) = -4 & FIELD_AT_OFFSET(object, s8, 0x15);
    }
}
