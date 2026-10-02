/* Draft: the complete 116-byte IT output has 2 differing import bytes.
 * All non-relocation instructions match; callee ownership must be proved
 * before a source call-target change. Preserved at the Japanese-base pivot.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "TYPES.H"
#include "OBJDISP.H"

struct ParticlePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ParticleSource {
    u8 padding[8];
    struct ParticlePosition position;
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
void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 BattleFx_ParticleScript[];

void BattleFx_SpawnRandomParticleAtPosition(const struct ParticleSource *source)
{
    struct ParticlePosition position;
    struct ParticleEffectObject *object;
    u32 rnd;

    if ((100 * Random16() >> 16) > 9)
        return;

    position.x = source->position.x;
    position.y = source->position.y;
    position.z = source->position.z;
    rnd = Random16();
    Vector_AddPolarOffset(rnd << 4, Random16(), &position);
    /* ⚓️'s particle is object 0x2a1; ☀️'s is 0x11d. */
    object = Object_Spawn(0x2a1, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_ParticleScript);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
