#include "TYPES.H"

s32 UiWork_FinalizeEntityMatchingLocalizedIdFar();
s32 ObjectTable_ReadActiveValue();

void ObjectTable_RunIfActive(void)
{
    if (ObjectTable_ReadActiveValue() != -1) {
        UiWork_FinalizeEntityMatchingLocalizedIdFar();
    }
}
