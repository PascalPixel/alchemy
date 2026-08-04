#include "layout_guard.h"
#include "types.h"

enum { RADIAL_EFFECT_COUNT_08096140 = 24 };

typedef struct Position_08096140 {
    s32 x;
    s32 y;
    s32 z;
} Position_08096140;

typedef struct EffectObject_08096140 {
    u8 state;
    u8 padding01[4];
    s8 active;
    u8 padding06[2];
    s32 x;
    s32 y;
    s32 z;
    s32 base_y;
    s32 scale_x;
    s32 scale_y;
    u8 padding20[8];
    s32 field28;
    s32 field2c;
    s32 field30;
    u8 padding34[0x21];
    s8 mode;
    u8 padding56[5];
    s8 field5b;
    u8 padding5c[2];
    s16 lifetime;
    u8 padding60[4];
    s16 radial_value;
    s16 radial_angle;
    u8 padding68[4];
    const void *callback;
} EffectObject_08096140;

typedef struct EmitterRecord_08096140 {
    s32 handle;
    u8 padding04[0x24];
    s32 field28;
    s32 field2c;
    u8 padding30[0x18];
} EmitterRecord_08096140;

typedef struct ActiveRecord_08096140 {
    u8 state;
    u8 padding01[4];
    s8 active;
    u8 padding06[0x42];
} ActiveRecord_08096140;

typedef struct EffectRuntime_08096140 {
    u8 padding000[0x58];
    EmitterRecord_08096140 emitters[RADIAL_EFFECT_COUNT_08096140];
} EffectRuntime_08096140;

typedef struct FieldState_08096140 {
    u8 padding000[0x1f4];
    u32 current_object;
} FieldState_08096140;

LAYOUT_OFFSET_GUARD(
    EffectObject08096140_X,
    EffectObject_08096140,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    EffectObject08096140_ScaleX,
    EffectObject_08096140,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    EffectObject08096140_Mode,
    EffectObject_08096140,
    mode,
    0x55);
LAYOUT_OFFSET_GUARD(
    EffectObject08096140_Callback,
    EffectObject_08096140,
    callback,
    0x6c);
LAYOUT_SIZE_GUARD(
    EmitterRecord08096140_Size,
    EmitterRecord_08096140,
    0x48);
LAYOUT_SIZE_GUARD(
    ActiveRecord08096140_Size,
    ActiveRecord_08096140,
    0x48);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08096140_Emitters,
    EffectRuntime_08096140,
    emitters,
    0x58);
LAYOUT_OFFSET_GUARD(
    FieldState08096140_CurrentObject,
    FieldState_08096140,
    current_object,
    0x1f4);

extern FieldState_08096140 Data_02000240;
extern EffectRuntime_08096140 *Data_03001f30;

s16 Func_080022ec(s32, s32);
s32 Func_080022f4(u32, s32);
void Func_080030f8(s32);
u32 Func_08004458(void);
void Func_08009080(EffectObject_08096140 *, s32);
void Func_08009098(EffectObject_08096140 *, const void *);
void Func_080091f0(s32, s32, s32);
void Func_08009240(EffectObject_08096140 *, s32);
void Func_08009248(s32, s32);
EffectObject_08096140 *Func_08092054(u32);
void Func_08092560(u32, s32, s32);
void Func_08092adc(u32, s32, s32);
void Func_080958a8(void);
void Func_080958e4(void);
void Func_08096bec(EffectObject_08096140 *, s32, u32);
EffectObject_08096140 *Func_08096c80(s32, s32, s32, s32);
void Func_080974d8(Position_08096140 *);
void Func_0809ba70(EmitterRecord_08096140 *, s32);
void Func_0809ba7c(EmitterRecord_08096140 *, const void *);
void Func_0809ba90(EmitterRecord_08096140 *, s32, s32, s32);
void Func_080b0048(s32);
void Func_080b0050(void);
void Func_080f9010(s32);

