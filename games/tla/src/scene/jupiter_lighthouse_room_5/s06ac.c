#include "types.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "event_inline.h"

struct SparkActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EffectParams {
  s32 count;
  s32 kind;
  s32 spread;
  s32 rise;
  u8 unk10[8];
  u16 tile;
  u16 unk1a;
  u8 unk1c[12];
};

struct SparkWork {
  u8 unk0000[0x1d9c];
  s16 resource;
};

extern s32 gData_020033dc;
extern u8 MsgFieldShowAnemosPower[];

void Resource_ResetEntry(s32 entry);
void HeapSlot_Release(s32 slot);
void Event_WaitTransitionFrames(void);
void Func_02001538(void);
void Func_020028b0(void);
void Func_020028b8(void);
void Func_02001c88(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 a5, s32 a6,
                   struct EffectParams *params);
void Actor_SetSingleChildFlags(struct SparkActor *object, s32 flags);
u32 Random16Far(void);

void Func_020006ac(void) {
  if (!Flag_IsSet(0xa0e)) {
    Flag_Set(0xa0e);
    Func_020028b0();
    FieldEvent_ResolvePendingActions(0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 1);
    PaletteFade_Interpolate(30);
    Event_WaitTransitionFrames();
    Actor_SetAnimation(0, 1);
    FieldEvent_Wait(30);
    FieldMessage_ShowCentered((s32)MsgFieldShowAnemosPower, 0, -2);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(30);
    Func_020028b8();
  }
}

void Func_02000724(void) { GameFlag_ClearBit(0xa0e); }

/// Actor 22 rises in a shower of sparks: one every other frame for 32 frames.
void Func_02000734(void) {
  struct EffectParams params;
  /* FAKEMATCH: the parameter block's pointer pinned to r6, where the native
   * keeps it across the loop; unpinned it takes r7 and the odd-frame test r6
   * (differ 22). */
  register struct EffectParams *p asm("r6");
  struct SparkActor *actor;
  u32 i;
  s32 x;
  s32 y;
  u32 random;
  u32 random2;
  u32 scaled;

  actor = ((struct SparkActor *)Actor_Get(22));
  Event_PlaySound(190);
  Actor_SetSingleChildFlags(((struct SparkActor *)Actor_Get(22)), 0);
  p = &params;
  p->count = 1;
  p->kind = 5;
  p->tile = 0x2a0;
  p->spread = 0x6666;
  p->rise = 0x30000;
  for (i = 0; i <= 31; i++) {
    FieldEvent_Wait(1);
    if (!(i & 1)) {
      random = Random16Far();
      /* FAKEMATCH: random * 24 written in steps with two barriers: ⚓️ loads x
       * between the multiply's add and its last shift (without the first
       * barrier differ 2, without the second differ 4). */
      scaled = random << 1;
      asm("");
      scaled += random;
      x = actor->x;
      asm("");
      x += (((scaled << 3) >> 16) << 16);
      x += -0xc0000;
      random2 = Random16Far();
      y = actor->y;
      /* FAKEMATCH: ⚓️ loads y before it shifts the second random value (differ
       * 2 without the barrier). */
      asm("");
      Func_02001c88(x, y + (((random2 << 5) >> 16) << 16) + 0x200000, actor->z,
                    0, -0x40000, 0, 0x1b0000, p);
    }
    if (i == 20)
      Actor_SetSpriteColorById(22, 0x100);
  }
  Actor_SetSpriteColorById(22, 0);
  Actor_SetSingleChildFlags(((struct SparkActor *)Actor_Get(22)), 1);
}
