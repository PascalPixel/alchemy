#include "TYPES.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_0200186e
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"

/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor_02003fb4 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02003fb4 *anchor;  /* 0x68 */
};

/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor_02004004 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02004004 *anchor;  /* 0x68 */
};

extern s32 Data_0200a518[];
extern s32 Data_0200a410[];
extern s32 Data_0200a338[];
extern s32 Data_0200a218[];
extern u8 Value_000022b9;
extern s32 Data_03001e40;

s32 Func_0200186e(s32, s32);
s32 Func_02001a0c(s32);
void Func_020030f6(s32);
void Func_02002d4e(void);
void Func_0200329a(void);
void Func_0200061c(s32);
void Func_02000644(s32);
void Func_02000658(s32);
void Func_0200066c(s32);
void Func_02000680(s32);
void Func_020006bc(s32);
s32 Func_02002766();
s32 Func_02002c98();
s32 Func_02002c9e();
void Func_02002cf8();
void Func_02002d62();
void Func_02002d70();
void Func_02003106(s32);
void Func_02002d9a(void);
void Func_02001d34(void);
void Func_02000608(s32);
void Func_02000630(s32);
void Func_02000694(s32);
void Func_020006a8(s32);
void Func_020006d0(s32);
void Func_02002c00(u8 *);
s32 Func_02002da4(s32, s32);
void Func_02002c3c(u8 *);
s32 Func_02002de0(s32, s32);
void Func_02003250(void);

