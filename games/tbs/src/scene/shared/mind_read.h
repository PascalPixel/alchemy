/// @file
/// Kolima and Log Rooms A: Mind Read cast in an event.
#include "types.h"
#include "event.h"
#include "ability_ids.h"

/// The caster begins Mind Read on the target.
static void Scene_StartMindRead(s32 casterId, s32 targetId) {
  FieldPsynergy_LoadResources(ABILITY_MIND_READ, 1);
  FieldPsynergy_SetupActorPair(casterId, targetId);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  wait(1);
}

/// Finishes Mind Read and releases its effect actors.
static void Scene_EndMindRead(void) {
  FieldPsynergy_RunHandler(2);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}
