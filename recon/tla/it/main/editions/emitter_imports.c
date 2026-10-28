#include "types.h"
#include "objdisp.h"

struct ParticlePosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ParticleEmitter {
  u8 padding[8];
  struct ParticlePosition position;
  u8 padding2[20];
  s32 travelOffset;
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
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);
void Actor_SetMode(struct ParticleEffectObject *, s32);
extern const u8 gBattleEffect_ParticleScript[];

void Marker_MoveLeaderAwayFromEmitter(struct ParticleEmitter *emitter) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 randomAngle;

  if (emitter->travelOffset >= -255 && emitter->travelOffset <= 255)
    emitter->active = 0;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = emitter->position.x;
  position.y = emitter->position.y;
  position.z = emitter->position.z;
  randomAngle = Random16();
  Vector_AddPolarOffset(randomAngle << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct ActorView *)object,
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
