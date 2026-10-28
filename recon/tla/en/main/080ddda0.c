// Under -mthumb-slow-loads (2026-10-26): differ 4 of 113 in tla-en, instruction
// order only. After a store the native builds a constant's half before the next
// load, where alchemy-gcc issues the load at once (AGENTS.md's recurring row);
// no empty asm placement lowers it.
// Near miss, differ 4 of 113 (was 16 through a gEffectWork symbol; this game
// reads the effect work from its heap slot): two scheduling pairs. The native
// starts the 0x100000 lift before it loads y, and reloads the first anchor
// from the frame before it finishes the flag address.
#include "types.h"
#include "objdisp.h"
#include "system.h"
#include "object_effect_scripts.h"

struct BattleEffectScene {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
  void *mainObject;
  void *child;
};

struct EffectChild {
  u8 reserved00[12];
  s32 y;
  u8 reserved10[0x55 - 0x10];
  u8 flag;
  u8 reserved56[0x6c - 0x56];
  void (*callback)(void);
};

#include "ram_buffer.h"

void BattleEffect_InitializeSharedScene(void);
void *BattleEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(void *object, s32 magnitude,
                                                s32 angle);
void Actor_CommitPosition(void *object);
void Sound_PlayFar(s32 sound);
void Actor_ApplyRandomChildValues(void);
void UpdateRisingParticleBurst(void *effect);
void BattleEffect_PrepareBufferInterpolation(void);

void FieldEffect_RunLiftOnTarget(void) {
  struct BattleEffectScene *scene;
  void *mainObject;
  void *child;
  void *anchors[2];
  s32 position[3];
  s32 x, y, z;
  s32 index;
  s32 startY;

  scene = Ram_HeapSlots->effectWork;
  child = scene->child;
  mainObject = scene->mainObject;
  if (child == 0)
    return;

  BattleEffect_InitializeSharedScene();
  *(void **)((u8 *)mainObject + 0x68) = child;
  Actor_StartScriptFar((struct ActorView *)mainObject,
                               (u32)(gBattleEffect_SharedParticleScript + 12));

  x = scene->x;
  position[0] = x;
  y = scene->y + 0x100000;
  position[1] = y;
  z = scene->z;
  position[2] = z;
  anchors[0] = BattleEffect_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  anchors[1] = BattleEffect_SpawnItemBreakMode3(position[0] - 0x200000, position[1],
                                            position[2], 0);

  WaitFrames(15);
  for (index = 0; index < 2; index++) {
    void *anchor = anchors[index];
    if (anchor != 0)
      Motion_SetTargetPositionFromMagnitudeAngle(anchor, 0xe0000,
                                                 *(u16 *)((u8 *)anchor + 6));
  }

  Actor_CommitPosition(anchors[0]);
  ((struct EffectChild *)child)->callback = Actor_ApplyRandomChildValues;
  Sound_PlayFar(130);
  ((struct EffectChild *)child)->flag = 4;

  startY = ((struct EffectChild *)child)->y;
  if (anchors[0] != 0 && anchors[1] != 0 && startY <= startY + 0x200000) {
    do {
      *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
      *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
      ((struct EffectChild *)child)->y += 0x4000;
      WaitFrames(1);
    } while (((struct EffectChild *)child)->y <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(anchors[0]);
  UpdateRisingParticleBurst(anchors[1]);
  BattleEffect_PrepareBufferInterpolation();
}
