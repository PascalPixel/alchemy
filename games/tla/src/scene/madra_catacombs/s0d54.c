#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct SceneWork {
    u8 unk000[0x1ac];
    s32 unk1ac;
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

s32 Func_02000d54(void)
{
    ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 0x204;
    if (gPartyState.scene == (s32)&SceneId_Ov65aRoom1)
        Func_02000dcc();
    else if (gPartyState.scene == (s32)&SceneId_Ov65aRoom3)
        Func_02000f2c();
    else if (gPartyState.scene == (s32)&SceneId_Ov65aRoom4)
        Func_02000fbc();
    else if (gPartyState.scene == (s32)&SceneId_Ov65aRoom5)
        Func_02000fd8();
    else if (gPartyState.scene == (s32)&SceneId_Ov65aRoom6)
        Func_0200102c();
    return 0;
}
