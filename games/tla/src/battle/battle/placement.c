/// @file
/// Battle objects made and placed on the battlefield.
// Part of battle.c.

#include "../battle_internal.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/system/system.h"

struct MotionObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);

/// Resets a battle actor, making its object when it has none: scale 0xe666,
/// the anchors cleared unless keep is set, no sprite, animation, effect or
/// state, facing ArcTan2(0, 0) + 0x8000, visible, then the actor object
/// script.
// The tail of The Broken Seal's BattleObject_Make, for an actor without a unit.
void BattleObject_Reset(struct BattleObj *actor, s32 keep) {
  struct MotionObject *object;
  s32 facingAngle;
  s32 zero;

  if (actor->object == 0)
    object = Func_080200c0(0xf000, 0, 0, 0);
  else
    object = actor->object;
  // The zero in a local of its own, set before the keep test, serves the 0
  // stores on both paths; as plain 0 stores, 36 of 63 instructions differ in
  // tla-en.
  zero = 0;
  actor->scale = 0xe666;
  actor->object = object;
  if (keep == 0) {
    actor->anchorZ = zero;
    actor->anchorX = zero;
    object->x = actor->anchorX;
    object->z = actor->anchorZ;
  }
  actor->palette = 0;
  actor->resource = 0;
  actor->overlay = 0;
  actor->animation = 0;
  actor->effect = 0;
  actor->animationEntry = 0;
  actor->activeConditions = 0;
  actor->effectEntry = 0;
  actor->active = 0;
  actor->selectedCondition = 0;
  actor->fading = 0;
  facingAngle = ArcTan2(0, 0) + 0x8000;
  object->angle = facingAngle;
  object->unk59 = 3;
  object->motionFlags = 2;
  actor->visible = 1;
  Actor_StartScriptFar((struct Actor *)object, Data_0812cacc);
}

