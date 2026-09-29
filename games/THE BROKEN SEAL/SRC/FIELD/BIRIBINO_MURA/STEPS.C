#include "MURA.H"
extern u8 MsgBiribinoBottomNotVisibleLooksVery[];
extern u8 MsgFieldPeeredWell[];

void FieldScene_RunScriptedSteps947And29DD(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgBiribinoBottomNotVisibleLooksVery, 1);
    Event_End();
}
