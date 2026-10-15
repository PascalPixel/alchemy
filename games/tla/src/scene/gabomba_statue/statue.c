#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct GateEventWork {
    u8 unknown_000[362];
    u16 gate_timer;
};

struct FieldPart {
    s16 kind;
};

struct FieldAnimation {
    u8 unknown_00[40];
    struct FieldPart *part;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020043a4(s32 value);
void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
void Battle_WaitMode0(s32 frames);
s32 Func_020053a0(void);
void Func_02005784(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_0200193c(void);
void ObjectDispatch_ApplyValueToChildren(struct ObjectRuntime *object, s32 value);
void Func_02001a6c(s32 value)
{
    Func_020043a4(value);
    if (value == 1)
        ((struct GateEventWork *)Ram_HeapSlots->event_work)->gate_timer = 200;
}

void Func_02001a8c(void)
{
    struct ObjectRuntime *object = Object_GetById(15);

    Func_020056b4(object);
    Func_0200580c(0);
    Battle_WaitMode0(20);
    Func_02005854(214);
    Func_02005624(object, 5);
    object->scale_y = 0x10000;
    Func_020056bc();
}

void Func_02001ac0(void)
{
    if (Func_020053a0() == 0)
        ((struct GateEventWork *)Ram_HeapSlots->event_work)->gate_timer = 201;
}

void Func_02001adc(s32 id)
{
    Func_02005784(id);
    Object_SetPartAttribute(Object_GetById(id), 15);
    ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(id)->priority_flags |= 2;
}

void Func_02001b0c(void)
{
    if (GameFlag_Test(265)) {
        if (!GameFlag_Test(512))
            Func_0200193c();
    } else {
        GameFlag_SetBit(512);
    }
}
void Func_02001b38(void)
{
    u32 i = 0;

next:
    {
        s32 id = i + 8;
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02001adc(id);
        object->motion_flags = 0;
        object->y = 0x240000;
        i++;
    }
    if (i < 1)
        goto next;
    Func_02001b0c();
}

void Func_02001b68(void)
{
    u32 i = 0;

next:
    {
        s32 id = i + 29;
        struct ObjectRuntime *object = Object_GetById(id);

        Func_02001adc(id);
        object->motion_flags = 0;
        object->y = 0x240000;
        i++;
    }
    if (i < 2)
        goto next;
    Func_02001b0c();
}
void Func_02001b98(void)
{
    s32 value = 16;
    s32 open = 0;
    u8 *event = Ram_HeapSlots->event_work;
    s32 i;

    if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
        open = 1;
        value = 0;
    }
    i = 8;
next:
    {
        struct ObjectRuntime *object = Object_GetById(i);

        if (object != NULL && object->animation_kind == 1) {
            struct FieldPart *part = ((struct FieldAnimation *)object->animation)->part;

            if (part != NULL) {
                switch (part->kind) {
                case 357:
                case 358:
                case 359:
                case 360:
                case 361:
                case 362:
                    ObjectDispatch_ApplyValueToChildren(object, value);
                    break;
                case 407:
                    object->movement_state = open;
                    break;
                }
            }
        }
    }
    i++;
    if (i < 64)
        goto next;
}
