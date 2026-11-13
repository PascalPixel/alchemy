#include "../shared/push.h"
#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

extern s32 gMagmaRock_BlockPushes[];

DEFINE_BLOCK_PUSH_ONTO(MagmaRock_PushBlockOnto, gMagmaRock_BlockPushes,
                       MapGrid_CheckCollision, Actor_SelectAnimation)

#include "../../field/map/cell_fill_byte.c"
