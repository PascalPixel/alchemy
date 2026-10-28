#include "types.h"
#include "system.h"
#include "field_particle.h"
#include "actor.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "fixed_math.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

// ⚓️'s effect work holds its slots 8 bytes earlier than ☀️'s.
struct FallbackScene {
  u8 unk00[0x50];
  struct FieldParticle slots[16];
};

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *pos);
void Camera_WorldToScreen(struct EffectVector *pos);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x, s32 z);
void Animation_ApplyChildValuesToRecordFar(void *object, s32 value);
void Func_080e0020(struct FieldParticle *effect);

void Func_080e0134(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *user = scene->mainObject;
  struct Actor *obj;
  struct FieldParticle *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Actor_Spawn(309, 0, 0, 0);
  step = 0;
  Actor_SetMode(obj, 0);
  if (obj == 0)
    return;

  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleEffect_Interpolate(Q16_ONE, 0x4000, step);
    obj->scaleX = scale;
    obj->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  wait(5);
  Actor_SetMode(obj, 1);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(109);

  slot = &((struct FallbackScene *)scene)->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080e0020);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  wait(8);
  Actor_Destroy(obj);
  wait(4);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}
