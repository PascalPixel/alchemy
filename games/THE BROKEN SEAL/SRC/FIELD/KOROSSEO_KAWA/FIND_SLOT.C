#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"

#define Object_SetMoveTarget ((void (*)(struct FieldActor *, s32, s32, s32))Object_SetMoveTarget)

enum CoordinatorMessage {
    MSG_ROBIN_GOT = 0x96a,
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_ROBIN_WILL_CHEER_FOR_WAY = 0x207f,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_OPERATOR_BRIDGE_WILL_ALSO_CHEER = 0x2094,
    MSG_THEY_CALL_BROKEN_BRIDGE = 0x2095,
    MSG_LOGS_KEY_CLEARING_STAGE = 0x2098,
    MSG_PLACE_NORMALLY_CALLED_LUMBER_WATER = 0x2099,
    MSG_SITE_FIRST_FINALS_BATTLE = 0x20cb,
    MSG_ASK_ATTENDANTS_FOR_EXPLANATIONS_STAGES = 0x20d4,
    MSG_WARRIORS_ENTER_FINALS_WITHOUT_ANY = 0x20d5,
    MSG_ROBIN_YOURE_CONTESTANT_IN_FINALS = 0x20e1,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8
};

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    s32 handle;
} ActiveSubjectSlot;

extern u8 Korosseo_GaugeGraphics[];
extern u8 Korosseo_UpdatePathRival[];
extern u8 HexDigits[];

typedef void(*SceneTask)(void);
void Korosseo_DrawGauge(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);

void Vector_AddPolarOffset();

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

s32 Object_CheckMovementCollision(struct FieldActor *, Position3 *);



void Script_WaitForEventTimeout(struct FieldActor *);

struct FieldActor *SceneActor_FindSlotAtTilePosition(Position3 *pos, struct FieldActor *subject)
{
    struct FieldActor **slots = gEventWork->placed_actors - 8;
    u32 i;

    for (i = 8; i <= 65; i++) {
        struct FieldActor *p = slots[i];

        if ((pos->x >> 20) == (p->x.fixed >> 20)
            && (pos->y >> 20) == (p->y.fixed >> 20)
            && (pos->z >> 20) == (p->z.fixed >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{

    struct FieldActor *subject;
    struct FieldActor *target;
    struct FieldActor *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)&gGameState + (idx << 1));
    subject = Object_GetById(handle);

    dir = subject->facing >> 12;

    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = subject->x.fixed + (s32)(step & 0xffff0000);
    pos.y = subject->y.fixed;
    step <<= 16;
    pos.z = subject->z.fixed + (s32)step;

    target = SceneActor_FindSlotAtTilePosition(&pos, subject);
    if (target == 0) {
        return;
    }

    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    blocker = SceneActor_FindSlotAtTilePosition(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    pos.x = target->x.fixed;
    pos.y = target->y.fixed + 0x100000;
    pos.z = target->z.fixed;

    blocker = SceneActor_FindSlotAtTilePosition(&pos, target);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) {
        return;
    }

    target->terrain_id = 2;
    zero = 0;

    step = KorosseoKawa_DirectionSteps[dir];
    pos.x = target->x.fixed + (s32)(step & 0xffff0000);
    pos.y = target->y.fixed;
    step <<= 16;
    pos.z = target->z.fixed + (s32)step;

    if (Object_CheckMovementCollision(target, &pos) > 0) {
        return;
    }

    Object_SetMode(subject, 8);
    Engine_TaskWait(15);

    target->speed = 0x3333;
    target->acceleration = 0x3333;
    Object_SetMoveTarget(target, pos.x, pos.y, pos.z);

    subject->speed = 0x3333;
    subject->acceleration = 0x3333;
    Object_SetMoveTarget(subject, pos.x, pos.y, pos.z);

    Engine_AudioPlayCue(0xee);
    Script_WaitForEventTimeout(target);
    Engine_AudioPlayCue(0x120);

    target->x.fixed = pos.x;
    target->z.fixed = pos.z;
    target->velocity_x = zero;
    target->velocity_z = zero;

    Object_SetMode(subject, 1);
}

struct FieldActor *SceneActor_FindOccupantAheadOfSubject(void)
{

    struct FieldActor *rec;
    s32 facing;
    Position3 pos;
    struct FieldActor *hit;

    rec = Object_GetById(((ActiveSubjectSlot *)&gGameState)->handle);

    facing = (rec->facing + 0x2000) & 0xc000;

    pos.x = (rec->x.fixed & 0xfff00000) + 0x80000;
    pos.y = rec->y.fixed;
    pos.z = (rec->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, facing, &pos);

    hit = SceneActor_FindSlotAtTilePosition(&pos, rec);
    if (hit == 0) {
        pos.x = (rec->x.fixed & 0xfff00000) + 0x80000;
        pos.y = rec->y.fixed;
        pos.z = (rec->z.fixed & 0xfff00000) + 0x80000;
        Vector_AddPolarOffset(0x200000, facing, &pos);

        hit = SceneActor_FindSlotAtTilePosition(&pos, rec);
    }

    return hit;
}
