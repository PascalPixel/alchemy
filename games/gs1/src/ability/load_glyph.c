#include "types.h"
#include "scene.h"

void *Ability_GetData(s32);

/* 取得項目の+4値を先頭引数として転送する。 */
void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Sys_SetRange(
        FIELD_AT_OFFSET(Ability_GetData(arg0), u8 *, 4),
        arg1,
        arg2,
        arg3,
        arg4);
}
