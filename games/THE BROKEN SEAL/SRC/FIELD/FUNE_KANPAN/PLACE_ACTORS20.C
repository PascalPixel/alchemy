#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
extern u8 FuneKanpan_LeadActionsA[];
extern u8 FuneKanpan_LeadActionsB[];
extern u8 FuneKanpan_LeadActionsC[];
extern u8 FuneKanpan_CrewScript[];
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
u8 *Object_GetById();

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);
void Event_CallWithLastActiveObjectId();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_SetPosition(21, 0x1060000, 0x2C20000);
    *(u16 *)(Object_GetById(21) + 6) = m;
    Actor_SetPosition(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Object_GetById(24) + 6) = z;
    }
    Actor_SetSpritePriority(24, 1);
    Actor_SetPosition(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Object_GetById(25) + 6) = x << 8;
    }
    Actor_SetSpritePriority(25, 1);
    Actor_SetPosition(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Object_GetById(26) + 6) = x << 8;
    }
    Actor_SetPosition(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Object_GetById(27) + 6) = m;
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(20, 0, 0);
}

void FieldScene_RunScene3af_0200185c(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0xcc0000, 0x2090000);
    record = Object_GetById(22);
    *(s32 *)(record + 12) = 0x100000;
    bits = 128;
    {
        u8 *record = ((s32 (*)())Object_GetById)(22);
        u8 value = record[89];

        record[89] = value | bits;
    }
    Actor_SetSpeed(22, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsA);
    {
        u8 *record = Object_GetById(21);

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

void FieldScene_RunScene3af_02001920(void)
{
    u8 *record;

    Event_Begin();
    Call1(Event_CallWithLastActiveObjectId, (u32)FuneKanpan_CrewScript);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Object_GetById(22);
    {
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        u8 *record = Object_GetById(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}
void FieldScene_RunScene3af_02004218(void);