void BattleObject_Make(struct BattleObj *actor, s32 unitId, s32 x, s32 y) {
  s32 fixedX;
  s32 fixedZ;
  s32 palette;
  s32 sprite;
  s32 registeredSprite;
  struct BattleUnit *unit;
  struct MotionObject *object;
  s32 facingAngle;
  s32 weaponSprite;

  fixedX = x * 65536;
  fixedZ = y * 65536;
  object = Func_080200c0(0xf000, fixedX, 0, fixedZ);
  unit = Unit_Get(unitId);
  palette = 0;
  registeredSprite = SummonSlot_RegisterActorSprites(unitId);

  if (unit->classIndex == 0) {
    sprite = BattleClass_GetSprite(unit->enemyId);
    if (registeredSprite == 0)
      palette = BattleClass_GetPalette(unit->enemyId);
    else
      sprite = registeredSprite;
  } else {
    switch (unit->enemyId) {
    case 0:
      sprite = 490;
      break;
    case 1:
      sprite = 495;
      break;
    case 3:
      sprite = 504;
      break;
    case 2:
      sprite = 500;
      break;
    case 4:
      sprite = 508;
      break;
    case 6:
      sprite = 517;
      break;
    case 7:
      sprite = 521;
      break;
    case 5:
    default:
      sprite = 513;
      break;
    }
    if ((u32)unitId > 7)
      palette = 1;
  }

  actor->scale = 0x10000;
  actor->effect = 686;

  switch (unit->enemyId) {
  case 0:
    actor->scale = 0x10000;
    break;
  case 1:
    actor->scale = 0x10000;
    break;
  case 3:
    actor->scale = 0x10000;
    break;
  case 2:
    actor->scale = 0x10000;
    break;
  case 7:
    actor->scale = 0x10ccc;
    break;
  case 5:
    actor->scale = 0xf333;
    break;
  case 4:
    actor->scale = 0x10000;
    break;
  case 6:
    actor->scale = 0xe666;
    break;
  case 234:
    actor->scale = 0xe666;
    break;
  case 235:
    actor->scale = 0xe666;
    break;
  case 236:
    actor->scale = 0xe666;
    break;
  case 237:
    actor->scale = 0xe666;
    break;
  case 291:
    actor->scale = 0x19999;
    break;
  case 302:
    actor->scale = 0x18ccc;
    break;
  case 339:
    actor->scale = 0x13333;
    break;
  case 340:
    actor->scale = 0x19999;
    break;
  case 333:
    actor->scale = 0x18000;
    break;
  case 347:
    actor->scale = 0x1cccc;
    break;
  case 358:
    actor->scale = 0x1cccc;
    break;
  case 245:
    actor->scale = 0x10000;
    break;
  case 246:
    actor->scale = 0xe666;
    break;
  case 385:
    actor->scale = 0x10000;
    break;
  case 386:
    actor->scale = 0xe666;
    break;
  case 348:
    actor->scale = 0x1b333;
    break;
  case 357:
    actor->scale = 0x18000;
    break;
  case 359:
    actor->scale = 0x18000;
    break;
  case 360:
    actor->scale = 0x18000;
    break;
  case 361:
    actor->scale = 0x18000;
    break;
  case 362:
    actor->scale = 0x18000;
    break;
  case 363:
    actor->scale = 0x18000;
    break;
  case 364:
    actor->scale = 0x18000;
    break;
  case 365:
    actor->scale = 0x18000;
    break;
  case 366:
    actor->scale = 0x18000;
    break;
  case 367:
    actor->scale = 0x18000;
    break;
  case 263:
    actor->scale = 0x13333;
    break;
  case 264:
    actor->scale = 0x13333;
    break;
  case 297:
    actor->scale = 0x10000;
    break;
  case 298:
    actor->scale = 0x14000;
    break;
  case 305:
    actor->scale = 0xe666;
    break;
  case 307:
    actor->scale = 0x18000;
    break;
  case 311:
    actor->scale = 0x14ccc;
    break;
  case 323:
    actor->scale = 0x13333;
    break;
  case 341:
    actor->scale = 0x10ccc;
    break;
  case 342:
    actor->scale = 0x10ccc;
    break;
  case 343:
    actor->scale = 0x11999;
    break;
  case 344:
    actor->scale = 0x11999;
    break;
  case 345:
    actor->scale = 0x13333;
    break;
  case 346:
    actor->scale = 0x13333;
    break;
  case 336:
    actor->scale = 0x18000;
    break;
  case 349:
    actor->scale = 0x13333;
    break;
  case 350:
    actor->scale = 0x18000;
    break;
  case 351:
    actor->scale = 0x18ccc;
    break;
  case 268:
    actor->scale = 0x14000;
    break;
  case 318:
    actor->scale = 0x14000;
    break;
  case 281:
  case 282:
  case 283:
  case 352:
  case 353:
  case 354:
  case 355:
  case 356:
    actor->scale = 0xe666;
    break;
  case 68:
  case 69:
  case 70:
    actor->scale = 0xf333;
    break;
  case 218:
  case 219:
  case 220:
  case 221:
  case 222:
  case 223:
    actor->scale = 0x14ccc;
    break;
  case 209:
  case 210:
  case 211:
    actor->scale = 0x17333;
    break;
  case 91:
  case 92:
  case 93:
  case 94:
  case 95:
  case 96:
  case 97:
    actor->scale = 0x11999;
    break;
  case 101:
  case 102:
  case 103:
    actor->scale = 0x18000;
    break;
  case 88:
    actor->scale = 0x18000;
    break;
  case 89:
  case 90:
    actor->scale = 0x11999;
    break;
  case 117:
  case 149:
    actor->scale = 0x12666;
    break;
  case 118:
  case 150:
    actor->scale = 0x13333;
    break;
  case 119:
  case 151:
    actor->scale = 0x14000;
    break;
  case 176:
  case 177:
  case 178:
    actor->scale = 0x14000;
    break;
  case 78:
  case 79:
    actor->scale = 0xc000;
    break;
  case 104:
  case 105:
  case 106:
    actor->effect = 685;
    actor->scale = 0x12666;
    break;
  case 107:
  case 108:
  case 109:
    actor->scale = 0x1028f;
    break;
  case 80:
    actor->scale = 0x14000;
    break;
  case 81:
  case 82:
  case 83:
  case 197:
  case 198:
  case 199:
  case 206:
  case 207:
  case 208:
    actor->scale = 0x11999;
    break;
  case 194:
    actor->scale = 0x14000;
    break;
  case 195:
    actor->scale = 0x10000;
    break;
  case 158:
  case 159:
  case 160:
    actor->scale = 0xcccc;
    break;
  case 378:
  case 379:
  case 380:
  case 381:
    actor->scale = 0xcccc;
    break;
  case 74:
  case 75:
  case 76:
  case 98:
  case 265:
  case 331:
  case 332:
  case 369:
  case 373:
  case 376:
    actor->scale = 0x16666;
    break;
  }

  actor->object = object;
  actor->anchorX = fixedX;
  actor->anchorZ = fixedZ;
  actor->palette = palette;
  actor->resource = sprite;
  weaponSprite = BattleUnit_LookupWeaponValueByClass(unitId);
  actor->animation = 0;
  actor->animationEntry = 0;
  actor->effectEntry = 0;
  actor->active = 0;
  actor->overlay = weaponSprite;
  actor->fading = 0;
  actor->visible = 0;
  if (unit->enemyId <= 1 && Inventory_GetEquippedItemFar(unit, 1) == 15) {
    if (unit->enemyId == 0) {
      sprite = 635;
      actor->resource = sprite;
    } else {
      sprite = 637;
      actor->resource = sprite;
    }
    actor->overlay = 0;
  }

  facingAngle = ArcTan2(y / 8, x) + 0x8000;
  object->angle = facingAngle;
  object->unk59 = 3;
  object->motionFlags = 2;
  if (unit->classIndex == 0) {
    object->scaleX = 0x14ccc;
    object->scaleY = 0x14ccc;
  } else {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
  }
  Actor_StartScriptFar((struct Actor *)object, Data_0812cacc);
}

