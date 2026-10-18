//--------------------------------------------------------------------------
// dispatch_type_handler.c
// Field Psynergy: the dispatchers that run each animation
//--------------------------------------------------------------------------

#include "types.h"
#include "ram_buffer.h"
#include "fx_scene.h"
#include "party_state.h"
#include "object_runtime.h"

// The Psynergy animations (BattleFxScene.animation) the dispatchers name.
// The Broken Seal has 1-16; ⚓️ adds 17-30.
enum FieldPsynergyAnimation {
  FIELD_FX_PUSH = 1,  // pushes the target the way the pad points
  FIELD_FX_WAVE = 2,  // the hue wave from the caster to the target
  FIELD_FX_PAUSE = 9, // stops an object in its tracks
};

void BattleFx_Run(void);
void BattleFx_DispatchRequestKind(void);
void BattleFx_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);

void BattleFx_RunItemBreakSequence(void);
void Func_080ddea8(void);
void Func_080df6f4(void);
void Func_080de21c(void);
void RunBattleEffect05(void);
void RunBattleEffect14(void);
void BattleFx_RunOrbitingParticles(void);
void RunBattleEffect03(void);
void BattleFx_RunBurstParticles(void);
void Func_080e03cc(void);
void BattleFx_RunEffect15(void);
void BattleFx_RunEffect16(void);
void Func_080decb8(void);
void Func_080dd820(void);
void Func_080e09c0(void);
void Func_080e1f2c(void);
void Func_080e3074(void);
void Func_080e3d04(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e9090(void);
void BattleFx_RunViewSway(void);
void Func_080e9af4(void);
void Func_080e4a34(void);
void BattleFx_RunEffect30(void);
void Func_080e7844(void);
void Func_080e82cc(void);
void BattleFx_ResumeObject(s32 id);
s32 BattleFx_FindNearestObject(s32 self, s32 animation);
s32 BattleFx_FilterObjectIdByFlags(s32 id);
void *Func_080cce94(s32 id);
void BattleFx_SetupObjectPair(s32 userId, s32 targetId);
void BattleFx_MarkChildAndRunFallbackTransition(s32 id);
void BattleEffect_PauseObject(s32 id);
void Func_080e0134(void);
void Func_080dd950(void);
void Func_080dc978(s32 sourceId, s32 targetId);
void Func_080dd054(s32 targetId);
void Func_080ddda0(s32 targetId);
void Func_080df1fc(s32 targetId);
void BattleFx_CallEffect04(s32 targetId);
void BattleFx_CallEffect05(s32 targetId);
void BattleFx_StartOrbitingParticles(s32 targetId);
void Func_080dda30(s32 targetId);
void BattleFx_CallEffect03AndStop(s32 targetId);
void BattleFx_CallEffect14(s32 targetId);
void BattleFx_CallEffect13(s32 targetId);
void Func_080e0978(s32 targetId);
void BattleFx_CallEffect23(s32 targetId);
void BattleFx_CallEffect26(s32 targetId);
void BattleFx_CallSpinEffect(s32 targetId);
void BattleFx_CallEffect20(s32 targetId);
void Func_080e8db0(s32 targetId);
void Func_080e92b4(s32 targetId);
void BattleFx_CallEffect24(s32 targetId);
void Func_080e7818(s32 targetId);
void BattleFx_CallEffect22(s32 targetId);
void BattleFx_CallEffect21(s32 targetId);
void BattleFx_CallEffect18(s32 targetId);

//--------------------------------------------------------------------------
// Runs the animation's handler for kind: 0 runs it with no target chosen, 1
// on the chosen target (secondObjectId), 2 finishes it and 3 runs the kind's
// extra step.
//--------------------------------------------------------------------------
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleFx_Run();
  else if (kind == 1)
    BattleFx_DispatchRequestKind();
  else if (kind == 2)
    BattleFx_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

// The Spanish and Italian scaffolds carry three of the handlers under other
// names (Func_080dc978 as Func_080cdec8, the far-call slot that reaches it
// there; their Func_080dd054 and Func_080decb8 label other functions), and
// the French one has BattleFx_CallEffect23 and Func_080e4a34 under the names of
// BattleFx_CallEffect24 and Func_080e9af4. Until those names are sorted out, the
// three editions keep both dispatchers on their scaffold (S4).
#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_FR) &&                    \
    !defined(TLA_EDITION_IT)
// Run the field Psynergy animation the scene names, with no target chosen
// yet: each kind runs its effect to the end.
void BattleFx_Run(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->animation;
  s32 targetId = scene->secondObjectId;
  s32 objectId;

  if (animation == FIELD_FX_PUSH)
    BattleFx_RunItemBreakSequence();
  else if (animation == 7)
    Func_080ddea8();
  else if (animation == 11)
    Func_080df6f4();
  else if (animation == 4)
    Func_080de21c();
  else if (animation == 5)
    RunBattleEffect05();
  else if (animation == 14)
    RunBattleEffect14();
  else if (animation == 6)
    BattleFx_RunOrbitingParticles();
  else if (animation == 3)
    RunBattleEffect03();
  else if (animation == 12)
    BattleFx_RunBurstParticles();
  else if (animation == 13)
    Func_080e03cc();
  else if (animation == 15)
    BattleFx_RunEffect15();
  else if (animation == 16)
    BattleFx_RunEffect16();
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 17)
    Func_080e09c0();
  else if (animation == 29)
    Func_080e1f2c();
  else if (animation == 25)
    Func_080e3074();
  else if (animation == 26)
    Func_080e3d04();
  else if (animation == 21)
    Func_080e572c();
  else if (animation == 18)
    Func_080e5d5c();
  else if (animation == 20)
    Func_080e68d0();
  else if (animation == 19)
    Func_080e9090();
  else if (animation == 27)
    BattleFx_RunViewSway();
  else if (animation == 24)
    Func_080e9af4();
  else if (animation == 23)
    Func_080e4a34();
  else if (animation == 30)
    BattleFx_RunEffect30();
  else if (animation == 22)
    Func_080e7844();
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the nearest one that
    // has an event to run; with none, play the fallback transition.
    struct PartyState *party = &gPartyState;
    s32 paused;

    /* FAKEMATCH: ⚓️ loads the party state before it builds the field offset
     * (differ 4 without the barrier). */
    asm volatile("");
    paused = party->pausedObject;
    /* FAKEMATCH: ⚓️ loads the paused id between the two halves of the -1
     * it compares with (differ 2 without the barrier). */
    asm volatile("");
    if (paused != -1) {
      BattleFx_ResumeObject(paused);
      party->pausedObject = -1;
    }
    objectId = BattleFx_FindNearestObject(scene->firstObjectId, animation);
    objectId = BattleFx_FilterObjectIdByFlags(objectId);
    if (Func_080cce94(objectId) != 0) {
      BattleFx_SetupObjectPair(scene->firstObjectId, objectId);
      BattleFx_MarkChildAndRunFallbackTransition(objectId);
      BattleEffect_PauseObject(objectId);
      party->pausedObject = objectId;
    } else {
      Func_080e0134();
    }
  } else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first.
    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    Func_080dc978(scene->firstObjectId, targetId);
  }
}

