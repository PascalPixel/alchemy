#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "battle_effect_work.h"
#include "mem.h"
#include "motobj.h"
#include "iwram_call.h"
#include "edition.h"

struct BattleFxWorkTail {
  u8 unk0000[0x78e4];
  s32 active;
  s32 unk78e8;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

u8 *GetMotionRecordFar(s32 *object, s32 mode);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Render_ResetTransformState(void);
void Transform_SetLookAt(u8 *source, u8 *destination);
s32 Render_ProjectPoint(const void *point, s32 *screen);

// The Japanese, French and Italian far-call tables do not reach
// BattleUnit_GetEffectHeightFar under that name (Japanese names another slot,
// the other two keep the slot unnamed on their scaffold); those three editions
// keep this function on their scaffold (S4).
#if (LANGUAGE != LANGUAGE_JAPANESE) && (LANGUAGE != LANGUAGE_FRENCH) &&                    \
    (LANGUAGE != LANGUAGE_ITALIAN)
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  u8 *camera;
  s32 *object;
  u8 *info;
  s32 scale;

  camera = Ram_HeapSlots->cameraWork;
  object = GetBattleObjectSlotFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  Render_ResetTransformState();
  Transform_SetLookAt(camera, camera + 12);
  scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen),
                       *(s32 *)(info + 12));
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}
#endif

void BattleFx_ProbeKeysForExtendedActor(void) {
  struct BattleFxWorkTail *work;
  struct BattleEffectArgument *effect;

  /* FAKEMATCH: the one-pass block loads the effect work before the effect
   * offset is started; assigned directly the offset is started first (differ
   * 2). */
  do {
    work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
  effect = work->effect;

  /* FAKEMATCH: the listing reads the held keys and drops them; InputState
   * declares only repeat volatile, as waitmode.c measured for the same read. */
  if (effect->actors[0] > 0x7f)
    (void)((volatile struct InputState *)&gInput)->held;
}

void Func_0815f16c(struct BattleEffectArgument *effect, s32 index);
void BattleFx_RunCanvasModeByCount(struct BattleEffectArgument *effect);
void Func_0815e9f4(struct BattleEffectArgument *effect);
void Func_0815e3ac(struct BattleEffectArgument *effect);

void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect) {
  struct BattleFxWorkTail *work;
  s32 kind;
  s32 value;

  Runtime_AllocateBlock(100, 0x60e);
  Runtime_AllocateHeapBlock(92, 0x7b7c);
  Runtime_AllocateHeapBlock(96, 0x4000);
  /* FAKEMATCH: the one-pass block loads the effect work before the effect
   * offset is started; assigned directly the offset is started first (differ
   * 2). */
  do {
    work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
  work->effect = effect;
  work->active = 1;
  kind = effect->kind;
  value = kind - 100;
  if ((u32)value <= 100) {
    Func_0815f16c(effect, value);
  } else if (kind == 210 || kind == 10) {
    BattleFx_RunCanvasModeByCount(effect);
  } else if (kind > 199) {
    Func_0815e9f4(effect);
  } else {
    Func_0815e3ac(effect);
  }
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
  Runtime_ReleaseHeapBlock(100);
}
