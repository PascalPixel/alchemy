// Draft of overlay 674's Func_02001a68, differ 2 of 144: the native loads the
// address of Func_02000df8 before it copies object 9 for the done flag; this
// draft copies first. alchemy permute matched it only by giving call.h's Call3
// a volatile int result, which is no source form.
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02003fbc(void);
void Func_02003fc4(void);
void *Runtime_AllocateBlock(s32 id, s32 size);
void ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 accel);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
extern const u8 gData_020045a4[];
extern const u8 gData_020045f0[];

void Func_02001a68(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object = Object_GetById(9);
  s32 result;
  u8 *done;

  ((void (*)())Func_02003e64)();

  Call1((void (*)())Func_02003fa4, 0);

  Call3((void (*)())ObjectMotion_SetAngleToward, 9, gPartyState.currentOwner,
        0);

  Call1((void (*)())Battle_WaitMode0, 10);

  Call1((void (*)())Func_02003f24, (s32)MsgYouAllSet);

  Call2((void (*)())Func_02003f2c, 9, 0);

  ((void (*)())Func_02003fc4)();

  result = Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0);
  if (result == 0) {
    ((void (*)())Func_02003fbc)();

    Call2((void (*)())Func_02003f2c, 9, 0);

    ((struct {
      u8 pad[0x295];
      u8 v;
    } *)Runtime_AllocateBlock(88, 0x298))
        ->v = 99;
    Call3((void (*)())ObjectMotion_Launch, gPartyState.currentOwner, 6, 0);

    Call3((void (*)())ObjectMotion_SetSpeedParameters, gPartyState.currentOwner,
          0x19999, 0xcccc);

    Call3((void (*)())ObjectMotion_SetPositionAndCommit,
          gPartyState.currentOwner, 152, 0x300);

    leader->update = Func_02001278;
    Call2((void (*)())Object_SetPartAttribute,
          Object_GetById(gPartyState.currentOwner), 15);

    Call2((void (*)())Object_SetModeById, 9, 3);

    Call1((void (*)())Battle_WaitMode0, 20);

    ((void (*)())Func_02003fc4)();

    Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x19999, 0xcccc);

    Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x19999, 0xcccc);

    done = (u8 *)object + 99;
    *done = 0;
    object->update = Func_02000df8;
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 9,
          (s32)gData_020045a4);

    Call1((void (*)())Battle_WaitMode0, 30);

    Object_GetById(10)->update = Func_02000df8;
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 10,
          (s32)gData_020045f0);

    do {
      Call1((void (*)())WaitFrames, 1);

    } while (*done == 0);
    ((void (*)())Func_020034ec)();

    Call1((void (*)())WaitFrames, 1);

    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    ((void (*)())Event_FinishScreenTransition)();

    ((void (*)())Event_WaitTransitionFrames)();

    ((void (*)())Func_02003fbc)();

    Call1((void (*)())Func_02003f7c, 4);

  } else {
    ((void (*)())Func_02003fbc)();

    Call1((void (*)())Func_02003f24, (s32)MsgYouAllSet + 4);

    Call2((void (*)())Func_02003f2c, 9, 0);

    Call2((void (*)())Func_02003f3c, 9, 0x4000);
  }
  ((void (*)())Func_02003e6c)();
}
