#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventWork15a {
    u8 unknown_000[0x15a];
    u16 flag;
};

extern u8 MsgFieldDoorWontBudge;
extern const u8 Data_020013c0[];
extern const u8 Data_020013c4[];

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010dc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020011e4(const void *data);
void Func_020011cc(const void *data);
void Func_020011ec(void);

void Func_02000474(void)
{
    struct EventWork15a *work = Ram_HeapSlots->event_work;

    Func_0200110c();
    Func_020011ac(0);
    UiText_ShowPositionedMessageAndWait((s32)&MsgFieldDoorWontBudge, 1);
    work->flag = 1;
    Func_02001114();
}

void Func_020004a4(struct ObjectRuntime *object)
{
    u8 *flags = &object->motion_flags;

    *flags = 3;
    do {
        WaitFrames(1);
    } while (object->velocity_y != 0);
    Func_02001214(188);
    WaitFrames(10);
    *flags = 0;
}

void Func_020004cc(void)
{
    struct ObjectRuntime *object = Object_GetById(11);
    s32 z;

    if ((object->x >> 20) == 40) {
        z = object->z >> 20;
        if (z == 13) {
            Func_0200110c();
            Func_020011ac(0);
            Func_020004a4(object);
            Func_020010dc(41, 13, 2, 1, 39, z);
            GameFlag_SetBit(0x8c8);
            Func_02001114();
        }
    }
}

void Func_0200051c(void)
{
    Func_020011e4(Data_020013c0);
}

void Func_0200052c(void)
{
    struct ObjectRuntime *object = Object_GetById(12);

    Func_020011ec();
    if ((object->x >> 20) == 44 && (object->z >> 20) == 26)
        GameFlag_SetBit(0x8ca);
}

void Func_02000558(void)
{
    Func_020011cc(Data_020013c4);
}
