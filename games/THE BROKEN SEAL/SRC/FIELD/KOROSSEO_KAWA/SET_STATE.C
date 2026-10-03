#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "KAWA.H"
extern u8 MsgKorosseoMatchAboutBeginPleaseTake[];

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

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};


/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 HexDigits[];

struct FieldActor *Object_GetById(s32);
void GameFlag_SetByte();
void GameFlag_SetByte(s32, s32);
void Korosseo_SelectSoloCompetitor(s32);
typedef void(*SceneTask)(void);
PartyInteractionRecord *GetPartyInteractionRecord(void);
s32 GetPartyMemberCount(void);
Rec *Owner_GetState(s32);

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* A countdown word this overlay owns at KorosseoKawa_Countdown: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 a, s32 b);

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);


void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c);

static inline void InitializeActorZero(void)
{
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

/* Selects a later line in the current dialogue. */
static __inline__ void AdvanceMessage(s32 amount)
{
    gEventWork->message += amount;
}

void SceneState_SetStateHalfword386To99WhenMatched(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 sel = gGameState.selected_actor;

    if (sel != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == sel
        && Engine_GameFlagIsSet(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 val = 99;

        *p = val;
    }
}

void FieldScene_RunNearestActor165Scene(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = gGameState.selected_actor;
    struct FieldActor *p = Object_GetById(n);
    s32 i;
    s32 *q;
    s32 base;

    Engine_EventBegin();
    for (i = 8; i <= 66; i++) {
        struct FieldActor *o = Object_GetById(i);

        if (o != 0 && o->active == 1 && ((struct AnimationObject *)o->sprite)->entries[0]->anim_id == 165) {
            s32 dx = (p->x.fixed - o->x.fixed) / 65536;
            s32 dy = (p->z.fixed - o->z.fixed) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Engine_EventSetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
    Engine_EventShowMessage(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    base = n << 4;
    GameFlag_SetByte(base + 880, p->x.fixed >> 20);
    {
        s32 v = p->z.fixed >> 20;

        GameFlag_SetByte(base + 888, v);
    }
    n++;
    if (n > 3) {
        Engine_EventRequestExit(10);
        Engine_GameFlagSet(282);
    } else {
        Korosseo_SelectSoloCompetitor(n);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        *q = 0;
    }
    Engine_EventEnd();
}
