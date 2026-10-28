#include "magma_rock.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "scene_ids.h"
#include "object_runtime.h"
#include "call.h"

extern s16 gData_02003d98[];
extern s16 gData_02003dac[];
extern s16 gData_02003dd2[];
extern s16 gData_02003de4[];
extern s16 gData_02003de8[];

void Func_02003aa4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c14(s16 *table);
void Func_0200074c(struct ObjectRuntime *object);

void Func_02000940(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gAdventure.scene == (s32)&SceneId_Ov691Room1) {
    Func_02003aa4(98, 18, 4, 3, 78, 21);
    Func_02003aa4(98, 24, 4, 3, 78, 24);
    Func_02003c14(gData_02003d98);
  } else if (gAdventure.scene == (s32)&SceneId_Ov691Room2) {
    if (gAdventure.entrance <= 3) {
      Func_02003c14(gData_02003dac);
    } else if (gAdventure.entrance <= 7) {
      Func_02003c14(gData_02003dd2);
    } else if (gAdventure.entrance <= 11) {
      Call6((void (*)())Func_02003aa4, 79, 46, 2, 1, 79, 36);
      Func_02003c14(gData_02003de4);
    } else {
      if (((s16 *)&work->target->z)[1] == 600)
        Call6((void (*)())Func_02003aa4, 104, 38, 1, 1, 103, 38);
      else
        Call6((void (*)())Func_02003aa4, 94, 44, 5, 1, 99, 38);
      Func_02003c14(gData_02003de8);
    }
  }
  Func_0200074c(*(struct ObjectRuntime **)(event + 0xcbc));
}
