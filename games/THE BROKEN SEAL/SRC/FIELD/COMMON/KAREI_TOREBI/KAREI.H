/* Declarations shared by Kalay's Tolbi-road sources (resource_3ae). */
#ifndef KAREI_H
#define KAREI_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

extern u8 *Data_03001ebc;

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

#include "TYPES.H"

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

extern s16 Data_02000240[];

/*
 * Data selector for resource_3ae.  The 164-byte owner includes its 13-word
 * literal pool.
 */

/* Per-call-site import labels.  All four sites reach the same event-flag test,
 * but each keeps its own name because the relocated call word differs. */

/*
 * All four symbols name the loader-relocated call word each site holds; they
 * reach one event-flag test.
 */

/*
 * Table getter for resource_3ae. The owner at 0x02000098 is eight bytes and
 * includes its one pool word at 0x0200009c: the pc-relative load reads that
 * word, so the word is part of the owner and not a following gap.
 *
 * The pool word is an address that is returned without being dereferenced.
 * Many getters share this body but each returns a different address, so the
 * constant here is not interchangeable with theirs.
 */

/*
 * The selector is the signed halfword at byte offset 448 of the resident
 * workspace, Data_02000240[224].  It is compared as a signed 32-bit value
 * against full words held in the pool, which is why the three constants are
 * spelled as the addresses of Value_ symbols rather than as immediates.
 * 0x93e and 0x950 are read as event-flag ids only from their use as the sole
 * argument of the flag test.
 */

/*
 * Pick one of eight in-image data tables from the scene selector and two
 * event flags. The selector is the signed halfword at offset 448 of the
 * shared workspace, and each arm compares it as a signed 32-bit value
 * against a full word, which is why the selector constants are spelled as
 * addresses. 0x93e and 0x950 are taken to be event-flag ids only from their
 * use here. The 164-byte owner includes its thirteen-word literal pool.
 */

#include "TYPES.H"

#include "TYPES.H"



s32 UiWindow_Create();
void UiWork_PushValueSlot();
void UiText_DrawCharacterAtOffset();
void UiText_DrawNumberInWindow();
void UiWork_Finalize();
void UiWork_Finalize();
void UiWork_Finalize();
void Party_AdjustSixDigitCounterA();
void Battle_ResetEffectCounter();
void Motion_LaunchFromFocusedObject();
void Battle_ResetEffectCounter();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Runs a fixed sequence of scene calls (positioning, timing, and animation
 * triggers) for scene 3ae, sequence A. */

#include "TYPES.H"


s32 PartyInventory_FindOwner();
s32 Inventory_Find();
void Inventory_Discard();
void Engine_EventShowMessage();

/* Contiguous unnamed leaf-owner run for resource_3ae. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

void Engine_EventBegin(void);

#include "TYPES.H"


#include "TYPES.H"


/* The sources call one another across files. */
void SceneState_ApplyValues14And0And5(void);
s32 SceneData_ReturnZero(void);

/* The scene table in the overlay's data. */
extern u8 KareiTorebi_SceneTable[];
u8 *SceneData_GetTable98a0(void);
void FieldScene_RunScene3ae_02000144(void);
void SceneDialogue_ShowLine1CF8(void);
void FieldScene_RunScene3ae_02000260(void);
void FieldScene_RunScene3ae_020002dc(void);
void SceneDialogue_RunActor12Event(void);
void FieldScene_RunActorThirteenFlagDialogue(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void SceneDialogue_RunActor8Message1f09(void);
void SceneDialogue_RunActor10Message1f15(void);
void FieldScene_RunScene3ae_020006c8(void);
s32 FieldScene_RunScene3ae_0200086c(void);
void FieldScene_RunScene3ae_020008cc(void);
void SceneState_SetRuntimeWord448To521AndSend303(void);
void FieldScene_PlaceSlots14And15(void);
void SceneActor_SetMode3AndFlagBit1(s32 no);
void FieldScene_RunScene3aeSequenceA(void);
void FieldScene_RunScene3aeSequenceB(void);

#endif
