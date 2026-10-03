#include "MAPCOPY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "CALL.H"

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

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};

/* Sets bits in an actor's flag byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

struct ScriptTable {
    u8 *script[3];
};

/* The three scripts a pillar effect may play. */
extern struct ScriptTable GomaHashira_PillarScripts;
void SceneEffect_AdvancePositionByAxisMode(u8 *o);

struct Sprite389 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

/*
 * The pillar effect's per-frame step and the small actor helpers before it.
 */
void ConfigureActorThirteenSceneParameters(void)
{
    Engine_ActorShowEmote(13, 256, 0);
    Engine_ActorJump(13, 2, 0);
    BattleFx_SetPhaseRequest(12, 40);
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

/* Spawn a pillar effect object: flags pick the script (low nibble) and the
 * palette (bits 16-19); the high half of layer picks the sprite layer, 0 to
 * copy the leader's, and its low half is stored at +102. */
void GomaHashira_SpawnPillarEffect(s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 flags, s32 layer)
{
    struct ScriptTable table;
    u8 *leader;
    u8 *obj;
    struct Sprite389 *spr;
    u32 sel;

    leader = (u8 *)Object_GetById(0);
    table = GomaHashira_PillarScripts;
    obj = (u8 *)Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = *(struct Sprite389 **)(obj + 80);
    Object_SetMode(obj, (flags + 1) & 15);
    Engine_ObjectSetScript(obj, table.script[flags & 15]);
    ObjectGroup_SetChildValue(obj, ((u32)flags >> 16) & 15);
    obj[85] = 0;
    ((u8 *)spr)[38] = 0;
    *(s32 *)(obj + 108) = (s32)SceneEffect_AdvancePositionByAxisMode;
    *(s32 *)(obj + 48) = a3;
    *(s32 *)(obj + 52) = a4;
    *(u16 *)(obj + 102) = *(u16 *)&layer;
    sel = (u32)layer >> 16;
    switch (sel) {
    case 0:
        spr->layer = (*(struct Sprite389 **)(leader + 80))->layer;
        break;
    case 1:
    case 2:
    case 3:
        obj[35] &= 0xfe;
        spr->layer = sel;
        break;
    }
}

/*
 * The scene tables, actor 8's column switch and the task that spawns
 * pillar effects while the pillar moves.
 */
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
    Engine_EventBegin();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Map_CopyCellAttributeRect(18, 40, 6, 3, a4, a5);
        Engine_GameFlagClear(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Map_CopyCellAttributeRect(24, 40, 6, 3, a4, a5);
        Engine_GameFlagSet(0x302);
    }
    Engine_EventEnd();
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
        v3a = Engine_RandomNext();
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
            v3b = Engine_RandomNext();
            nb = -nb;
            Call7(GomaHashira_SpawnPillarEffect, a0, *(s32 *)(rec4 + 12), a2, 0, nb, (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}
