#include "layout_guard.h"
#include "types.h"

typedef struct Vec3_08098cd8 {
    s32 x;
    s32 y;
    s32 z;
} Vec3_08098cd8;

typedef struct EffectNode_08098cd8 {
    u8 padding00[0x1c];
    u8 code;
} EffectNode_08098cd8;

typedef struct Object_08098cd8 {
    void *active;
    u8 padding04[2];
    s16 owner;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[8];
    s32 field_28;
    u8 padding2c[4];
    s32 size_x;
    s32 size_y;
    u8 padding38[0x18];
    EffectNode_08098cd8 *effect;
    u8 padding54;
    u8 flags;
} Object_08098cd8;

typedef struct TrailEntry_08098cd8 {
    Object_08098cd8 *object;
    u8 padding04[0x24];
    s32 field_28;
    s32 field_2c;
    u8 padding30[0x18];
} TrailEntry_08098cd8;

typedef struct SceneState_08098cd8 {
    s32 owner;
    Vec3_08098cd8 fallback;
    Object_08098cd8 *anchor;
    Object_08098cd8 *existing;
    u8 padding18[8];
    s8 use_anchor;
    u8 padding21[0x13];
    s8 existing_boost;
    s8 suppress_existing;
    u8 padding36[0x22];
    TrailEntry_08098cd8 trails[12];
} SceneState_08098cd8;

LAYOUT_OFFSET_GUARD(
    Object08098cd8_ScaleX,
    Object_08098cd8,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    Object08098cd8_SizeX,
    Object_08098cd8,
    size_x,
    0x30);
LAYOUT_OFFSET_GUARD(
    Object08098cd8_Effect,
    Object_08098cd8,
    effect,
    0x50);
LAYOUT_SIZE_GUARD(Object08098cd8_Size, Object_08098cd8, 0x58);
LAYOUT_SIZE_GUARD(TrailEntry08098cd8_Size, TrailEntry_08098cd8, 0x48);
LAYOUT_OFFSET_GUARD(
    SceneState08098cd8_Trails,
    SceneState_08098cd8,
    trails,
    0x58);

extern SceneState_08098cd8 *Data_03001f30;
extern s32 Data_02000434;

void Func_080030f8(s32);
void Func_08003f3c(u8);
void Func_0800447c(s32, s32, Vec3_08098cd8 *);
void Func_08009080(Object_08098cd8 *, s32);
void Func_08009098(Object_08098cd8 *, u32);
void Func_08009150(Object_08098cd8 *, s32, s32, s32);
s32 Func_080091d8(Object_08098cd8 *, Vec3_08098cd8 *);
void Func_08009240(Object_08098cd8 *, s32);
void Func_08009248(Object_08098cd8 *, s32);
s32 Func_08009250(Object_08098cd8 *, Vec3_08098cd8 *);
s32 Func_0808e4b4(u32, s32, s32 *);
void Func_08096b28(s32, s32, s32);
EffectNode_08098cd8 *Func_08096c48(
    EffectNode_08098cd8 *,
    EffectNode_08098cd8 *);
Object_08098cd8 *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080974d8(Vec3_08098cd8 *);
void Func_0809ba70(TrailEntry_08098cd8 *, s32);
void Func_0809ba7c(TrailEntry_08098cd8 *, u32);
void Func_0809ba90(TrailEntry_08098cd8 *, s32, s32, s32);
void Func_080f9010(s32);

/*
 * Build the arrival trail around the scene anchor, launch a central object
 * with three linked children, and redirect the group toward its destination
 * before handing the resulting event back to the field runtime.
 */
