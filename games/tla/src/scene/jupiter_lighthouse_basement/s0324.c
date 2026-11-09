#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "frame.h"
#include "field_sprite.h"
#include "event_inline.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct CellTarget {
  u8 unk00[6];
  s16 x;
  s16 z;
};

s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_0200015c(struct Actor *object);
void Func_020004dc(s32 mode);
void Func_02001d3c(s32 id);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

s32 Func_02000324(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room1) {
    Func_020004dc(1);
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 4)
      Func_02001d3c(gMainRoutineWork.currentOwner);
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 6)
      Func_02001d3c(gMainRoutineWork.currentOwner);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2) {
    *(volatile u16 *)0x04000052 = 0x1008;
    *(volatile u16 *)0x04000050 = 0x3f10;
    Func_02000298();
  }
  return 0;
}
