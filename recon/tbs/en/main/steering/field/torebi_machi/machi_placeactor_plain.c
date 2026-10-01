/* NONMATCHING: 2026-10-01 brief Wave2 PlaceActor plain-source attempt.
 * Removing this one source device changes ConfigureAndPlaceActorOneHundredTwo.
 * Remaining difference: a direct call changes ConfigureAndPlaceActorOneHundredTwo from mov r0, #102 to lsl r1, r1, #14 (24/24 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
/* Tolbi town: the page effect the scene starts. */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H"
#include "IWRAM_CALL.H"
#include "CALL.H"

extern u8 MsgTorebiFaceAwayTolbi[];
extern u8 MsgTorebiTossLuckyMedal[];

extern u8 MsgTorebiBrotherMeanDuring[];
extern u8 MsgTorebiHaHaHa[];
extern u8 MsgTorebiHoorayFinalsSeven[];
extern u8 MsgTorebiMainStreetTolbi[];
extern u8 MsgTorebiMamaToldShare[];
extern u8 MsgTorebiMamaWhyListen[];
extern u8 MsgTorebiOldestShouldntShare[];
extern u8 MsgTorebiWaahBigBrother[];
extern u8 MsgTorebiWaahSaidMoney[];
extern u8 MsgTorebiWheeFestivalColosso[];
extern u8 MsgTorebiWinBigUsed[];
extern u8 MsgTorebiYayEasyRun[];

extern u8 MsgTorebiChildrenLoveSouvenirs[];
extern u8 MsgTorebiSomeoneLiveMore[];

extern u8 MsgTorebiPatientLittleGuy[];

extern u8 MsgTorebiFestivalLongerUsual[];
extern u8 MsgTorebiFinalsWerent[];
extern u8 MsgTorebiInnsFullStaying[];
extern u8 MsgTorebiLeftovers[];
extern u8 MsgTorebiWaahBuySweets[];
extern u8 MsgTorebiWantTestLuck[];

extern u8 MsgTorebiSeenAnyoneWho[];

void SceneState_SetValues31_2_4(void)
;

/* Tolbi town: the distance between two actor positions, through the
 * resident square root. */
s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
;

/* Tolbi town: actor proximity, the scene tables and the first dialogue steps. */
s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
;

s32 SceneActor_UpdatePartnerProximity(u8 *self)
;

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *TorebiMachi_GetEntrances(void)
;

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
;

/* The eight-byte owner includes the pool word holding this address. */
u8 *TorebiMachi_GetExits(void)
;

/* The eight-byte owner includes the pool word holding this address. */
u8 *TorebiMachi_GetPlacements(void)
;

void FieldScene_RunScene3b5_02000224(void)
;

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Engine_MapCopyCellAttributes(3, 32, 1, 1, a, b);
    Engine_MapObjectSetPosition(102, 0x00380000, 0x01a80000);
}

void HideActorOneHundredTwo(void)
;

void SceneDialogue_RunMessage0e36(void)
;

void SceneDialogue_RunMessage0e37(void)
;

void FieldScene_RunSupplementalSequenceTwo(void)
;

void FieldScene_RunSiblingsTalk(void)
;

/* Tolbi town: the event table by story progress and two actor lines. */
u8 *TorebiMachi_SelectEvents(void)
;

void SceneDialogue_RunActor15Message1f92(void)
;

void SceneDialogue_RunActor24Message1f9d(void)
;

void FieldScene_RunPatientTalk(void)
;

/* Tolbi town: actor lines, the entry setup and the first sequence. */
void FieldScene_RunScene3b5_02000568(void)
;

void SceneDialogue_RunActor27Message1fa3(void)
;

void SceneDialogue_RunActor24Message235f(void)
;

void FieldScene_RunScene3b5_020005dc(void)
;

void SceneScript_SetupActors(void)
;

/*
 * Update callback: draw the object at the party leader's sprite priority, in
 * both places its sprite keeps it, and clear its priority flags.
 */
void SceneActor_CopyPlayerModeToActor(union FieldObject *object)
;

s32 TorebiMachi_ApplyEntryState(s32 a0)
;

void FieldScene_RunScene3b5SequenceA(void)
;

void SceneState_SetValue30ThenCall(void)
;

void SceneState_PassWorkHalfword16C(void)
;

void FieldScene_RunPrimarySequence(void)
;

/* Tolbi town: actor 9's reset and its map tiles. */
void FieldScene_ResetActor9AndDrawTiles(void)
;
