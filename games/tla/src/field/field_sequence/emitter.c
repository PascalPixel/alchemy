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

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
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

    Actor_StartScriptFar((struct Actor *)object,
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

#include "actor.h"

struct EmitterWork {
  u8 unk000[0x14];
  struct Actor *actors[(0xca4 - 0x14) / 4];
  s8 revealActive;
};

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[2];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

extern const u8 gData_080effd8[];
extern const u8 Data_080f01b8[];
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void BattleEffect_SpawnRandomParticleAtPosition(void);

/// Shows the picture of item above actor (in front of the controlled owner,
/// on the centre of the cell), clearing Reveal's sparkles first.
struct Actor *BattleEffect_StartRandomParticleEmitter(s32 actor, s32 item) {
  struct EmitterWork *work = Ram_HeapSlots->eventWork;
  struct Actor *picture = work->actors[actor];
  struct Actor *object;
  struct EffectVisual *visual;
  s32 position[3];
  u8 *buffer;
  s32 tile;
  s32 i;
  s32 zero;
  s32 mask;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x;
  position[1] = picture->y;
  position[2] = picture->z;
  if (actor == Party_GetLeaderActor()) {
    Vector_AddPolarOffset(0x100000, picture->facing, position);
    position[0] = (position[0] & 0xfff00000) + 0x80000;
    position[2] = (position[2] & 0xfff00000) + 0x80000;
  }
  if (work->revealActive != 0) {
    object = Ram_HeapSlots->scriptObjects;
    i = 63;
    do {
      if (object->script != NULL) {
        if (object->update == BattleEffect_SpawnRandomParticleAtPosition)
          Actor_Destroy(object);
        if (object->script == (s32 *)gBattleEffect_ParticleScript)
          Actor_Destroy(object);
      }
      i--;
      object++;
    } while (i >= 0);
  }
  wait(3);
  picture =
      Func_080200c0(489, position[0], position[1] + 0x100000, position[2]);
  if (picture == NULL)
    return NULL;
  Actor_StartScriptFar(picture, gData_080effd8);
  visual = picture->sprite;
  zero = 0;
  visual->value1a = zero;
  visual->value1b = zero;
  visual->flags05 &= zero - 33;
  mask = ((u8 *)visual)[9] & 0x0f;
  {
    s32 clear = 13;

    clear = -clear;
    mask &= clear;
  }
  mask |= 4;
  ((u8 *)visual)[9] = mask;
  picture->motionFlags = 2;
  picture->terrainHeight = position[1];
  picture->velocityY = 0x20000;
  picture->gravity = 0x4000;
  buffer = HeapSlot_Alloc(68, 0x608);
  Func_08038248(item);
  tile = VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
  visual->tile = tile;
  HeapSlot_Release(68);
  picture->update = Marker_MoveLeaderAwayFromEmitter;
  if (work->revealActive != 0) {
    struct Actor *marker = Ram_HeapSlots->scriptObjects;
    s32 j;

    for (j = 0; j <= 63; j++, marker++) {
      if (marker->script != NULL && marker->script == (s32 *)Data_080f01b8 &&
          marker->x >> 20 == picture->x >> 20 &&
          marker->z >> 20 == picture->z >> 20) {
        Actor_Destroy(marker);
        break;
      }
    }
  }
  return picture;
}

void Actor_ReleaseIfPresent(struct Actor *object) {
  if (object != 0)
    Actor_Destroy(object);
}

/// Nothing in the image calls it.
void Func_080cf34c(void) {}
