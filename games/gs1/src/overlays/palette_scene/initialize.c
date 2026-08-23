#include "palette_scene.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_020019f0();
void Func_02001a14();
void Func_02001ae0(s16);
void Func_02001b0a(s32, s32, s32);

void PaletteScene_Initialize(void) {
    void *scene;

    scene = *(void **)0x03001EBC;
    Func_020019f0();
    Func_02001b0a(0, 0, 0);
    Func_02001ae0(M2C_FIELD(scene, s16 *, 0x16C));
    Func_02001a14();
}
