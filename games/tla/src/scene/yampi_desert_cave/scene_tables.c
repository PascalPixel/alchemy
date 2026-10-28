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
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020018e8();
void GameFlag_SetBit();

void Func_02000cec(void) {
  Value1((s32 (*)())GameFlag_SetBit, 0x271);
  Call2((void (*)())Func_020018e8, 17, 17);
  return;
}

void Func_02001870();
void Func_02001878();
void Func_020018d0();
void Func_02001930();

void Func_02000d04(void) {
  ((void (*)())Func_02001870)();
  Call1((void (*)())Func_02001930, 0);
  Call3((void (*)())Func_020018d0, gSequenceParentWork.currentOwner, 0x101, 32);
  ((void (*)())Func_02001878)();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }
