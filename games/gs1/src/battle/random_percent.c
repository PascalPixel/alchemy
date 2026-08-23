#include "battle_random.h"

u32 BattleRandomPercent(void);

u32 BattleRandomPercent(void) {
    return (u32) (BattleRandom16() * 0x64) >> 0x10;
}
