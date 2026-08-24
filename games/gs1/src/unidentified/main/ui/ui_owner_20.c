#include "types.h"

s32 GameFlag_Set(s32);
void ObjectEffect_PrepareContextEffect(s32);

void Func_08094368(void) {
    ObjectEffect_PrepareContextEffect(0x19);
    GameFlag_Set(0x121);
}
