#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "call.h"
#include "b5_context.h"





void BattleFx_SetPhaseRequest(s32, s32);
void GomaHashira_SpawnPillarEffect();

enum StagedPlacementMessage {
    MSG_GOT_WOW_THATS_PRETTY_IMPRESSIVE = 0x132f
};

struct ScriptTable {
    u8 *script[3];
};

struct PillarEffect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 rate_first;
    s32 rate_second;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x22];
    s16 plane;
};

enum PillarEffectPlane {
    PLANE_XY = 0,
    PLANE_XZ = 1,
    PLANE_YZ = 2
};


struct Sprite389 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

void ConfigureActorThirteenSceneParameters(void)
{
    Actor_ShowEmote(13, 256, 0);
    Engine_ActorJump(13, 2, 0);
    BattleFx_SetPhaseRequest(12, 40);
}

void SceneEffect_AdvancePositionByAxisMode(struct PillarEffect *effect)
{
    s16 plane = effect->plane;

    switch (plane) {
    case PLANE_XY:
        effect->x += effect->rate_first;
        effect->target_x = effect->x;
        effect->y += effect->rate_second;
        effect->target_y = effect->y;
        break;
    case PLANE_XZ:
        effect->x += effect->rate_first;
        effect->target_x = effect->x;
        effect->z += effect->rate_second;
        effect->target_z = effect->z;
        break;
    case PLANE_YZ:
        effect->y += effect->rate_first;
        effect->target_y = effect->y;
        effect->z += effect->rate_second;
        effect->target_z = effect->z;
        break;
    }
}

s32 OverlayObject_ApplyValue15(s32 actor)
{
    ObjectGroup_SetChildValue(actor, 15);
    return 0;
}

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

u8 *SceneData_GetScriptTable(void) { return GomaHashira_Scripts; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SceneData_GetMessageTable(void) { return GomaHashira_Messages; }

u8 *SceneData_GetActorTable(void) { return GomaHashira_Actors; }

void ConfigureSceneForActorEightColumn(void)
{
    struct FieldActor *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Object_GetById(8);
    x = actor->x.fixed;
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Engine_EventBegin();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Engine_MapCopyCellAttributes(18, 40, 6, 3, a4, a5);
        Engine_GameFlagClear(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Engine_MapCopyCellAttributes(24, 40, 6, 3, a4, a5);
        Engine_GameFlagSet(0x302);
    }
    Engine_EventEnd();
}

void FieldScene_RunPrimarySequence(void)
{
    s32 flags;
    s32 a0;
    s32 a2;
    struct FieldActor *rec4;
    s32 rec7a, reca, v1a, v2a, v3a, ta, ua, na;
    s32 rec7b, recb, v1b, v2b, v3b, tb, ub, nb;

    flags = gFrameCount & 7;
    if (flags == 0) {
        rec7a = (u8 *)Object_GetById(9);
        v1a = Engine_RandomNext();
        a0 = *(s32 *)(rec7a + 8);
        a0 = a0 + (s32)((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = Object_GetById(9);
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
        Call7(GomaHashira_SpawnPillarEffect, a0, rec4->y.fixed, a2, 0, na, (s32)((u32)(v3a << 1) >> 16), flags);
        flags = gFrameCount & 15;
        if (flags == 0) {
            rec7b = (u8 *)Object_GetById(9);
            v1b = Engine_RandomNext();
            a0 = *(s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Object_GetById(9);
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
            Call7(GomaHashira_SpawnPillarEffect, a0, rec4->y.fixed, a2, 0, nb, (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
}
