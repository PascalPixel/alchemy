#include "types.h"

#define ObjectEffect_BeginContextEffect25 Func_08094368

s32 GameFlag_Set(s32);
void ObjectEffect_PrepareContextEffect(s32);

void ObjectEffect_BeginContextEffect25(void) {
    ObjectEffect_PrepareContextEffect(0x19);
    GameFlag_Set(0x121);
}
