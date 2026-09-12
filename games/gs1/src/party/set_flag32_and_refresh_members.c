#include "scene.h"
#include "game_flags.h"
#include "types.h"

/* party/set_flag32_and_refresh_members.c */
void Owner_RefreshDerivedData(s32 arg0);
s32 Party_Check(s32);

void Party_SetFlag32AndRefreshMembers(void)
{
    GameFlag_Set(0x20);
    Owner_RefreshDerivedData(0);
    Owner_RefreshDerivedData(1);
    Owner_RefreshDerivedData(5);
    Party_Check(0);
    Party_Check(1);
    Party_Check(5);
}
