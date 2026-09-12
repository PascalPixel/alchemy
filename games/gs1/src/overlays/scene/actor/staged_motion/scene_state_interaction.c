#include "types.h"

#define SceneState_SetFlag204AndConfigureRegion49_46 Func_02001280
#define SceneState_RunTablePairByActor22State Func_02001590
extern s32 Data_0200f748[];
extern s32 Data_0200f6cc[];
extern s32 Data_0200f7c4[];

void Func_02007242(void);
s32 Func_02004612(s32, s32, s32, s32);
void Func_02007246(s32);
void Func_020071ea(s32, s32, s32, s32, s32, s32);
void Func_02007276(void);
u8 *Func_0200757a(s32);
s32 Func_0200753a(s32);
void Func_02002a46(s32, s32);
void Func_02002a54(s32, s32);

void SceneState_SetFlag204AndConfigureRegion49_46(void)
{
    s32 p5, p6;
    Func_02007242();
    Func_02004612(0, 13, 10, 1);
    Func_02007246(0x204);
    p5 = 20;
    p6 = 50;
    Func_020071ea(49, 46, 8, 4, p5, p6);
    Func_02007276();
}

void SceneState_RunTablePairByActor22State(void)
{
    u8 *rec = Func_0200757a(22);
    if (Func_0200753a(0x823) != 0) {
        u8 *q = rec;
        s32 v;
        q += 100;
        v = *(s16 *)q;
        if (v == 1) {
            Func_02002a46((s32)Data_0200f748, (s32)Data_0200f6cc);
        } else if (v == 2) {
            Func_02002a54((s32)Data_0200f748, (s32)Data_0200f7c4);
        }
    }
}
