/* NONMATCHING: 312 of 312 bytes, about 35 aligned differences (2026-09-24).
 * Remaining: the reference keeps the window in r7 and the pose table in r8,
 * rematerializing the cursor buffer address; here the window lands in r8. */
#include "TYPES.H"

extern u8 Data_0000116e[];

void Engine_ActorSetPosition();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
void Engine_EventWait();
s32 Engine_DebugCreateWindow();
void Engine_DebugDrawTextResource();
s32 Main_08015400();
s32 Main_08015228();
void Main_08015230();
void Main_08015238();
void Engine_DebugFinalizeWindow();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

s32 ShindenHeya_Func020027d4(void)
{
    u8 cursor[12];
    s32 handle;
    s32 win;
    s32 text;
    s32 sel;
    s32 *tbl;

    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(0, 0, 0);
    Call2(Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    win = Engine_DebugCreateWindow(2, 7, 25, 5, 1);
    text = (s32)Data_0000116e;
    Engine_DebugDrawTextResource(text, win, 16, 0);
    if (Value1(Main_08015400, 1) == 0)
        Engine_DebugDrawTextResource(text + 2, win, 16, 16);
    else
        Engine_DebugDrawTextResource(text + 1, win, 16, 16);
    Main_08015228(cursor, &handle);
    Main_08015230(cursor, 72, 60);
    sel = 0;
    if ((*(s32 *)0x03001c94 & 1) == 0) {
        tbl = (s32 *)0x0200c11c;
        do {
            if ((*(s32 *)0x03001b04 & 192) != 0)
                sel ^= 1;
            Main_08015230(cursor, tbl[(*(u32 *)0x03001800 >> 1) & 15] + 24, (sel << 4) + 60);
            Engine_EventWait(1);
        } while ((*(s32 *)0x03001c94 & 1) == 0);
    }
    Main_08015238(handle);
    Engine_DebugFinalizeWindow(win, 1);
    return sel;
}
