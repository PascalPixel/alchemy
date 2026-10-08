#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"

#define NULL ((void *)0)
#define REC_ID 16
#define QUERY_FLAG 0x200

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"

extern void *gWorkSlot[];

struct PillarWork {
    u8 unknown_00[24];
    struct FieldActor *rider;
};

#define PILLAR_WORK (*(struct PillarWork **)&gWorkSlot[36])

extern u8 TakaraHashira_ActionTable[];
extern u16 TakaraHashira_ShiftSteps1[];
extern u16 TakaraHashira_ShiftSteps2[];
extern u16 TakaraHashira_ShiftSteps3[];
extern u16 TakaraHashira_ShiftSteps4[];
extern u16 TakaraHashira_ShiftSteps5[];

void CopyAndOffsetCoordinatePreset(void);
void TakaraHashira_JitterBackgroundScroll(void);
s32 SceneActor_UpdateBit1ByPositionToSlotZero();
s32 TakaraHashira_UpdateActorPriority();

void WaitFrames(s32 frames);
void Object_SetMode(struct FieldActor *object, s32 mode);

extern u8 TakaraHashira_ShakenScroll[];
extern u8 *gEffectWork;

extern u8 gCell[][2];
extern s32 TakaraHashira_ShakeChance;

s32 FieldScene_QueryActorFootprint();
void Vector_AddPolarOffset();
void Vector_AddPolarOffset();
s32 Object_CheckMovementCollision();
s32 Object_CheckMovementCollision();
void Map_DrawCellPanel(s32 x, s32 y, s32 width, s32 height, s32 plane, s32 destX, s32 destY);
void Runtime_SetIrqHandler();
void Runtime_SetIrqHandler();
s32 FieldScene_RunScene3b3SequenceD();
void TakaraHashira_UpdatePillarActors();
void FieldScene_RunScene3b3SequenceE();
int FieldScene_RunScene3b3SequenceD(void);
void battle_owner_69();

struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *origin);

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);
