#include "TYPES.H"

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

extern u8 *gBattleWork;
extern u8 *gBattleFxWork;
extern struct DmaQueue gWorkSlot;
void WaitFrames(s32);
void QueueIoWriteDelay2(u32, u32);
void _call_via_r3(void *, s32);
void BattleMotion_ProjectScaledPosition(s32, struct ScreenPosition *);
void BattlePresentation_ConfigurePaletteFade(s32, u16, s32);
void BattleFx_InitializeStarField(s32);
void Graphics_ResetVramBlockAndReleaseHeapBlocks(s32);
void Graphics_ScaleRgb555Clamped(void *, void *, s32, s32);
void Func_080c9020(void);
void Func_080c9030(void);
void BattlePresentation_PrepareSceneFar(s32);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);

static inline void QueueObjectUpdate(void *destination)
{
    volatile u16 *ime = (u16 *)0x04000208;
    u16 saved = *ime;
    *ime = (u16)(u32)ime;
    if (gWorkSlot.count <= 31) {
        struct DmaRequest *request = &gWorkSlot.requests[gWorkSlot.count++];
        request->destination = destination;
        request->control = 0x84000002;
        request->trigger = 0x84000002;
    }
    *ime = saved;
}

void BattleFx_PlayUnitElementEffect(s32 actor, s32 value, s32 mode, s32 parameter)
{
    struct EffectTask task_a;
    struct EffectTask task_b;
    struct ScreenPosition position_a;
    struct ScreenPosition position_b;
    u8 *battle = gBattleWork;
    s32 i;

    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFade(1, FIELD(battle, u16, 0x648), 0);
    _call_via_r3((void *)0x03000164, 0x4000);
    QueueIoWriteDelay2(0x04000000, 0x3741);
    QueueIoWriteDelay2(0x0400000c, 0x3741);
    QueueIoWriteDelay2(0x3741, 0x0400000c);
    WaitFrames(1);
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x3f44;
    *(volatile u16 *)0x04000048 = 0x3f;
    *(volatile u16 *)0x0400004a = 0x11;

    switch (mode) {
    case 0:
    {
        s32 fade = 0;
        QueueIoWriteDelay2(0x04000050, 0x1088);
        BattleFx_InitializeStarField(value);
        for (i = 0; i <= 44; i++, fade += 0x444) {
            u8 *object = gBattleFxWork + 156;
            if (i <= 24) {
                s32 intensity = 0x10000 - fade;
                FIELD(battle, s32, 0x644) = intensity;
                Graphics_ScaleRgb555Clamped(battle + 0x544, (void *)0x050000c0, intensity, 0x80);
            }
            BattleMotion_ProjectScaledPosition(actor, &position_a);
            FIELD(object, s32, 0x13c4) = (64 - position_a.x) << 8;
            FIELD(object, s32, 0x13c8) = (64 - position_a.y) << 8;
            QueueObjectUpdate(object + 0x13c4);
            FIELD(object, s32, 0x13cc) = 1;
            WaitFrames(1);
        }
        Graphics_ResetVramBlockAndReleaseHeapBlocks(value);
        break;
    }
    case 1:
        BattlePresentation_PrepareSceneFar(value);
        for (i = 39; i >= 0; i--) {
            u8 *object = gBattleFxWork;
            BattleMotion_ProjectScaledPosition(actor, &position_b);
            FIELD(object, s32, 0x13c4) = (64 - position_b.x) << 8;
            FIELD(object, s32, 0x13c8) = (64 - position_b.y) << 8;
            QueueObjectUpdate(object + 0x13c4);
            FIELD(object, s32, 0x13cc) = 1;
            WaitFrames(1);
        }
        BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
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
