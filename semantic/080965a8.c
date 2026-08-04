#include "layout_guard.h"
#include "types.h"

typedef struct Vec3_080965a8 {
    s32 x;
    s32 y;
    s32 z;
} Vec3_080965a8;

typedef struct Component_080965a8 {
    u8 padding00[9];
    u8 flags;
    u8 padding0a[0x1e];
    s16 *kind;
} Component_080965a8;

typedef struct EffectNode_080965a8 {
    u8 padding00[0x1c];
    u8 code;
} EffectNode_080965a8;

typedef struct Actor_080965a8 Actor_080965a8;
struct Actor_080965a8 {
    u8 padding00[6];
    u16 attribute;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[3];
    u8 mode;
    s32 motion_x;
    s32 motion_y;
    s32 motion_z;
    u8 padding30[8];
    s32 field_38;
    s32 field_3c;
    s32 field_40;
    u8 padding44[0x0c];
    Component_080965a8 *component;
    u8 padding54;
    s8 state;
    u8 padding56[4];
    u8 flags;
    s8 field_5b;
    u8 padding5c[0x0c];
    Actor_080965a8 *parent;
    u32 update;
};

typedef struct EffectRecord_080965a8 {
    Actor_080965a8 *object;
    u8 padding04[0x3c];
    s8 state;
    u8 padding41[4];
    s8 active;
    u8 padding46[2];
} EffectRecord_080965a8;

typedef struct EffectManager_080965a8 {
    u8 padding00[0x58];
    EffectRecord_080965a8 records[24];
} EffectManager_080965a8;

LAYOUT_OFFSET_GUARD(
    Actor080965a8_Component,
    Actor_080965a8,
    component,
    0x50);
LAYOUT_OFFSET_GUARD(
    Actor080965a8_Parent,
    Actor_080965a8,
    parent,
    0x68);
LAYOUT_OFFSET_GUARD(
    Actor080965a8_Update,
    Actor_080965a8,
    update,
    0x6c);
LAYOUT_SIZE_GUARD(EffectRecord080965a8_Size, EffectRecord_080965a8, 0x48);

void Func_080030f8(s32);
void Func_08003f3c(u8);
void Func_08009080(Actor_080965a8 *, s32);
Actor_080965a8 *Func_080090c8(s16, s32, s32, s32);
void Func_080090d0(Actor_080965a8 *);
void Func_080091e0(Actor_080965a8 *, s32);
void Func_08009240(Actor_080965a8 *, s32);
void Func_08009248(Actor_080965a8 *, s32);
Actor_080965a8 *Func_08092054(s32);
void Func_08092560(s32, s32, s32);
void Func_080925cc(s32, s32);
void Func_08092adc(s32, s32, s32);
void Func_080958a8(void);
void Func_080958e4(void);
EffectNode_080965a8 *Func_08096c48(
    Component_080965a8 *,
    EffectNode_080965a8 *);
void Func_080974d8(Vec3_080965a8 *);
void Func_0809ba70(EffectRecord_080965a8 *, s32);
void Func_0809ba7c(EffectRecord_080965a8 *, u32);
void Func_0809ba90(EffectRecord_080965a8 *, s32, s32, s32);
void Func_080b0048(s32);
void Func_080b0050(void);
void Func_080f9010(s32);

/*
 * Run the scripted clone attack: create and link eight afterimages, raise and
 * remove the actor chain, then emit a 24-record trailing effect before final
 * teardown.
 */
void Func_080965a8(s32 slot)
{
    Actor_080965a8 *actor = Func_08092054(slot);
    EffectManager_080965a8 *manager;
    Actor_080965a8 *clones[8];
    Actor_080965a8 *parent;
    EffectNode_080965a8 *effect = 0;
    Vec3_080965a8 position;
    s16 kind;
    u8 effect_code;
    s32 i;

    if (actor == 0)
        return;

    Func_080958a8();
    manager = *(EffectManager_080965a8 **)0x03001f30;
    Func_080b0048(0x204084);
    Func_080030f8(30);
    actor->field_5b = 0;
    Func_080f9010(0xad);
    Func_080925cc(slot, 1);
    Func_080f9010(0xaf);
    Func_080925cc(slot, 1);
    Func_080030f8(20);

    Func_080f9010(0x98);
    Func_08092560(slot, 3, 14);
    Func_080f9010(0x98);
    Func_08092560(slot, 5, 16);
    Func_080f9010(0x98);
    Func_08092560(slot, 7, 18);
    Func_080030f8(20);

    kind = *actor->component->kind;
    parent = actor;
    for (i = 0; i < 8; i++) {
        Actor_080965a8 *clone =
            Func_080090c8(kind, actor->x, actor->y, actor->z);

        clones[i] = clone;
        if (clone != 0) {
            clone->scale_x = 0xf000;
            clone->scale_y = 0xf000;
            clone->state = 0;
            clone->mode = 2;
            clone->flags |= 1;
            clone->update = 0x08096575;
            clone->attribute = actor->attribute;
            Func_08009240(clone, 9);
            Func_08009080(clone, 0);
            Func_080091e0(clone, 0);
            effect = Func_08096c48(clone->component, effect);
            clone->parent = parent;
            parent = clone;
        }
    }

    effect_code = effect->code;
    Func_080f9010(0x99);
    actor->state = 0;
    for (i = 0; i < 15; i++) {
        actor->y += 0x80000;
        Func_080030f8(1);
    }

    Func_080090d0(actor);
    for (i = 0; i < 8; i++)
        Func_080090d0(clones[i]);
    if (effect_code != 0x60)
        Func_08003f3c(effect_code);

    Func_080030f8(10);
    Func_08092adc(*(s32 *)0x02000434, 0x4000, 0);
    Func_080030f8(20);
    Func_08009080(Func_08092054(*(s32 *)0x02000434), 28);
    Func_080030f8(20);

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    Func_080974d8(&position);
    for (i = 0; i < 24; i++) {
        EffectRecord_080965a8 *record = &manager->records[i];

        Func_0809ba90(record, 0xf0, position.x, position.z);
        Func_0809ba7c(record, 0x0809641d);
        Func_0809ba70(record, 7);
        Func_08009248(record->object, 9);
        Func_080030f8(1);
    }

    Func_080030f8(120);
    for (i = 0; i < 24; i++) {
        if (manager->records[i].active != 0)
            manager->records[i].state = 2;
    }
    Func_080030f8(50);
    Func_080b0050();
    Func_080958e4();
}
