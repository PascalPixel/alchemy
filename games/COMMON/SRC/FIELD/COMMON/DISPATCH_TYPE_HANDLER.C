#include "PROTO.H"
#include "TYPES.H"
#include "SCENE.H"
s32 BattleFx_Run(void);
s32 BattleFx_DispatchRequestKind(void);


void Field_DispatchTypeHandler(s32 kind)
{
    if (kind == 0) {
        BattleFx_Run();
    } else if (kind == 1) {
        BattleFx_DispatchRequestKind();
    } else if (kind == 2) {
        BattleFx_ClearChildValueOnMismatch();
    } else if (kind == 3) {
        FieldEvent_RunTypeHandler();
    }
}
