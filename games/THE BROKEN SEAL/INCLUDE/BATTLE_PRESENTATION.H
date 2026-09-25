#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

#include "TYPES.H"

struct BattleCamera {
    u8 unknown_00[12];
    s32 pos[3];
    s32 unknown_18;
    s32 *follow_pos;
    s32 distance;
    u8 unknown_24[16];
    u16 pitch;
    u16 yaw;
};

struct BattlePresentationTransition {
    s32 target_yaw;
    s32 frames;
    u8 unknown_08[12];
    s32 flag;
};

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattlePresentation_RunAction(s16 *action);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
