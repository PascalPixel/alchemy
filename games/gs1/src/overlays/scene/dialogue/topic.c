#include "types.h"

#define SceneData_GetTable9A08 Func_02000030
#define SceneData_GetTable9ac8 Func_0200003c
#define SceneData_SelectTable9cfcByState Func_02000044
#define FieldScene_RunActorCueBranch Func_02000074
#define FieldScene_RunPromptDialogueE19 Func_020000bc
#define SceneDialogue_RunMessage0e34 Func_02000154
#define SceneDialogue_RunMessage0e35 Func_02000178
#define FieldScene_RunIndexedStep0 Func_0200019c
#define SceneData_SelectTable9e1cByState Func_020001a8
#define SceneDialogue_PickTopicVariantId Func_02000d70
#define OverlayObject_SetField54 Func_02000e44
#define SceneState_InitFourActorRecordsAndInstallTask Func_020016a8
extern s16 Data_02000240[];
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

void Func_020019e2(s32);
s32 Func_020019f2(s32, s32);
s32 Func_020019c2(s32, s32);
void Func_020019ac(s32);
void Func_02001a02(s32);
void Func_02001a0a_cue(s32);
void Func_02001a22(s32, s32);
void Func_02001a2a(s32);
s32 Func_02001a3a(s32, s32);
s32 Func_02001a0a_prompt(s32, s32);
void Func_020019f4(s32);
void Func_02001a4a(s32);
void Func_02001a52(s32);
void Func_02001a6a(s32, s32);
void Func_02001a74(void);
void Func_02001ac2(s32);
s32 Func_02001ad4(s32, s32);
void Func_02001a90(void);
void Func_02001a98(void);
void Func_02001ae6(s32);
s32 Func_02001af8(s32, s32);
void Func_02001ab4(void);
void Func_02000a9a(s32);
s32 Func_020025f8(void);
s32 Func_02002610(void);
s32 Func_02002608(s32, s32);
u8 *Func_02002786(s32);
u8 *Func_02003036();
void Func_02002f8c();
u8 *Func_02003042();
void Func_02002f98();
void Func_02002f80();

void Func_02000e5c();           /* the installed per-frame task */

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
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Func_020019e2(cue);
    Func_020019f2(object, 0);
    if (Func_020019c2(0, 0) == 0) {
        Func_020019ac(10);
        Func_02001a02(cue + 1);
    } else {
        Func_02001a0a_cue(cue + 2);
    }
    Func_02001a22(object, 0);
}

void FieldScene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)&Value_00000e19;
    Func_02001a2a(cue);
    Func_02001a3a(object, 0);
    if (Func_02001a0a_prompt(0, 0) == 0) {
        Func_020019f4(10);
        Func_02001a4a(cue + 1);
    } else {
        Func_02001a52(cue + 2);
    }
    Func_02001a6a(object, 0);
}

void SceneDialogue_RunMessage0e34(void)
{
    Func_02001a74();
    Func_02001ac2(0xE34);
    Func_02001ad4(-1, 0);
    Func_02001a90();
}

void SceneDialogue_RunMessage0e35(void)
{
    Func_02001a98();
    Func_02001ae6(0xE35);
    Func_02001af8(-1, 0);
    Func_02001ab4();
}

void FieldScene_RunIndexedStep0(void)
{
    Func_02000a9a(0);
}

s32 SceneData_SelectTable9e1cByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
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
        topic = (s32)((unsigned int)(Func_020025f8() * 5) >> 16);
    }

    cursor = SceneWork_Bytes[308 + topic];

    /* `lsls #1 / lsrs #16` - a 0/1 coin flip from the same random source. */
    variant = Func_02002608(cursor + (s32)((unsigned int)(Func_02002610() * 2) >> 16) + 4, 3);

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
    Func_02002f8c(Func_02003036(20), 2);
    Func_02002f98(Func_02003042(21), 2);

    /* The task word names in-image code with the Thumb bit set, not a runtime
     * address; the locals keep it and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = (void (*)(void))0x02008e5d;

        Func_02002f80(task, budget);
    }
}
