#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct ScreenPosition { s32 x; s32 y; };
struct DmaRequest { void *destination; u32 control; u32 trigger; };
struct DmaQueue { u16 count; u16 reserved; struct DmaRequest requests[32]; };
struct EffectTask {
    s32 value;
    u8 reserved_04[4];
    s32 actor_a;
    s32 actor_b;
    s32 enabled_a;
    s32 enabled_b;
    s32 mode;
    s32 parameter;
    s32 finished;
    u8 reserved_20[4];
    s16 actor_id;
};

extern u8 *Data_03001e74;
extern u8 *Data_03001eec;
extern struct DmaQueue Data_03001e50;
void WaitFrames(s32);
void Func_0800387c(u32, u32);
void Func_080072f0(void *, s32);
void Func_080b845c(s32, struct ScreenPosition *);
void Func_080c0774(s32, u16, s32);
void Func_080c1470(s32);
void Func_080c16d0(s32);
void Func_080c1724(void *, void *, s32, s32);
void Func_080c9020(void);
void Func_080c9030(void);
void Func_080c9038(s32);
void Func_080c9040(void);

static inline void QueueObjectUpdate(void *destination)
{
    volatile u16 *ime = (u16 *)0x04000208;
    u16 saved = *ime;
    *ime = (u16)(u32)ime;
    if (Data_03001e50.count <= 31) {
        struct DmaRequest *request = &Data_03001e50.requests[Data_03001e50.count++];
        request->destination = destination;
        request->control = 0x84000002;
        request->trigger = 0x84000002;
    }
    *ime = saved;
}

void Func_080c1798(s32 actor, s32 value, s32 mode, s32 parameter)
{
    struct EffectTask task_a;
    struct EffectTask task_b;
    struct ScreenPosition position_a;
    struct ScreenPosition position_b;
    u8 *battle = Data_03001e74;
    s32 i;

    WaitFrames(1);
    Func_080c0774(1, FIELD(battle, u16, 0x648), 0);
    Func_080072f0((void *)0x03000164, 0x4000);
    Func_0800387c(0x04000000, 0x3741);
    Func_0800387c(0x0400000c, 0x3741);
    Func_0800387c(0x3741, 0x0400000c);
    WaitFrames(1);
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x3f44;
    *(volatile u16 *)0x04000048 = 0x3f;
    *(volatile u16 *)0x0400004a = 0x11;

    switch (mode) {
    case 0:
    {
        s32 fade = 0;
        Func_0800387c(0x04000050, 0x1088);
        Func_080c1470(value);
        for (i = 0; i <= 44; i++, fade += 0x444) {
            u8 *object = Data_03001eec + 156;
            if (i <= 24) {
                s32 intensity = 0x10000 - fade;
                FIELD(battle, s32, 0x644) = intensity;
                Func_080c1724(battle + 0x544, (void *)0x050000c0, intensity, 0x80);
            }
            Func_080b845c(actor, &position_a);
            FIELD(object, s32, 0x13c4) = (64 - position_a.x) << 8;
            FIELD(object, s32, 0x13c8) = (64 - position_a.y) << 8;
            QueueObjectUpdate(object + 0x13c4);
            FIELD(object, s32, 0x13cc) = 1;
            WaitFrames(1);
        }
        Func_080c16d0(value);
        break;
    }
    case 1:
        Func_080c9038(value);
        for (i = 39; i >= 0; i--) {
            u8 *object = Data_03001eec;
            Func_080b845c(actor, &position_b);
            FIELD(object, s32, 0x13c4) = (64 - position_b.x) << 8;
            FIELD(object, s32, 0x13c8) = (64 - position_b.y) << 8;
            QueueObjectUpdate(object + 0x13c4);
            FIELD(object, s32, 0x13cc) = 1;
            WaitFrames(1);
        }
        Func_080c9040();
        break;
    case 2:
        task_a.value = value;
        task_a.actor_a = actor;
        task_a.actor_b = actor;
        task_a.enabled_a = 1;
        task_a.enabled_b = 1;
        task_a.mode = 1;
        task_a.parameter = parameter;
        task_a.finished = 0;
        task_a.actor_id = actor;
        Func_080c9020();
        break;
    default:
        task_b.value = value;
        task_b.actor_a = actor;
        task_b.actor_b = actor;
        task_b.enabled_a = 1;
        task_b.enabled_b = 1;
        task_b.mode = 1;
        task_b.parameter = 0;
        task_b.finished = 0;
        task_b.actor_id = actor;
        Func_080c9030();
        break;
    }
}