void Func_08098cd8(void)
{
    SceneState_08098cd8 *state = Data_03001f30;
    Object_08098cd8 *existing = state->existing;
    Object_08098cd8 *main_object;
    Object_08098cd8 *children[3];
    EffectNode_08098cd8 *effect = 0;
    Vec3_08098cd8 position;
    s32 event_result;
    s32 event;
    s32 i;
    u8 effect_code;

    Func_08097384();
    Func_080f9010(0x82);

    for (i = 0; i < 12; i++) {
        TrailEntry_08098cd8 *trail = &state->trails[i];

        position.x = state->anchor->x;
        position.y = state->anchor->y + 0x100000;
        position.z = state->anchor->z;
        Func_080974d8(&position);
        Func_0809ba90(trail, 0x11c, position.x, position.z);
        Func_0809ba7c(trail, 0x08098b11);
        Func_0809ba70(trail, 7);
        Func_08009248(trail->object, 9);
        trail->field_2c = 0x0b333;
        trail->field_28 = 0x0b333;
        Func_080030f8(2);
    }

    position.x = state->anchor->x;
    position.y = state->anchor->y + 0x100000;
    position.z = state->anchor->z;
    Func_0800447c(0x80000, state->owner, &position);
    main_object =
        Func_08096c80(0xd7, position.x, position.y, position.z);
    if (main_object == 0) {
        Func_0809748c();
        return;
    }

    main_object->scale_x = 0x4000;
    main_object->scale_y = 0x4000;
    main_object->owner = (s16)state->owner;
    main_object->size_x = 0x40000;
    main_object->size_y = 0x40000;
    main_object->flags = 0;
    Func_08009080(main_object, 5);
    Func_08009240(main_object, 3);

    if (main_object->scale_x < 0x10000) {
        do {
            main_object->scale_x += 0x500;
            main_object->scale_y = main_object->scale_x;
            Func_080030f8(1);
        } while (main_object->scale_x <= 0xffff);
    }
    Func_080030f8(3);

    for (i = 0; i < 3; i++) {
        Object_08098cd8 *child = Func_08096c80(
            0xd7,
            main_object->x,
            main_object->y,
            main_object->z);

        children[i] = child;
        if (child != 0) {
            child->scale_x = 0xf000;
            child->scale_y = 0xf000;
            child->owner = (s16)state->owner;
            child->size_x = 0x40000;
            child->size_y = 0x40000;
            child->flags = 0;
            Func_08009080(child, 5);
            Func_08009240(child, 2);
            effect = Func_08096c48(child->effect, effect);
        }
    }

    effect_code = effect->code;
    if (state->use_anchor != 0) {
        position.x = state->anchor->x;
        position.y = state->anchor->y + 0x100000;
        position.z = state->anchor->z;
        Func_0800447c(0x380000, state->owner, &position);
    } else {
        position.x = state->fallback.x;
        position.y = state->fallback.y + 0x100000;
        position.z = state->fallback.z;
    }

    Func_08009150(main_object, position.x, position.y, position.z);
    Func_08009098(main_object, 0x0809f12c);

    for (i = 0; i < 3; i++) {
        if (children[i] != 0) {
            Func_080030f8(3);
            Func_08009150(
                children[i],
                position.x,
                position.y,
                position.z);
            Func_08009098(children[i], 0x0809f0b4);
        }
    }

    if (main_object->active != 0) {
        i = 0;
        do {
            Func_080030f8(1);
            i++;
        } while (i <= 0x3b && main_object->active != 0);
    }

    if (existing != 0 && state->suppress_existing == 0) {
        if (state->existing_boost != 0)
            existing->field_28 = 0x80000;

        position.x = existing->x;
        position.y = existing->y;
        position.z = existing->z;
        Func_0800447c(0x100000, state->owner, &position);
        if (Func_080091d8(existing, &position) == 0 &&
            Func_08009250(existing, &position) == 0) {
            existing->size_x = 0x10000;
            existing->size_y = 0x10000;
            Func_08009150(existing, position.x, position.y, position.z);
        }
    }

    event = Func_0808e4b4(0x50000005, 4, &event_result);
    if (event != 0)
        Func_08096b28(event, Data_02000434, event_result);

    Func_080030f8(10);
    Func_0809748c();
    Func_080030f8(20);
    if (effect_code != 0x60)
        Func_08003f3c(effect_code);
}
