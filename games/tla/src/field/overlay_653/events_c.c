#include "local.h"

void Func_0200063c(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    Func_02000668(leader, 0);
}
