#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_REG.H"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 Data_020026f0;
extern s32 Data_020026f4;
extern u16 Data_0200271c[];
extern u16 Data_0200273c[];
extern u16 Data_02002740[];
extern u32 gFrameCount;

s32 __modsi3();
s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001e28();
void Func_02001ef8();
void Func_02001e48();
void Func_02001ed0();
void ObjectDispatch_SetSingleChildFlags();
void WaitFrames();
void Motion_CamBounds();
struct FieldActor *Func_02001ea8();
void Func_02001dd8();
void Func_02001f08();
void Func_02001de0();
void Object_AttachWorkTargetToObject();
void Func_02001e50();

void Func_020005e4(void)
{
    u16 colors[16];
    s32 phase;
    s32 i;
    s32 offset;

    phase = (Data_020026f0 / 4) % 5;
    if (Data_020026f4 != 0) {
        u8 *work = Ram_HeapSlots->event_work;

        if (*(s16 *)(work + 358) != 0 && (HALF_AT(work, 358) & 0x3ff) != 153)
            return;
        if (*(s8 *)(work + 0xca4) != 0)
            return;
        if (*(s16 *)(work + 350) != 0)
            return;
    }
    i = 0;
    offset = 4;
next:
    {
        s32 slot = (phase + i) % 5;
        u16 *dst = colors;

        asm volatile("" : "+l"(dst)); /* FAKEMATCH: ⚓️ rebuilds the buffer address each pass */
        dst[slot] = HALF_AT(Data_0200271c, offset);
    }
    i++;
    offset += 2;
    if (i <= 4)
        goto next;
    Dma_Set(colors, (void *)0x05000184, 0x84000002, REG_DMA3);
    Data_020026f0++;
}

void Func_020006a0(s32 mode)
{
    Data_020026f4 = 1;
    if (mode != 2) {
        Iwram_CopyWords(Data_0200271c, (void *)0x05000180, 32);
        Iwram_CopyWords(Data_0200273c, (void *)0x050001a0, 32);
    }
    if (GameFlag_Test(0xa00) || mode == 1) {
        Data_020026f0 = 0;
        Scheduler_AddOrUpdateCallback(Func_020005e4, 1152);
    } else {
        Dma_Set(Data_02002740, (void *)0x05000184, 0x84000002, REG_DMA3);
    }
}

void Func_02000724(s32 id)
{
    struct FieldActor *object;
    s32 height;

    object = Object_GetById(id);
    object->motion_flags = 4;
    height = object->terrain_height;
    asm volatile("" : "+l"(height)); /* FAKEMATCH: ⚓️ loads the ground height before it clears the word */
    WORD_AT(object, 68) = 0;
    object->y = height + 0x200000;
    {
        u8 *flags = &object->priority_flags;

        *flags |= 128;
    }
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
    Func_02001e28(object->x, object->z, 0, (height >> 19) + 6);
    Func_02001ef8(object->x, object->z, 0, 128);
}
