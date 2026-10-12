#include "room.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6acRoom3;
extern const u8 Data_02005c84[];
extern const u8 Data_02005ce0[];
extern const u8 Data_02005d3c[];

void Func_02005548(s32 scene, s32 a1);
void Func_0200328c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);

s32 Func_0200140c(void)
{
    struct PartyState *party;
    s32 flag = 0xa30;

    /* FAKEMATCH: ⚓️ builds the flag before it loads the party state's address */
    asm volatile("" : "+l"(flag));
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party state's address before the flag test */
    asm("" : "+l"(party));
    if (GameFlag_Test(flag))
        Func_02005548(party->map + ((s32)&SceneId_Ov6acRoom3 - (s32)&SceneId_Ov6a8Room2), 0x5000000);
    return 0;
}

void Func_02001448(void)
{
    s32 id = 11;

    /* FAKEMATCH: ⚓️ builds the id after the other arguments */
    asm("" : "+l"(id));
    Func_0200328c(id, 12, 13, 0x4000, Data_02005c84);
}

void Func_02001468(void)
{
    s32 id = 19;

    /* FAKEMATCH: ⚓️ builds the id after the other arguments */
    asm("" : "+l"(id));
    Func_0200328c(id, 21, 22, 0x4000, Data_02005ce0);
    id = 3;
    /* FAKEMATCH: ⚓️ builds the index after the value */
    asm("" : "+l"(id));
    Func_020027ac(id, 131);
}

void Func_02001490(void)
{
    s32 id = 14;

    /* FAKEMATCH: ⚓️ builds the id after the other arguments */
    asm("" : "+l"(id));
    Func_0200328c(id, 15, 16, 0x4000, Data_02005d3c);
}
