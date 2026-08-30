#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunConditionalSceneSetup:
 * 20 calls, a resource-dependent branch, actor state, and workspace updates
 * across the complete callable owner. */

#define FieldScene_RunConditionalSceneSetup Func_020011c4

void Func_02002e32();
void *Func_02003d8a();
void Func_02003da8();
void *Func_02003dbc();
s32 Func_02003e1a();
void Func_02003e62();
void Func_02003e70();
void Func_02003e74();
void Func_02003e78();
void Func_02003e9a();
void Func_02003eb2();
void Func_02003f00();
void *Func_02003f06();
void Func_02003f1a();
void Func_02003f26();
void Func_02003f64();
void Func_02003f6c();
void Func_02003f78();
void Func_02003f84();

void Func_020011c4(void)
{
    void *scene;

    Func_02003dbc();
    scene = (void *)Func_02003d8a(0x8a4);
    if (scene != 0) {
        Func_02003e62(17, 0, 40);
        Func_02003e78(0x206f);
        Func_02002e32(17);
        Func_02003eb2(17, 12288, 20);
    } else {
    Func_02003e74(17, 2);
    Func_02003e9a(0x206d);
    Func_02003eb2(17, 0);
    scene = Func_02003f06();
    *(u8 *)((u8 *)(scene) + 85) = 0;
    Func_02003da8(1);
    Func_02003f00(0x00066666, 0xcccc);
    Func_02003f1a(35389440, -1, 13631488, 1);
    Func_02003f26();
    *(u32 *)(*(u32 *)0x03001ebc + 448) = 512;
    *(u32 *)(*(u32 *)0x03001ebc + 456) = 32;
    Func_02003f78();
    Func_02003f84();
    if (Func_02003e1a(0x8a3) != 0) {
        Func_02003f64(70);
    } else {
        Func_02003f6c(7);
    }
    }
    Func_02003e70();
}
