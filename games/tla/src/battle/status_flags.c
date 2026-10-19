#include "types.h"
#include "battle_unit.h"

struct TlaBattleObj {
  struct MotionObject *object;
  u16 resource;
  u16 overlay;
  u16 animation;
  u16 effect;
  s32 anchorX;
  s32 anchorZ;
  s32 palette;
  s32 scale;
  u16 activeConditions;
  u8 selectedCondition;
  s8 cycleTimer;
  void *animationEntry;
  void *effectEntry;
  s16 active;
  u8 fading;
  u8 visible;
};

struct BattleUnit *UnitGet(s32 unitId);

/// Builds a battle object's status icon flags from its unit: poison
/// (1) or deadly poison (2), delusion (0x20), stun (4, not for nine large
/// enemies), Psy Seal (8), haunt (0x40), sleep (0x10) and the Grim Reaper's
/// countdown (bit 6 + count); an object marked visible keeps its flags.
void BattleUnit_BuildStatusFlags(s32 id, struct TlaBattleObj *output) {
  u32 flags = 0;

  if (output->visible == 0) {
    struct BattleUnit *state = UnitGet(id);
    s8 mode = state->poison;

    if (mode == 1)
      flags = 1;
    if (mode == 2)
      flags |= mode;
    if (state->delusion != 0)
      flags |= 0x20;
    if (state->stun != 0) {
      flags |= 4;
      switch (state->enemyId) {
      case 91:
      case 92:
      case 93:
      case 94:
      case 95:
      case 96:
      case 97:
      case 348:
      case 357:
        flags &= ~4;
        break;
      }
    }
    if (state->psySeal != 0)
      flags |= 8;
    if (state->haunt != 0)
      flags |= 0x40;
    if (state->sleep != 0)
      flags |= 0x10;
    if (state->grimReaper != 0)
      flags |= 1 << (state->grimReaper + 6);
    output->activeConditions = flags;
  }
}
