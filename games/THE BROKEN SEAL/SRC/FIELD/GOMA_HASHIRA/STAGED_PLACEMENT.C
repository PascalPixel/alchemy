/*
 * The scene tables, actor 8's column switch and the task that spawns
 * pillar effects while the pillar moves.
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

#include "TYPES.H"
#include "CALL.H"

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};

u8 *SceneData_GetScriptTable(void) { return GomaHashira_Scripts; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetMessageTable(void) { return GomaHashira_Messages; }

u8 *SceneData_GetActorTable(void) { return GomaHashira_Actors; }

void ConfigureSceneForActorEightColumn(void)
{
    u8 *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = (u8 *)Object_GetById(8);
    x = *(s32 *)(actor + 8);
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Event_Begin();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Goma_CopyCellAttributes(18, 40, 6, 3, a4, a5);
        GameFlag_Clear(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Goma_CopyCellAttributes(24, 40, 6, 3, a4, a5);
        GameFlag_Set(0x302);
    }
    Event_End();
}

void FieldScene_RunPrimarySequence(void)
{
    volatile s32 *state = (volatile s32 *)&gFrameCount;
    s32 flags;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 rec7a, reca, v1a, v2a, v3a, ta, ua, na;
    s32 rec7b, recb, v1b, v2b, v3b, tb, ub, nb;

    flags = *state & 7;
    if (flags == 0) {
        rec7a = (u8 *)Object_GetById(9);
        v1a = Engine_RandomNext();
        a0 = *(s32 *)(rec7a + 8);
        a0 = a0 + (s32)((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = (u8 *)Object_GetById(9);
        reca = (s32)Object_GetById(9);
        a2 = *(s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Engine_RandomNext();
        ta = (s32)((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Random_Next();
        na = -na;
        Call7(GomaHashira_SpawnPillarEffect, a0, *(s32 *)(rec4 + 12), a2, 0, na, (s32)((u32)(v3a << 1) >> 16), flags);
        flags = *state & 15;
        if (flags == 0) {
            rec7b = (u8 *)Object_GetById(9);
            v1b = Engine_RandomNext();
            a0 = *(s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = (u8 *)Object_GetById(9);
            recb = (s32)Object_GetById(9);
            a2 = *(s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Engine_RandomNext();
            tb = (s32)((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Random_Next();
            nb = -nb;
            Call7(GomaHashira_SpawnPillarEffect, a0, *(s32 *)(rec4 + 12), a2, 0, nb, (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}
