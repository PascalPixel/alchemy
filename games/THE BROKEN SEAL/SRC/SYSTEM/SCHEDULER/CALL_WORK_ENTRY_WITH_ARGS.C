#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 Sound_CommandTable[];

/* The existing veneer branches through r2; its third word is the target,
   while r0/r1 carry the entry's arguments. No result is consumed. */
void _call_via_r2(s32 arg0, s32 arg1, s32 target);

/* A typed indirect entry measured the same 20-byte extent but selected r3.
   Keep the runtime's existing r2 call-slot transport, with its result unused. */
void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1)
{
    /* FAKEMATCH: the existing chained address assignment keeps both table
       loads in r2. Separate assignments load the address into r3, changing
       two instruction bytes in the same 20-byte extent. */
    s32 address;
    s32 target;

    address = (target = (s32)Sound_CommandTable);
    target = *(s32 *)address;
    _call_via_r2(arg0, arg1, target);
}
