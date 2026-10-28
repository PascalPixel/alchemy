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
#include "ram_buffer.h"

struct ParticleEmitter {
  u8 padding[8];
  struct ParticlePosition position;
  u8 padding2[20];
  s32 travelOffset;
  u8 padding3[41];
  u8 active;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *);
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);

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

struct RevealWork {
  u8 unk000[0xca4];
  s8 revealActive;
};

/// Returns 1 while Reveal is active, else 0.
// As ☀️'s Field_IsRevealActive.
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter) {
  s32 revealState;
  u32 isActive = 0;
  struct RevealWork *work = Ram_HeapSlots->eventWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}
