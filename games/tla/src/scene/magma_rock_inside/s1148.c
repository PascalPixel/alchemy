#include "local.h"

struct MagmaRockInsideWork {
    s32 unknown_00;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_10;
    struct ObjectRuntime *target;
};

void Func_02000ed0(struct ObjectRuntime *object);
void Func_02004a5c(s32 x, s32 y, s32 z, s32 kind);
void Func_02004b18(void);
void Func_02004bd4(void);
void Func_02004c98(struct ObjectRuntime *object, s32 a);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e90(s32 value);

void Func_02001148(void)
{
    struct MagmaRockInsideWork *work = Ram_HeapSlots->effect_work;
    struct ObjectRuntime *object;

    Func_02004d40();
    Func_02004e90(0);
    Battle_WaitMode0(10);
    object = work->target;
    object->update = NULL;
    Object_SetPartAttribute(object, 7);
    Battle_WaitMode0(3);
    Object_SetPartAttribute(work->target, 0);
    Battle_WaitMode0(1);
    Func_02000ed0(work->target);
    Func_02004d48();
}

void Func_02001190(s32 mode, s32 cue)
{
    struct MagmaRockInsideWork *work = Ram_HeapSlots->effect_work;

    if (cue == 0)
        Func_02004b18();
    if (mode == 2) {
        struct ObjectRuntime *object = work->target;

        Func_02004a5c(object->x, object->y + 0xf0000, object->z, 1);
    }
    if (mode == 3) {
        struct ObjectRuntime *object = work->target;
        struct ObjectRuntime *target;

        Func_02004a5c(object->x, object->y + 0xf0000, object->z, 8);
        target = work->target;
        target->x = 0;
        target->y = 0;
        target->z = 0;
    }
    if (cue == 0x3e7)
        Func_02004bd4();
    if (cue == 178)
        Func_02004c98(work->target, 3);
}
