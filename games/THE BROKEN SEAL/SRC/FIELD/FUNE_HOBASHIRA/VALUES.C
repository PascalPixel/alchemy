#include "FUNE.H"

s32 SceneData_GetDifferenceOfPairSums(void)
{
    s32 a;
    s32 b;

    a = SceneData_GetValueByFirstSetFlag(0);
    a += SceneData_GetValueByFirstSetFlag(2);
    b = SceneData_GetValueByFirstSetFlag(1);
    b += SceneData_GetValueByFirstSetFlag(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (GameFlag_IsSet(base + i) != 0)
            return FuneHobashira_FlagValues[i];
    }
    return 0;
}
