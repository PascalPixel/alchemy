#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 FuneKanpan_LeadActionsA[];
extern u8 FuneKanpan_LeadActionsB[];
extern u8 FuneKanpan_LeadActionsC[];
extern u8 FuneKanpan_CrewScript[];
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
u8 *Object_GetById();

enum MultiEncounterMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_TOLD_WERE_LEAVING_SOON_SET = 0x1d36,
    MSG_IF_WE_DONT_LEAVE_SOON = 0x1d37,
    MSG_SOMEBODY_STOP_THEM = 0x1d6f,
    MSG_THEY_CANT_PLANNING_MUTINY = 0x1d70,
    MSG_DIDNT_DO_ANYTHING = 0x1d8d,
    MSG_NOW_WE_HAVE_PROTECT_SHIP = 0x1e08,
    MSG_HAVE_MAKE_THEM_PROMISE_HELP = 0x1e09,
    MSG_PREPARATIONS_READY = 0x1e39,
    MSG_AYE_CAPTAIN_SEA_MONSTERS = 0x1e41,
    MSG_THANK_ROBIN_DID_GOOD_AGAINST = 0x1ee1,
    MSG_CAN_SEE_LAND = 0x1ee5,
    MSG_ROBIN_DONT_TALK_LIKE_SHOULDNT = 0x1f53,
    MSG_ROBIN_TALKED_PASSENGERS_DIDNT_TOUR = 0x1f55,
    MSG_SEE_YOURE_GOING_GO_FOR = 0x1f5b,
    MSG_HOW_WAS_ROBIN_DID_EXPLORE = 0x1f69
};

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);
void Event_CallWithLastActiveObjectId();


/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{

    f(a0);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif

#if defined(TBS_EDITION_JA)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif
#if defined(TBS_EDITION_DE)
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_SetPosition(21, 0x1060000, 0x2C20000);
    *(u16 *)(Object_GetById(21) + 6) = m;
    Actor_SetPosition(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Object_GetById(24) + 6) = z;
    }
    Actor_SetSpritePriority(24, 1);
    Actor_SetPosition(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Object_GetById(25) + 6) = x << 8;
    }
    Actor_SetSpritePriority(25, 1);
    Actor_SetPosition(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Object_GetById(26) + 6) = x << 8;
    }
    Actor_SetPosition(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Object_GetById(27) + 6) = m;
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(20, 0, 0);
}

void FieldScene_RunScene3af_0200185c(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0xcc0000, 0x2090000);
    record = Object_GetById(22);
    *(s32 *)(record + 12) = 0x100000;
    bits = 128;
    {
        u8 *record = ((s32 (*)())Object_GetById)(22);
        u8 value = record[89];

        record[89] = value | bits;
    }
    Actor_SetSpeed(22, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsA);
    {
        u8 *record = Object_GetById(21);

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

void FieldScene_RunScene3af_02001920(void)
{
    u8 *record;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Object_GetById(22);
    {
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        u8 *record = Object_GetById(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}
void FieldScene_RunScene3af_02004218(void);
