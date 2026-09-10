#include "types.h"

#define SceneState_WaitUntilWordC41cIs22 Func_02000178
#define SceneDialogue_RunFlagGatedPromptInteraction Func_02001d64
#define SceneState_SetHalfword1000To9 Func_02002f94
extern u32 Data_0200c41c;
extern s16 Data_02000240[];
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;
extern u16 Data_02001000;

void Func_02003c86(s32);
void Func_02003c9a(s32);
void Func_02005b3c(void);
void Func_02005994(s32, s32);
void Func_02005aca(s32);
void Func_02005ae2(s32, s32);
s32 Func_02005a04(s32);
s32 Func_02005a14(s32);
s32 Func_020059f6(s32);
void Func_02005a44(s32);
void Func_02005b1a(s32);
s32 Func_02005b2a(s32, s32);
s32 Func_02005aaa(s32, s32);

void SceneState_WaitUntilWordC41cIs22(void)
{
    s32 i;

    Func_02003c86(10);
    i = 0;
    if (Data_0200c41c != 22) {
        do {
            Func_02003c9a(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (Data_0200c41c != 22);
    }
}

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b)
{
    s32 v;
    s32 id;
    s32 r;

    Func_02005b3c();
    Func_02005994(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02005aca(id);
    Func_02005ae2(a, 0);
    if (Func_02005a04(b + 512) != 0) {
        return 2;
    }
    if (Func_02005a14(b + 520) != 0) {
        r = Func_020059f6(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_02005a44(b + 520);
    Func_02005b1a((s32)&Value_0000207c);
    Func_02005b2a(a, 0);
    return Func_02005aaa(0, 0);
}

void SceneState_SetHalfword1000To9(void)
{
    u16 *p = &Data_02001000;
    s32 v = 9;

    *p = v;
}