// Run the field Psynergy animation the scene names on the target the player
// chose (BattleFxScene.secondObjectId).
void BattleFx_DispatchRequestKind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->animation;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  if (animation == FIELD_FX_PUSH)
    Func_080dd054(targetId);
  else if (animation == 7)
    Func_080ddda0(targetId);
  else if (animation == 11)
    Func_080df1fc(targetId);
  else if (animation == 4)
    BattleFx_CallEffect04(targetId);
  else if (animation == 5)
    BattleFx_CallEffect05(targetId);
  else if (animation == 6)
    BattleFx_StartOrbitingParticles(targetId);
  else if (animation == 12)
    Func_080dda30(targetId);
  else if (animation == 3)
    BattleFx_CallEffect03AndStop(targetId);
  else if (animation == 14)
    BattleFx_CallEffect14(targetId);
  else if (animation == 13)
    BattleFx_CallEffect13(targetId);
  else if (animation == 16)
    BattleFx_RunEffect16();
  else if (animation == 17)
    Func_080e0978(targetId);
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 15)
    BattleFx_RunEffect15();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 23)
    BattleFx_CallEffect23(targetId);
  else if (animation == 26)
    BattleFx_CallEffect26(targetId);
  else if (animation == 25)
    BattleFx_CallSpinEffect(targetId);
  else if (animation == 20)
    BattleFx_CallEffect20(targetId);
  else if (animation == 19)
    Func_080e8db0(targetId);
  else if (animation == 27)
    Func_080e92b4(targetId);
  else if (animation == 24)
    BattleFx_CallEffect24(targetId);
  else if (animation == 30)
    Func_080e7818(targetId);
  else if (animation == 22)
    BattleFx_CallEffect22(targetId);
  else if (animation == 21)
    BattleFx_CallEffect21(targetId);
  else if (animation == 18)
    BattleFx_CallEffect18(targetId);
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first; a target that
    // is not the paused object stops walking while the wave runs.
    struct PartyState *party;

    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party state before it builds the field offset
     * (differ 2 without the barrier). */
    asm volatile("");
    if (party->pausedObject != scene->secondObjectId)
      ((struct ObjectRuntime *)scene->child)->movementState = 1;
    Func_080dc978(scene->firstObjectId, targetId);
  } else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the target.
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: ⚓️ loads the party state before it builds the field offset
     * (differ 4 without the barrier). */
    asm volatile("");
    if (party->pausedObject != -1) {
      BattleFx_ResumeObject(party->pausedObject);
      party->pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    party->pausedObject = targetId;
    BattleFx_MarkChildAndRunFallbackTransition(targetId);
  }
}
#endif
