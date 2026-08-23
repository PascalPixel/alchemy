#include "types.h"
#include "fixed_math.h"
#include "object_effect.h"

struct ParticlePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ParticleEmitter {
    u8 padding[8];
    struct ParticlePosition position;
    u8 padding2[20];
    s32 travel_offset;
    u8 padding3[41];
    u8 active;
};

struct ParticleChild {
    u8 padding[9];
    u8 flags;
};

struct ParticleEffectObject {
    u8 padding[80];
    struct ParticleChild *child;
};

#define OBJECT_0808EEE4_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ParticlePosition_size[
    sizeof(struct ParticlePosition) == 0x0c ? 1 : -1
];
typedef char ParticleEmitter_travel_offset_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, travel_offset) == 0x28 ? 1 : -1
];
typedef char ParticleEmitter_active_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, active) == 0x55 ? 1 : -1
];
typedef char ParticleEffectObject_child_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEffectObject, child) == 0x50 ? 1 : -1
];
#undef OBJECT_0808EEE4_OFFSET

extern u32 Random16(void);
extern void RotateVectorByMagnitude(s32, s32, struct ParticlePosition *);
extern void Object_SetCallback(struct ParticleEffectObject *, void *);
extern void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 Data_0809e87c[];

#define EmitRandomParticleEffect Func_0808eee4
void Func_0808eee4(struct ParticleEmitter *emitter)
{
    struct ParticlePosition position;
    struct ParticleEffectObject *object;
    u32 random_angle;

    if (emitter->travel_offset >= -255 && emitter->travel_offset <= 255)
        emitter->active = 0;

    if ((100 * Random16() >> 16) > 9)
        return;

    position.x = emitter->position.x;
    position.y = emitter->position.y;
    position.z = emitter->position.z;
    random_angle = Random16();
    RotateVectorByMagnitude(random_angle << 4, Random16(), &position);
    object = (struct ParticleEffectObject *)Object_Spawn(
        0x11D, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, (void *)Data_0809e87c);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
