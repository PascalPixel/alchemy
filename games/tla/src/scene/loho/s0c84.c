#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern char SceneId_Ov6A6Room1;
extern char SceneId_Ov6A6Room2;
extern char MsgLoho57;
extern const u8 gData_02001ae0[];
extern const u8 gData_02001aec[];
extern const u8 gData_02001dd4[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001348_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02000c84(void) {
  GameFlag_SetBit(0x201);
  Actor_GetById(22)->collisionFlags &= 0xfe;
  if (GameFlag_Test(0xff7) == 0)
    Func_02001348_(66, PIXELS(520), PIXELS(136));
}

const u8 *Func_02000cc0(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1)
    return gData_02001aec;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001dd4;
  return gData_02001ae0;
}

void Func_02000d00(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

  if (facing == 0xc000)
    FieldMessage_Show((s32)&MsgLoho57, 1);
}
