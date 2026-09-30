/*
 * Draft: UiText_DrawWorkValueWithLabel does not yet match; 2 halfwords differ from ☀️'s C, first at +0x2a (data).
 * Links as recon/tla/raw/080f9224.s.
 */
#include "TYPES.H"

struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern struct SharedWork080a23c0 gGameState;
extern u8 MsgCoinsLabel[];

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gGameState.resource, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
}
