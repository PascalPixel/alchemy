// Draft, differ 46 of 250: the party meets person 11 on the ledge: they talk,
// the ground shakes, the screen blends out and back while the ledge changes,
// the party gains the item and person 11 joins the leader (flag 0x9ba).
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e5c(void);
void Func_02004fcc(s32 mode);
void FieldMessage_Show(s32 message, s32 position);
void Func_02004f2c(s32 id, s32 animation, s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId);
void Func_02004ec4(s32 id, s32 target);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004f0c(s32 message);
void Func_02004f14(s32 id, s32 flags);
void FieldEvent_WaitFrames(s32 frames);
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action);
void Func_02004e24(s32 x, s32 y, s32 z);
void Func_02004e2c(void);
void Func_02004ddc(void);
void WaitFrames(s32 frames);
void Func_020015d0(s32 a, s32 b);
void Func_02004dd4(void);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Party_LearnAbility(s32 unit, s32 ability);
void GameFlag_SetBit(s32 flag);
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
s32 Func_02004e64(void);

s32 Func_020019cc(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  volatile u16 level;
  s32 message = (s32)MsgGaiaRockInside09;

  Func_02004e5c();
  Call1((void (*)())Func_02004fcc, 0);
  Call2((void (*)())FieldMessage_Show, message, 1);
  Call3((void (*)())Func_02004f2c, gAdventure.currentOwner, 0x100, 0);
  if (leader->z >> 19 <= 39) {
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2,
          gAdventure.currentOwner, 0x1b8, 0x148);
    ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  }
  Call2((void (*)())Func_02004ec4, 11, gAdventure.currentOwner);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2,
        gAdventure.currentOwner, 0x1d6, 0x162);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 11, 0x1be, 0x162);
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 11);
  Call3((void (*)())ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xc000,
        0);
  Call3((void (*)())ObjectMotion_ArmCallback, 11, 0, 0);
  Func_02004f0c(message + 1);
  Call2((void (*)())Func_02004f14, 11, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, gAdventure.currentOwner, 0x8000,
        0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, gAdventure.currentOwner,
        3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xc000,
        0);
  Call3((void (*)())ObjectMotion_ArmCallback, 11, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2,
        gAdventure.currentOwner, 0x1d2, 0x156);
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Call2((void (*)())FieldMessage_Show, message + 2, 1);
  Call3((void (*)())Func_02004e24, 0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02004e24, -1, -1, 0xe666);
  Func_02004e2c();
  Func_02004ddc();
  level = REG_BLDALPHA & 0xff;
  if (level != 0) {
  fadeOut:
    REG_BLDALPHA = level | 0x1000;
    level--;
    WaitFrames(1);
    if (level != 0)
      goto fadeOut;
  }
  REG_DISPCNT &= ~0x200;
  Call2((void (*)())Func_020015d0, 12, 4);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  if (level != 8) {
  fadeIn:
    REG_BLDALPHA = level | 0x1000;
    level++;
    WaitFrames(1);
    if (level != 8)
      goto fadeIn;
  }
  Func_02004dd4();
  Call3((void (*)())Func_02004ebc, 12, 0, 0);
  Call2((void (*)())Party_LearnAbility, 4, 139);
  Call1((void (*)())GameFlag_SetBit, 0x9ba);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 11, 0x13333, 0x9999);
  Call2((void (*)())Object_SetModeById, 11, 2);
  leader = Object_GetById(gAdventure.currentOwner);
  if (leader != 0)
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 11,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 11);
  Call3((void (*)())Func_02004ebc, 11, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  return Func_02004e64();
}
