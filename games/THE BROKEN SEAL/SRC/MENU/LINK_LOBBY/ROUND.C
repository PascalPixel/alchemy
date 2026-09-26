/* Apply the link round's result, update records, and reopen lobby dialogue.
 * Reconstructed from this owner's complete own-ROM listing and registered
 * draft; exact 1060-byte extent, including literal pools (2026-09-26). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
s32 Main_0808a070(s32, s32);

void Main_08000378(s32 value);
void Scene_DrawThreeDigitValue(s32 value);
void Local_02000128(s32 mode);
void Main_08009190(s32 value);
s32 Local_02000f30(s32 index);
s32 Main_080770e0(s32 counter);
void Main_080770e8(s32 counter, s32 value);
void Main_08015120(s32 value, s32 digits);
void Scene_ShowDialoguePair292c(void);
void Scene_ShowDialoguePair292a(void);
void State_RunQueryWithInterruptMasterSaved(void);
void Main_08000300(void);
void Main_080003b8(void (*callback)(void), s32 value);
void Main_08077260(s32 value);
void Main_080b5108(void);

extern u16 Data_02000240_t[][1];
extern u16 Data_02002224[];
extern u8 Data_00000054;
extern u8 Data_00000041;
extern u8 Data_0000004c;
extern u8 Data_0000004b;
extern s32 Data_02009f50;
extern s32 Data_02009f4c;
extern u8 Value_0000293e;

union GameStateRows {
    u8 bytes[512][2];
    u16 halves[512][1];
    s32 words[256];
};

#define ROW(n) Data_02000240_t[n][0]

static __inline__ s32 Value1(s32 (*fn)(), s32 value)
{
    return fn(value);
}

static __inline__ void Call1(void (*fn)(), s32 value)
{
    fn(value);
}

static __inline__ void Call2(void (*fn)(), s32 left, s32 right)
{
    fn(left, right);
}

static __inline__ void Name_StoreLetter(s32 letter, u16 *dst, s32 index)
{
    dst[index] = letter;
}

s32 LinkLobby_RunRoundResult(void)
{
    s32 i;
    u32 score;
    struct EventWork *ew;
    void (*callback)(void);

    Data_02009f50 = 0;
    Data_02009f4c = 0;
    gEventWork->start_transition = 0x201;
    Main_08000378(2);
    Scene_DrawThreeDigitValue(ROW(344));
    {
        s32 a = 13, b = 10;
        ((void (*)())Engine_MapCopyCellAttributes)(11, 11, 1, 1, a, b);
    }
    Local_02000128(4);
    Engine_TaskWait(1);
    Main_08009190(5);
    /* FAKEMATCH: word-sized locals retain halfword-mode link constants,
     * giving the short literal-pool reach used by the original name stores. */
    Name_StoreLetter((u16)(s32)&Data_00000054, Data_02002224, 4);
    Name_StoreLetter((u16)(s32)&Data_00000041, Data_02002224, 5);
    Name_StoreLetter((u16)(s32)&Data_0000004c, Data_02002224, 6);
    Name_StoreLetter((u16)(s32)&Data_0000004b, Data_02002224, 7);
    for (i = 0; i < 8; i++) {
        Engine_GameFlagClear(0x2f0 + i);
        if (Local_02000f30(i)) {
            Engine_GameFlagSet(0x2f0 + i);
        }
    }
    if ((s16)ROW(225) == 8) {
        s32 wins;
        s32 msg;
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(5);
        {
            union GameStateRows *state = (union GameStateRows *)Data_02000240_t;

            state->halves[338][0]++;
            state->halves[341][0]++;
        }
        wins = (s8)Value1(Main_080770e0, 0x3f8);
        msg = wins * 2 + 2;
        if (msg > 14) {
            msg = 14;
        }
        v = Value1(Main_080770e0, 1000);
        if (v == 2) {
            Call2(Main_080770e8, 1000, 0);
            wins++;
            msg++;
        } else {
            Call2(Main_080770e8, 1000, v + 1);
        }
        {
        union GameStateRows *state = (union GameStateRows *)Data_02000240_t;

        Engine_ActorFaceActor(8, state->words[125], 0);
        Call1(Engine_EventSetMessage, (s32)&Value_0000293e + msg);
        Engine_EventOpenMessage(8, 0);
        if (Main_0808a070(0, 0) == 0) {
            if (wins > 90) {
                wins = 90;
            }
            Call2(Main_080770e8, 0x3f8, wins);
        } else {

            Engine_GameFlagClear(0x173);
            Call2(Main_080770e8, 0x3f8, -1);
            /* FAKEMATCH: one scalar holds the row address, then its score. */
            score = (u32)state->halves[341];
            Call2(Main_08015120, *(u16 *)score, 5);
            score = *(u16 *)score;
            if (state->halves[340][0] < score) {
                state->halves[340][0] = score;
                Engine_EventSetMessage(0x293c);
                Engine_EventOpenMessage(8, 0);
                Scene_ShowDialoguePair292c();
            } else {
                Engine_EventSetMessage(0x2939);
                Engine_EventOpenMessage(8, 0);
            }
            Local_02000128(0);
        }
        }
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 9) {

        ROW(339)++;
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(5);
        Engine_ActorFaceActor(8, *(s32 *)Data_02000240_t[250], 0);
        /* FAKEMATCH: one scalar holds the row address, then its score. */
        score = (u32)&ROW(341);
        Call2(Main_08015120, *(u16 *)score, 5);
        score = *(u16 *)score;
        if (ROW(340) < score) {
            ROW(340) = score;
            Engine_EventSetMessage(0x293c);
            Engine_EventOpenMessage(8, 0);
            Scene_ShowDialoguePair292c();
        } else {
            Engine_EventSetMessage(0x293a);
            Engine_EventOpenMessage(8, 0);
        }
        ROW(341) = 0;
        Engine_GameFlagClear(0x173);
        Call2(Main_080770e8, 0x3f8, -1);
        Local_02000128(0);
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 10) {
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(0);
        Local_02000128(4);
        if (Value1(Engine_GameFlagIsSet, 1000)) {
            ew = gEventWork;
            Call1(Engine_GameFlagClear, 1000);
            ew->raised_trigger = 2;
            Engine_GameFlagClear(0x304);
            Engine_TaskWait(20);
            State_RunQueryWithInterruptMasterSaved();
            Local_02000128(0);
            Local_02000128(4);
        } else {
            ROW(342)++;
            v = ROW(345) + 1;
            ROW(345) = v;
            /* FAKEMATCH: reuse the other score branches' address slot. */
            score = (u32)&ROW(344);
            if (*(u16 *)score < (u16)v) {
                *(u16 *)score = v;
            }
            Scene_DrawThreeDigitValue(*(u16 *)score);
            Scene_ShowDialoguePair292a();
            Engine_GameFlagSet(0x304);
            Engine_GameFlagSet(0x305);
        }
        Engine_EventEnd();
    } else if ((s16)ROW(225) == 11) {
        s32 v;

        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(0);
        Local_02000128(4);
        if ((v = Engine_GameFlagIsSet(0x173)) == 0) {
            ROW(343)++;
            ROW(345) = v;
            Scene_ShowDialoguePair292a();
        }
        Engine_GameFlagSet(0x304);
        Engine_GameFlagClear(0x305);
        Engine_EventEnd();
    } else {
        Main_08000300();
        Engine_GameFlagClear(0x172);
        Call2(Main_080770e8, 0x3f8, -1);
        if (*(u8 *)Data_02000240_t[277] != 0) {
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_ActorFaceActor(8, *(s32 *)Data_02000240_t[250], 0);
            Engine_EventSetMessage(0x2929);
            Engine_EventShowMessage(8, 0);
            Engine_EventEnd();
        }
        *(u8 *)Data_02000240_t[277] = 0;
        *(u8 *)0x03001d08 = 0;
        Local_02000128(0);
        Local_02000128(4);
    }
    callback = (void (*)(void))0x2008149;
    Engine_TaskAddCallback(callback, 0xc80);
    Main_080003b8(callback, 1);
    if ((s16)ROW(225) != 8 || !Engine_GameFlagIsSet(0x173)) {
        Main_08077260(1);
        Main_080b5108();
    }
    return 0;
}
