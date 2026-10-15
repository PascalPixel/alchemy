#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct SceneWork {
    u8 unknown_000[0x1ac];
    s32 unknown_1ac;
};

extern char SceneId_Ov65aRoom1;
extern char SceneId_Ov65aRoom2;
extern char SceneId_Ov65aRoom3;
extern char SceneId_Ov65aRoom4;
extern char SceneId_Ov65aRoom5;
extern char SceneId_Ov65aRoom6;
extern const u8 Data_020013d0[];
extern const u8 Data_02001400[];
extern const u8 Data_02001430[];
extern const u8 Data_020014a8[];
extern const u8 Data_0200159c[];
extern const u8 Data_020015b4[];
extern const u8 Data_0200165c[];
extern const u8 Data_020016bc[];
extern const u8 Data_020016ec[];
extern const u8 Data_02001914[];
extern const u8 Data_02001920[];
extern const u8 Data_020019f8[];
extern const u8 Data_02001a1c[];
extern const u8 Data_02001aac[];
extern const u8 Data_02001b54[];
extern const u8 Data_02001bcc[];

void Func_02000dcc(void);
void Func_02000f2c(void);
void Func_02000fbc(void);
void Func_02000fd8(void);
void Func_0200102c(void);

const u8 *Func_02000318(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov65aRoom4)
        return Data_02001400;
    if (gPartyState.scene == (s32)&SceneId_Ov65aRoom5)
        return Data_02001430;
    return Data_020013d0;
}

const u8 *Func_02000358(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov65aRoom2)
        return Data_020014a8;
    return 0;
}
