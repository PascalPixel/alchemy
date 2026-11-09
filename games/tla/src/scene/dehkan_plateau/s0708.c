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

void Func_02000708(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003dd8(position, 0);
  if (target != NULL) {
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
      Sound_Play(125);
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
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        wait(8);
        Func_02003ba0(helper, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        wait(16);
        Func_02000c4c(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
        wait(2);
        Sound_Play(188);
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
}
