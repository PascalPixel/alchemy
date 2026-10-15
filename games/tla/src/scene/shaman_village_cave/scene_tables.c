#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02001060();
void GameFlag_SetBit();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Func_0200004c(s32 arg0, s32 arg1) {
  if (arg1 != 8)
    goto L_02008072;
  Call1((void (*)())GameFlag_SetBit, 0x9a9);
  Call6((void (*)())Func_02001060, 0, 2, 1, 1, 2, arg1);
L_02008072:
  if (arg1 != 10)
    goto L_02008094;
  Call1((void (*)())GameFlag_SetBit, 0x9aa);
  Call6((void (*)())Func_02001060, 0, 32, 1, 1, 14, 38);
L_02008094:
  return;
}
