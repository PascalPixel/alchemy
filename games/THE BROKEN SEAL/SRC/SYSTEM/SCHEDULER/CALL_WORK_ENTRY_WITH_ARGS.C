#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 Sound_CommandTable[];

/* This RAM cell holds the current two-argument entry. The arguments are
   transported unchanged; their interpretation belongs to that entry. */
void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1)
{
    void (*entry)(s32, s32);

    entry = (void (*)(s32, s32))*(u32 *)Sound_CommandTable;
    entry(arg0, arg1);
}
