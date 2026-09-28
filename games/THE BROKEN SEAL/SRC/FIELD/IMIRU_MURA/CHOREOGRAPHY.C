#include "IMIRU.H"

void ActorPresentation_ApplyTableA5ecToActorNine(void)
{
    extern s32 ImiruMura_TurnScript[];

    Actor_EnableActionCallback(9, (s32)ImiruMura_TurnScript);
    Event_ShowMessage(9, 0);
}

void SceneState_StoreTable96adToWork(void)
{
    u8 *work;

    Psynergy_Begin(93, 1);
    work = *(u8 **)gEffectWork;
    Psynergy_SetTarget(3, 9);
    *(s32 *)(work + 36) = (s32)ActorPresentation_ApplyTableA5ecToActorNine;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjects();
}

void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    Object_SetPalette(work, 9);
    Actor_SetSpriteFlags(work, 0);

    work->f24 = 0x8000;
    work->f28 = 0x8000;
}

void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
{
    object->effect.x += (s16)object->effect.spin << 8;
    object->effect.y += 0x8000;
    object->effect.scale_x += 0x7ae;
    object->effect.scale_y += 0x7ae;
    object->effect.spin += 2;
    if (--object->effect.countdown == 0) {
        Engine_ObjectDispatchRelease(object);
    }
}

void FieldScene_RunScene399SequenceA(void)
{
    extern u32 Data_03001e40;
    u32 *frame;
    union FieldObject *object;

    frame = &Data_03001e40;
    if (Value2_02001794(Engine_MathModulo, *frame, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1cf0000, 0, 0x1240000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 30, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1400000, 0x200000, 0x1640000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 10, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x760000, 0, 0x460000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 50, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1560000, 0, 0x7c0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Engine_MathModulo, *frame + 80, 60) == 0) {
        object = (union FieldObject *)Value4(Engine_ObjectCreate, 222, 0x1af0000, 0, 0xab0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
}
