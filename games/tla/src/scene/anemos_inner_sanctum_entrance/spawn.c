#include "../shared/actors.h"
#include "anemos_inner_sanctum_entrance.h"
#include "field.h"

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);

DEFINE_SCENE_CREATE_IMPORTED_ACTOR(Scene_CreateImportedActor, 0x22)
