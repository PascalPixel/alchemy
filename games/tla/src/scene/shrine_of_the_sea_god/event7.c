#include "shrine_of_the_sea_god.h"
#include "text/msg_ids.h"

extern const u8 Data_02003c10[];

void Func_020004b0(void)
{
    struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

    Func_020039d8();
    Func_02003b08(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgFieldStatueLikeDailaShrine, 1);
    work->unk15a = 1;
    Func_020039e0();
}

void Func_020004e0(void)
{
    Func_020039d8();
    Func_02003b08(0);
    ObjectMotion_EnableActionAndResetMotion(11);
    ShrineOfTheSeaGod_Jump(11, 0x100, 20);
    ObjectMotion_SetAngleToward(11, gPartyState.currentOwner, 0);
    Func_02003a70((s32)MsgFieldStartledCompanion);
    Func_02003a80(11, 0);
    Func_02003a80(11, 0);
    Object_SetModeById(11, 4);
    Func_02003a78(11, 0, 20);
    ObjectMotion_EnableActionAndSetCallback(11, Data_02003c10);
    Func_020039e0();
}

void Func_02000554(void)
{
    struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

    work->unk15a = 1;
}
