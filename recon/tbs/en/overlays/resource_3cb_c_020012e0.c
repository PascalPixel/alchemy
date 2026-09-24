/* NONMATCHING: 1064 of 1060 bytes, 289 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: structure written in full; the four name halfwords (link symbols 0x54 0x41 0x4c 0x4b) are short-range pool constants in the reference, which dumps a pool inside the flag loop; the candidate loads them as words with the pool at the end, and the game-state base and wins/message counters sit in different registers (289 edits). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

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

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000054;
extern u8 Data_00000041;
extern u8 Data_0000004c;
extern u8 Data_0000004b;

#define ROW(n) (*(u16 *)Data_02000240_t.halves[n])

s32 Local_020012e0(void)
{
    s32 i;
    s32 wins;
    s32 msg;
    s32 v;
    u16 *row;
    u16 *name;
    u8 *ew;
    void (*callback)(void);

    *(s32 *)0x02009f50 = 0;
    *(s32 *)0x02009f4c = 0;
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
    name = (u16 *)0x02002224;
    name[4] = (u32)&Data_00000054;
    name[5] = (u32)&Data_00000041;
    name[6] = (u32)&Data_0000004c;
    name[7] = (u32)&Data_0000004b;
    for (i = 0; i < 8; i++) {
        Engine_GameFlagClear(0x2f0 + i);
        if (Local_02000f30(i)) {
            Engine_GameFlagSet(0x2f0 + i);
        }
    }
    if (Data_02000240_t.halves[225][0] == 8) {
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(5);
        ROW(338)++;
        ROW(341)++;
        wins = (s8)Main_080770e0(0x3f8);
        msg = wins * 2 + 2;
        if (msg > 14) {
            msg = 14;
        }
        v = Main_080770e0(1000);
        if (v == 2) {
            Main_080770e8(1000, 0);
            wins++;
            msg++;
        } else {
            Main_080770e8(1000, v + 1);
        }
        Engine_ActorFaceActor(8, Data_02000240_t.words[125], 0);
        Engine_EventSetMessage(0x293e + msg);
        Engine_EventOpenMessage(8, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
            if (wins > 90) {
                wins = 90;
            }
            Main_080770e8(0x3f8, wins);
        } else {
            Engine_GameFlagClear(0x173);
            Main_080770e8(0x3f8, -1);
            row = &ROW(341);
            Main_08015120(*row, 5);
            if (ROW(340) < *row) {
                ROW(340) = *row;
                Engine_EventSetMessage(0x293c);
                Engine_EventOpenMessage(8, 0);
                Scene_ShowDialoguePair292c();
            } else {
                Engine_EventSetMessage(0x2939);
                Engine_EventOpenMessage(8, 0);
            }
            Local_02000128(0);
        }
        Engine_EventEnd();
    } else if (Data_02000240_t.halves[225][0] == 9) {
        ROW(339)++;
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(5);
        Engine_ActorFaceActor(8, Data_02000240_t.words[125], 0);
        row = &ROW(341);
        Main_08015120(*row, 5);
        if (ROW(340) < *row) {
            ROW(340) = *row;
            Engine_EventSetMessage(0x293c);
            Engine_EventOpenMessage(8, 0);
            Scene_ShowDialoguePair292c();
        } else {
            Engine_EventSetMessage(0x293a);
            Engine_EventOpenMessage(8, 0);
        }
        ROW(341) = 0;
        Engine_GameFlagClear(0x173);
        Main_080770e8(0x3f8, -1);
        Local_02000128(0);
        Engine_EventEnd();
    } else if (Data_02000240_t.halves[225][0] == 10) {
        Engine_EventBegin();
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Local_02000128(0);
        Local_02000128(4);
        if (Engine_GameFlagIsSet(1000)) {
            ew = (u8 *)gEventWork;
            Engine_GameFlagClear(1000);
            *(u16 *)(ew + 0x182) = 2;
            Engine_GameFlagClear(0x304);
            Engine_TaskWait(20);
            State_RunQueryWithInterruptMasterSaved();
            Local_02000128(0);
            Local_02000128(4);
        } else {
            ROW(342)++;
            v = ++ROW(345);
            if (ROW(344) < (u16)v) {
                ROW(344) = v;
            }
            Scene_DrawThreeDigitValue(ROW(344));
            Scene_ShowDialoguePair292a();
            Engine_GameFlagSet(0x304);
            Engine_GameFlagSet(0x305);
        }
        Engine_EventEnd();
    } else if (Data_02000240_t.halves[225][0] == 11) {
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
        Main_080770e8(0x3f8, -1);
        if (Data_02000240_t.bytes[277][0] != 0) {
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_ActorFaceActor(8, Data_02000240_t.words[125], 0);
            Engine_EventSetMessage(0x2929);
            Engine_EventShowMessage(8, 0);
            Engine_EventEnd();
        }
        Data_02000240_t.bytes[277][0] = 0;
        *(u8 *)0x03001d08 = 0;
        Local_02000128(0);
        Local_02000128(4);
    }
    callback = (void (*)(void))0x2008149;
    Engine_TaskAddCallback(callback, 0xc80);
    Main_080003b8(callback, 1);
    if (Data_02000240_t.halves[225][0] != 8 || !Engine_GameFlagIsSet(0x173)) {
        Main_08077260(1);
        Main_080b5108();
    }
    return 0;
}
