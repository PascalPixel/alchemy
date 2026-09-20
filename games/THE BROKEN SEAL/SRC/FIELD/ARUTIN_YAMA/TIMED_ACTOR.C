#include "TYPES.H"

/*
 * Arutin mountain: a timed actor that bobs toward the ground. While a delay
 * is pending it counts down; with no vertical velocity it sinks one step per
 * tick and settles on the ground (firing the landing cue on first contact);
 * when the timer lapses it reactivates and starts the fall.
 */

#define SceneMotion_UpdateTimedActor Func_02001770
struct SceneMotion {
    u8 unk_00[12]; s32 y; u8 unk_10[4]; s32 ground;
    u8 unk_18[16]; s32 velocity; u8 unk_2c[47]; u8 state;
    u8 unk_5c[8]; s16 timer, delay; s32 active;
};
extern void Func_020052d4();
extern void Func_0200527a(struct SceneMotion *, s32);
extern void Func_020054e4(s32);
extern void Func_02005310();
extern void Func_02005520(s32);
extern void Func_020052dc(struct SceneMotion *, s32);
static __inline__ void Call3(void (*f)(), s32 a, s32 b, s32 c)
{ f(a,b,c); }
void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Call3(Func_020052d4,-1,-1,0xe666);
    }
    if (work->velocity == 0) {
        Func_0200527a(work,1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Func_020054e4(229);
                work->active = 0;
                work->delay = 4;
                Call3(Func_02005310,0x10000,0,0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Func_02005520(152);
        work->active = 1;
        Func_020052dc(work,2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}
