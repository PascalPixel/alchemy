// Draft, differ 81 of 488 (87 before; 2026-10-26, the event work read after the
// controlled owner): written from the listing in the form of The
// Broken Seal's Field_ProcessStep. One step of the leader onto the cell at
// (x, y, z) of layer: records the cell (exit code, push attributes), checks
// warp regions, and while walking rolls encounters, regains PP and recovers
// Djinn every 0x10000 of distance, counts the hazard and wears Avoid off; a
// hazard or poison that hurts the party lists who fell. This game has no
// flinch and counts the hazard on cells with attribute bit 3. Left: the
// native reloads the leader from its stack slot after the targetX test (one
// instruction more, which also moves the literal pool's padding), the
// temporaries of the layer's cells and the 0x80000000 test take other low
// registers, and the usual order differences (pool loads and stores early).
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "object_runtime.h"
#include "map.h"
#include "iwram_call.h"

// The event work as this function reads it.
struct StepWork {
  u8 unk000[0x154];
  s16 exit;     ///< the cell's exit code (1 to 230)
  s16 pushCell; ///< the cell's attributes when any of 0xe2 is set
  u8 unk158[0xc];
  s16 encounterEnemyGroup;
  s16 psynergy; ///< a pending Psynergy request
  u8 unk168[2];
  s16 raisedEvent;
  s16 fallenCount;
  s16 survivorCount;
  s16 fallenOwners[8];
  u8 unk180[0x1c];
  s32 walkingDistanceScale;
  s32 walkingDistance;
  struct MapCell *currentCell;
  struct MapCell *previousCell;
};

// A unit as this function reads it.
struct UnitStepView {
  u8 unk000[0x38];
  s16 hp;
  u8 unk03a[0xf7];
  s8 poison;
};

// gAdventure as this function reads it: struct Adventure's unk24c[6] holds the
// hazard's limit, mode (0 counts steps on hazard cells, 1 counts frames) and
// damage, which adoption names in adventure.h; read through one struct,
// as the native's alias order needs (through struct Adventure and a view of the
// hazard fields, differ 103).
struct PartyStepView {
  u8 unk000[0x212];
  u8 renderMode;
  u8 unk213[5];
  u8 activeOwners[8];
  u8 unk220[0x2c];
  s16 hazardLimit;
  s16 hazardMode;
  s16 hazardDamage;
  u16 hazardSteps;
  u8 unk254[0xa];
  s16 sceneVariant;
  u8 unk260[4];
  s32 avoidDistance;
};
#define gPartyStep (*(struct PartyStepView *)&gAdventure)
extern struct MapCell gMapBlocks[];
extern struct MapCell gMapCellBuffer[];

s32 EventRuntime_GetControlledOwner(void);
struct ObjectRuntime *Func_080cb8e8(void);
u32 Party_CountActiveOwnersFar(void);
struct UnitStepView *Unit_Get(s32 owner);
void Func_080cb1dc(s32 x, s32 y, s32 z);
s32 GameFlag_Test(s32 flag);
s32 Encounter_RollWorldMap(s32 *position, s32 speed);
s32 Encounter_RollTerrain(s32 zone, s32 speed);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
s32 DjinnRecovery_CountDownFar(void);
void Sound_PlayFar(s32 cue);
void Event_ClearValidPackedIds(void);
s32 BattleParty_ApplyStatusDamage(void);
void Func_080d489c(s32 owner, s32 mode);
void BattleParty_ApplyHealthDelta(s32 amount, s32 percent);

