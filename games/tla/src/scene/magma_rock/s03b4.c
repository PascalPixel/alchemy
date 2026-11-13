#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

extern s32 gData_02003d10[];

/* VIEW(s03b4): routine Actor_FindInCell as struct StagedActor *Actor_FindInCell(struct FieldPosition *position, struct StagedActor *self); module/scene/magma_rock.h has struct Actor *Actor_FindInCell(s32 *position, struct Actor *self). */
extern struct StagedActor *Actor_FindInCell__s03b4(struct FieldPosition *position,
                                     struct StagedActor *self) __asm__("Actor_FindInCell");
/* VIEW(s03b4): routine Func_02003a54 as void Func_02003a54(struct StagedActor *object, s32 mode); module/scene/magma_rock.h has void Func_02003a54(struct Actor *object, s32 a). */
extern void Func_02003a54__s03b4(struct StagedActor *object, s32 mode) __asm__("Func_02003a54");
/* VIEW(s03b4): routine Func_02003aac as s32 Func_02003aac(struct StagedActor *object, struct FieldPosition *position); module/scene/staff_credit.h has void Func_02003aac(struct Actor *object). */
extern s32 Func_02003aac__s03b4(struct StagedActor *object, struct FieldPosition *position) __asm__("Func_02003aac");

void Func_020003b4(void) {
  struct FieldPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = ((struct StagedActor *)Actor_GetById(gMainRoutineWork.currentOwner));
  dir = lead->facing >> 12;
  step = gData_02003d10[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell__s03b4(&dst, lead);
  if (next == 0)
    return;

  step = gData_02003d10[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell__s03b4(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell__s03b4(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gData_02003d10[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (Func_02003aac__s03b4(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Func_02003a54__s03b4(lead, 8);
  rate = 0x3333;
  System_Wait(15);
  Event_PlaySound(185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  FieldActor_SetMoveTarget(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  FieldActor_SetMoveTarget(lead, dst.x, dst.y, dst.z);
  Actor_WaitForMove(next);
  Event_NoOp();
  next->x.fixed = dst.x;
  {
    /* FAKEMATCH(temporary): z read ahead of velocityX's store; as dst.z at its
     * store, 4 of 175 instructions differ in tla-en. */
    s32 z = dst.z;

    next->velocityX = 0;
    next->z.fixed = z;
    next->velocityZ = 0;
  }
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part.pixel << 16;
  lead->z.fixed = lead->z.part.pixel << 16;
  Func_02003a54__s03b4(lead, 1);
}

#include "../../field/map/cell_fill_byte.c"
