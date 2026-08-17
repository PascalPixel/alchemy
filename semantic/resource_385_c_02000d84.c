#include "types.h"

/*
 * Complete actor-18 three-cue movement scene through its literal pool.  The
 * actor enters at y=408, performs three evenly timed cues, moves to y=392,
 * and records story flag 0x858 before the scene closes.
 */











extern void Func_02001e62(s32 cue);
extern void Func_02001e76(void);
extern void Func_02001e74(s32 frames);
extern void Func_02001ee4(s32 actor, s32 mode);
extern void Func_02001eae(s32 actor, s32 width, s32 height);
extern void Func_02001eda(s32 actor, s32 x, s32 y);
extern void Func_02001e98(s32 frames);
extern void Func_02001f4c(s32 actor, s32 value, s32 frames);
extern void Func_02001f0e(s32 actor, s32 mode, s32 value);
extern void Func_02001eb4(s32 frames);
extern void Func_02001f1e(s32 actor, s32 mode, s32 value);
extern void Func_02001ec4(s32 frames);
extern void Func_02001f2e(s32 actor, s32 mode, s32 value);
extern void Func_02001ed4(s32 frames);
extern void Func_02001f28(s32 actor, s32 x, s32 y);
extern void Func_02001ee6(s32 frames);
extern void Func_02001f9a(s32 actor, s32 value, s32 frames);
extern void Func_02001ee0(s32 flag);
extern void Func_02001f0c(void);
void Func_02000d84(void)
{
    Func_02001e62(231);
    Func_02001e76();
    Func_02001e74(10);
    Func_02001ee4(18, 2);
    Func_02001eae(18, 0xcccc, 0x6666);
    Func_02001eda(18, 216, 408);
    Func_02001e98(10);
    Func_02001f4c(18, 0x4000, 20);

    Func_02001f0e(18, 6, 0);
    Func_02001eb4(30);
    Func_02001f1e(18, 6, 0);
    Func_02001ec4(30);
    Func_02001f2e(18, 6, 0);
    Func_02001ed4(30);

    Func_02001f28(18, 216, 392);
    Func_02001ee6(10);
    Func_02001f9a(18, 0x4000, 20);
    Func_02001ee0(0x858);
    Func_02001f0c();
}
