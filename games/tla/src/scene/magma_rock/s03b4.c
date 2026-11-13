#include "../shared/push.h"
#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

extern s32 gData_02003d10[];

DEFINE_BLOCK_PUSH_ONTO(Func_020003b4, gData_02003d10, MapGrid_CheckCollision,
                       Func_02003a54)

#include "../../field/map/cell_fill_byte.c"
