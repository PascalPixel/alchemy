#include "types.h"
#include "ram_buffer.h"
#include "battle.h"
#include "btlwork.h"

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
s32 EnmCntDec(s32 class_id, s32 n);
s32 BtlUnitSetFar(s32 slot, s32 unit, s32 mask);
s32 EnmUpd(void);
struct BtlObj *BtlObjGet(s32 object_id);
void BtlObjPut(void);

void Func_08119734(s32 id)
{
    struct BtlWork *work = Ram_HeapSlots->battle_work;
    s32 unit;
    s32 *object;

    if (id == 78)
        unit = 373;
    else
        unit = 376;
    ActivateBattleObjectSlot(128);
    BattleMotion_DestroyAllSlotObjects();
    EnmCntDec(unit, 1);
    BtlUnitSetFar(128, unit, 255);
    work->enm_no[0] = 128;
    work->enm_no[1] = 255;
    EnmUpd();
    object = (s32 *)BtlObjGet(128);
    BtlObjMake((struct BtlObj *)object, 128, object[3] / 0x10000, object[4] / 0x10000);
    BtlObjPut();
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
