/* NONMATCHING: 786 of 784 bytes, 43 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the Sol Sanctum light scene is written in full with its constants passed through call wrappers; left: the 0x1c8 frame offset and the event-globals address swap r0/r1 at the first store, the three halfword clears derive 0xe04 from 0xe00 instead of 0xe02, and the end block and mid-function pool shift by 2 bytes (43 edits). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Main_0808a2f8(void);
void Main_0808a5e0(s32 value);
void SetSolShindenActorStep(s32 actor_step, s32 wait_frames);

/* The IWRAM event globals: the event work, then the field work at +0x14. */
struct EventGlobals {
    struct EventWork *event;
    u8 unknown_04[0x10];
    u8 *field;
};

extern struct EventGlobals Data_03001ebc;
extern u8 Data_00000000[];

void Local_02002614(void)
{
    struct FieldActor *leader;
    u8 *field;
    u8 zero;
    s32 frames;

    Engine_EventBegin();
    Call4((void (*)())Engine_CameraMoveTo, 0x11e0000, -1, 0x860000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call2((void (*)())Engine_ColorBufferApplySource, 0x7fff, 0);
    Call2((void (*)())Engine_ColorBufferApplyTarget, 0x7fff, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    Call3((void (*)())Engine_ActorSetPosition, 1, 0x1180000, 0x860000);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xa000, 0);
    {
        u8 *ev = (u8 *)Data_03001ebc.event;

        frames = 0x1c8;
        *(s32 *)(ev + frames) = 1;
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call2((void (*)())Engine_ColorBufferApplyTarget, 0x2051cc, 1);
    Engine_ColorBufferInterpolate(120);
    Engine_TaskWait(120);
    Engine_GameFlagSet(0x201);
    Engine_GameFlagClear(0x200);
    Engine_GameFlagClear(0x202);
    Call2((void (*)())Engine_ColorBufferApplyTarget, 0x10000, 2);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(100);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2((void (*)())Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_EventWait(60);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x2000, 20);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xe000, 40);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x9000, 40);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0x5000, 80);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x8000, 10);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0x1000, 60);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorFaceDirection, 0, 0x6000, 0);
    Call3((void (*)())Engine_ActorFaceDirection, 1, 0xe000, 20);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventSetMessage(0x1119);
    Engine_EventOpenMessage(1, 0);
    Engine_UiWorkWaitThenFinalizeCapacity(0, 0);
    Engine_ActorSetAnimation(0, 1);
    Engine_EventWait(40);
    Engine_AudioPlayCue(107);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
    Engine_EventWait(40);
    Main_0808a2f8();
    Engine_ActorGet(0)->unknown_5a &= ~1;
    Engine_ActorGet(1)->unknown_5a &= ~1;
    Call3((void (*)())Engine_ActorJump, 0, 4, 0);
    Call3((void (*)())Engine_ActorJump, 1, 4, 0);
    Call3((void (*)())Engine_ActorSetDestination, 0, 0x12c, 0x82);
    Call3((void (*)())Engine_ActorSetDestination, 1, 0x10a, 0x90);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(40);
    Engine_ActorGet(0)->unknown_5a |= 1;
    Engine_ActorGet(1)->unknown_5a |= 1;
    Main_0808a5e0(141);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 2);
    SetSolShindenActorStep(1, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_ActorSetAnimation(1, 2);
    leader = Engine_ActorGet(0);
    if (leader != NULL) {
        Call3((void (*)())Engine_ActorSetDestination, 1, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(1);
    Call3((void (*)())Engine_ActorSetPosition, 1, 0, 0);
    field = Data_03001ebc.field;
    *(u16 *)(field + 0xe00) = 0;
    *(u16 *)(field + 0xe02) = 0;
    *(u16 *)(field + 0xe04) = 0;
    zero = (u32)Data_00000000;
    field[0x2a00] = zero;
    field[0x2a01] = 1;
    field[0x2a02] = 1;
    field[0x2a03] = 1;
    *(u16 *)0x05000000 = 0;
    Data_03001ebc.event->start_transition = 0x204;
    *(s32 *)((u8 *)Data_03001ebc.event + frames) = 16;
    Engine_EventEnd();
}
