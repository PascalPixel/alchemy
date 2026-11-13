#ifndef TLA_TRIAL_ROAD_H
#define TLA_TRIAL_ROAD_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "event_inline.h"

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

struct TrialRoadMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct TrialRoadMapWork {
  u8 unk000[0x108];
  struct TrialRoadMapLayer layers[3];
};

/// Trial Road's progress, kept in the inventory snapshot while the trial runs.
struct TrialRoadState {
  s16 order[8]; ///< the dealt stage patterns, two to a stage
  s16 pattern;
  s16 mirrored;
  s16 rivalTimer; ///< frames the rival has spent at its current chest
  s16 stage[2];   ///< the stages the leader (0) and the rival (1) are on
  s16 leaderBest; ///< the furthest stage the leader has reached
  s16 rivalBest;  ///< the furthest stage the rival has reached
};

extern u8 gInventorySnapshot[];

#define gTrialRoadState ((struct TrialRoadState *)(gInventorySnapshot + 0x88))

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Import_FieldTryJumpForward(void);
void Func_02005a38(void);

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Func_02005a50(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_02005a60(s32 a, s32 b, s32 c);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

static __inline__ void Func_02005a50_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02005a50(a, b, c, d, e, f, g);
}

/// The store-flag form of a logical not.
// The equality tests of this stage return 1 - (difference != 0) with no
// branch, which the compiler emits only for !value on a register (as plain
// comparisons they compile to a compare and branch).
static __inline__ s32 IsZero(s32 value) { return !value; }

#endif
