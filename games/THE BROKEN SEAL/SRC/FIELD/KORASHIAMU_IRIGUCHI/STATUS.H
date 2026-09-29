#ifndef KORASHIAMU_IRIGUCHI_STATUS_H
#define KORASHIAMU_IRIGUCHI_STATUS_H

/* Declarations shared by the Colosso entrance's scene code. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum StatusDispatchMessage {
    MSG_HOPING_GET_IN_SEE_COLOSSO = 0x2057,
    MSG_COLOSSOS_LUCKY_GUESS_MAKES_BATTLES = 0x205c,
    MSG_WANT_WIN_BIG_PUT_ON = 0x205d,
    MSG_DO_KNOW_LUCKY_GUESS = 0x205e,
    MSG_SILENCE = 0x206d,
    MSG_HAVE_MAKE_THROUGH_COUNTLESS_MATCHES = 0x206e,
    MSG_HAVE_PERFECT_TECHNIQUES_LIKE_THOSE = 0x206f,
    MSG_ROBIN_ONLY_ONE_ENTERING_FINALS = 0x20f8,
    MSG_PREPARE_YOURSELVES_CONTESTANTS_FINALS_WILL = 0x2112,
    MSG_IM_RATED_AS_SECOND_BEST = 0x2114,
    MSG_WAAH_DONT_FRIGHTEN_ME = 0x2116,
    MSG_WHO_HAVE_COME_QUESTION_ME = 0x2118,
    MSG_DEKKA_MUST_WIN_FINALS_DEKKA = 0x211b,
    MSG_AM_NAVAMPA_GONDOWAN_SIXTH_RANKED = 0x211f,
    MSG_IM_BUFORD_SEVENTH_SEED = 0x2122,
    MSG_YOU_READY_FOR_FINALS = 0x2125,
    MSG_SEE_YOUVE_MADE_THROUGH_YOUR = 0x2134,
    MSG_COLOSSEUM_ALREADY_FULL_IF_REALLY = 0x2251,
    MSG_WANT_WATCH_FINALS_FROM_GOOD = 0x2254,
    MSG_HOLD_ON_SECOND = 0x2256,
    MSG_WANT_TRY_OUT_LUCKY_GUESS = 0x225a,
    MSG_LOOK_IM_SORRY_BUT_WE = 0x225e
};

#if defined(TBS_EDITION_JA)
#define SCENE_STEP_VALUE 0x22c1
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_STEP_VALUE 0x211a
#else
#define SCENE_STEP_VALUE 0x2138
#endif

/* Fields of the engine's event work, read and written by word. */
#define SCENE_WORK (*(u8 **)&gEventWork)
#define SCENE_PHASE (*(s32 *)(SCENE_WORK + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))
#define RECORD_17 17

/* The scene's action tables, in the overlay's read-only data. */
extern u8 KorashiamuIriguchi_ActionTable1[];
extern u8 KorashiamuIriguchi_ActionTable2[];
extern u8 KorashiamuIriguchi_ActionTable3[];
extern u8 KorashiamuIriguchi_ActionTable4[];
extern u8 KorashiamuIriguchi_ActionTable5[];
extern u8 KorashiamuIriguchi_ActionTable6[];
extern u8 KorashiamuIriguchi_ActionTable7[];
extern u8 KorashiamuIriguchi_ActionTable8[];
extern u8 KorashiamuIriguchi_ActionTable9[];

/* Import veneers outside the engine vocabulary, named by the main-image code
 * each one reaches; the main image leaves the routine that stores the party's
 * items in gInventorySnapshot unnamed, so its veneer is named for that.
 * Old-style declarations: arities vary between call sites. */
void Party_AddActiveOwner();
void Party_RemoveActiveOwner();
void Object_SetActionCallbackAndRefreshById();
void Object_SetActionById();
void InventorySnapshot_Take();

void FieldScene_RunMiddleSequence(void);

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ __attribute__((always_inline)) void bump_step_now(s32 amount)
{
    gEventWork->message += amount;
}

#endif
