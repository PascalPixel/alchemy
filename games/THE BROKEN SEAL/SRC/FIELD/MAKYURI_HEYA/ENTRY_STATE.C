#include "TYPES.H"
#include "FIELD_EVENT.H"

void Makyuri_SpawnLightObjects(s32 count, s32 base);
void Makyuri_ClearPalette(void);
void Main_0808a408(s32 value);
void Main_0808a5e0(s32 value);
void Main_080091a0(void);
void Main_08009198(void);
void Main_08015210(s32 message, s32 a1, s32 a2);
void SetEffectRecordMode();
void FieldScene_RedrawActorFootprint(s32 actor);
void FieldScene_RunSupplementalSequenceOne(s32 mode);
void FieldScene_RunScene39b_02001208(void);
void SceneEffect_SpawnParticleRowsByMode(s32 mode);
void FieldScene_RunPrimarySequence(s32 mode);
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_ColorBufferInterpolate(s32 frames);
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call5(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    f(a0, a1, a2, a3, a4);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Local_02004c68(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_00000036[];
extern u8 Data_00000037[];
extern u8 Data_00000038[];
extern u8 Data_00000039[];

#define TASK_D59 ((void (*)(void))0x2008d59)
#define TASK_2C5 ((void (*)(void))0x200a2c5)
#define TASK_DCD ((void (*)(void))0x200adcd)
#define TASK_649 ((void (*)(void))0x200a649)

/* Mercury Lighthouse rooms: set the blend and lights, then per area and entrance restore the pushed pillars, lifts and cells from the story flags. */
s32 Scene_RunSelectorEntry(void)
{
    struct FieldActor *actor;
    u32 i;
    s32 flag;
    s32 x;
    s32 set;

    Engine_GameFlagSet(0x111);
    Data_02000240_t.halves[289][0] = 11;
    Data_02000240_t.halves[288][0] = (s32)Data_00000039;
    gEventWork->start_transition = 0x204;
    /* FAKEMATCH: the do/while and the held value keep the blend constant in a register ahead of its address load. */
    do {
        s32 blend = 0x3f40;
        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    {
        s32 alpha = 0x1010;
        *(volatile u16 *)0x04000052 = alpha;
    }
    Makyuri_SpawnLightObjects(21, 0x2001000);
    Main_0808a408(0);
    if (Engine_GameFlagIsSet(0x875))
        Call2((void (*)())Engine_TaskAddCallback, TASK_D59, 0xc80);
    else
        Makyuri_ClearPalette();
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000036) {
        switch (Data_02000240_t.halves[225][0]) {
        case 1:
        case 2:
        case 3:
            if (Engine_GameFlagIsSet(0x875))
                Call6((void (*)())Engine_MapCopyCellAttributes, 66, 5, 27, 23, 2, 5);
            break;
        case 5:
        case 6:
            Main_0808a5e0(170);
            break;
        case 15:
            SetEffectRecordMode(Engine_ActorGet(0), 1);
        case 4:
            if (Engine_GameFlagIsSet(0x876)) {
                Engine_TaskWait(1);
                Call3((void (*)())Engine_ActorSetPosition, 9, 168 << 18, 128 << 16);
                Call3((void (*)())Engine_ActorSetPosition, 10, 176 << 18, 192 << 16);
                Call3((void (*)())Engine_ActorSetPosition, 11, 162 << 18, 240 << 16);
            } else if (!Engine_GameFlagIsSet(0x109)) {
                Engine_GameFlagClear(0x310);
                Engine_GameFlagClear(0x311);
                Engine_GameFlagClear(0x312);
                Engine_GameFlagClear(0x313);
            }
            SetEffectRecordMode(Engine_ActorGet(9), 1);
            SetEffectRecordMode(Engine_ActorGet(10), 1);
            SetEffectRecordMode(Engine_ActorGet(11), 1);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            SetEffectRecordMode(Engine_ActorGet(12), 1);
            if (Engine_GameFlagIsSet(0x310)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 119, 9, 109, 11, 1, 1);
                if (Engine_GameFlagIsSet(0x311))
                    Call6((void (*)())Engine_MapCopyCellsTo, 118, 9, 104, 13, 1, 1);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            }
            break;
        case 9:
        case 10:
            if (!Engine_GameFlagIsSet(0x873)) {
                Call3((void (*)())Engine_ActorSetPosition, 3, 174 << 18, 158 << 18);
                Call3((void (*)())Engine_ActorFaceDirection, 3, 0, 0);
            } else {
                Call3((void (*)())Engine_ActorSetPosition, 8, 194 << 18, 158 << 18);
                Call6((void (*)())Engine_MapCopyCellAttributes, 110, 39, 5, 1, 46, 39);
            }
            break;
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000037) {
        switch (Data_02000240_t.halves[225][0]) {
        case 1:
        case 2:
            SetEffectRecordMode(Engine_ActorGet(8), 1);
            SetEffectRecordMode(Engine_ActorGet(9), 1);
            if (Engine_GameFlagIsSet(0x302)) {
                Engine_TaskWait(1);
                Engine_AudioPlayCue(211);
                Call3((void (*)())Engine_ActorSetPosition, 8, 184 << 16, 132 << 18);
                Call6((void (*)())Engine_MapCopyCellAttributes, 11, 31, 1, 4, 9, 31);
                Call6((void (*)())Engine_MapCopyCellAttributes, 7, 30, 1, 4, 11, 31);
                Call6((void (*)())Engine_MapCopyCellsTo, 74, 58, 70, 32, 1, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 74, 59, 70, 34, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 76, 60, 74, 38, 3, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 77, 60, 76, 38, 2, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 75, 58, 86, 41, 1, 3);
                Call6((void (*)())Engine_MapCopyCellsTo, 75, 59, 86, 43, 1, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 76, 59, 80, 49, 2, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 77, 59, 82, 49, 2, 1);
            }
            break;
        case 3:
        case 4:
            Engine_MapRedraw();
            Engine_TaskWait(1);
            if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 5, 2, 5, 11, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 9, 1, 9, 7, 1, 2);
            }
            if (Engine_GameFlagIsSet(0x874)) {
                Call3((void (*)())Engine_ActorSetPosition, 11, 176 << 15, 216 << 16);
                Engine_ActorGet(11)->y.fixed += -0x20000;
                Engine_ActorGet(11)->target_y = Engine_ActorGet(11)->y.fixed;
                Call6((void (*)())Engine_MapCopyCellsTo, 9, 1, 9, 7, 1, 2);
                Call6((void (*)())Engine_MapCopyCellsTo, 5, 2, 5, 11, 1, 1);
                Call6((void (*)())Engine_MapCopyCellAttributes, 9, 5, 1, 1, 9, 10);
            }
            break;
        case 7:
        case 8:
        case 9:
            SetEffectRecordMode(Engine_ActorGet(10), 1);
            if (Engine_GameFlagIsSet(0x306)) {
                FieldScene_RunSupplementalSequenceOne(0);
                Call6((void (*)())Engine_MapCopyCellAttributes, 42, 41, 4, 1, 42, 39);
                Call6((void (*)())Engine_MapCopyCellAttributes, 42, 40, 4, 1, 42, 41);
                Call3((void (*)())Engine_ActorSetPosition, 10, 176 << 18, 160 << 18);
            }
            break;
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000038) {
        switch (Data_02000240_t.halves[225][0]) {
        case 4:
        case 5:
        case 6:
            SetEffectRecordMode(Engine_ActorGet(15), 1);
            SetEffectRecordMode(Engine_ActorGet(16), 1);
            SetEffectRecordMode(Engine_ActorGet(17), 1);
            SetEffectRecordMode(Engine_ActorGet(18), 1);
            SetEffectRecordMode(Engine_ActorGet(19), 1);
            SetEffectRecordMode(Engine_ActorGet(0), 1);
            i = 0;
            x = 158 << 18;
            flag = 0x330;
            for (; i <= 3; i++) {
                if (Engine_GameFlagIsSet(flag))
                    Call3((void (*)())Engine_ActorSetPosition, i + 15, x, 176 << 15);
                else if (Engine_GameFlagIsSet(flag + 1))
                    Call3((void (*)())Engine_ActorSetPosition, i + 15, x + (128 << 14), 176 << 15);
                x += 128 << 15;
                flag += 2;
            }
            if (Engine_GameFlagIsSet(0x338)) {
                Call3((void (*)())Engine_ActorSetPosition, 19, 230 << 18, 176 << 15);
                Call6((void (*)())Engine_MapCopyCellAttributes, 53, 10, 1, 1, 58, 7);
            } else if (Engine_GameFlagIsSet(0x339)) {
                Call3((void (*)())Engine_ActorSetPosition, 19, 238 << 18, 176 << 15);
                Call6((void (*)())Engine_MapCopyCellAttributes, 53, 10, 1, 1, 58, 7);
            }
            break;
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x348)) {
                Call3((void (*)())Engine_ActorSetPosition, 20, 228 << 17, 164 << 17);
                Call6((void (*)())Engine_MapCopyCellAttributes, 29, 20, 1, 1, 31, 20);
            }
            break;
        case 3:
        case 13:
            FieldScene_RunScene39b_02001208();
        case 1:
        case 2:
        case 12:
            Main_0808a5e0(170);
            break;
        case 7:
        case 8:
        case 9:
            if (Engine_GameFlagIsSet(0x109) && Engine_GameFlagIsSet(0x256)) {
                Engine_ActorGet(0)->y.fixed = -0x20000;
                Engine_ActorGet(0)->target_y = Engine_ActorGet(0)->y.fixed;
                Call6((void (*)())Engine_MapCopyCellsTo, 6, 29, 10, 23, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 10, 28, 10, 18, 1, 2);
            }
            if (Engine_GameFlagIsSet(0x878)) {
                Call3((void (*)())Engine_ActorSetPosition, 8, 168 << 16, 188 << 17);
                Engine_ActorGet(8)->y.fixed += -0x20000;
                Engine_ActorGet(8)->target_y = Engine_ActorGet(8)->y.fixed;
                Call6((void (*)())Engine_MapCopyCellsTo, 6, 29, 10, 23, 1, 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 10, 28, 10, 18, 1, 2);
                Call6((void (*)())Engine_MapCopyCellAttributes, 10, 16, 1, 1, 10, 19);
                Engine_MapRedraw();
            }
            break;
        case 16:
            Engine_TaskWait(1);
            Call3((void (*)())Engine_ActorSetPosition, 10, 204 << 18, 152 << 18);
            Call3((void (*)())Engine_ActorSetPosition, 11, 194 << 18, 144 << 18);
            SetEffectRecordMode(Engine_ActorGet(0), 1);
            SceneEffect_SpawnParticleRowsByMode(0);
            FieldScene_RunPrimarySequence(1);
        case 14:
            if (Data_02000240_t.halves[225][0] == 14)
                Engine_AudioPlayCue(211);
            SetEffectRecordMode(Engine_ActorGet(9), 1);
            Call2((void (*)())Engine_ActorSetSpritePriority, 10, 2);
            Engine_ActorGet(10)->unknown_22 = 2;
            SetEffectRecordMode(Engine_ActorGet(11), 1);
            SetEffectRecordMode(Engine_ActorGet(12), 1);
            SetEffectRecordMode(Engine_ActorGet(13), 1);
            SetEffectRecordMode(Engine_ActorGet(14), 1);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            if (Data_02000240_t.halves[225][0] != 14)
                break;
            if (!Engine_GameFlagIsSet(0x109)) {
                Engine_GameFlagClear(0x318);
                Engine_GameFlagClear(0x319);
                Engine_GameFlagClear(0x31a);
                Engine_GameFlagClear(0x31b);
                break;
            }
            SceneEffect_SpawnParticleRowsByMode(0);
            if (Engine_GameFlagIsSet(0x319)) {
                FieldScene_RunPrimarySequence(2);
                if (Engine_ActorGet(9)->z.fixed >> 20 == 44)
                    Call2((void (*)())Engine_TaskAddCallback, TASK_2C5, 0xc80);
            } else if (Engine_GameFlagIsSet(0x31a)) {
                FieldScene_RunPrimarySequence(1);
            } else if (!Engine_GameFlagIsSet(0x31b)) {
                FieldScene_RunPrimarySequence(0);
            }
            break;
        case 15:
            Engine_EventBegin();
            SetEffectRecordMode(Engine_ActorGet(8), 1);
            Call2((void (*)())Engine_ActorSetChildValue, 0, 15);
            Call2((void (*)())Engine_ActorSetSpriteFlags, Engine_ActorGet(0), 0);
            Engine_ActorGet(8)->y.fixed = 144 << 16;
            Engine_ActorGet(8)->motion_flags = 0;
            *(s32 *)&Engine_ActorGet(8)->unknown_44[0] = 0;
            *(s32 *)&Engine_ActorGet(8)->unknown_44[4] = 0x4ccc;
            gEventWork->start_transition = 0x100;
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_ActorGet(8)->motion_flags = 3;
            Engine_AudioPlayCue(189);
            Engine_EventWait(32);
            Engine_AudioPlayCue(188);
            SetEffectRecordMode(Engine_ActorGet(8), 2);
            Call3((void (*)())Engine_WorkSetValuesIfNonNegative, 192 << 10, 192 << 10, 128 << 9);
            Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            Engine_EventWait(60);
            Engine_EventRequestExit(16);
            Engine_EventEnd();
            break;
        }
    }
    if (Data_02000240_t.halves[224][0] == (s32)Data_00000039) {
        switch (Data_02000240_t.halves[225][0]) {
        case 10:
            Call2((void (*)())Engine_TaskAddCallback, TASK_DCD, 0xc80);
            if (Engine_GameFlagIsSet(0x109))
                break;
            FieldScene_RunScene39b_02001208();
            Main_0808a5e0(170);
            Call2((void (*)())Engine_ColorBufferApplySource, 128 << 9, 0);
            Call2((void (*)())Engine_ColorBufferApplyTarget, 0x10003, 1);
            Engine_ColorBufferInterpolate(30);
            Engine_EventWaitForScreen();
            Call2((void (*)())Engine_ActorSetAnimation, 0, 1);
            Engine_EventWait(30);
            Call3((void (*)())Main_08015210, 0x1633, 0, 0);
            Call2((void (*)())Engine_ColorBufferApplyTarget, 128 << 9, 0);
            Engine_ColorBufferInterpolate(30);
            break;
        case 15:
            Call3((void (*)())Engine_ActorSetPosition, 3, 0, 0);
            Main_080091a0();
            /* FAKEMATCH: the do/while keeps this zero from being shared, as a pool halfword, with the later zero stores. */
            do { s32 z = 0; *(volatile u16 *)0x04000050 = z; } while (0);
            Engine_ActorGet(9)->scale_x = -0x10000;
            Call2((void (*)())Engine_ActorSetSpritePriority, 14, 1);
            Call2((void (*)())Engine_ActorSetSpritePriority, 15, 1);
            Call2((void (*)())Engine_ActorSetSpritePriority, 16, 1);
            set = Engine_GameFlagIsSet(0x109);
            if (set) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 104, 34, 5, 4, 40, 34);
                Call6((void (*)())Engine_MapCopyCellsTo, 45, 91, 40, 91, 5, 4);
                break;
            }
            Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
            Call6((void (*)())Engine_MapCopyCellAttributes, 116, 36, 3, 4, 52, 36);
            Engine_ActorGet(10)->motion_flags = set;
            SetEffectRecordMode(Engine_ActorGet(10), 1);
            Engine_EventBegin();
            Call4((void (*)())Engine_CameraMoveTo, -1, -1, -1, 0);
            Engine_EventGetViewCenter()->motion_flags = set;
            Call2((void (*)())Engine_ActorSetSpritePriority, 0, 1);
            Call2((void (*)())Engine_ActorSetSpritePriority, 13, 1);
            Local_02004c68();
            Main_08009198();
            Engine_EventEnd();
            break;
        case 12:
            Engine_EventWait(1);
            SetEffectRecordMode(Engine_ActorGet(0), 1);
        case 11:
            Call6((void (*)())Engine_MapCopyCellAttributes, 104, 34, 5, 4, 40, 34);
            Call6((void (*)())Engine_MapCopyCellsTo, 45, 91, 40, 91, 5, 4);
            if (!Engine_GameFlagIsSet(0x881)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 30, 45, 50, 45, 5, 6);
                Call6((void (*)())Engine_MapCopyCellsTo, 50, 105, 50, 109, 5, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
            } else {
                Call2((void (*)())Engine_ActorSetSpritePriority, 14, 1);
                Call2((void (*)())Engine_ActorSetSpritePriority, 15, 1);
                Call2((void (*)())Engine_ActorSetSpritePriority, 16, 1);
            }
            Engine_ActorGet(9)->scale_x = -0x10000;
            if (!Engine_GameFlagIsSet(0x82b)) {
                Call3((void (*)())Engine_ActorSetPosition, 3, 0, 0);
                Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
                Call2((void (*)())Engine_ActorSetSpritePriority, 10, 1);
                Call6((void (*)())Engine_MapCopyCellAttributes, 116, 37, 3, 3, 52, 37);
                Call6((void (*)())Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
                Call2((void (*)())Engine_TaskAddCallback, TASK_649, 0xc80);
                break;
            }
            if (!Engine_GameFlagIsSet(0x871)) {
                if (!Engine_GameFlagIsSet(0x870)) {
                    Call3((void (*)())Engine_ActorFaceDirection, 3, 0, 0);
                    Call2((void (*)())Engine_ActorSetAnimation, 3, 16);
                } else {
                    Call3((void (*)())Engine_ActorSetPosition, 3, 210 << 18, 158 << 18);
                    Call3((void (*)())Engine_ActorFaceDirection, 3, 192 << 8, 0);
                }
                SetEffectRecordMode(Engine_ActorGet(3), 1);
                Call6((void (*)())Engine_MapCopyCellsTo, 126, 35, 116, 35, 1, 2);
                Call2((void (*)())Engine_TaskAddCallback, TASK_649, 0xc80);
            } else {
                Call3((void (*)())Engine_ActorSetPosition, 3, 0, 0);
                Call3((void (*)())Engine_ActorSetPosition, 10, 206 << 18, 150 << 18);
                Call6((void (*)())Engine_MapCopyCellAttributes, 116, 36, 3, 4, 52, 36);
                Engine_ActorGet(10)->collision_flags = 254;
                Engine_EventWait(1);
            }
            Engine_ActorGet(10)->motion_flags = 0;
            SetEffectRecordMode(Engine_ActorGet(10), 1);
            break;
        }
    }
    return 0;
}