/* Run the scripted radial effect and its two object-spawn waves. */
void Func_08096140(s32 target_id)
{
    u32 current_id = Data_02000240.current_object;
    EffectObject_08096140 *current = Func_08092054(current_id);
    EffectObject_08096140 *target = Func_08092054((u32)target_id);
    EffectRuntime_08096140 *runtime;
    Position_08096140 position;
    s32 index;

    if (target == 0)
        return;

    Func_080958a8();
    runtime = Data_03001f30;
    Func_080b0048(0x201204);
    Func_080030f8(0x1e);

    target->field5b = 0;
    Func_080f9010(0x98);
    Func_08092560((u32)target_id, 4, 0x0f);
    Func_080f9010(0x98);
    Func_08092560((u32)target_id, 4, 0x0f);
    Func_080030f8(0x1e);

    target->callback = (const void *)0x0809592d;
    Func_080f9010(0x99);
    Func_08092560((u32)target_id, 8, 0x16);
    Func_080f9010(0x8c);
    Func_080091f0(0x14ccc, 0x14ccc, 0x10000);
    target->callback = (const void *)0x08095f9d;
    Func_08009080(target, 3);
    Func_080030f8(0x5a);
    Func_08092adc(current_id, 0x4000, 0);
    Func_080030f8(0x14);
    Func_08009080(Func_08092054(current_id), 0x1c);
    Func_080030f8(0x1e);
    Func_080091f0(0x19999, 0x19999, 0x10000);

    position.x = target->x;
    position.y = target->y;
    position.z = target->z;
    Func_080974d8(&position);
    for (index = 0; index < RADIAL_EFFECT_COUNT_08096140; index++) {
        EmitterRecord_08096140 *emitter = &runtime->emitters[index];

        Func_0809ba90(emitter, 0x11c, position.x, position.z);
        Func_0809ba7c(emitter, (const void *)0x08096049);
        Func_0809ba70(emitter, 7);
        Func_08009248(emitter->handle, 0x0b);
        emitter->field28 = 0x8000;
        emitter->field2c = Func_08004458() + 0x18000;
        Func_080030f8(1);
    }

    Func_080030f8(0x8c);
    for (index = 0; index < RADIAL_EFFECT_COUNT_08096140; index++) {
        ActiveRecord_08096140 *record =
            (ActiveRecord_08096140 *)
                ((u8 *)runtime + 0x98 + index * 0x48);

        if (record->active != 0)
            record->state = 2;
    }

    Func_080030f8(0x14);
    Func_080091f0(1, 1, 1);
    Func_080030f8(0x1e);

    for (index = 0; index < RADIAL_EFFECT_COUNT_08096140; index++) {
        EffectObject_08096140 *object;
        s32 scale;

        position.x = current->x;
        position.y = current->y + 0x780000;
        position.z = current->z;
        object = Func_08096c80(
            0x11c, position.x, position.y, position.z);
        if (object != 0) {
            scale = Func_080022f4(Func_08004458(), 3) + 0x10000;
            object->scale_y = scale;
            object->scale_x = scale;
            object->radial_value = 0x64;
            object->radial_angle = Func_080022ec(index << 16, 0x18);
            object->callback = (const void *)0x08095fcd;
            object->mode = 0;
            Func_08009080(object, 7);
            Func_08009240(object, 0x0b);
        }
    }

    Func_080030f8(0x64);
    Func_080f9010(0x120);
    Func_080030f8(1);
    Func_080f9010(0x97);

    position.x = current->x;
    position.y = current->y + 0x120000;
    position.z = current->z;
    for (index = 0; index < 8; index++) {
        EffectObject_08096140 *object = Func_08096c80(
            0x11c, position.x, position.y, position.z);

        if (object == 0)
            break;
        object->scale_y = 0x9999;
        object->scale_x = 0x9999;
        object->mode = 2;
        object->field28 = 0x50000;
        object->base_y = object->y;
        object->field30 = Func_08004458() + 0x16666;
        Func_08096bec(object, 0x200000, Func_08004458());
        Func_08009240(object, 0x0b);
        object->lifetime = 8;
        Func_08009098(object, (const void *)0x0809f0b0);
    }

    Func_080030f8(0x0f);
    Func_080b0050();
    Func_080958e4();
}
