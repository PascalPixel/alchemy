#include "party_count_interaction_record.h"

extern PartyInteractionRecord *Func_02005e6a(void);
extern void Func_02005e5e(void);
extern s32 Func_02005e3a(void);
extern void Func_02005f04();
extern s32 Func_02005f24(s32 actorId, s32 mode);
extern void Func_02005eae(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_02005ebc(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_02005ee0(s32 actorId, s32 x, s32 y);
extern void Func_02005e9e(s32 step);
extern void Func_02005ef8(s32 actorId, s32 x, s32 y);
extern void Func_02005ff0(void);
extern void Func_02005ffc(void);
extern void Func_02005fc2(s32 step);
extern void Func_02005f6a(s32 cue);
extern void Func_02005f82(s32 actorId, s32 mode);
extern void Func_02005ee6(void);

#define GetPartyInteractionRecord Func_02005e6a
#define FinishPartyInteractionRecordRead Func_02005e5e
#define GetPartyMemberCount Func_02005e3a
#define SetInteractionCue Func_02005f04
#define CheckActorInteraction Func_02005f24
#define PlaceSelectedActor Func_02005ee0
#define SetInteractionStep Func_02005e9e
#define PlaceActorZero Func_02005ef8
#define PlaceSupportActor Func_02005f04
#define StartInteractionPhase Func_02005ff0
#define AdvanceInteractionPhase Func_02005ffc
#define SelectInteractionStep Func_02005fc2
#define SetLargePartyInteractionCue Func_02005f6a
#define RunLargePartyInteraction Func_02005f82
#define FinishPartyCountInteraction Func_02005ee6
static inline void InitializeActorZero(void)
{
    Func_02005eae(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_02005ebc(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyA(s32 actorId)
{
#include "run_party_count_interaction_body.inc"
}
