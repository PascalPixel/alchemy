#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KYUDEN.H"
extern u8 MsgFieldPeeredWell[];
extern u8 MsgTorebiItsFilledWithFreshClean[];

void FieldScene_RunStepWithValue29e0(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgTorebiItsFilledWithFreshClean, 1);
    Event_End();
}
