#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001f30[];
void ResetSceneTransitionEffect(void);

void MapEvent_RunTileTriggerSequence(void);
void FieldEvent_ShowStatusMessage(void);

void FieldEvent_RunTypeHandler(void)
{
    u32 type;

    type = (s16)FIELD_AT_OFFSET(*(void **)((u32)&Data_03001f30), s16 *, 0x1E);
    switch (type) {
    case 8:
        ResetSceneTransitionEffect();
        return;
    case 10:
        MapEvent_RunTileTriggerSequence();
        return;
    case 16:
        FieldEvent_ShowStatusMessage();
        return;
    }
}
