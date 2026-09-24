#include "TYPES.H"

struct BattleActorDefinition;
struct BattleActorDefinition *Owner_GetStateFar(s32);

s32 Summon_FindSlot(void)
{
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 0x80;
        if (((u8 *)Owner_GetStateFar(id))[0x12A] == 0)
            break;
    }
    if (i == 6)
        return -1;
    return id;
}
