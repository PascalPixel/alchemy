#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_RUNTIME.H"

struct EventWorkCbc {
    u8 unknown_000[0xcbc];
    void *active;
};

extern const u8 Data_020013c0[];

extern const u8 Data_020013c4[];

extern const u8 Data_020013ca[];

struct ObjectRuntime *Object_GetById(s32 id);

void GameFlag_SetBit(s32 flag);

void Func_020011e4(const void *data);

void Func_020011cc(const void *data);

void Func_020011ec(void);

void Func_0200014c(const void *data);

void Func_020008d0(void *active);

void Func_02000b38(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

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
