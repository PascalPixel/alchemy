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
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);

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
