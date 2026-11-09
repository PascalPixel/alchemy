#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "event_inline.h"

void Func_0200097c(void);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001120(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  Func_02001120(srcX, srcY, width, height, destX, destY, layer);
}

void Func_0200077c(void) {
  if (GameFlag_Test(0x820) && GameFlag_Test(0x9b1) && !GameFlag_Test(0x9ad))
    Func_0200097c();
}

void Func_020007ac(void) {
  struct FieldActor *object;
  s32 moved;

  MapCopy(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Test(63)) {
    object =
        ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
    Func_02001010();
    Event_ResolvePendingActions(0);
    moved = 0;
    if ((object->x >> 20) == 7 && (object->z >> 20) == 23) {
      Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 104, 376);
      moved = 1;
    }
    Actor_SetHorizontalPositionWithTerrain(14, 120 << 16, 376 << 16);
    wait(1);
    Actor_Jump(14, 4, 10);
    GameFlag_SetBit(0x300);
    if (moved)
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
    Func_02001018();
  }
}

void Func_0200085c(void) { MapCopy(9, 23, 1, 1, 4, 10, 0); }

void Func_0200087c(void) { MapCopy(9, 23, 1, 1, 6, 9, 0); }

void Func_0200089c(void) { MapCopy(9, 23, 1, 1, 6, 6, 0); }

void Func_020008bc(void) { MapCopy(12, 5, 1, 1, 12, 6, 0); }

void Func_020008dc(void) { MapCopy(9, 23, 1, 1, 13, 7, 0); }

void Func_020008fc(void) { MapCopy(9, 23, 1, 1, 14, 6, 0); }

void Func_0200091c(void) { MapCopy(9, 23, 1, 1, 21, 6, 0); }

void Func_0200093c(void) { MapCopy(9, 23, 1, 1, 22, 6, 0); }

void Func_0200095c(void) { MapCopy(9, 23, 1, 1, 23, 7, 0); }
