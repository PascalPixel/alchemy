#include "scene.h"
#include "effect_0809b11c.h"
#include "types.h"
#include "object_lookup.h"
#include "global_cells.h"

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
extern void UiWindow_Close(u32 arg0, u32 arg1);

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
s32 WaitFrames(s32);

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
