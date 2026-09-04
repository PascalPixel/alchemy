#include "types.h"

#define ObjectEffect_BeginContextEffect26 Func_08094354

s32 GameFlag_Set(s32);
void ObjectEffect_PrepareContextEffect(s32);

void ObjectEffect_BeginContextEffect26(void) {
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_Set(0x120);
}
