#include "types.h"
#include "adventure.h"
#include "object_runtime.h"
#include "flag_call.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void Object_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Func_0200129c(s32 id, s32 x, s32 z);

void Func_0200115c(void) {
  if (!Flag_IsSet(0x230)) {
    struct ObjectRuntime *object = Object_GetById(9);
    struct ObjectRuntime *owner = Object_GetById(gAdventure.currentOwner);
    s32 other;

    Flag_Set(0x230);
    Call3((void (*)())Func_0200129c, 9, 296 << 16, 160 << 16);
    object->speedLimit = 0x18000;
    object->velocityY = 0xf0000;
    Call3((void (*)())ObjectMotion_SetPositionAndCommit, 9, 344,
          ((owner->z >> 19) << 3) + 24);
    ObjectMotion_EnableActionAndResetMotion(8);
    object->speedLimit = 0x10000;
    other = 9;
    /* FAKEMATCH: the native loads the second argument before the owner; a
     * direct call and a forwarding inline load the owner first. */
    __asm__ volatile("" : "+r"(other));
    Object_TurnToFaceEachOther(gAdventure.currentOwner, other, 0);
  }
}
