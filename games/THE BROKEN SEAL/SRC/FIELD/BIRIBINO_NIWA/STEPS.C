#include "NIWA.H"
extern u8 MsgBiribinoUponCloserInspectionSeemsDried[];
extern u8 MsgFieldPeeredWell[];

void FieldScene_RunStepWithValueFd2(void)
{
    Event_Begin();
    Actor_SetPosition(0xD, 0, 0);
    GameFlag_Set(0xFD2);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

void FieldScene_RunStepWithValue29de(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgBiribinoUponCloserInspectionSeemsDried, 1);
    Event_End();
}
