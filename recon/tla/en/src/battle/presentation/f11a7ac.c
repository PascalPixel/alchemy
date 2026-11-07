#include "types.h"
#include "motobj.h"
#include "objdisp.h"
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

extern const s32 Data_0812cacc[];

struct MotionObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
struct BattleUnit *Unit_Get(s32 unitId);
s32 SummonSlot_RegisterActorSprites(s32 unitId);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *unit, s32 slot);
s32 ArcTan2(s32 y, s32 x);

void BattleObject_Make(struct TlaBattleObj *actor, s32 unitId, s32 x, s32 y) {
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
  Actor_StartScriptFar((struct ActorView *)object, Data_0812cacc);
}
