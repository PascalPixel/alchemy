#include "types.h"

#define GetPartyInteractionRecord Func_02005e6a
#define FinishPartyInteractionRecordRead Func_02005e5e
#define GetPartyMemberCount Func_02005e3a
#define SetInteractionCue Func_02005f04
#define CheckActorInteraction Func_02005f24
#define PlaceSelectedActor Func_02005ee0
#define SetInteractionStep Func_02005e9e
#define PlaceActorZero Func_02005ef8
#define PlaceSupportActor Func_02005f04_a
#define StartInteractionPhase Func_02005ff0
#define AdvanceInteractionPhase Func_02005ffc
#define SelectInteractionStep Func_02005fc2
#define SetLargePartyInteractionCue Func_02005f6a
#define RunLargePartyInteraction Func_02005f82
#define FinishPartyCountInteraction Func_02005ee6
#define RunPartyCountInteractionCopyA Func_02001f14

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_02005e6a(void);
void Func_02005e5e(void);
s32 Func_02005e3a(void);
void Func_02005f04();
void Func_02005f04_a();
s32 Func_02005f24(s32 actorId, s32 mode);
void Func_02005eae(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02005ebc(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_02005ee0(s32 actorId, s32 x, s32 y);
void Func_02005e9e(s32 step);
void Func_02005ef8(s32 actorId, s32 x, s32 y);
void Func_02005ff0(void);
void Func_02005ffc(void);
void Func_02005fc2(s32 step);
void Func_02005f6a(s32 cue);
void Func_02005f82(s32 actorId, s32 mode);
void Func_02005ee6(void);

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
