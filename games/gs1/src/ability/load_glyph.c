#include "types.h"

#define Ability_LoadGlyph Func_0801a3d0

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0801a404(u8, s32, s32, s32, s32);
void *Ability_GetData(s32);

/* 取得項目の+4値を先頭引数として転送する。 */
void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Func_0801a404(
        FIELD_AT_OFFSET(Ability_GetData(arg0), u8 *, 4),
        arg1,
        arg2,
        arg3,
        arg4);
}
