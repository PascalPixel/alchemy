#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

#include "TYPES.H"

s32 BattlePresentation_RunAction(s16 *action);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
