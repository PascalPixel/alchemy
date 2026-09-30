/*
 * Actor 13's departure and the scene initialiser that restores the pillars.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"

extern u8 *gWork;
/* The game state read as halfwords: 225 is the entrance. */
extern s16 Data_02000240[];

/* Frames actor 13 has idled; ACTOR13_IDLE.C owns it. */
extern s32 GomaHashira_Actor13Frames;

/* The scene's tables, laid out after the code. */
extern u8 GomaHashira_Scripts[];
extern u8 GomaHashira_Messages[];
extern u8 GomaHashira_Actors[];

/* Imports this overlay shares with the staged-actor module, by its names. */
u8 *Object_GetById();
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);

void BattleFx_SetPhaseRequest(s32, s32);
void GomaHashira_SpawnPillarEffect();
void FieldScene_RunPillarSequence(void);
void GomaHashira_DriveActor13Idle(void);

/*
 * The event services under the names the staged-actor module gives their
 * imports, spelled as FIELD_EVENT.H spells its own.
 */
static __inline__ void Goma_Wait(s32 frames)
{
    Battle_WaitMode0(frames);
}

static __inline__ void Goma_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Goma_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                               s32 dest_x, s32 dest_y)
{
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

static __inline__ void ConfigureFirst(s32 actor, s32 angle, s32 zero)
{
    Actor_ShowEmote(actor, angle, zero);
}

static __inline__ void ConfigureSecond(s32 actor, s32 mode, s32 zero)
{
    Actor_Jump(actor, mode, zero);
}

static __inline__ void ConfigureThird(s32 actor, s32 value)
{
    BattleFx_SetPhaseRequest(actor, value);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

#include "TYPES.H"
extern u8 MsgGomaGotWowThatsPrettyImpressive[];

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunActor13Departure(void)
{
    Call1(Engine_TaskRemoveCallback, (s32)GomaHashira_DriveActor13Idle);
    Event_Begin();
    Actor_ShowEmote(13, 0x100, 30);
    Actor_RunRepeatedMotion(13, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Event_SetMessage((s32)MsgGomaGotWowThatsPrettyImpressive);
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(13, 3);
    Goma_Wait(30);
    *(u8 *)(Object_GetById(10) + 35) &= 253;
    Goma_SetSpeed(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    SetFlagBits(Object_GetById(10) + 35, 2);
    GameFlag_Set(0x869);
    Event_End();
}

s32 FieldScene_SetupPillarsOnEntry(void)
{
    u8 *record;
    u8 *work;

    work = gWork;
    *(s32 *)((s32)work + 0x1c0) = 0x204;
    *(s32 *)((s32)work + 0x1c8) = 24;
    SetFlagBits(Object_GetById(9) + 89, 16);
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(8, 0x1580000, 0x680000);
        Goma_CopyCellAttributes(24, 40, 6, 3, 18, 6);
    } else {
        Goma_CopyCellAttributes(18, 40, 6, 3, 18, 6);
    }
    if (GameFlag_IsSet(0x300) != 0) {
        Actor_SetPosition(9, 0, 0);
        Goma_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    if (GameFlag_IsSet(0x301) != 0) {
        Actor_SetPosition(10, 0x2680000, 0xe80000);
        if ((u32)(((u16)Data_02000240[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Object_GetById(10) + 34) = 2;
        record = Value1(Object_GetById, 10);
        *(s32 *)((s32)record + 12) = *(s32 *)((s32)record + 12) - 1;
        {
            u8 bits = 2;
            u8 *flags = Object_GetById(10) + 35;

            *flags |= bits;
        }
        Goma_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
L_0200131c:
    if (Data_02000240[225] == 99) {
        Event_OpenScreen();
        Event_WaitForScreen();
        Actor_SetPosition(9, 0x1800000, 0xc00000);
        Goma_Wait(60);
        *(u8 *)(Object_GetById(9) + 34) = 2;
        Actor_MoveToAndWait(9, 0x198, 192);
        Call1((void (*)())Battle_WaitMode0, 60);
        FieldScene_RunPillarSequence();
    }
    if (Data_02000240[282] != 0) {
        GomaHashira_Actor13Frames = 0;
        Call2(Engine_TaskAddCallback, (s32)GomaHashira_DriveActor13Idle, 0xc80);
    }
    return 0;
}
