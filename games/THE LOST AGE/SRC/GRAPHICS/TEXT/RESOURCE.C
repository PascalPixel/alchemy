#include "TYPES.H"
#include "RESOURCE.H"

/* ☀️'s: hand the text resource's slot back to the resource table. */
void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
