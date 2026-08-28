#include "types.h"

struct RandomParticleEmitterResource {
    u8 unknown_00[6];
    u16 value_06;
    s32 x;
    s32 y;
    s32 z;
};

struct RandomParticleEmitterPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct RandomParticleEmitterVisual {
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

struct RandomParticleEmitterPackedField {
    u16 value : 10;
};

struct RandomParticleEmitterObject {
    s32 value_00;
    u8 unknown_04[0x24];
    u32 value_28;
    u8 unknown_2c[0x1c];
    u32 value_48;
    u8 unknown_4c[4];
    struct RandomParticleEmitterVisual *visual;
    u8 unknown_54[24];
    void (*callback)(void);
};

extern u8 *Data_03001ebc;
extern const u8 Data_0809e6c0[];
extern const u8 Data_0809e87c[];

void RotateVectorByMagnitude(s32 angle, s32 magnitude, struct RandomParticleEmitterPosition *pos);
struct RandomParticleEmitterObject *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Object_SetCallback(struct RandomParticleEmitterObject *object, s32 callback);
void Object_Destroy(struct RandomParticleEmitterObject *object);
void *Func_080048b0(s32 asset_id, s32 size);
void WaitFrames(s32 frames);
void Func_08015250(s32 value);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_08002dd8(s32 value);
void EmitRandomParticleEffect(void);
void BattleEffect_SpawnRandomParticleAtPosition(const void *source);

#define BattleEffect_StartRandomParticleEmitter Func_0808ef70

struct RandomParticleEmitterObject *BattleEffect_StartRandomParticleEmitter(s32 value, s32 flags)
{
    struct RandomParticleEmitterObject *object;
    s32 state_or_count = (s32)Data_03001ebc;
    u32 offset = (value * 4) + 0x14;
    s32 resource_or_spawn_z =
        (s32)*(struct RandomParticleEmitterResource **)(state_or_count + offset);
    struct RandomParticleEmitterPosition pos;
    s32 spawn_x;

    if (resource_or_spawn_z == 0)
        return 0;

    pos.x = ((struct RandomParticleEmitterResource *)resource_or_spawn_z)->x;
    pos.y = ((struct RandomParticleEmitterResource *)resource_or_spawn_z)->y;
    pos.z = ((struct RandomParticleEmitterResource *)resource_or_spawn_z)->z;
    RotateVectorByMagnitude(
        0x100000,
        ((struct RandomParticleEmitterResource *)resource_or_spawn_z)->value_06,
        &pos);
    spawn_x = (pos.x & 0xfff00000) + 0x80000;
    resource_or_spawn_z = (pos.z & 0xfff00000) + 0x80000;

    if (*(s16 *)(state_or_count + 0xcb8) != 0) {
        struct RandomParticleEmitterObject *entry =
            *(struct RandomParticleEmitterObject **)((u8 *)&Data_03001ebc - 88);

        state_or_count = 63;

        do {
            if (entry->value_00 != 0) {
                if (entry->callback == (void (*)(void))BattleEffect_SpawnRandomParticleAtPosition) {
                    Object_Destroy(entry);
                }
                if (entry->value_00 == (s32)Data_0809e87c) {
                    Object_Destroy(entry);
                }
            }
            state_or_count--;
            entry++;
        } while (state_or_count >= 0);
    }

    WaitFrames(3);
    object = Func_080090c8(22, spawn_x, 0x100000, resource_or_spawn_z);
    if (object == 0)
        return 0;

    Object_SetCallback(object, (s32)Data_0809e6c0);
    {
        struct RandomParticleEmitterVisual *visual = object->visual;
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

        ((struct RandomParticleEmitterPackedField *)((u8 *)visual + 8))->value =
            Func_08003fa4(visual->value_1c, 128, (u8 *)handle + 0x400);

        Func_08002dd8(17);
        object->callback = (void (*)(void))EmitRandomParticleEffect;
    }

    return object;
}
