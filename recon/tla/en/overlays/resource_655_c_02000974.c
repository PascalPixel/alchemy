// Draft, differ 79 of 122: eight puffs (type 285) in a row over the
// leader, flung outwards when mode is set. A goto loop. Left: register
// choice (the native keeps the leader in r7 and the count in r10).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
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

extern const u8 gData_02003e00[];

void Func_02000974(s32 mode) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 frame = 0;
  u32 i;

  i = 0;
loop:
  {
    struct Actor *object;
    struct EffectView *effect;

    object = Func_02003bb0(
        285, leader->x + (i << 17) - 0x80000,
        leader->y - ((((Random16Far() * 7) << 1) >> 16) << 16) - 0x20000,
        leader->z);
    if (object == NULL)
      goto skip;
    effect = (struct EffectView *)object;
    frame = Func_02003d80(effect->sprite, frame);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_02003ba0(object, 2);
    Func_02003ba8(object, gData_02003e00);
    object->scaleX = 0xb333;
    object->scaleY = 0xb333;
    effect->sprite->priority = 1;
    if (mode != 0) {
      effect->velocityX = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
      effect->velocityZ = Math_Sine(i << 13);
    } else {
      effect->velocityX = mode;
      effect->velocityZ = mode;
    }
    effect->velocityY = -((((u32)Random16Far() << 14) >> 16) + 0x6000);
    effect->spin = (((u32)Random16Far() << 9) >> 16) - 0x100;
    effect->scaleRateX = -0x800;
    effect->scaleRateY = -0x600;
    effect->update = Effect_AdvanceMotionAtLeaderPriority;
  }
skip:
  i++;
  if (i <= 7)
    goto loop;
}
