#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "call.h"

void Func_02000df8();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void PartyInventory_GiveItem(s32 item, s32 count);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);
void Func_02000c90(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02000aac(void)
{
    Func_02000d28();
}

void Func_02000ab4(void)
{
    Func_02000d30();
}

void Func_02000abc(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    if (mode == 1)
        Func_02000c90(a, b, c, d, e, f);
}

void Func_02000ae0(s32 mode, s32 id, s32 x, s32 z)
{
    struct ObjectRuntime *object;
    s32 scale;

    object = Object_GetById(id);
    if (mode == 1) {
        Func_02000df8(id, x << 16, z << 16);
        object->scale_x = 0;
        object->scale_y = 0;
    }
    if (mode == 2) {
        /* FAKEMATCH: ⚓️ loads the scale before it builds the bound. */
        __asm__ volatile("" : "+l"(object));
        while (object->scale_x < 0x10000) {
            WaitFrames(1);
            scale = object->scale_x;
            /* FAKEMATCH: and loads it again before it builds the step. */
            __asm__ volatile("" : "+l"(scale));
            scale += 0x51e;
            object->scale_x = scale;
            object->scale_y = scale;
        }
        object->scale_x = 0x10000;
        object->scale_y = 0x10000;
    }
}

void Func_02000b50(s32 mode, s32 id, s32 x, s32 z)
{
    struct ObjectRuntime *object;

    object = Object_GetById(id);
    if (mode == 2) {
        Call3((void (*)())Func_02000df8, id, x << 16, z << 16);
        object->y = 0x200000;
        object->motion_flags = 3;
        object->gravity = 0x170a;
        Call1((void (*)())WaitFrames, 50);
    }
}
