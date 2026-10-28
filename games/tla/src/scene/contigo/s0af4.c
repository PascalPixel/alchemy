#include "scene_tables.h"
#include "contigo.h"

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

static inline struct ContigoGlow *Contigo_Create(s32 type, s32 x, s32 y,
                                                 s32 z) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer (direct,
   * differ 2). */
  return Func_02003fec(type, x, y, z);
}

/// The glow that grows over actor 17 and sinks away again, then the scene goes
/// on.
void Func_02000af4(void) {
  struct ContigoGlow *effect;
  u32 i;

  Func_0200406c();
  Func_02004204(0);
  if (GameFlag_Test(0x9b2))
    FieldMessage_Show((s32)MsgContigo163, 1);
  else
    FieldMessage_Show((s32)MsgContigo64, 1);
  Contigo_ObjMode(17, 3);
  Func_020041d4(0x10000, 0);
  Func_020041cc(0x204084, 1);
  Func_020041dc(16);
  Func_02004264();
  Func_0200427c(Actor_GetById(17), 2);
  effect = Contigo_Create(280, 0x2c20000, 0, 0x740000);
  if (effect != 0) {
    Actor_SetSingleChildFlags(effect, 0);
    Func_02003fdc(effect, 2);
    effect->motionFlags = 0;
    effect->scaleX = 0x1999;
    effect->scaleY = 0x1999;
    i = 0;
  grow:
    {
      s32 x;

      /* FAKEMATCH: the one-pass block keeps the scale load ahead of the 0x666
       * step, as the native has it (differ 2 without). */
      do {
        x = effect->scaleX;
      } while (0);
      effect->scaleX = x + 0x666;
    }
    effect->scaleY += 0x666;
    wait(1);
    i++;
    if (i < 28)
      goto grow;
  }
  Task_Add(Func_02000a80, 0x480);
  Contigo_Wait(120);
  Task_RemoveFar(Func_02000a80);
  if (effect != 0) {
    i = 0;
  shrink:
    effect->scaleX += 0xfffff99a;
    /* FAKEMATCH: two volatile accesses, so the y load depends on the Y scale
     * store and is issued before the 0x8000 step is built, as the native does;
     * plain, the scheduler builds the step first (differ 2). */
    *(volatile s32 *)&effect->scaleY += 0xfffff99a;
    *(volatile s32 *)&effect->y += 0x8000;
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
    Contigo_MessageSet((s32)MsgContigo164);
  else
    Contigo_MessageSet((s32)MsgContigo65);
  Contigo_ObjVar(17, 2);
  Contigo_MessagePut(17, 0);
  Contigo_ObjMode(17, 1);
  Contigo_Jump(14, 259, 0);
  Contigo_ObjTurn(14, FACING_WEST, 20);
  Contigo_MessagePut(14, 0);
  Contigo_ObjFace(15, FACING_EAST);
  Contigo_MessagePut(15, 0);
  Contigo_ObjMode(14, 3);
  Contigo_ObjModeWait(15, 3);
  Contigo_ObjTurn(15, 0xd000, 0);
  Contigo_ObjFace(14, 0x3000);
  Func_02004074();
}
