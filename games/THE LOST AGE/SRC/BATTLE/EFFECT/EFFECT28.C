#include "TYPES.H"

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

u32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *);
struct ParticleEffectObject *Object_Spawn(s32, s32, s32, s32);
void Object_SetCallback(struct ParticleEffectObject *, const void *);
void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 BattleFx_ParticleScript[];

void BattleFx_EmitRandomParticleFromEmitter(struct ParticleEmitter *emitter)
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
    Vector_AddPolarOffset(random_angle << 4, Random16(), &position);
    /* ⚓️'s particle is object 0x2a1; ☀️'s is 0x11d. */
    object = Object_Spawn(0x2a1, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, BattleFx_ParticleScript);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
