#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventWorkCbc {
    u8 unk000[0xcbc];
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

void Func_0200093c(void)
{
    struct EventWorkCbc *work = Ram_HeapSlots->eventWork;

    /* FAKEMATCH: ⚓️ loads the event work before it builds the offset */
    asm volatile("");
    if (work->active != 0)
        Func_020008d0(work->active);
}
