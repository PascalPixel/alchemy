/* NONMATCHING: not-yet-c, 312 candidate / 312 reference bytes, 48 differing
 * halfwords and 28 normalized edits (2026-09-26). Whole owner is 020027d4
 * through 0200290c, including its five pool words; no neighbor is included.
 *
 * Own-ROM call veneers lead to UiTextResource_Initialize (0801c0dc),
 * SetPosition (0801c154), Release (0801c17c), and
 * SaveState_CountRecordsExcludingFlagged (0801f730). The existing text
 * resource implementation and Menu_RunSelection establish a 12-byte cursor
 * record, not the unrelated 16-byte MenuCursor. Messages 04462..04464 name
 * the Great Healer, file-screen and restart choices.
 *
 * Bounded hypothesis 1: a typed 12-byte cursor record plus the verified
 * void initialization/position signatures should permit stack-address
 * rematerialization at each call. Complete normalized comparison is
 * unchanged from the byte-array baseline: 312 bytes, 48 halfwords, 28 edits.
 * Reference owns handle at sp+4, cursor at sp+8, window r7, table r8 and
 * selection r5. Candidate retains the cursor address in r6 across four
 * calls, puts window in r8 and reloads the table inside the loop. The
 * allocator diagnostic identifies the cursor expression as pseudo 48,
 * five uses across 43 instructions. Stop at the checkpoint; do not sweep
 * pointer spellings without another ownership/control-flow hypothesis.
 */
#include "TYPES.H"

extern u8 Data_0000116e[];

struct TextObject {
    u8 storage[12];
};

void Engine_ActorSetPosition();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate();
void Engine_EventWait();
s32 Engine_DebugCreateWindow();
void Engine_DebugDrawTextResource();
s32 SaveState_CountRecordsExcludingFlaggedFar(s32 flag);
void UiTextResource_InitializeFar(struct TextObject *object, s32 *slot);
void UiTextResource_SetPositionFar(struct TextObject *object, s32 x, s32 y);
void UiTextResource_ReleaseFar(s32 slot);
void Engine_DebugFinalizeWindow();



/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

s32 ShindenHeya_ChooseRestartOption(void)
{
    struct TextObject cursor;
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
    if (Value1(SaveState_CountRecordsExcludingFlaggedFar, 1) == 0)
        Engine_DebugDrawTextResource(text + 2, win, 16, 16);
    else
        Engine_DebugDrawTextResource(text + 1, win, 16, 16);
    UiTextResource_InitializeFar(&cursor, &handle);
    UiTextResource_SetPositionFar(&cursor, 72, 60);
    sel = 0;
    if ((*(s32 *)0x03001c94 & 1) == 0) {
        tbl = (s32 *)0x0200c11c;
        do {
            if ((*(s32 *)0x03001b04 & 192) != 0)
                sel ^= 1;
            UiTextResource_SetPositionFar(&cursor, tbl[(*(u32 *)0x03001800 >> 1) & 15] + 24, (sel << 4) + 60);
            Engine_EventWait(1);
        } while ((*(s32 *)0x03001c94 & 1) == 0);
    }
    UiTextResource_ReleaseFar(handle);
    Engine_DebugFinalizeWindow(win, 1);
    return sel;
}
