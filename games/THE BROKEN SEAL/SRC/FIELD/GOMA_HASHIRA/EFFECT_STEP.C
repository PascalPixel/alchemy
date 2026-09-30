/*
 * The pillar effect's per-frame step and the small actor helpers before it.
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

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};

void ConfigureActorThirteenSceneParameters(void)
{
    ConfigureFirst(13, 256, 0);
    ConfigureSecond(13, 2, 0);
    ConfigureThird(12, 40);
}

void SceneEffect_AdvancePositionByAxisMode(u8 *o)
{
    s16 v = *(s16 *)(o + 102);

    switch (v) {
    case 0:
        *(s32 *)(o + 8) += *(s32 *)(o + 48);
        *(s32 *)(o + 56) = *(s32 *)(o + 8);
        *(s32 *)(o + 12) += *(s32 *)(o + 52);
        *(s32 *)(o + 60) = *(s32 *)(o + 12);
        break;
    case 1:
        *(s32 *)(o + 8) += *(s32 *)(o + 48);
        *(s32 *)(o + 56) = *(s32 *)(o + 8);
        *(s32 *)(o + 16) += *(s32 *)(o + 52);
        *(s32 *)(o + 64) = *(s32 *)(o + 16);
        break;
    case 2:
        *(s32 *)(o + 12) += *(s32 *)(o + 48);
        *(s32 *)(o + 60) = *(s32 *)(o + 12);
        *(s32 *)(o + 16) += *(s32 *)(o + 52);
        *(s32 *)(o + 64) = *(s32 *)(o + 16);
        break;
    }
}

s32 OverlayObject_ApplyValue15(s32 actor)
{
    ObjectGroup_SetChildValue(actor, 15);
    return 0;
}

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}
