#include "types.h"

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

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_0200593a(void);
void Func_0200592e(void);
s32 Func_0200590a(void);
void Func_020059cc(s32 cue);
s32 Func_020059ec(s32 actorId, s32 mode);
void Func_0200597e(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_0200598c(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_020059b0(s32 actorId, s32 x, s32 y);
void Func_0200596e(s32 step);
void Func_020059c8(s32 actorId, s32 x, s32 y);
void Func_020059d4(s32 actorId, s32 x, s32 y);
void Func_02005aa8(void);
void Func_02005ab4(void);
void Func_02005a82(s32 step);
void Func_02005a32(s32 cue);
void Func_02005a4a(s32 actorId, s32 mode);
void Func_020059b6(void);

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
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    FinishPartyInteractionRecordRead();

    if (GetPartyMemberCount() <= 1) {
        SetInteractionCue(0x20e5);
        if (CheckActorInteraction(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            PlaceSelectedActor(actorId, x, y + 0x40);
            SetInteractionStep(15);
            PlaceActorZero(0, x, y);
            PlaceSupportActor(0, x, y + 0x20);
            StartInteractionPhase();
            AdvanceInteractionPhase();
            SelectInteractionStep(11);
        }
    } else {
        SetLargePartyInteractionCue(0x20e8);
        RunLargePartyInteraction(actorId, 0);
    }

    FinishPartyCountInteraction();
}
