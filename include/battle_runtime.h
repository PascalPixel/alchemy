#ifndef ALCHEMY_BATTLE_RUNTIME_H
#define ALCHEMY_BATTLE_RUNTIME_H

#include "battle_event.h"
#include "battle_types.h"

/* Battle-facing views of the polymorphic runtime lookup functions. */
struct BattleUnit *Func_08077008(s32 unit_id);
void Func_08077010(s32 unit_id);
struct BattleAction *Func_08077080(s32 action_id);
s32 Func_080770c0(s32 flag_id);
void Func_08077128(s32 unit_id);
s32 Func_080771a0(void);
s32 Func_080772b8(s32 effect_id);

#define BattleUnit_Get          Func_08077008
#define BattleUnit_Recalculate  Func_08077010
#define BattleUnit_UpdateRatios Func_08077128
#define BattleAction_Get        Func_08077080
#define BattleFlag_Test         Func_080770c0
#define BattleRandom_Next       Func_080771a0
#define BattleEffect_Classify   Func_080772b8

#endif
