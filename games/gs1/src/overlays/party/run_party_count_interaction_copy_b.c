#include "party_count_interaction_record.h"

extern PartyInteractionRecord *Func_020073ca(void);
extern void Func_020073be(void);
extern s32 Func_0200739a(void);
extern void Func_0200747c(s32 cue);
extern s32 Func_0200749c(s32 actorId, s32 mode);
extern void Func_0200740e(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_0200741c(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_02007448(s32 actorId, s32 x, s32 y);
extern void Func_020073fe(s32 step);
extern void Func_02007460(s32 actorId, s32 x, s32 y);
extern void Func_0200746c(s32 actorId, s32 x, s32 y);
extern void Func_02007578(void);
extern void Func_02007584(void);
extern void Func_0200753a(s32 step);
extern void Func_020074e2(s32 cue);
extern void Func_020074fa(s32 actorId, s32 mode);
extern void Func_02007446(void);

#define RunPartyCountInteractionCopyB Func_020029ac
#define GetPartyInteractionRecord Func_020073ca
#define FinishPartyInteractionRecordRead Func_020073be
#define GetPartyMemberCount Func_0200739a
#define SetInteractionCue Func_0200747c
#define CheckActorInteraction Func_0200749c
#define PlaceSelectedActor Func_02007448
#define SetInteractionStep Func_020073fe
#define PlaceActorZero Func_02007460
#define PlaceSupportActor Func_0200746c
#define StartInteractionPhase Func_02007578
#define AdvanceInteractionPhase Func_02007584
#define SelectInteractionStep Func_0200753a
#define SetLargePartyInteractionCue Func_020074e2
#define RunLargePartyInteraction Func_020074fa
#define FinishPartyCountInteraction Func_02007446
static inline void InitializeActorZero(void)
{
    Func_0200740e(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_0200741c(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyB(s32 actorId)
{
#include "run_party_count_interaction_body.inc"
}
