#include "types.h"
#include "ram_buffer.h"
#include "battle.h"
#include "battle_work.h"

struct BattleFxRequest {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    u8 unknown_1c[8];
    s16 field_24;
    u8 unknown_26[0x32];
};

void Func_08138050(struct BattleFxRequest *request, s32 value);

s32 ActivateBattleObjectSlot(s32 object_id);
s32 EnemyCountDecrement(s32 class_id, s32 n);
s32 BattleUnitSetFar(s32 slot, s32 unit, s32 mask);
s32 EnemyUpdate(void);
struct BattleObj *BattleObjGet(s32 object_id);
void BattleObjPut(void);

void Func_08119734(s32 id)
{
    struct BattleWork *work = Ram_HeapSlots->battle_work;
    s32 unit;
    s32 *object;

    if (id == 78)
        unit = 373;
    else
        unit = 376;
    ActivateBattleObjectSlot(128);
    BattleMotion_DestroyAllSlotObjects();
    EnemyCountDecrement(unit, 1);
    BattleUnitSetFar(128, unit, 255);
    work->enemy_ids[0] = 128;
    work->enemy_ids[1] = 255;
    EnemyUpdate();
    object = (s32 *)BattleObjGet(128);
    BattleObjMake((struct BattleObj *)object, 128, object[3] / 0x10000, object[4] / 0x10000);
    BattleObjPut();
}

void Func_081197a8(s32 value)
{
    struct BattleFxRequest request;

    request.field_10 = 0;
    request.field_08 = 128;
    request.field_0c = 0;
    request.field_14 = 1;
    request.field_24 = 128;
    request.field_04 = 0;
    request.field_00 = 0;
    request.field_18 = 0;
    Func_08138050(&request, value);
}
