// Under -mthumb-slow-loads (26 October 2026): differ 6 of 413 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, each local pinned to each register,
// each call argument copied into its argument register (pinned) at the call
// or up to three statements before it, five minutes of permute; none reaches
// 0.
// Draft of Func_0811e3ac, from the listing (no ☀️ twin found): for a plan
// whose outcome is 15 to 17 and that has not failed, saves a battle object and
// shows a class's sprite in its place: the actor's own object when actor and
// first target are on opposite sides (single), else the first object of the
// other side (created when empty, and the whole side faded and released).
// Abilities 600 to 631 choose the class (9 for 628 and any other). The sprite
// loads into slot 7 from a fresh 32 KB buffer (single) or Data_02018000, then
// falls from 40 pixels up over 30 frames, turning, while the side fades back.
// Returns the saved object (Func_0811e7dc shows it, Func_0811e830 puts it
// back), or NULL. Reached from Func_0811ea0c and Func_0811f088.
// Near miss, differ 18 of 413 (2026-10-24), instruction order only: seven
// loads the native issues one slot earlier and two copies after the store or
// address they follow. Forms measured: the class chosen into a local and
// stored once (170 with a store per case: saved then outranks the gcse copy
// of &saved->unitId for r8), the side bit through a u8 (186 as one
// expression), separate locals for the actor's object and the shown object,
// single declared first (its slot above battleObject's), the first
// BattleObject_Reset call with one argument (the native leaves r1 as it is),
// and the alpha written (i << 8) | (16 - i).
#include "types.h"
#include "battle.h"
#include "battle_type.h"
#include "battle_session.h"
#include "battle_display.h"
#include "motobj.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "mem.h"

struct SavedBattleObj {
  struct BattleObj object;
  u32 *buffer;
  u8 unitId;
  u8 single;
  u8 reactivate;
  u16 classId;
};

extern u32 Data_02018000[];

void WaitFrames(s32 frames);
void *Heap_AllocEwram(s32 size);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
void BattleObject_Reset();
void Object_SetMode(struct MotionObject *object, s32 mode);
void Sound_PlayFar(s32 cue);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);
void BattleMotion_ReleaseObjectSlotByValue(s32 objectId);

struct SavedBattleObj *Func_0811e3ac(struct BattlePlan *plan,
                                     struct BattlePresentationWork *work) {
  s32 single;
  struct BattleObj *battleObject;
  struct SavedBattleObj *saved;
  u16 ids[14];
  struct MotionObject *object;
  struct MotionObject *actor;
  s32 classId;
  s32 count;
  s32 i;

  if ((u32)(plan->outcome - 15) > 2 || plan->failure != 0)
    return NULL;
  {
    u8 sides = (plan->actorId ^ plan->targetIds[0]) & 0x80;
    single = sides != 0;
  }
  saved = Heap_AllocEwram(sizeof(struct SavedBattleObj));
  saved->single = single;
  saved->reactivate = 0;
  switch (plan->abilityId) {
  case 600:
    classId = 117;
    break;
  case 601:
    classId = 137;
    break;
  case 602:
    classId = 182;
    break;
  case 603:
    classId = 299;
    break;
  case 604:
    classId = 296;
    break;
  case 605:
    classId = 278;
    break;
  case 606:
    classId = 316;
    break;
  case 607:
    classId = 317;
    break;
  case 608:
    classId = 185;
    break;
  case 609:
    classId = 158;
    break;
  case 610:
    classId = 150;
    break;
  case 611:
    classId = 311;
    break;
  case 612:
    classId = 212;
    break;
  case 613:
    classId = 323;
    break;
  case 614:
    classId = 191;
    break;
  case 615:
    classId = 159;
    break;
  case 616:
    classId = 162;
    break;
  case 617:
    classId = 155;
    break;
  case 618:
    classId = 263;
    break;
  case 619:
    classId = 197;
    break;
  case 620:
    classId = 215;
    break;
  case 621:
    classId = 99;
    break;
  case 622:
    classId = 196;
    break;
  case 623:
    classId = 160;
    break;
  case 624:
    classId = 281;
    break;
  case 625:
    classId = 163;
    break;
  case 626:
    classId = 302;
    break;
  case 627:
    classId = 188;
    break;
  case 629:
    classId = 242;
    break;
  case 630:
    classId = 203;
    break;
  case 631:
    classId = 218;
    break;
  default:
    classId = 9;
    break;
  }
  saved->classId = classId;
  if (!single) {
    saved->unitId = (plan->actorId ^ 0x80) & 0x80;
    if (BattleObject_Get(saved->unitId)->object == NULL) {
      saved->reactivate = 1;
      BattleObject_Reset(BattleObject_Get(saved->unitId));
      BattleObject_Get(saved->unitId)->scale = 0x13333;
    }
  } else {
    saved->unitId = plan->actorId;
  }
  work->actor = saved->unitId;
  actor = BattleObject_Get(plan->actorId)->object;
  Object_SetMode(actor, 3);
  ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)actor, 16);
  QueueIoWriteDelay2(0x04000050, 0x3f40);
  Sound_PlayFar(154);
  WaitFrames(10);
  if (single) {
    BattlePres_SetActorRecordMode(saved->unitId, 1);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++)
      BattlePres_SetActorRecordMode(ids[i], 1);
  }
  for (i = 0; i != 16; i++) {
    REG_BLDALPHA = (i << 8) | (16 - i);
    WaitFrames(1);
  }

  battleObject = BattleObject_Get(saved->unitId);
  object = battleObject->object;
  if (single) {
    ReleaseBattleObjectRecords(saved->unitId);
  } else {
    count = BattleParty_ListActorIds((saved->unitId != 0) + 1, ids);
    for (i = 0; i < count; i++) {
      ReleaseBattleObjectRecords(ids[i]);
      BattleMotion_ReleaseObjectSlotByValue(ids[i]);
    }
  }
  Iwram_CopyWords(saved, BattleObject_Get(saved->unitId), 44);
  BattleObject_Reset(BattleObject_Get(saved->unitId), single);
  BattleObject_Get(saved->unitId)->scale = 0x13333;
  if (single)
    saved->buffer = Heap_AllocEwram(0x8000);
  else
    saved->buffer = Data_02018000;
  i = ResourceSlot_LoadFar(7, saved->buffer,
                           BattleClass_GetSprite(saved->classId),
                           BattleClass_GetPalette(saved->classId));
  if (i != 0) {
    battleObject->resource = BattleClass_GetSprite(saved->classId) + 0x7000;
    if (!single)
      saved->buffer = NULL;
  } else {
    battleObject->resource = BattleClass_GetSprite(saved->classId);
    if (single)
      Heap_Release(saved->buffer);
    saved->buffer = (u32 *)i;
  }
  ids[0] = saved->unitId;
  ids[1] = 255;
  Func_0811b75c((s16 *)ids, 1, 0);
  object->y = 0x280000;
  object->angle = plan->targetIds[0] <= 7 ? 0 : 0x8000;
  BattlePres_SetActorRecordMode(saved->unitId, 1);
  for (i = 0; i <= 29; i++) {
    object->y = (29 - i) * 3 << 16;
    object->velocityY = 0;
    object->angle += 0x2222;
    if (i <= 15)
      REG_BLDALPHA = ((16 - i) << 8) | i;
    WaitFrames(1);
  }
  BattlePres_SetActorRecordMode(saved->unitId, 0);
  return saved;
}
