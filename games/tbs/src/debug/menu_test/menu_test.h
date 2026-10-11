#ifndef MENU_TEST_H
#define MENU_TEST_H

#include "types.h"
#include "keystate.h"
#include "owner_state.h"
#include "ownerval.h"
#include "game_state.h"
#include "fx_scene.h"
#include "fieldobj.h"
#include "text/msg_ids.h"

#include "character.h"

extern struct BattleFxScene *gEffectWork;
extern struct SceneEvent	 MenuTest_Events[];
extern struct SceneEntrance	 MenuTest_Entrances[];
extern u32					 MenuTest_Exits[];
extern struct ScenePlacement MenuTest_Placements[];
extern s32					 MenuTest_SlotOffsets;
extern s32					 MenuTest_BenchmarkCharacter;

u8	 Engine_TaskWait();

#endif
