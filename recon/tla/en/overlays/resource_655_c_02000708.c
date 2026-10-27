// Draft, differ 77 of 191: Func_020002f8 with actor 8 helping. Left:
// register choice.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
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
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct ObjectRuntime *object, struct ObjectRuntime *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct ObjectRuntime *object, s32 mode);
void Func_02003ba8(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct ObjectRuntime *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct ObjectRuntime *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct ObjectRuntime *object);

/// The effect fields of an ObjectRuntime, as field_effect.h names them.
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
  void (*update)(struct ObjectRuntime *object);
};

void Func_02000708(u16 *list) {
  struct ObjectRuntime *leader;
  struct ObjectRuntime *helper;
  struct ObjectRuntime *target;
  s32 position[3];

  if (Func_02003d20() != 0)
    return;
  leader = Object_GetById(gPartyState.currentOwner);
  helper = Object_GetById(8);
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

    if (Object_GetById(id) != target) {
      list += 2;
      goto test;
    }
    flag = list[1];
    Func_02003c28();
    if (GameFlag_Test(flag) == 0) {
      Func_02003ba0(helper, 1);
      Func_020006ec(helper, target);
      WaitFrames(1);
      Func_02003df8(125);
      Object_SetPartAttribute(Object_GetById(8), 7);
      WaitFrames(2);
      Func_02003ba0(helper, 0);
      ObjectVisual_CopyAttributes(id, 9);
      Object_SetPartAttribute(Object_GetById(8), 0);
      WaitFrames(2);
      Object_SetPartAttribute(Object_GetById(8), 7);
      WaitFrames(4);
      Object_SetPartAttribute(Object_GetById(8), 0);
      Func_02000974(0);
      Func_020006ec(target, helper);
      WaitFrames(1);
      GameFlag_SetBit(flag);
    } else {
      s32 done = flag + 1;
      s32 result = GameFlag_Test(done);

      if (result == 0) {
        Func_02003ba0(helper, 0);
        Func_020006ec(helper, target);
        WaitFrames(1);
        leader->motionFlags = result;
        Func_02003df8(185);
        Call3((void (*)())Func_02003c00, 0x10000, 0x40000, 0x10000);
        Func_02000974(0);
        WaitFrames(8);
        Func_02003ba0(helper, 2);
        Func_02000c4c(-2, target->terrainId);
        Func_02000974(1);
        WaitFrames(16);
        Func_02000c4c(-2, target->terrainId);
        WaitFrames(4);
        Call3((void (*)())Func_02003c00, -1, -1, 0xe666);
        WaitFrames(8);
        leader->motionFlags = 3;
        WaitFrames(5);
        Object_SetPositionAndResetMotion(helper, 0, 0, 0);
        WaitFrames(2);
        Func_02003df8(188);
        Func_02000aa8();
        WaitFrames(20);
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
