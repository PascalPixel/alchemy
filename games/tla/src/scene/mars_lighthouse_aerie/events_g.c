#include "mars_lighthouse_aerie.h"
#include "version.h"
#include "scene_tables.h"

void Func_02004da0(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(2);
#endif
  Actor_FaceActor(2, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId(MsgIUnderstandYour);
  Event_ShowMessage(2, 0);
#if TLA_PATCH_1
  Actor_FaceDirectionAndWait(2, FACING_WEST);
  Func_02004d50(2);
#else
  MarsLighthouseAerie_ObjTurn(2, FACING_WEST);
#endif
  Func_02005614();
}

void Func_02004df0(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(20);
#endif
  FieldEvent_SetMessageId(MsgUhhhhnnnICan);
  Event_ShowMessage(20, 0);
#if TLA_PATCH_1
  Func_02004d50(20);
#endif
  Func_02005614();
}

void Func_02004e20(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(22);
#endif
  FieldEvent_SetMessageId(MsgKoffKoffYou);
  Event_ShowMessage(22, 0);
#if TLA_PATCH_1
  Func_02004d50(22);
#endif
  Func_02005614();
}

void Func_02004e50(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(21);
#endif
  FieldEvent_SetMessageId(MsgUnnnnIAm);
  Event_ShowMessage(21, 0);
#if TLA_PATCH_1
  Func_02004d50(21);
#endif
  Func_02005614();
}
