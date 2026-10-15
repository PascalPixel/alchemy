/* Near miss, differ 24 of 94: The Broken Seal's ResetSceneTransitionEffect with
 * this game's offsets. Three spots: the three heap slot loads at the top come
 * in another order (the native loads the event work before the effect work
 * and holds the transition work in r1 until after the flag read), the two
 * halfword arguments index through r2 where this uses r3, and the pooled
 * zero for the phase byte is loaded one row early in the native. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

void Audio_PlayCue(s32 no);
void FieldEffect_WatchLeaderDistance(void);
void Func_080dd748(s32 value);
void BattleFx_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleFx_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);

struct EffectWork_080dd950 {
    u8 unk00[24];
    s16 value18;
    s16 value1a;
    u8 unk1c[24];
    s8 childOption;
};

void Func_080dd950(void)
{
    u8 *state = Ram_HeapSlots->eventWork;
    struct EffectWork_080dd950 *work = Ram_HeapSlots->effectWork;
    u8 *scene = (u8 *)Ram_HeapSlots->displayTransition;
    void *ev;
    s16 zero;

    if (*(s8 *)(state + 0xca4) != 0) {
        Audio_PlayCue(167);
        Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);
        zero = 0;
        *(s8 *)(state + 0xca4) = zero;
        *(s16 *)(state + 0xca6) = zero;
        Func_080dd748(0);
        BattleFx_ApplyColorToTargetBuffer(0x10000, 1);
        BattleFx_StartBufferInterpolation(1);
        BattleFx_ApplyColorToSourceBuffer(0, 0);
        BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
        BattleFx_StartBufferInterpolation(30);
        WaitFrames(1);
        ev = Func_080ce458(0x40000005, 8, Func_080ce31c(8));
        if (ev != NULL)
            EventRuntime_RunPendingEvent(ev, work->value18, work->value1a);
        if (work->childOption == 0) {
            scene[0x53e] = 0;
            scene[0x53c] = 1;
            scene[0x53d] = 1;
            WaitFrames(10);
        }
    }
}
