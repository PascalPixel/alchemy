/*
 * resource_3ca:02000430 Scene_RunExtendedPresentationSequence - draft; the
 * range links as disassembly (section .text.x02008430 of the overlay
 * listing).
 *
 * Remaining: the last call passes a zero that the reference loads from the
 * literal pool (ldr r0, last pool word 0). Written as a plain zero below,
 * GCC passes mov r0, #0 and the pool loses its last word, four bytes
 * shorter; a u16 or u8 zero and the backdrop's blank variable give the
 * same. The unit matched only while the zero was a link-time symbol named
 * after its own value.
 */
#include "TYPES.H"

extern u8 gMapWork[];
extern u8 *gEventWork;
extern const s32 BabiFune_ActionScriptA[];
extern const s32 BabiFune_ActionScriptB[];
extern const s32 BabiFune_ActionScriptC[];
extern const s32 BabiFune_ActionScriptD[];

void Engine_EventBegin();
void Engine_CameraMoveTo();
void Engine_MapCopyCellAttributes();
void Engine_EventSetMessage();
void Motion_LaunchFromFocusedObject();
void Engine_ActorWaitForMove();
void Engine_EventWait();
void Engine_ActorRunRepeatedMotion();
void Engine_EventShowMessage();
void Engine_ActorSetAnimationAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
s32 UiText_OpenMessageAtObject();
s32 Inventory_PromptAndSetObjectMode();
void Engine_ActorShowEmote();
void Engine_AudioPlayCue();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void Object_RefreshSelectorById();
void ObjectMotion_CommitPositionAndActivate();
void Engine_CameraWaitForMove();
void MusicCommand_SetPitchAndUpdateFrequency();
void BabiFune_ScheduleFade();
void * Engine_ActorGet();
void Engine_CameraSetSpeed();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_GameFlagSet();
void Event_SetPairWork1c0Far();

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