/* One import, four call sites, four distinct veneer addresses. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

u8 *FacingTargetScene_GetScriptData(void)
{
    return (u8 *)0x02009fd0;
}

s32 Func_02000090(void)
{
    return 0;
}

s32 SceneData_SelectTableA1DCOrA198(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return 0x0200A1DC;
    }
    return 0x0200A198;
}

s32 SceneData_SelectTableByStoryFlags(void)
{
    extern u8 Data_02000240[];

    u8 *b = Data_02000240;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)Data_0200a518;
    if (GameFlag_IsSet(0x87a) != 0)
        tbl = Data_0200a410;
    else if (GameFlag_IsSet(0x815) != 0)
        tbl = Data_0200a338;
    else
        tbl = Data_0200a218;
    Func_02001a0c((s32)tbl);
    return (s32)tbl;
}

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_000022b9;

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

s32 SceneData_SelectTableByStoryFlagsB(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[225] == 19) {
        if (GameFlag_IsSet(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (GameFlag_IsSet(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (GameFlag_IsSet(0x87A) != 0) {
        return 0x0200A980;
    }
    if (GameFlag_IsSet(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}

void FieldScene_RunScene377_020001e0(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(30);
    Event_SetMessage(0xf5b);
    Actor_FaceEachOther(0, 16, 10);
    Event_ShowMessageAndWait(16, 0, 6);
    Actor_ShowEmote(16, 0x102, 0);
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 4);
    Event_End();
}

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Event_Begin();
    Actor_FaceActor(0xE, 0, 0xA);
    Event_SetMessage(0x11AA);
    Event_OpenMessage(0xE, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0xE, 0);
    } else {
        work = *(void **)0x03001EBC;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Event_AskYesNo(0xE, 0);
    }
    Event_End();
}

void SceneState_SetWork448To521AndRun(s32 object)
{
    extern u8 *Data_03001ebc;

    if (GameFlag_IsSet(0x834) != 0) {
        Func_02001d34();
    }
    *(s32 *)(Data_03001ebc + 448) = 521;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(object);
}

void SceneState_SetValue123Mode1(void)
{
    Audio_PlayCue(0x7B);
    Func_02000608(1);
}

void FieldScene_RunStep7BThen2(void)
{
    Audio_PlayCue(0x7B);
    Func_0200061c(2);
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(0x7B);
    Func_02000630(3);
}

void FieldScene_RunStep7BThen4(void)
{
    Audio_PlayCue(0x7B);
    Func_02000644(4);
}

void FieldScene_RunStep80Then5(void)
{
    Audio_PlayCue(0x80);
    Func_02000658(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Audio_PlayCue(0x7B);
    Func_0200066c(6);
}

void FieldScene_RunStep80Then7(void)
{
    Audio_PlayCue(0x80);
    Func_02000680(7);
}

void SceneState_SetValue129Mode8(void)
{
    Audio_PlayCue(0x81);
    Func_02000694(8);
}

void SceneState_SetValue129Mode9(void)
{
    Audio_PlayCue(0x81);
    Func_020006a8(9);
}

void FieldScene_RunStep7BThen10(void)
{
    Audio_PlayCue(0x7B);
    Func_020006bc(10);
}

void SceneState_ApplyValues123And11(void)
{
    Audio_PlayCue(0x7B);
    Func_020006d0(11);
}

void FieldScene_RunScene377_02000e34(void)
{
    u32 i;
    s32 record;

    record = Func_02002766(0);
    if ((u32)(*(u16 *)(record + 6) + -0x2000) > 0x9000) {
        Inn_Open(0, 13);
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x87a) != 0) {
            Actor_RunRepeatedMotion(13, 2);
            Actor_FaceActor(13, 0, 10);
            if (GameFlag_IsSet(0x300) == 0) {
                Event_SetMessage(0x1c14);
                Event_ShowMessage(13, 0);
                GameFlag_Set(0x300);
            }
            Event_SetMessage(0x1c15);
            Event_AskYesNo(13, 0);
            Actor_FaceDirection(13, 0x9000, 10);
        } else {
            if (GameFlag_IsSet(0x815) != 0) {
                Event_SetMessage(0x11a9);
            } else {
                Event_SetMessage(0xf58);
            }
            Event_ShowMessage(13, 0);
        }
        Event_End();
    }
}

void SceneDialogue_ShowLine1C13WithActor16Steps(void)
{
    Event_Begin();
    Actor_FaceActor(0x10, 0, 0xA);
    Event_SetMessage(0x1C13);
    Event_ShowMessage(0x10, 0);
    Actor_FaceDirection(0x10, 0xB000, 0xA);
    GameFlag_Set(0x301);
    Event_End();
}

void SceneDialogue_RunActorThirteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(0x1C1B);
    Event_ShowMessage(0xD, 0);
    GameFlag_Set(0x81C);
    Event_End();
}

void SceneDialogue_RunActor16LineAndFlag81c(void)
{
    Event_Begin();
    Event_SetMessage(0x1C1A);
    Event_ShowMessage(0x10, 0);
    GameFlag_Set(0x81C);
    Event_End();
}

void ActorPresentation_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Map_CopyCellsTo(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Map_CopyCellAttributes(25, 15, 2, 2, extent, extent);
    }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 rec7;

    Func_02002cf8(a0);
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(0, 0, 0);
    Actor_SetPosition(18, 0x1e00000, 0xca0000);
    Task_Wait(1);
    Camera_FollowActor(18, 1);
    rec7 = 0;
    actor = (struct FieldActor *)Value4(Func_02002c98, 22, 0x1480000, 0x20000, 0xc30000);
    actor->motion_flags = rec7;
    sprite = actor->sprite;
    actor->y.fixed = 0x50000;
    sprite->part_count = rec7;
    sprite->full_color = 0;
    sprite->palette = 0;
    rec7 = Value2(Func_02002c9e, 17, 0x608);
    Item_LoadIcon(224);
    Vram_Load(sprite->vram_block, 128, rec7 + 0x400);
    Heap_Release(17);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
    Event_OpenScreen();
    Actor_SetSpeed(18, 0x10000, 0x8000);
    Actor_WalkToAndWait(18, 0x1e0, 176);
    Actor_WalkToAndWait(18, 0x1a4, 164);
    Actor_WalkToAndWait(18, 0x146, 185);
    Actor_FaceDirection(18, 0x4000, 10);
    Engine_ObjectSetScript(actor, 0x2009eac);
    Func_02002d62(actor);
    Object_SetScript((s32)actor, 0x2009ecc);
    Func_02002d70(actor);
    Event_Wait(20);
    Engine_ObjectDispatchRelease(actor);
    Actor_Jump(18, 2, 20);
    Actor_FaceDirection(18, 0, 40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(22);
}

void SceneEffect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((Data_03001e40 & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c00(obj);
    }
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        Object_SetPartPalettes(obj, Func_02002da4((s32)((u32)Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c3c(obj);
    }
}

void OverlayObject_ApplyRandomSlotOnOddFrames(s32 obj)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_02002de0((u32)Data_03001e40 >> 1, 6);

        Object_SetPartPalettes(obj, slot);
    }
}

void SceneEffect_UpdateAnchoredRiseFrame(struct Actor_02003fb4 *self)
{
    struct Actor_02003fb4 *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Math_Sin(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void OverlayObject_UpdateArcFromAnchor(struct Actor_02004004 *obj)
{
    struct Actor_02004004 *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Math_Sin(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

void SceneState_SetValue140Mode0(void)
{
    Psynergy_Begin(0x8C, 0);
}

void FieldScene_RunSingleStep(void)
{
    Func_02003250();
}

void FieldScene_RunActor8TwoStep(void)
{
    Func_020030f6(8);
    Func_02002d4e();
}

void FieldScene_RunStep17(void)
{
    Func_02003106(17);
    Func_02002d9a();
}

void FieldScene_RunSixStepSequence17e4(void)
{
    Psynergy_Begin(0x94, 1);
    Psynergy_SetTarget(8, 0x11);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Func_0200329a();
}
