#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void BattleFx_InitializeSlots(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);
void Shop_RestoreSceneTiles(s32 mode);
void Shop_InitEffect(void);
void Camera_WorldToScreen(s32 *pos);
void EffectSlot_Initialize(u8 *object, s32 type, s32 x, s32 z);
void EffectSlot_SetCallback(u8 *object, void (*update)());
void EffectSlot_SetObjectMode(u8 *object, s32 mode);
void ObjectGroup_SetChildValueUnlessFifteen(s32 handle, s32 frame);
s32 IwramUnsignedDivide(s32 value, s32 divisor);
void OverlayObject_UpdateThreeStateMotion();

/* The overlay object records (72 bytes each), from +88. */
extern u8 *gEffectWork;

/* Releases 24 objects (type 284) one per frame from a point by the cave
 * wall, each with a random frame and speed, opens the wall's cells, then
 * marks every object still active once the screen has faded. */
void ArutamiraDou_ReleaseWallBurst(void)
{
    u8 *work;
    s32 pos[3];
    s32 *p;
    u8 *object;
    s32 n;

    BattleFx_InitializeSlots();
    work = gEffectWork;
    Shop_RestoreSceneTiles(0x202108);
    p = pos;
    p[0] = 0x1f80000;
    p[1] = 0x180000;
    p[2] = 0x900000;
    Camera_WorldToScreen(p);
    object = work + 88;
    for (n = 23; n >= 0; n--) {
        s32 speed;

        EffectSlot_Initialize(object, 284, p[0], p[2]);
        EffectSlot_SetCallback(object, OverlayObject_UpdateThreeStateMotion);
        EffectSlot_SetObjectMode(object, 7);
        ObjectGroup_SetChildValueUnlessFifteen(*(s32 *)object, (u32)(Engine_RandomNext() * 7) >> 16);
        speed = IwramUnsignedDivide(Engine_RandomNext(), 3) + 0x18000;
        *(s32 *)(object + 44) = speed;
        *(s32 *)(object + 40) = speed;
        Engine_TaskWait(1);
        object += 72;
    }
    Engine_TaskWait(80);
    Call6((void (*)())Engine_MapCopyCellsTo, 41, 55, 3, 2, 30, 55);
    Call6((void (*)())Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
    Engine_TaskWait(50);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_TaskWait(30);
    {
        s32 mode = 2;
        u8 *state = work + 152;

        for (n = 23; n >= 0; n--) {
            if (((s8 *)state)[5] != 0) {
                *state = mode;
            }
            state += 72;
        }
    }
    Engine_MapWaitWorkValuesBelow256();
    Shop_InitEffect();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
