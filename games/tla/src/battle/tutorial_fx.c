#include "types.h"
#include "ram_buffer.h"
#include "battle.h"
#include "battle_work.h"

struct BattleFxRequest {
  s32 field00;
  s32 field04;
  s32 field08;
  s32 field0c;
  s32 field10;
  s32 field14;
  s32 field18;
  u8 unk1c[8];
  s16 field24;
  u8 unk26[0x32];
};

void Func_08138050(struct BattleFxRequest *request, s32 value);

s32 BattleObject_MarkForRelease(s32 objectId);
s32 BattleEnemy_ClaimNameLetter(s32 classId, s32 n);
s32 BattleUnit_SetFar(s32 slot, s32 unit, s32 mask);
s32 Enemy_Update(void);
struct BattleObj *BattleObject_Get(s32 objectId);
void BattleObject_Put(void);

void Func_08119734(s32 id) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 unit;
  s32 *object;

  if (id == 78)
    unit = 373;
  else
    unit = 376;
  BattleObject_MarkForRelease(128);
  BattleMotion_DestroyAllSlotObjects();
  BattleEnemy_ClaimNameLetter(unit, 1);
  BattleUnit_SetFar(128, unit, 255);
  work->enemyIds[0] = 128;
  work->enemyIds[1] = 255;
  Enemy_Update();
  object = (s32 *)BattleObject_Get(128);
  BattleObject_Make((struct BattleObj *)object, 128, object[3] / 0x10000,
                object[4] / 0x10000);
  BattleObject_Put();
}

void Func_081197a8(s32 value) {
  struct BattleFxRequest request;

  request.field10 = 0;
  request.field08 = 128;
  request.field0c = 0;
  request.field14 = 1;
  request.field24 = 128;
  request.field04 = 0;
  request.field00 = 0;
  request.field18 = 0;
  Func_08138050(&request, value);
}
