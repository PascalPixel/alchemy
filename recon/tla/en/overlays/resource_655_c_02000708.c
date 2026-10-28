// Draft, differ 77 of 191: Func_020002f8 with actor 8 helping. Left:
// register choice.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
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

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Actor_GetById(gSequenceParentWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003dd8(position, 0);
  if (target == NULL)
    return;
  goto test;
next:
  {
    s32 id = list[0];
    s32 flag;

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Func_02003c28();
    if (GameFlag_Test(flag) == 0) {
      Func_02003ba0(helper, 1);
      Func_020006ec(helper, target);
      wait(1);
      Func_02003df8(125);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(2);
      Func_02003ba0(helper, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      wait(2);
      Actor_SetPartAttribute(Actor_GetById(8), 7);
      wait(4);
      Actor_SetPartAttribute(Actor_GetById(8), 0);
      Func_02000974(0);
      Func_020006ec(target, helper);
      wait(1);
      GameFlag_SetBit(flag);
    } else {
      s32 done = flag + 1;
      s32 result = GameFlag_Test(done);

      if (result == 0) {
        Func_02003ba0(helper, 0);
        Func_020006ec(helper, target);
        wait(1);
        leader->motionFlags = result;
        Func_02003df8(185);
        Call3((void (*)())Func_02003c00, 0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02003ba0(helper, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, target->terrainId);
        wait(4);
        Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Func_02003df8(188);
        Func_02000aa8();
        wait(20);
        GameFlag_SetBit(done);
      }
    }
    Func_02003c30();
    return;
  }
test:
  if (*list != 0xffff)
    goto next;
}
