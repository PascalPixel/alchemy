#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

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

extern s32 FuneKanpan_FlagGroupEntries[];
extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

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

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(base + i) != 0) return FuneKanpan_FlagGroupEntries[i];
    }
    return 0;
}
