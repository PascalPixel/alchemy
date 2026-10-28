#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_Stop(s32 id);
void Actor_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Func_0200129c(s32 id, s32 x, s32 z);

void Func_0200115c(void) {
  if (!Flag_IsSet(0x230)) {
    struct Actor *object = Actor_GetById(9);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 other;

    Flag_Set(0x230);
    Call3((void (*)())Func_0200129c, 9, PIXELS(296), PIXELS(160));
    object->speedLimit = 0x18000;
    object->velocityY = PIXELS(15);
    Call3((void (*)())Actor_SetPositionAndCommit, 9, 344,
          ((owner->z >> 19) << 3) + 24);
    Actor_Stop(8);
    object->speedLimit = 0x10000;
    other = 9;
    /* FAKEMATCH: the native loads the second argument before the owner; a
     * direct call and a forwarding inline load the owner first. */
    __asm__ volatile("" : "+r"(other));
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, other, 0);
  }
}
