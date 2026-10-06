#include "TYPES.H"
#include "RESOURCE.H"

void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
