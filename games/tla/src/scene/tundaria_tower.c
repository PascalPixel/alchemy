#include "types.h"
#include "field_event.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

void Func_02002f80();
void Func_02000338();

void Func_02000404(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = ((struct FieldActor *)Actor_GetById(row[0]));
  animation = row[1];
  flag = row[2];

  Actor_SetSingleChildFlags(object, 0);
  Func_02002f80(object, (u16)animation + GameFlag_Test((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  Func_02000338(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "scene_ids.h"

extern const u8 gData_020033d0[];
extern const u8 gData_020033f0[];

const struct ActorPlacement *Func_02000488(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct ActorPlacement *)gData_020033d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct ActorPlacement *)gData_020033f0;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02003618[];
extern const u8 gData_02003648[];
extern const u8 gData_02003660[];
extern const u8 gData_02003690[];
extern const u8 gData_02003708[];
extern const u8 gData_020037c8[];

const struct Trigger *Func_020004c8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom2)
    return (const struct Trigger *)gData_02003618;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom3)
    return (const struct Trigger *)gData_02003648;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom4)
    return (const struct Trigger *)gData_02003660;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom5)
    return (const struct Trigger *)gData_02003690;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct Trigger *)gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct Trigger *)gData_020037c8;
  return (const struct Trigger *)gData_02003618;
}

void Func_02003048(void);
void Func_02003050(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void TundariaTower_02002fe8(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

static inline void TundariaTower_02002fc0(s32 a0, s32 a1, s32 a2, s32 a3,
                                          s32 a4, s32 a5) {
  Map_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000548(void) {
  Func_02003048();
  TundariaTower_02002fe8(19, 55, 7, 8, 24, 46);
  TundariaTower_02002fc0(24, 102, 24, 110, 7, 8);
  GameFlag_SetBit(2370);
  Func_02003050();
}

void Func_0200058c(void) {
  Func_02003048();
  TundariaTower_02002fe8(37, 55, 7, 8, 32, 46);
  TundariaTower_02002fc0(32, 102, 32, 110, 7, 8);
  GameFlag_SetBit(2371);
  Func_02003050();
}
