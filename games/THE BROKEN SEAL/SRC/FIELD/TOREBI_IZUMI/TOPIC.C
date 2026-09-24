#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Signed topic cursors share the scene workspace with its halfword state. */
extern s8 SceneWork_Bytes[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];
extern u8 Value_00000e39;
extern u8 Value_00000e19;
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];
extern s32 Data_0200a018[];     /* in-image, file offset 0x2018: 5 topics x 3 ids */
extern u8 Data_0200a05a[];      /* in-image 0x205a: four X tile coordinates */
extern u8 Data_0200a05e[];      /* in-image 0x205e: four Z tile coordinates */
extern u16 Data_0200a062[];     /* in-image 0x2062: four headings */
extern u8 Data_0200a070[];      /* scratch EWRAM above the image */
extern u8 Data_0200a0d0[];      /* scratch EWRAM above the image: 4 x 24 bytes */

void Func_02000a9a(s32);
s32 Func_02002608(s32, s32);
u8 *Func_02002786(s32);
u8 *Func_02003036();
u8 *Func_02003042();
void Func_02002f80();

u8 *SceneData_GetTable9A08(void)
{
    return (u8 *)0x02009a08;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable9ac8(void)
{
    return (u8 *)0x02009ac8;
}

s32 SceneData_SelectTable9cfcByState(void)
{
    if (gGameState.scene == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Event_SetMessage(cue);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(object, 0);
}

void FieldScene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)&Value_00000e19;
    Event_SetMessage(cue);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(object, 0);
}

void SceneDialogue_RunMessage0e34(void)
{
    Event_Begin();
    Event_SetMessage(0xE34);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void SceneDialogue_RunMessage0e35(void)
{
    Event_Begin();
    Event_SetMessage(0xE35);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void FieldScene_RunIndexedStep0(void)
{
    Func_02000a9a(0);
}

s32 SceneData_SelectTable9e1cByState(void)
{
    if (gGameState.scene == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}

s32 SceneDialogue_PickTopicVariantId(s32 topic)
{
    s32 cursor;
    s32 variant;

    if (topic < 0) {
        return 0;
    }

    /* Topic 5 means "any": reduce a 16-bit random to 0..4. */
    if (topic == 5) {
        topic = (s32)((unsigned int)(Random_Next() * 5) >> 16);
    }

    cursor = SceneWork_Bytes[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Func_02002608(cursor + (s32)((unsigned int)(Random_Next() * 2) >> 16) + 4, 3);

    SceneWork_Bytes[308 + topic] = (s8)variant;

    return Data_0200a018[topic * 3 + variant];
}

void OverlayObject_SetField54(s32 arg0, s32 arg1)
{
    u8 *entry = Func_02002786(arg0);

    if (entry != 0) {
        u8 *field = entry + 0x54;

        *field = arg1;
    }
}

/*
 * The 148-byte owner includes its eight-word literal pool: those words lie
 * past the return and are read only by the pc-relative loads.
 * Field names are descriptive only: the 24-byte record stride and the cleared
 * halfwords at +14..+20 are read off the stores alone, and the second heading
 * is 0x0001 rather than a multiple of 0x4000 -- the byte is certain, its
 * meaning is not.
 */
void SceneState_InitFourActorRecordsAndInstallTask(void)
{
    u8 *work = Data_0200a070;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    xtbl = Data_0200a05a;
    rec = Data_0200a0d0;
    htbl = Data_0200a062;
    ztbl = Data_0200a05e;

    do {
        *(s32 *)(rec + 0) = (s32)*xtbl << 16;
        *(s32 *)(rec + 8) = (s32)*ztbl << 16;
        *(s32 *)(rec + 4) = 0;
        *(u16 *)(rec + 12) = *htbl;
        *(u16 *)(rec + 14) = 0;
        *(u16 *)(rec + 16) = 0;
        *(u16 *)(rec + 18) = 0;
        *(u16 *)(rec + 20) = 0;

        i++;
        xtbl++;
        ztbl++;
        htbl++;
        rec += 24;
    } while (i != 4);

    *(s32 *)(work + 4) = (s32)0xffe20000;      /* -30.0 in 16.16 */
    *(s32 *)(work + 8) = 0;
    *(s32 *)(work + 12) = 0x640000;            /* 200 << 15, i.e. 100.0 */
    *(s32 *)(work + 64) = 0;
    *(s32 *)(work + 68) = 0;
    *(s32 *)(work + 72) = 0;
    *(s32 *)(work + 76) = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Object_SetAnimation(Func_02003036(20), 2);
    Object_SetAnimation(Func_02003042(21), 2);

    /* The task word names in-image code with the Thumb bit set, not a runtime
     * address; the locals keep it and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = (void (*)(void))0x02008e5d;

        Func_02002f80(task, budget);
    }
}
