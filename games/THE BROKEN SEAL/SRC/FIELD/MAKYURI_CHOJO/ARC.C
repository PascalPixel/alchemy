#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

void FieldScene_ConfigureValue93Scene(void)
{
    Psynergy_Begin(93, 1);
    Psynergy_SetTarget(24, 9);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjectsFar();
}

void SceneEffect_UpdateArcPosition(struct OverlayObject *object)
{
    struct OverlayObject *parent;
    u16 angle;

    parent = object->linked_object;
    angle = object->angle_64;
    object->coordinate_08 = parent->coordinate_08 + Math_Cos(angle) * (object->field_30 + 28);
    object->coordinate_10 = (Math_Sin(angle) << 4) + 0x900000;
    object->coordinate_38 = object->coordinate_08;
    object->coordinate_40 = object->coordinate_10;
    object->angle_64 -= 0x200;
}
