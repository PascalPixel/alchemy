/* Pending-action service trial, 2026-10-02.
 * EN ordinary compiler: 30 instruction bytes match the current listing;
 * its trailing two-byte alignment halfword is outside the function symbol.
 * Full six-edition external-call identity and maintained linkage remain pending.
 * Draft only; no credit or source-readiness claim. */
#include "TYPES.H"

void Func_080d22a8(void);
void Func_080d3be8(s32 message);
void Func_080d407c(s32 actor, s32 flags);
void Func_080d2350(void);

void Func_080cdea8(s32 actor, s32 message)
{
    Func_080d22a8();
    Func_080d3be8(message);
    Func_080d407c(actor, 0);
    Func_080d2350();
}
