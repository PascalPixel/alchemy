// Draft, differ 99 of 145: Func_02000d3c's companion: the map's two scrolling
// layers (0x108 and 0x178) climb 352 pixels at once while a layer fades in
// (frames 17 to 79), then after a screen transition the leader is placed
// (cue 308) and the layers sink back a pixel a frame while the layer fades
// out (frames 33 to 95 counted down); then cues 298 and 125 and flag 0x206.
// Left: the first loop's registers and the order of the layer updates; the second loop is close.
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "system/io_reg.h"

void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001ee8(s32 skip);
void Func_02001d60(void);
void Func_02001d40(void);
void WaitTicks(s32 ticks);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(u32 id, s32 x, s32 z);
void FieldEvent_WaitTicks(s32 ticks);
void Func_02001f10(s32 cue);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void GameFlag_SetBit(s32 flag);

void Func_02000be8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step, fade, alpha;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  step = 1;
  i = 0;
rise:
  if ((u32)(i - 17) <= 62) {
    fade = (step + 1) / 4;
    alpha = 16 - fade;
    REG_BLDALPHA = (fade << 8) | alpha;
    step++;
  }
  *(s32 *)(lower + 12) += 0x10000;
  *(s32 *)(upper + 12) += 0x10000;
  i++;
  if (i < 352)
    goto rise;
  Func_02001d40();
  WaitTicks(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0xcccc, 0x6666);
  ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 328, 184);
  FieldEvent_WaitTicks(20);
  Func_02001f10(308);
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 2);
  if (i != 0) {
  sink:
    if ((u32)(i - 33) <= 62) {
      fade = (step + 1) / 4;
      alpha = 16 - fade;
      REG_BLDALPHA = (fade << 8) | alpha;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    i--;
    WaitTicks(1);
    if (i != 0)
      goto sink;
  }
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  Func_02001f10(298);
  WaitTicks(1);
  Func_02001f10(125);
  FieldEvent_WaitTicks(20);
  GameFlag_SetBit(0x206);
  Func_02001da8();
}
