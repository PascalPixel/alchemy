#include "types.h"

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
#define RunPartyCountInteractionCopyB Func_020029ac

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_020073ca(void);
void Func_020073be(void);
s32 Func_0200739a(void);
void Func_0200747c(s32 cue);
s32 Func_0200749c(s32 actorId, s32 mode);
void Func_0200740e(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_0200741c(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02007448(s32 actorId, s32 x, s32 y);
void Func_020073fe(s32 step);
void Func_02007460(s32 actorId, s32 x, s32 y);
void Func_0200746c(s32 actorId, s32 x, s32 y);
void Func_02007578(void);
void Func_02007584(void);
void Func_0200753a(s32 step);
void Func_020074e2(s32 cue);
void Func_020074fa(s32 actorId, s32 mode);
void Func_02007446(void);

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
