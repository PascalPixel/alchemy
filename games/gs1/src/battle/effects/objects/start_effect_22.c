#include "types.h"
#define ObjectTable_Get Func_0808ba1c

/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
void *ObjectTable_Get(u32 object);

struct EffectVisual_0808f1c0 {
    u8 unknown_00[5];
    u8 flags_a;
    u8 unknown_06[3];
    u8 flags_b;
    u8 unknown_0a[18];
    u8 value_1c;
    u8 unknown_1d[9];
    u8 value_26;
    u8 value_27;
};

struct EffectResource_0808f1c0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectObject_0808f1c0 {
    u8 unknown_00[80];
    struct EffectVisual_0808f1c0 *visual;
    u8 unknown_54[24];
    void (*callback)(void);
};

extern s32 Data_02000240[];

struct EffectObject_0808f1c0 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void *Func_080048b0(s32 asset_id, s32 size);
void Func_08015250(s32 value);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_08002dd8(s32 value);
void Func_0808f0d8(struct EffectObject_0808f1c0 *object);
void Func_080030f8(s32 frames);
void Func_08009080(struct EffectObject_0808f1c0 *object, s32 mode);
void Func_080090d0(struct EffectObject_0808f1c0 *object);
void EmitRandomParticleEffect(void);

void Func_0808f1c0(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(Data_02000240[125]);
    void *handle = Func_080048b0(17, 0x608);
    struct EffectObject_0808f1c0 *object = Func_080090c8(
        22, resource->x, resource->y + 0x240000, resource->z);

    if (object != 0) {
        struct EffectVisual_0808f1c0 *visual = object->visual;
        s32 mask;

        s32 zero = 0;
        visual->value_26 = zero;
        visual->value_27 = zero;

        visual->flags_a &= zero - 33;

        mask = visual->flags_b & 0x0f;
        {
            s32 clear = 13;
            clear = -clear;
            mask &= clear;
        }
        mask |= 4;
        visual->flags_b = mask;

        Func_08015250(value);
        Func_08003fa4(visual->value_1c, 128, (u8 *)handle + 0x400);
        Func_08002dd8(17);

        if (flags & 1)
            object->callback = (void (*)(void))EmitRandomParticleEffect;
        if (flags & 2)
            Func_0808f0d8(object);

        Func_080030f8(80);
        Func_08009080((struct EffectObject_0808f1c0 *)resource, 1);
        Func_080090d0(object);
    }
}
