#include "party_count_interaction_record.h"

extern PartyInteractionRecord *Func_0200593a(void);
extern void Func_0200592e(void);
extern s32 Func_0200590a(void);
extern void Func_020059cc(s32 cue);
extern s32 Func_020059ec(s32 actorId, s32 mode);
extern void Func_0200597e(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_0200598c(s32 actorId, s32 horizontalRate, s32 verticalRate);
extern void Func_020059b0(s32 actorId, s32 x, s32 y);
extern void Func_0200596e(s32 step);
extern void Func_020059c8(s32 actorId, s32 x, s32 y);
extern void Func_020059d4(s32 actorId, s32 x, s32 y);
extern void Func_02005aa8(void);
extern void Func_02005ab4(void);
extern void Func_02005a82(s32 step);
extern void Func_02005a32(s32 cue);
extern void Func_02005a4a(s32 actorId, s32 mode);
extern void Func_020059b6(void);

#define GetPartyInteractionRecord Func_0200593a
#define FinishPartyInteractionRecordRead Func_0200592e
#define GetPartyMemberCount Func_0200590a
#define SetInteractionCue Func_020059cc
#define CheckActorInteraction Func_020059ec
#define PlaceSelectedActor Func_020059b0
#define SetInteractionStep Func_0200596e
#define PlaceActorZero Func_020059c8
#define PlaceSupportActor Func_020059d4
#define StartInteractionPhase Func_02005aa8
#define AdvanceInteractionPhase Func_02005ab4
#define SelectInteractionStep Func_02005a82
#define SetLargePartyInteractionCue Func_02005a32
#define RunLargePartyInteraction Func_02005a4a
#define FinishPartyCountInteraction Func_020059b6
#define RunPartyCountInteraction Func_02001c7c

static inline void InitializeActorZero(void)
{
    Func_0200597e(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_0200598c(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteraction(s32 actorId)
{
#include "run_party_count_interaction_body.inc"
}
