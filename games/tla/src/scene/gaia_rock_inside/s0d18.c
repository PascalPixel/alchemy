#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct CellPoint {
  s16 position[2];
};

extern const struct CellPoint gData_020051d8[12];
extern const u8 gData_020051b8[];
extern const u8 gData_02005118[];
extern const u8 gData_02005168[];

void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_02004e5c(void);
void Func_02004fdc(void);

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02000d18(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;
  s32 height = 8;
  s32 sourceX = 117;
  s32 sourceY = 20;

  Func_02004ff4(104, sourceY, 1, height, height, sourceX, sourceY);
  if (GameFlag_Test(0x9c4))
    return;
  count = 0;
  for (i = 0; i <= 11; i++) {
    if (GetMapCellCollision(2, gData_020051d8[i].position[0] << 20,
                      gData_020051d8[i].position[1] << 20) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_Get(15);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  if (leader->z >> 19 > 53 && leader->x >> 19 > 83 && leader->x >> 19 <= 91) {
    leader->speedLimit = 0x20000;
    Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
    Actor_SetScript(gMainRoutineWork.currentOwner,
                                            gData_020051b8);
  }
  Actor_SetScript(15, gData_02005118);
  FieldEvent_Wait(60);
  Actor_SetScript(15, gData_02005168);
  FieldEvent_Wait(30);
  MapGrid_CopyCellsLayered(117, 20, 7, 8, 104, 20);
  MapGrid_CopyCellsLayered(111, 12, 1, 6, 111, 20);
  {
    s32 destX = 40;
    s32 destY = 84;
    MapGrid_CopyCellAttributes(49, destY, 8, 8, destX, destY);
  }
  FieldEvent_Wait(30);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1ea0000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  Event_PlaySound(204);
  guard->motionFlags = 3;
  ((struct BoulderSprite *)guard->animation)->layer = 3;
  FieldEvent_Wait(10);
  Actor_SetPosition(15, 0, 0);
  Func_02004fdc();
  Event_SetFlag(0x9c4);
  FieldEvent_SetExitRequest(9);
}
