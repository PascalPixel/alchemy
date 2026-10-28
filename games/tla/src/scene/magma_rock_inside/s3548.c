#include "local.h"
#include "call.h"

extern s16 gData_0200525c[];
extern s16 gData_020052bc[];
extern s16 gData_0200531c[];
extern s16 gData_0200537c[];

void Func_02000800(s32 id, s16 *script, s32 flag);

void Func_02003548(void) {
  /* FAKEMATCH: forwarding the first flag call through call.h keeps 0x302 from
   * being shared with the script calls, which build it again after the scene
   * test (a direct call: differ 56). */
  Call1((void (*)())GameFlag_SetBit, 0x302);
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov692Room4) {
    s16 *script = gData_0200525c;

    /* FAKEMATCH: the native loads the first script before it builds 0x302;
     * sched2 puts the constant's longer chain first without the barrier (differ
     * 2). */
    asm("");
    Func_02000800(15, script, 0x302);
    Func_02000800(16, gData_0200525c, 0x302);
    Func_02000800(17, gData_020052bc, 0x302);
    Func_02000800(18, gData_020052bc, 0x302);
    Func_02000800(19, gData_0200531c, 0x302);
    Func_02000800(20, gData_0200531c, 0x302);
    Func_02000800(21, gData_0200537c, 0x302);
    Func_02000800(22, gData_0200537c, 0x302);
    if (Actor_GetById(16)->step == 0) {
      Actor_GetById(16)->step = 10;
      Actor_GetById(18)->step = 10;
      Actor_GetById(20)->step = 10;
      Actor_GetById(22)->step = 10;
    }
    Actor_SetModeById(23, 4);
    Actor_SetModeById(24, 4);
    Actor_SetModeById(25, 4);
    Actor_SetModeById(26, 2);
  }
}

void Func_02003630(struct Actor *arg) {
  struct SequenceParentWork *party = &gSequenceParentWork;
  /* FAKEMATCH: the object pinned to r5 and assigned after the barrier is
   * copied after gSequenceParentWork's address loads; as a plain parameter, 2
   * instructions differ in tla-en. */
  register struct Actor *object asm("r5");
  struct Actor *owner;
  s16 ownerY;
  s16 y;
  u8 *flags;

  asm("");
  object = arg;
  owner = Actor_GetById(party->currentOwner);
  ownerY = owner->y >> 16;
  y = object->y >> 16;
  flags = &object->priorityFlags;

  if (ownerY != y || (s16)(owner->z >> 16) < (s16)(object->z >> 16)) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(object, 3);
  }
}
