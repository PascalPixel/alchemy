#include "types.h"

struct DisplayQueueEntry {
    u32 control;
    u32 address;
    u32 flags;
};

struct DisplayQueue {
    u16 count;
    u16 reserved;
    struct DisplayQueueEntry entries[32];
};

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

struct SceneCameraObject {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    s32 anchor;
};

extern u8 *Data_03001eec;
extern struct Position Data_03001ad0;
extern struct SceneCameraObject Data_03001ce0;
extern struct DisplayQueue Data_02002090;
extern volatile u16 Data_04000208;
extern void Func_080cd4b4(void);

void Func_080072f0(u32, s32, s32, u32);
s32 Func_08004278(u32);
void Func_080b5038(s32, u16, s32);
void Func_080b5048(u16, s32);

/* IME-disable idiom `*ime = (u16)(u32)ime;` is proven exact in
   games/gs1/src/runtime/scheduler/callback_scheduler.c
   (`Data_04000208 = (u16)&Data_04000208;`, adopted, asm deleted). The
   surrounding bounded-queue-insert-under-guard shape parallels the
   still-unadopted draft QueueObjectUpdate in
   games/gs1/recon/en/main/080c1798.c (a different queue at
   Data_03001e50, not Data_02002090) -- useful as a lead, not proof. */
static inline void QueueDisplayRequest(u32 control)
{
    struct DisplayQueue *queue = &Data_02002090;
    volatile u16 *ime = &Data_04000208;
    u32 saved = *ime;
    s32 count;
    *ime = (u16)(u32)ime;
    count = queue->count;
    if (count <= 31) {
        struct DisplayQueueEntry *entry = &queue->entries[count];
        u32 *destination = &entry->control;

        queue->count = count + 1;
        *destination++ = control;
        *destination++ = 0x04000000;
        *destination = 0x00020000;
    }
    *ime = saved;
}

void Func_080cdbc0(void)
{
    struct Position *pos = &Data_03001ad0;
    u8 *actor = Data_03001eec;
    u8 *battle = *(u8 **)((u8 *)&Data_03001eec - 120);

    Audio_PlayCue(0x121);

    pos->x = *(s32 *)(actor + 0x77a0);
    pos->y = *(s32 *)(actor + 0x77a4);

    Data_03001ce0.field0c = 120;
    Data_03001ce0.anchor = 120;

    *(volatile u16 *)0x0400000c = 0x787;
    Func_080072f0(0x06004000, 0x4000, 0x0400000c, 0x03000164);

    Func_08004278((u32)Func_080cd4b4);

    pos->y = 32;
    QueueDisplayRequest(0x7341);

    *(volatile u16 *)0x04000050 = 0;
    WaitFrames(1);

    Func_080b5038(2, *(u16 *)(battle + 0x648), 7);
    WaitFrames(1);

    battle += 0x648;
    {
        s32 i;
        s32 elapsed;
        for (i = 0, elapsed = 0; i != 8; i++, elapsed += 3) {
            Func_080b5048(*(u16 *)battle, 21 - elapsed);
            WaitFrames(1);
        }
    }

    QueueDisplayRequest(0x7541);
    WaitFrames(1);
}
