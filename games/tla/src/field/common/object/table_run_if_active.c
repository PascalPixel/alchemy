//--------------------------------------------------------------------------
// table_run_if_active.c
// Field: finish the speaker display.
//--------------------------------------------------------------------------

#include "types.h"

s32 UiWork_FinalizeEntityMatchingLocalizedIdFar();
s32 ObjectTable_ReadActiveValue();

//--------------------------------------------------------------------------
// If the actor has graphics, use its graphics ID as speaker and end the window.
// id: actor ID.
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id) {
  s32 no;

  no = ObjectTable_ReadActiveValue(id);
  if (no != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}
