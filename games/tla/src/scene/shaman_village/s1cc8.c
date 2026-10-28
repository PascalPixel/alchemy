#include "shaman_village.h"

void Func_02001cc8(void) {
  struct ShamanVillageObject *object = Actor_GetById(14);

  if (object->x >> 20 == 32 && object->z >> 20 == 11) {
    Func_020048c4();
    Func_02004a24(0);
    object->motionFlags = 3;
    do {
      wait(1);
      /* FAKEMATCH: one more use of the object in the loop gives it r5 and the
       * speed r6; without it, 14 instructions differ in tla-en. */
      __asm__("" : : "l"(object));
    } while (object->velocityY != 0);
    Import_SoundPlay(188);
    wait(10);
    object->motionFlags = 0;
    ShamanVillage_BlendStart(30, 11, 1, 1, 32, 11);
    wait(1);
    GameFlag_SetBit(0x94e);
    Func_020048cc();
  }
}

void Func_02001d3c(void) {
  Func_020048c4();
  Func_02004a24(0);
  Import_SoundPlay(158);
  ShamanVillage_MapCopy(43, 40, 7, 6, 1, 2);
  FieldEvent_Wait(20);
  ShamanVillage_ObjPlace(15, 0x780000, 0xc20000);
  ShamanVillage_ObjSpeed(gSequenceParentWork.currentOwner, 0x26666, 0x13333);
  Actor_Jump(gSequenceParentWork.currentOwner, 4, 0);
  Actor_GetById(gSequenceParentWork.currentOwner)->actionFlags &= ~1;
  ShamanVillage_ObjWalk(gSequenceParentWork.currentOwner, 120, 220);
  FieldEvent_Wait(1);
  {
    u8 *flags = &Actor_GetById(gSequenceParentWork.currentOwner)->actionFlags;

    u32 value = *flags;

    /* FAKEMATCH: the barrier keeps the bit out of r2 until the flags are read,
     * as the native has; as a plain |= the bit is built first (one reordered
     * row). */
    __asm__("" : "+l"(value) : : "r2");
    *flags = value | 1;
  }
  ShamanVillage_ObjSpeed(15, 0x6666, 0x3333);
  ShamanVillage_ObjWalk(15, 120, 208);
  Func_02004944(16, 15);
  Func_02004944(17, 15);
  wait(1);
  ShamanVillage_ObjSpeed(16, 0xcccc, 0x6666);
  ShamanVillage_ObjSpeed(17, 0xcccc, 0x6666);
  Actor_ResetAndSetPositionInMode2(16, 104, 212);
  ShamanVillage_ObjWalk(17, 136, 212);
  Actor_SetModeById(16, 1);
  ShamanVillage_ObjArm(16, 0x4000, 0);
  ShamanVillage_ObjArm(17, 0x4000, 0);
  Actor_StartRepeatedMotion(15, 1);
  Func_0200498c((s32)MsgShamanMoapaGiveMeRoom);
  ShamanVillage_MessagePut(15, 0);
  ShamanVillage_CamSpeed(0xcccc, 0x1999);
  Func_020049fc()->motionFlags = 0;
  ShamanVillage_CamMove(0x780000, 0x320000, 0xe00000, 1);
  ShamanVillage_ObjSpeed(gSequenceParentWork.currentOwner, 0x19999, 0xcccc);
  ShamanVillage_ObjWalk(gSequenceParentWork.currentOwner, 120, 256);
  ShamanVillage_ObjTurn(gSequenceParentWork.currentOwner, 0xc000);
  ShamanVillage_ObjSpeed(gSequenceParentWork.currentOwner, 0x6666, 0x3333);
  Actor_GetById(gSequenceParentWork.currentOwner)->actionFlags &= ~1;
  ShamanVillage_ObjWalk(gSequenceParentWork.currentOwner, 120, 264);
  FieldEvent_Wait(1);
  Actor_GetById(gSequenceParentWork.currentOwner)->actionFlags |= 1;
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 259, 20)
  ShamanVillage_MessagePut(0x800f, 0);
  Actor_StartRepeatedMotion(17, 1);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(17, 259, 0)
  ShamanVillage_MessagePut(0x8011, 0);
  Actor_StartRepeatedMotion(16, 1);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(16, 259, 0)
  ShamanVillage_MessagePut(0x8010, 0);
  Motion_SetModeAndWaitAnimation(15, 3);
  ShamanVillage_MessagePut(0x800f, 0);
  GameFlag_SetBit(0x94f);
  Func_020048cc();
}