void Scene_RunExtendedPresentationSequence(void)
{
    u8 *runtime;
    u8 *scene;
    s32 blank;
    void *p176;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Call6(Engine_MapCopyCellAttributes, 18, 0, 3, 1, 18, 12);
    Call1(Engine_EventSetMessage, 10494);
    Call4(Motion_LaunchFromFocusedObject, 1, -10, 16, 49152);
    Call4(Motion_LaunchFromFocusedObject, 3, 0, 24, 49152);
    Call4(Motion_LaunchFromFocusedObject, 2, 10, 16, 49152);
    Call1(Engine_ActorWaitForMove, 1);
    Call1(Engine_EventWait, 50);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 3, 16384, 0);
    Call1(Engine_EventWait, 40);
    Call2(Engine_EventShowMessage, 3, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 0, 16384, 0);
    Call3(Engine_ActorFaceDirection, 1, 16384, 0);
    Call3(Engine_ActorFaceDirection, 2, 16384, 0);
    Call1(Engine_EventWait, 60);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 60);
    Call2(Engine_ActorRunRepeatedMotion, 1, 2);
    Call1(Engine_EventWait, 30);
    Call3(Engine_ActorFaceDirection, 1, 57344, 0);
    Call1(Engine_EventWait, 20);
    UiText_OpenMessageAtObject(1, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorShowEmote, 1, 258, 50);
        Call2(Engine_EventShowMessage, 1, 0);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
    } else {
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorShowEmote, 1, 258, 50);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
        Call2(Engine_EventShowMessage, 1, 0);
    }
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 2, 32768, 0);
    Call1(Engine_EventWait, 30);
    Call3(Engine_ActorShowEmote, 2, 257, 50);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 1, 0, 0);
    Call1(Engine_EventWait, 30);
    Call3(Engine_ActorShowEmote, 1, 257, 60);
    Call2(Engine_ActorRunRepeatedMotion, 3, 2);
    Call1(Engine_EventWait, 30);
    Call3(Engine_ActorFaceDirection, 3, 40960, 0);
    Call1(Engine_EventWait, 30);
    Call2(Engine_EventShowMessage, 3, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorShowEmote, 1, 256, 50);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorRunRepeatedMotion, 2, 2);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 3, 49152, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorSetAnimationAndWait, 3, 4);
    Call1(Engine_EventWait, 20);
    UiText_OpenMessageAtObject(3, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorRunRepeatedMotion, 1, 2);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorFaceDirection, 1, 57344, 0);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 1, 0);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
    } else {
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorRunRepeatedMotion, 1, 2);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorFaceDirection, 1, 57344, 0);
        Call1(Engine_EventWait, 20);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
        Call2(Engine_EventShowMessage, 1, 0);
    }
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorShowEmote, 0, 258, 50);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 2, 40960, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorSetAnimationAndWait, 0, 3);
    Call1(Engine_EventWait, 30);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorSetAnimationAndWait, 3, 3);
    Call1(Engine_EventWait, 20);
    UiText_OpenMessageAtObject(3, 0);
    Call3(Engine_ActorFaceDirection, 1, 57344, 0);
    Call3(Engine_ActorFaceDirection, 3, 49152, 0);
    Call3(Engine_ActorFaceDirection, 2, 40960, 0);
    Call1(Engine_EventWait, 20);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Call1(Engine_EventWait, 30);
        Call2(Engine_ActorSetAnimationAndWait, 3, 3);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 3, 0);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
    } else {
        Call1(Engine_EventWait, 30);
        Call2(Engine_ActorSetAnimationAndWait, 3, 3);
        Call1(Engine_EventWait, 20);
        (*(u16 *)(*(u8 **)&gEventWork + 472))++;
        Call2(Engine_EventShowMessage, 3, 0);
    }
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 1, 49152, 0);
    Call1(Engine_EventWait, 30);
    Call2(Engine_EventShowMessage, 1, 0);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 0, 49152, 0);
    Call1(Engine_EventWait, 30);
    Call1(Engine_EventWait, 10);
    Call3(Engine_ActorFaceDirection, 2, 49152, 0);
    Call1(Engine_EventWait, 30);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 2, 0);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorSetAnimationAndWait, 3, 3);
    Call1(Engine_EventWait, 20);
    Call2(Engine_EventShowMessage, 3, 0);
    Call1(Engine_AudioPlayCue, 17);
    Call1(Engine_EventWait, 10);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 40);
    Call3(Engine_ActorSetSpeed, 0, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 1, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 2, 78643, 39321);
    Call3(Engine_ActorSetSpeed, 3, 78643, 39321);
    Call2(Engine_ActorEnableActionCallback, 0, (s32)BabiFune_ActionScriptA);
    Call1(Engine_EventWait, 50);
    Call4(Engine_CameraMoveTo, 17301504, -1, 13107200, 1);
    Call2(Engine_ActorEnableActionCallback, 1, (s32)BabiFune_ActionScriptB);
    Call1(Engine_EventWait, 50);
    Call2(Engine_ActorEnableActionCallback, 2, (s32)BabiFune_ActionScriptC);
    Call1(Object_RefreshSelectorById, 2);
    Call3(Engine_ActorFaceDirection, 0, 16384, 0);
    Call3(Engine_ActorFaceDirection, 1, 16384, 0);
    Call3(Engine_ActorFaceDirection, 2, 16384, 0);
    Call3(ObjectMotion_CommitPositionAndActivate, 3, 0, -32);
    Call1(Engine_EventWait, 30);
    Call3(Engine_ActorFaceDirection, 3, 16384, 0);
    Call1(Engine_EventWait, 60);
    Call3(Engine_ActorFaceDirection, 3, 32768, 0);
    Call1(Engine_EventWait, 20);
    Call2(Engine_ActorEnableActionCallback, 3, (s32)BabiFune_ActionScriptD);
    Call1(Object_RefreshSelectorById, 3);
    Call3(Engine_ActorFaceDirection, 3, 49152, 0);
    Call1(Engine_EventWait, 20);
    Call4(Engine_CameraMoveTo, 14155776, -1, 11010048, 1);
    Engine_CameraWaitForMove();
    Call1(Engine_EventWait, 20);
    Call3(Engine_ActorFaceDirection, 0, 32768, 0);
    Call3(Engine_ActorFaceDirection, 1, 32768, 0);
    Call3(Engine_ActorFaceDirection, 2, 32768, 0);
    Call3(Engine_ActorFaceDirection, 3, 32768, 0);
    Call1(Engine_EventWait, 40);
    Call2(Engine_ActorSetAnimation, 0, 3);
    Call2(Engine_ActorSetAnimation, 1, 3);
    Call2(Engine_ActorSetAnimation, 3, 3);
    Call2(Engine_ActorSetAnimationAndWait, 2, 3);
    Call1(Engine_EventWait, 30);
    Call1(Engine_AudioPlayCue, 67);
    Call1(MusicCommand_SetPitchAndUpdateFrequency, 240);
    BabiFune_ScheduleFade();
    Call1(Engine_EventWait, 80);
    scene = *(u8 **)gMapWork;
    p176 = Engine_ActorGet(8);
    *(s32 *)(p176 + 52) = 131;
    *(s32 *)(p176 + 48) = 131072;
    *(s32 *)(scene + 284) = -0x3000;
    Call3(ObjectMotion_CommitPositionAndActivate, 8, 60, 0);
    *(s32 *)(scene + 284) = -0x6000;
    Call3(ObjectMotion_CommitPositionAndActivate, 8, 60, 0);
    Call1(Engine_EventWait, 80);
    Call1(Engine_EventWait, 100);
    Call2(Engine_CameraSetSpeed, 144179, 655);
    Call4(Engine_CameraMoveTo, 52953088, -1, 11010048, 1);
    Call1(Engine_EventWait, 300);
    runtime = *(u8 **)(gMapWork + 76);
    *(u32 *)(runtime + 448) = 256;
    /* FAKEMATCH: keep the backdrop zero before its address calculation. */
    do {
        blank = 0;
    } while (0);
    *(u16 *)0x05000000 = blank;
    *(u32 *)(runtime + 456) = 96;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call1(Engine_EventWait, 30);
    Call1(Engine_GameFlagSet, 282);
    Call2(Event_SetPairWork1c0Far, 0, 9);
}