void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y) {
  index *= 2;
  *x = gBattlePlacement_StepPairs[index];
  *y = gBattlePlacement_StepPairs[index + 1];
}

/// Lays out the actors of a summon in a column: each stands at x = -80
/// (nearer, at -50, for a few large enemies) and z steps down by half the
/// width of each actor on either side, centred on the column's middle.
/// Actors 254 and 255 are 25 wide; others are 27 when their summon entry is
/// flagged and 38 when not.
// The Broken Seal's function of the same name, keyed by the enemy id, with
// nine enemies forward where it names two.
void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions,
                            s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  for (index = 0; index != count; index++) {
    u32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;
      u32 enemy;

      spacing = 25;
      if ((u16)(actorIds[index] - 254) > 1) {
        actor = Unit_Get(actorIds[index]);
        spacing = BattleClass_IsSmall(actor->enemyId) ? 27 : 38;
        enemy = actor->enemyId;
        if (enemy == 357 || enemy == 348 || enemy == 91 || enemy == 92 ||
            enemy == 93 || enemy == 94 || enemy == 95 || enemy == 96 ||
            enemy == 97) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if ((u16)(actorIds[index] - 254) > 1) {
      struct BattleUnit *actor = Unit_Get(actorIds[index]);

      spacing = BattleClass_IsSmall(actor->enemyId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}

s32 Enemy_GetFreeSlot(void) {
  s32 i;
  s32 id;

  for (i = 0; i <= 5; i++) {
    id = i + 0x80;
    if (((u8 *)Unit_Get(id))[0x12A] == 0)
      break;
  }
  if (i == 6)
    return -1;
  return id;
}

// Enemy_Update: this game skips leading empty slots (254) and lays out up to
// the last live one.

void Enemy_Update(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 unitIds[14];
  s32 x[6];
  s32 z[6];
  s32 last = -1;
  s32 count = 0;
  s32 i;
  s32 total;

  for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++) {
    if (work->enemyIds[i] == 0xfe) {
      if (count == 0)
        continue;
    } else {
      last = count;
    }
    unitIds[count] = work->enemyIds[i];
    count++;
  }
  total = last + 1;
  Summon_LayoutPositions(unitIds, total, x, z);
  for (i = 0; i < total; i++) {
    s32 id = unitIds[i];

    if (id != 0xfe) {
      struct BattleObj *object = BattleObject_Get(id);

      object->anchorX = x[i] << 16;
      object->anchorZ = z[i] << 16;
    }
  }
}

// BattleUnit_RefreshPlacement: The Broken Seal's source plus the reserve
// owners; the second fill loop needs its pointer set inside the for
// initialiser.

void BattleUnit_RefreshPlacement(void) {
  u16 ids[14];
  s32 x[6];
  s32 z[6];
  s32 count;
  s32 i;
  s32 place;
  s32 stand;
  s32 n;
  s32 value;
  u8 *p;
  struct BattleWork *work;

  place = 0;
  stand = 0;
  work = Ram_HeapSlots->battleWork;
  count = BattleParty_PrepareActiveOwners(ids);
  count += BattleParty_PrepareReserveOwners(ids + count);
  value = 0xff;
  for (i = 13; i >= 0; i--)
    work->placement[i] = value;
  for (n = 5, p = &work->placement[13], value = 13; n >= 0; n--, value--)
    *p-- = value;
  for (i = 0; i < count; place++, i++) {
    s32 id = ids[i];

    work->placement[id] = place;
    BattleObject_Make(BattleObject_Get(id), id,
                      gBattlePlacement_StepPairs[place * 2],
                      gBattlePlacement_StepPairs[place * 2 + 1]);
  }
  for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++)
    ids[i] = work->enemyIds[i];
  count = i;
  Summon_LayoutPositions(ids, count, x, z);
  for (i = 0; i < count; i++, stand++) {
    s32 id = work->enemyIds[i];

    if (id != 0xfe)
      BattleObject_Make(BattleObject_Get(id), id,
                        x[stand], z[stand]);
  }
}

/// Returns 1 if the unit at this formation position is in the list.
/// list: unit IDs, terminated by 0xff, at most 14 entries.
/// no: formation position (0-7 allies, 8-13 enemies).
s32 BattlePlacement_ContainsId(s16 *list, s32 no) {
  u32 i;

  if (no > 7)
    no += 120; // Enemy unit IDs start at 128.
  i = 0;
  do {
    u16 id = *list++;

    if (id == 0xff)
      break;
    if (id == no)
      return 1;
  } while (++i < 14);
  return 0;
}
