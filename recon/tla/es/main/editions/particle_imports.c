#include "types.h"
#include "objdisp.h"

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
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);
void Actor_SetMode(struct ParticleEffectObject *, s32);
extern const u8 gBattleEffect_ParticleScript[];

void BattleEffect_SpawnRandomParticleAtPosition(
    const struct ParticleSource *source) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 random;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = source->position.x;
  position.y = source->position.y;
  position.z = source->position.z;
  random = Random16();
  Vector_AddPolarOffset(random << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct DispatchObject *)object,
                                 (u32)gBattleEffect_ParticleScript);
    Actor_SetMode(object, 0);
    mask = 13;
    flags = object->child->flags;
    mask = -mask;
    mask &= flags;
    mask |= 4;
    object->child->flags = mask;
  }
}
