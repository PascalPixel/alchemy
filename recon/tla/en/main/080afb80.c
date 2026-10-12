/* Near miss, differ 2 of 23 (one reordered pair): after UnitGet the native
 * loads the level (ldrb r1, [r0, #15]) before it starts the experience
 * field's offset (movs r3, #146); this, The Broken Seal's source, starts the
 * offset first. From the sched2 dump: both have priority 36; the offset's
 * first half only anti-depends on the call and is preferred over the load,
 * which data-depends on it. Measured: the level read into a local first
 * gives the native order but keeps the unit in r5 and adds the offset after
 * the argument copy (differ 5); the same with a pointer to the field
 * (differ 5); an empty barrier after the local's read (differ 7). */
#include "types.h"
#include "owner_state.h"
#include "battle_unit.h"

struct LevelUpResult;

u32 Owner_GetLevelThreshold(s32 owner, s32 level);
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res);

struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(s32 owner_no, struct LevelUpResult *value)
{
    struct BtlUnit *owner;

    owner = (struct BtlUnit *)UnitGet(owner_no);
    if ((owner->experience >= Owner_GetLevelThreshold(owner_no, owner->level + 1)) &&
        (Owner_LevelUp(owner_no, value) != NULL)) {
        return value;
    }
    return 0;
}
