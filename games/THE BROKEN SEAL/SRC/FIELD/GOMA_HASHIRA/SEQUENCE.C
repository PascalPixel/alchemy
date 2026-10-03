#include "MAPCOPY.H"
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

static __inline__ void Goma_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunActor13Departure from mov r0, #13 to lsl r2, r2, #9 (82/82 assembly lines). */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Goma_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                               s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: a direct call changes FieldScene_SetupPillarsOnEntry from mov r2, #6 to str r3, [sp] (183/185 assembly lines). */
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

#include "TYPES.H"
#include "CALL.H"
extern u8 MsgGomaGotWowThatsPrettyImpressive[];

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunActor13Departure(void)
{
    Engine_TaskRemoveCallback((s32)GomaHashira_DriveActor13Idle);
    Engine_EventBegin();
    Actor_ShowEmote(13, 0x100, 30);
    Engine_ActorRunRepeatedMotion(13, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Engine_EventSetMessage((s32)MsgGomaGotWowThatsPrettyImpressive);
    Event_ShowMessage(13, 0);
    Engine_ActorSetAnimationAndWait(13, 3);
    Battle_WaitMode0(30);
    *(u8 *)((u8 *)Object_GetById(10) + 35) &= 253;
    Goma_SetSpeed(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    SetFlagBits((u8 *)Object_GetById(10) + 35, 2);
    GameFlag_Set(0x869);
    Engine_EventEnd();
}

s32 FieldScene_SetupPillarsOnEntry(void)
{
    u8 *record;
    u8 *work;

    work = gWork;
    *(s32 *)((s32)work + 0x1c0) = 0x204;
    *(s32 *)((s32)work + 0x1c8) = 24;
    SetFlagBits((u8 *)Object_GetById(9) + 89, 16);
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
        *(u8 *)((u8 *)Object_GetById(10) + 34) = 2;
        record = (u8 *)Object_GetById(10);
        *(s32 *)((s32)record + 12) = *(s32 *)((s32)record + 12) - 1;
        {
            u8 bits = 2;
            u8 *flags = (u8 *)Object_GetById(10) + 35;

            *flags |= bits;
        }
        Goma_CopyCellAttributes(36, 48, 5, 1, 36, 14);
    }
L_0200131c:
    if (Data_02000240[225] == 99) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Actor_SetPosition(9, 0x1800000, 0xc00000);
        Battle_WaitMode0(60);
        *(u8 *)((u8 *)Object_GetById(9) + 34) = 2;
        Actor_MoveToAndWait(9, 0x198, 192);
        Battle_WaitMode0(60);
        FieldScene_RunPillarSequence();
    }
    if (Data_02000240[282] != 0) {
        GomaHashira_Actor13Frames = 0;
        Call2(Engine_TaskAddCallback, (s32)GomaHashira_DriveActor13Idle, 0xc80);
    }
    return 0;
}
