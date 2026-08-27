#include "types.h"

struct EffectResource_0808ef70 {
    u8 unknown_00[6];
    u16 value_06;
    s32 x;
    s32 y;
    s32 z;
};

struct Position_0808ef70 {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectVisual_0808ef70 {
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

struct PackedField08_0808ef70 {
    u16 value : 10;
};

struct EffectObject_0808ef70 {
    s32 value_00;
    u8 unknown_04[0x24];
    u32 value_28;
    u8 unknown_2c[0x1c];
    u32 value_48;
    u8 unknown_4c[4];
    struct EffectVisual_0808ef70 *visual;
    u8 unknown_54[24];
    void (*callback)(void);
};

extern u8 *Data_03001ebc;
extern const u8 Data_0809e6c0[];
extern const u8 Data_0809e87c[];

void RotateVectorByMagnitude(s32 angle, s32 magnitude, struct Position_0808ef70 *pos);
struct EffectObject_0808ef70 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Object_SetCallback(struct EffectObject_0808ef70 *object, s32 callback);
void Object_Destroy(struct EffectObject_0808ef70 *object);
void *Func_080048b0(s32 asset_id, s32 size);
void WaitFrames(s32 frames);
void Func_08015250(s32 value);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_08002dd8(s32 value);
void EmitRandomParticleEffect(void);
void BattleEffect_SpawnRandomParticleAtPosition(const void *source);

struct EffectObject_0808ef70 *Func_0808ef70(s32 value, s32 flags)
{
    struct EffectObject_0808ef70 *object;
    u8 *state = Data_03001ebc;
    u32 offset = (value * 4) + 0x14;
    struct EffectResource_0808ef70 *resource =
        *(struct EffectResource_0808ef70 **)(state + offset);
    struct Position_0808ef70 pos;
    s32 spawn_x;
    s32 spawn_z;

    if (resource == 0)
        return 0;

    pos.x = resource->x;
    pos.y = resource->y;
    pos.z = resource->z;
    RotateVectorByMagnitude(0x100000, resource->value_06, &pos);
    spawn_x = (pos.x & 0xfff00000) + 0x80000;
    spawn_z = (pos.z & 0xfff00000) + 0x80000;

    if (*(s16 *)(state + 0xcb8) != 0) {
        struct EffectObject_0808ef70 *entry =
            *(struct EffectObject_0808ef70 **)((u8 *)&Data_03001ebc - 88);
        s32 i = 63;

        do {
            if (entry->value_00 != 0) {
                if (entry->callback == (void (*)(void))BattleEffect_SpawnRandomParticleAtPosition) {
                    Object_Destroy(entry);
                }
                if (entry->value_00 == (s32)Data_0809e87c) {
                    Object_Destroy(entry);
                }
            }
            i--;
            entry++;
        } while (i >= 0);
    }

    WaitFrames(3);
    object = Func_080090c8(22, spawn_x, 0x100000, spawn_z);
    if (object == 0)
        return 0;

    Object_SetCallback(object, (s32)Data_0809e6c0);
    {
        struct EffectVisual_0808ef70 *visual = object->visual;
        void *handle;
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

        object->value_28 = 0x20000;
        object->value_48 = 0x4000;

        handle = Func_080048b0(17, 0x608);
        Func_08015250(flags);

        ((struct PackedField08_0808ef70 *)((u8 *)visual + 8))->value =
            Func_08003fa4(visual->value_1c, 128, (u8 *)handle + 0x400);

        Func_08002dd8(17);
        object->callback = (void (*)(void))EmitRandomParticleEffect;
    }

    return object;
}