void Func_080cb2b8(u32 layer, s32 x, s32 y, s32 z) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 leaderId = EventRuntime_GetControlledOwner();
  struct StepWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Func_080cb8e8();
  s16 previousHp[8];
  struct MapCell *tile;
  struct UnitStepView *unit;
  s32 partyHurt;
  u32 partyCount;
  u32 i;
  u32 event;
  u32 attributes;
  s32 zone;
  s32 speed;
  s32 step;
  s32 distance;
  s32 amount;

  partyHurt = 0;
  if (work == NULL)
    return;

  partyCount = Party_CountActiveOwnersFar();
  for (i = 0; i < partyCount; i++)
    previousHp[i] = Unit_Get(gPartyStep.activeOwners[i])->hp;

  if (*((s8 *)work + 0x18a) == 3) {
    tile = &gMapBlocks[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
    event = tile->collisionCode;
    attributes = 0;
  } else {
    if (layer <= 2)
      tile = map->layers[layer].cells;
    else
      tile = gMapCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    event = tile->collisionCode;
    attributes = tile->attributeB;
  }
  work->previousCell = work->currentCell;
  work->currentCell = tile;
  if (attributes != 0)
    Func_080cb1dc(x, y, z);
  if (event - 1 <= 229)
    work->exit = event;
  if (attributes & 0xe2)
    work->pushCell = attributes;

  if ((gPartyStep.renderMode <= 1 || gPartyStep.renderMode == 6 ||
       gPartyStep.renderMode == 7) &&
      leader != NULL && leader->targetX != (s32)0x80000000) {
    zone = 0;
    speed = leader->speedLimit;
    if (GameFlag_Test(0x167))
      speed <<= 1;
    if (*((s8 *)work + 0x18a) == 3) {
      work->encounterEnemyGroup = Encounter_RollWorldMap(&leader->x, speed);
    } else {
      if (attributes & 4)
        zone = 1;
      work->encounterEnemyGroup = Encounter_RollTerrain(zone, speed);
    }

    step = Iwram_MulQ16(work->walkingDistanceScale, leader->speedLimit);
    if (!zone)
      step /= 2;
    distance = work->walkingDistance + step;
    work->walkingDistance = distance;
    if (distance > 0xffff) {
      amount = distance / 0x10000;
      work->walkingDistance = distance & 0xffff;
      BattleParty_ApplyDrain(amount, 0);
      if (DjinnRecovery_CountDownFar())
        Sound_PlayFar(139);
      Event_ClearValidPackedIds();
      partyHurt = BattleParty_ApplyStatusDamage();
    }

    if (gPartyStep.hazardMode == 0 && (attributes & 8)) {
      if (work->previousCell->attributeB & 8)
        gPartyStep.hazardSteps += leader->speedLimit / 0x10000;
      else
        gPartyStep.hazardSteps = gPartyStep.hazardLimit / 2;
    }

    if (gPartyStep.avoidDistance != 0 && gPartyStep.sceneVariant != 2) {
      gPartyStep.avoidDistance -= leader->speedLimit;
      if (gPartyStep.avoidDistance <= 0) {
        gPartyStep.avoidDistance = 1;
        if (work->psynergy == 0)
          work->psynergy = 0x2096;
      }
    }
  }

  if (gPartyStep.hazardMode == 1) {
    gPartyStep.hazardSteps++;
    if ((s16)gPartyStep.hazardSteps == gPartyStep.hazardLimit / 2)
      Func_080d489c(leaderId, 0x101);
    if ((s16)gPartyStep.hazardSteps == gPartyStep.hazardLimit)
      Func_080d489c(leaderId, 0x100);
  }
  if ((s16)gPartyStep.hazardSteps >= gPartyStep.hazardLimit) {
    amount = gPartyStep.hazardDamage;
    gPartyStep.hazardSteps = 0;
    BattleParty_ApplyHealthDelta(-(amount & 255), amount & 0x100);
    partyHurt++;
  }

  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    Func_080d489c(leaderId, 0x102);
    for (i = 0; i < partyCount; i++) {
      unit = Unit_Get(gPartyStep.activeOwners[i]);
      if (unit->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gPartyStep.activeOwners[i];
        work->raisedEvent = -1;
        unit->poison = 0;
      }
    }
  }
}
