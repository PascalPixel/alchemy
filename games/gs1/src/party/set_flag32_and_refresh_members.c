#include "game_flags.h"

#define Party_SetFlag32AndRefreshMembers Func_08077f40

s32 Func_08077428(s32);
void Owner_RefreshDerivedData(s32 arg0);

void Party_SetFlag32AndRefreshMembers(void) {
    GameFlag_Set(0x20);
    Owner_RefreshDerivedData(0);
    Owner_RefreshDerivedData(1);
    Owner_RefreshDerivedData(5);
    Func_08077428(0);
    Func_08077428(1);
    Func_08077428(5);
}
