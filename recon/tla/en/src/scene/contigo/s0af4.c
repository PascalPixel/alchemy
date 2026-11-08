#include "scene_tables.h"
#include "../../../../../../games/tla/src/scene/contigo/contigo.h"

struct ContigoGlow {
  u8 unk00[0x0c];
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x35];
  u8 motionFlags;
};

void FieldMessage_Show(s32 message, s32 position);
void Func_020041cc(s32 color, s32 b);
void Func_020041d4(s32 color, s32 b);
void Func_020041dc(s32 frames);
void Func_02004264(void);
void Func_0200426c(void);
void Func_02004274(void);
void Func_0200427c(struct ContigoObject *object, s32 mode);
struct ContigoGlow *Func_02003fec(s32 type, s32 x, s32 y, s32 z);
void Func_02003fdc(struct ContigoGlow *object, s32 mode);
void Func_02003ff4(struct ContigoGlow *object);
void Actor_SetSingleChildFlags(struct ContigoGlow *object, u32 value);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02000a80(void);

/// The glow that grows over actor 17 and sinks away again, then the scene goes
/// on.
void Func_02000af4(void) {
  struct ContigoGlow *effect;
  s32 x = PIXELS(706);
  s32 z = PIXELS(116);
  u32 i;

  Func_0200406c();
  Func_02004204(0);
  if (GameFlag_Test(SPLIT_CONSTANT(0x900, 178)))
    FieldMessage_Show((s32)MsgContigo163, 1);
  else
    FieldMessage_Show((s32)MsgContigo64, 1);
  Actor_SetModeById(17, 3);
  Func_020041d4(0x10000, 0);
  Func_020041cc(SPLIT_CONSTANT(0x204000, 132), 1);
  Func_020041dc(16);
  Func_02004264();
  Func_0200427c(Actor_GetById(17), 2);
  effect = Func_02003fec(280, x, 0, z);
  if (effect != 0) {
    Actor_SetSingleChildFlags(effect, 0);
    Func_02003fdc(effect, 2);
    effect->motionFlags = 0;
    effect->scaleX = 0x1999;
    effect->scaleY = 0x1999;
    i = 0;
  grow:
    effect->scaleX += 0x666;
    effect->scaleY += 0x666;
    wait(1);
    i++;
    if (i < 28)
      goto grow;
  }
  Task_Add(Func_02000a80, 0x480);
  FieldEvent_Wait(120);
  Task_RemoveFar(Func_02000a80);
  if (effect != 0) {
    i = 0;
  shrink:
    effect->scaleX += 0xfffff99a;
    effect->scaleY += 0xfffff99a;
    effect->y += 0x8000;
    wait(1);
    i++;
    if (i < 28)
      goto shrink;
    Func_02003ff4(effect);
  }
  Func_0200427c(Actor_GetById(17), 0);
  Func_02004274();
  Func_0200426c();
  Func_020041cc(0x10000, 1);
  Func_020041dc(16);
  if (GameFlag_Test(0x9b2))
    Func_0200412c((s32)MsgContigo164);
  else
    Func_0200412c((s32)MsgContigo65);
  Motion_SetVarCbAndRefresh(17, 2);
  Func_02004144(17, 0);
  Actor_SetModeById(17, 1);
  Contigo_Jump(14, 259, 0);
  Contigo_ObjTurn(14, FACING_WEST, 20);
  Func_02004144(14, 0);
  Func_0200415c(15, FACING_EAST);
  Func_02004144(15, 0);
  Actor_SetModeById(14, 3);
  Motion_SetModeAndWaitAnimation(15, 3);
  Contigo_ObjTurn(15, 0xd000, 0);
  Contigo_ObjFace(14, 0x3000);
  Func_02004074();
}
