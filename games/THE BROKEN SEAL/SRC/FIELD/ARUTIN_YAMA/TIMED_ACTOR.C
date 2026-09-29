#include "TYPES.H"
#include "ARUTIN.H"

/*
 * Arutin mountain: a timed actor that bobs toward the ground. While a delay
 * is pending it counts down; with no vertical velocity it sinks one step per
 * tick and settles on the ground (firing the landing cue on first contact);
 * when the timer lapses it reactivates and starts the fall.
 */

extern void Engine_WorkSetValuesIfNonNegative();
extern void Engine_ObjectSetAnimation(struct SceneMotion *, s32);
extern void Engine_AudioPlayCue(s32);
extern void Engine_WorkSetValuesIfNonNegative();
extern void Engine_AudioPlayCue(s32);
extern void Engine_ObjectSetAnimation(struct SceneMotion *, s32);
static __inline__ void Call3(void (*f)(), s32 a, s32 b, s32 c)
{ f(a,b,c); }
void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Call3(Engine_WorkSetValuesIfNonNegative,-1,-1,0xe666);
    }
    if (work->velocity == 0) {
        Engine_ObjectSetAnimation(work,1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Engine_AudioPlayCue(229);
                work->active = 0;
                work->delay = 4;
                Call3(Engine_WorkSetValuesIfNonNegative,0x10000,0,0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Engine_AudioPlayCue(152);
        work->active = 1;
        Engine_ObjectSetAnimation(work,2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}
