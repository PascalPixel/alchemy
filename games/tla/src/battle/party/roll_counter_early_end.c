#include "types.h"
#include "battle_party.h"
#include "owner_state.h"
#include "scene.h"
#include "battle_unit.h"

s32 BattleRandomFar(void);

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct BattleUnit *state = Unit_Get(unitId);

  if (turns <= 5) {
    s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (BattleRandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

s32 BattleUnit_TickAttackModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);

  if (state->attackTurns != 0) {
    s32 zero;
    state->attackTurns--;
    zero = 0;
    if ((s8)state->attackTurns == 0) {
      state->attackStage = zero;
      return 1;
    }
    if (state->attackStage < 0) {
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->attackTurns, 30) != 0) {
        state->attackStage = zero;
        state->attackTurns = zero;
        return 1;
      }
    }
  }
  return 0;
}

s32 BattleUnit_TickDefenseModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);

  if (state->defenseTurns != 0) {
    state->defenseTurns--;
    if (state->defenseTurns == 0) {
      state->defenseStage = 0;
      return 1;
    }
    if (state->defenseStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->defenseTurns, 20) != 0) {
      state->defenseStage = 0;
      state->defenseTurns = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickResistanceModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);

  if (state->resistanceTurns != 0) {
    state->resistanceTurns--;
    if (state->resistanceTurns == 0) {
      state->resistanceStage = 0;
      return 1;
    }
    if (state->resistanceStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->resistanceTurns, 20) !=
            0) {
      state->resistanceStage = 0;
      state->resistanceTurns = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickDelusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);
  if (state->delusion != 0) {
    state->delusion--;
    if (state->delusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->delusion, 30) != 0) {
      state->delusion = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickConfusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);
  if ((u8)state->confusion != 0) {
    (*(u8 *)&state->confusion)--;
    if (state->confusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)state->confusion, 60) != 0) {
      state->confusion = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickCharm(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);
  if (state->charm != 0) {
    state->charm--;
    if (state->charm == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->charm, 70) != 0) {
      state->charm = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickStun(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);
  if (state->stun != 0) {
    state->stun--;
    if (state->stun == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->stun, 40) != 0) {
      state->stun = 0;
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_TickSleep(s32 unitId) {
  struct BattleUnit *state = Unit_Get(unitId);
  if (state->sleep != 0) {
    state->sleep--;
    if (state->sleep == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->sleep, 50) != 0) {
      state->sleep = 0;
      return 1;
    }
  }
  return 0;
}

s32 Battle_AdvanceCounterAndCheckChance(s32 id) {
  s32 next;
  s32 expired;
  u8 remaining;
  s32 turns;
  s32 packed;
  struct BattleUnit *unit;

  unit = Unit_Get(id);
  packed = unit->psySeal;
  turns = packed & 0xFF;
  if (turns != 0) {
    if ((u32)turns > 7U) {
      turns += 0xF8;
      unit->psySeal = turns;
      packed = turns;
    }
    if (packed & 7) {
      next = packed + 0xFF;
      unit->psySeal = next;
      packed = next;
    }
    expired = 1;
    remaining = packed;
    if (remaining == 0)
      return expired;
    if ((u32)remaining <= 7U &&
        BattleUnit_RollCounterEarlyEnd(id, unit->psySeal, 0x1E) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  expired = 0;
  return expired;
}
