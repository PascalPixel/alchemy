#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "fixed_math.h"
#include "object_efx.h"
#include "effect_0809b11c.h"
#include "sound_ids.h"

extern u8 RomBytes_03001f30[];
extern u8 RomBytes_0809f0b0[];
extern u8 RomBytes_0809f0d4[];

/* battle/effects/fx_finish_scene_and_release_heap_block.c */
/* battle/effects/scene_transition/finish_and_release_heap_block.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern void ScheduleCallback(s32);

struct Outer0808e0b0 *Object_GetById(u32);
void BattleFx_PrepareBufferInterpolation(void);

extern u8 RomBytes_08097645;

void BattleFx_FinishSceneAndReleaseHeapBlock(void)
{
    void *work;

    work = *(void **)ADDR_03001EA8;
    Battle_Run();
    ScheduleCallback((s32)&RomBytes_08097645);
    Battle_Apply(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Battle_Check(0x16);
}

/* battle/effects/color/advance_hue_cycle.c */
extern u8 *gIw;

void BattleFx_AdvanceHueCycle(void)
{
    u8 *base = gIw;
    s32 out1 = 0;
    s32 out2 = 0;
    s32 out3 = 0;
    s32 offset;
    u8 *p0;
    u8 *p1;
    u8 *p2;

    Battle_SetMode((s32)(*(u16 *)(base + 0x28E)) << 16, &out1, &out2, &out3);

    offset = 0x28B;
    p0 = base + offset;
    *p0 = (u8)((out1 >> 18) + 4);
    offset += 1;
    p1 = base + offset;
    *p1 = (u8)((out2 >> 18) + 4);
    offset += 1;
    p2 = base + offset;
    *p2 = (u8)((out3 >> 18) + 4);

    *(u16 *)(base + 0x28E) += 4;

    *p0 &= 0x1F;
    *p1 &= 0x1F;
    *p2 &= 0x1F;

    if (*(u16 *)(base + 0x28E) >= 360) {
        *(u16 *)(base + 0x28E) = 0;
    }
}

/* battle/effects/color/compute_hue_channels.c */
void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum)
{
    *maximum = Battle_Place(value + 0x780000, 0, 0x1F0000);
    *center = Battle_Place(value, 0, 0x1F0000);
    *minimum = Battle_Place(value + 0xFF880000, 0, 0x1F0000);
}

/* battle/effects/object_control/set_callback_when_target_unset.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern void Object_SetCallback(struct ItemBreakFragmentObject *, const void *);
extern const u8 RomBytes_080a0128[];

struct Target_08097a54 {
    u8 unknown_00[0x38];
    s32 x;
    s32 y;
    s32 z;
};

void BattleFx_SetCallbackWhenTargetUnset(struct Target_08097a54 *target)
{
    s32 ty;
    s32 tx;

    tx = target->x;
    if (tx == 0x80000000) {
        ty = target->y;
        if ((ty == tx) && (target->z == ty)) {
            Object_SetCallback(target, (s32)RomBytes_080a0128);
        }
    }
}

/* ui/fill_bank15_palette_grey.c */
/* ui/fill_bank15_palette_grey.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ScheduleCallbackAfterFrames(s32, s32);

void Ui_FillBank15PaletteGrey(void)
{
    volatile s16 *p;

    FIELD_AT_OFFSET(*(void **)ADDR_03001E8C, s8, RENDER_MODE_OFS) = 1;
    p = (s16 *)0x050001E2;
    *p = 0x739C;
    p += 2;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    {
        s32 n = 0x480;
        ScheduleCallbackAfterFrames((s32)Ui_Run, n);
    }
}

/* ui/set_bank15_palette_and_clear_render_mode.c */

extern u8 gCell[];

void Ui_SetBank15PaletteAndClearRenderMode(void)
{
    void *work;

    work = *(void **)ADDR_03001E8C;
    ScheduleCallback((s32)Ui_Run);
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    Ui_Apply(gCell[0x205], gCell[0x206]);
    *((u8 *)work + RENDER_MODE_OFS) = 0;
}

/* battle/effects/fx_get_cycled_table_word.c */
/* battle/effects/fx_get_cycled_table_word.c */
/* battle/effects/misc/get_cycled_table_word.c */
extern struct BattleEffectScene *RomBytes_03001ae8;
extern u16 RomBytes_0809f0f8[];

u16 BattleFx_GetCycledTableWord(void)
{
    return RomBytes_0809f0f8[(RomBytes_03001ae8 >> 4) & 15];
}

/* battle/effects/item_break/update_fragment.c */
struct ItemBreakFragmentPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ItemBreakFragmentSource {
    u8 reserved_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[70];
    u8 flag_5a;
    u8 reserved_5b[13];
    struct ItemBreakFragmentSource *target;
};

struct ItemBreakFragmentObject {
    u8 reserved_00[72];
    s32 field_48;
    u8 reserved_4c[9];
    u8 mode_55;
    u8 reserved_56[8];
    u16 field_5e;
};

