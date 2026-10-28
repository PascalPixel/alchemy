#include "local.h"

void Func_02001670(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;

  object->action += 0x100;
  sprite->rotation = Math_Sine(object->action) / 64 + 0x600;
}

void Func_020015e8(struct Actor *object);
void Func_02001630(struct Actor *object);

/// Scatters objects 17 to 21 under cue 184, lines them up 32 pixels apart
/// at z 824, then settles them under cue 298 and leaves object 21 rocking;
/// sets flags 0x109 and 0x955 and clears 0x12f.
void Func_0200169c(void) {
  s32 i;

  Func_02002998();
  Func_02002a48(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_020029d8(gMainRoutineWork.currentOwner, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002a70(184);
  /* FAKEMATCH: two goto loops; as for loops loop.c hoists the zero and the
   * update address out of them, where the native loads them on each pass
   * (differ 18). */
  i = 17;
scatter:
  {
    struct Actor *object = Actor_GetById(i);

    object->speedLimit = 0x4000;
    object->acceleration = 0x400;
    object->update = (void (*)(void))Func_020015e8;
    object->action = Random16Far();
    object->unk66 = 0;
  }
  if (++i <= 21)
    goto scatter;
  FieldEvent_Wait(60);
  Func_02002958(64, 69, 11, 4, 87, 53);
  Actor_ResetAndSetPosition_(17, 392, 824);
  Actor_ResetAndSetPosition_(18, 424, 824);
  Actor_ResetAndSetPosition_(19, 456, 824);
  Actor_ResetAndSetPosition_(20, 488, 824);
  Actor_ResetAndSetPosition_(21, 520, 824);
  FieldEvent_Wait(240);
  Func_02002958(64, 66, 11, 7, 87, 50);
  i = 17;
settle:
  Actor_GetById(i)->update = (void (*)(void))Func_02001630;
  if (++i <= 21)
    goto settle;
  Func_02002a70(298);
  FieldEvent_Wait(60);
  {
    struct Actor *object = Actor_GetById(21);

    object->update = (void (*)(void))Func_02001670;
    object->action = 0;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x109);
  GameFlag_ClearBit(0x12f);
  Func_02002a18(77);
  Func_020029a0();
  GameFlag_SetBit(0x955);
}
