#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "objdisp.h"
#include "field_event.h"

struct ParticleSource {
    u8 padding[8];
    struct ParticlePosition position;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *);
struct ParticleEffectObject *Object_Spawn(s32, s32, s32, s32);

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
