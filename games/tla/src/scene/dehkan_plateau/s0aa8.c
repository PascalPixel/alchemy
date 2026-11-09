#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

extern const u8 gData_02003e30[];

void Func_02000aa8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 frame;
  u32 i;

  if (leader == NULL)
    return;
  frame = 0;
  for (i = 0; i <= 16; i++) {
    {
      struct Actor *object = ((struct Actor *)Import_ActorCreate(
          285, leader->x, leader->y, leader->z));
      struct EffectView *effect = (struct EffectView *)object;

      if (object == NULL)
        goto skip;
      frame = Func_02003d80(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02003ba0(object, 2);
      Func_02003ba8(object, gData_02003e30);
      effect->velocityX = Math_Cosine(i << 12);
      effect->velocityY = 0;
      effect->velocityZ = Math_Sine(i << 12);
      effect->velocityX += effect->velocityX >> 1;
      effect->velocityX += (((Random16Far() * 3) << 14) >> 16) + -0x6000;
      effect->velocityZ += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
      effect->spin = (((u32)Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct EffectView *)leader)->sprite->priority;
    }
  skip:
  }
}
