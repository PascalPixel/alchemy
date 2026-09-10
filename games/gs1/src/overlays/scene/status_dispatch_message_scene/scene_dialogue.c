#include "types.h"

#define SceneDialogue_RunActor10MessageByFlag962 Func_020002a0
#define SceneDialogue_RunActor13MessageByFlag962 Func_020002e4
#define SceneDialogue_ShowLine2118WithActor15Steps Func_0200050c
void Func_02002e98(void);
s32 Func_02002e66(s32 flag);
void Func_02002f48(s32 msg);
void Func_02002f60(s32 id, s32 arg1);
void Func_02002f58(s32 msg);
void Func_02002f80(s32 id, s32 arg1);
void Func_02002ecc(void);
void Func_02002edc(void);
s32 Func_02002eaa(s32 flag);
void Func_02002fc2(s32 id, s32 arg1, s32 arg2);
void Func_02002f98(s32 msg);
void Func_02002fb0(s32 id, s32 arg1);
void Func_02002fa8(s32 msg);
void Func_02002fc0(s32 id, s32 arg1);
void Func_02002f1c(void);
void Func_02003104();
void Func_020031aa_a();
void Func_020031aa_b();
void Func_02002164();
void Func_02002174();
void Func_020031a0();
void Func_02003198();
void Func_0200218a();
void Func_020021a8();
void Func_0200314c();

/* Contiguous unnamed leaf-owner run for resource_3b9. */

void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Func_02002e98();
    if (Func_02002e66(0x962)) {
        Func_02002f48(0x2251);
        Func_02002f60(10, 0);
    } else {
        Func_02002f58(0x2057);
        Func_02002f80(10, 0);
    }
    Func_02002ecc();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Func_02002edc();
    if (Func_02002eaa(0x962)) {
        Func_02002fc2(13, 258, 40);
        Func_02002f98(0x2254);
        Func_02002fb0(13, 0);
    } else {
        Func_02002fa8(0x205c);
        Func_02002fc0(13, 0);
    }
    Func_02002f1c();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Func_02003104();
    Func_020031aa_a(0x2118);
    Func_02002164(15);
    Func_020031aa_b(15, 0, 20);
    Func_02002174(15);
    Func_020031a0(15, 3);
    Func_02003198(15, 0);
    Func_0200218a(15);
    Func_020021a8(15, 20480);
    Func_0200314c();
}
