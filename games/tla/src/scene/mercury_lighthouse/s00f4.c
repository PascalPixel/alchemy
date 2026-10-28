#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "field_sprite.h"

extern u32 gData_020007e4;
extern u32 gFrameCount;
extern const u8 gData_02000618[];

u32 Random16Far(void);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000540(struct Actor *object, s32 mode);
void Func_02000548(struct Actor *object, const void *script);
struct Actor *Func_02000550(s32 type, s32 x, s32 y, s32 z);

void Func_020000f4(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 phase;
  s32 x;
  s32 z;

  Actor_SetSpritePriority(8, 3);
  Actor_SetSpritePriority(9, 3);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  phase = gFrameCount & 15;
  if (phase == 0) {
    object = Func_02000550(0x2a2, 0, 0, 0);
    if (object != NULL) {
      sprite = object->animation;
      x = Random16Far() * 40 + 0x32c0000;
      /* FAKEMATCH(temporary): z is built before the motion flag and x are
       * stored, as in the native; as object->z = 0x2b80000, 12 of 76
       * instructions differ in tla-en. */
      z = 0x2b80000;
      object->motionFlags = phase;
      object->x = x;
      object->y = 0xa00000;
      object->z = z;
      Actor_SetSingleChildFlags(object, 0);
      object->priorityFlags &= ~1;
      sprite->priority = 2;
      Func_02000548(object, gData_02000618);
      Func_02000540(object, 1);
    }
  }
  gData_020007e4++;
}
