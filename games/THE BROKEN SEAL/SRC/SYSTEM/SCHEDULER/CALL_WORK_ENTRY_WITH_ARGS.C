#include "PROTO.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 Sound_CommandTable[];


void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1)
{
    /* FAKEMATCH: the chained assignment keeps both table loads in r2; separate assignments select r3 for the address. */
    s32 address;
    s32 target;

    address = (target = (s32)Sound_CommandTable);
    target = *(s32 *)address;
    _call_via_r2(arg0, arg1, target);
}