extern s32 ArcTan2(s32, s32);
extern u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern void RotateVectorByMagnitude(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetMode(struct ItemBreakFragmentObject *, s32);

void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source)
{
    struct ItemBreakFragmentSource *target;
    struct ItemBreakFragmentPosition position;
    struct ItemBreakFragmentObject *object;
    s32 steering_delta;
    s32 drift_magnitude;

    target = source->target;
    if (target != 0) {
        s32 dx = target->x - source->x;
        s32 dz = target->z - source->z;

        if (dx != 0 || dz != 0) {
            steering_delta = (s16)(ArcTan2(dz, dx) - source->angle);
            if (steering_delta > 0x1000)
                steering_delta = 0x1000;
            if (steering_delta < -0x1000)
                steering_delta = -0x1000;
            source->angle += steering_delta;
        }
        source->flag_5a = 0;
    }

    position.x = source->x;
    position.y = source->y - (Rand() << 4) - 0x80000;
    position.z = source->z;
    drift_magnitude = Rand() * 3;
    drift_magnitude <<= 4;
    RotateVectorByMagnitude(drift_magnitude, Rand(), &position);

    object = Object_Spawn(0x11D, position.x, position.y, position.z);
    if (object != 0) {
        object->mode_55 = 2;
        object->field_48 = 0x1999;
        Object_SetMode(object, 0);
        object->field_5e = 12;
        Object_SetCallback(object, RomBytes_0809f0b0);
    }
}

/* battle/effects/orbiting_particles/update_return.c */
struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *effect)
{
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Rand();
        RotateVectorByMagnitude(0x1e0000, (u16)angle, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*state_pointer)++;
        return;
    }

    if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0) {
            (*state_pointer)++;
            goto next_state;
        }
        return;
    }

    if (state == 2) {
        effect->target_x = effect->origin_x;
        effect->target_z = effect->origin_z;
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        effect->flag42 = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && EffectSlot_HasReachedTarget(effect) == 0)
        FunctionHead_0809bb34(effect);
}

/* battle/effects/item_break/run_item_break_sequence.c */
struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};


void *BattleFx_StartItemBreak(void *object);

void WaitFrames(s32);
void UpdateRisingParticleBurst(void *object);

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = RomBytes_03001f30;
    object = scene->main_object;

    do {
        FunctionHead_08097384();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    Battle_Do(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}

/* battle/effects/item_break/start.c */
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void BattleFx_UpdateItemBreakFragment(void *);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern void Audio_PlayCue(s32);

void *BattleFx_StartItemBreak(void *source)
{
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 vel;
    u32 rotation_jitter;
    s32 zero;
    void *parent;
    void *child;

    angle = (*(u16 *)((s8 *)source + 6) + 0x2000) & 0xc000;
    parent = Object_Spawn(0xd7, *(s32 *)((s8 *)source + 8),
                           *(s32 *)((s8 *)source + 12) + 0x100000,
                           *(s32 *)((s8 *)source + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = (s32)BattleFx_UpdateItemBreakFragment;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x11d, *(s32 *)((s8 *)source + 8),
                              *(s32 *)((s8 *)source + 12) + 0x100000,
                              *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Object_SetCallback(child, &RomBytes_0809f0d4);
            fragment_scale = Rand() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = fragment_scale;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            vel = Rand();
            *(s32 *)((s8 *)child + 0x28) = vel - Rand();
            fragment_height = Rand() * 0x18 + 0x80000;
            rotation_jitter = Rand();
            Motion_SetTargetPositionFromMagnitudeAngle(
                child, fragment_height,
                          ((rotation_jitter - Rand()) >> 3) +
                          *(u16 *)((s8 *)source + 6));
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    return parent;
}

/* battle/effects/obj/snap_scale_to_full.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Object_CommitPosition();

void BattleFx_SnapScaleToFull(void *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = FIELD_AT_OFFSET(obj, s32 *, 0x18);
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            FIELD_AT_OFFSET(obj, s32 *, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32 *, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}

/* object/dispatch/apply_value_to_kind200_children.c */
void ObjectDispatch_ApplyValueToKind200Children(int arg0)
{
  s32 cnt;
  u8 *p;
  u8 kind;
  void *child;
  void *rec;
  void *obj;
  obj = *((void **)ADDR_03001E64);
  cnt = 0x3F;
  do
  {
    if (1)
    {
      if ((*((s32 *)(((u8 *)obj) + 0))) != 0)
      {
        kind = *(p = (u8 *)(((u8 *)obj) + 0x54));
        if (kind == 1)
        {
          child = *((void **)(((u8 *)obj) + 0x50));
          rec = *((void **)(((u8 *)child) + 0x28));
          if ((*((s16 *)(((u8 *)rec) + 0))) == 0xC8)
          {
            *((s8 *)(((u8 *)rec) + 5)) = arg0;
            *((u8 *)(((u8 *)child) + 0x25)) = kind;
          }
        }
      }
      cnt -= 1;
    }
    obj += 0x70;
  }
  while (cnt >= 0);
}
