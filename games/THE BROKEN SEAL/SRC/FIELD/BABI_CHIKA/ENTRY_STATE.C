#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void BattleFx_StartFadeOverlay(s32 value);
void FieldScene_PlaceAndPinSlots8And9(void);
void FieldScene_PlaceAndPinSlots10And11(void);
void FieldScene_RunScene3c4_02002480(void);
void FieldScene_RunLateSequenceHead(void);
void BabiChika_MarkActorCells(void);
void BabiChika_SettleSteps(s32 wait);

s32 SceneActor_SetFlagBitByRelativeDepth();
s32 OverlayObject_SetYAboveLinkedActor();
void BabiChika_UpdateTrackedActor(void);

#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor)
#define SCENE_TASK BabiChika_UpdateTrackedActor

/* Entry setup for the underground passage: by area and entrance, restores the lift cells, pins and parks the paired actors and re-applies each flagged block. */
s32 FieldScene_InitializeActorGroups(void)
{
    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_BabiChika1 || gGameState.scene == (s32)&SceneId_BabiChika2) {
        BattleFx_StartFadeOverlay(0);
        gGameState.retreat_entrance = 1;
        gGameState.retreat_scene = (s32)&SceneId_BabiChika1;
    }
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
            if (Engine_GameFlagIsSet(0x982)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 4, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 9, 73, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 81, 9, 75, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 9, 77, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 9, 79, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 11, 78, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 13, 79, 3, 2);
            } else {
                if (!Engine_GameFlagIsSet(0x983))
                    break;
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 13, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 85, 11, 74, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 13, 75, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 85, 11, 76, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 11, 78, 3, 2);
                ((void (*)())Engine_MapCopyCellsTo)(18, 83, 13, 79, 3, 2);
            }
            break;
        case 3:
        case 4:
            FieldScene_PlaceAndPinSlots8And9();
            Object_GetById(8)->motion_flags = 0;
            Object_GetById(9)->motion_flags = 0;
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(8), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(9), 0);
            Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
            ((void (*)())Engine_TaskAddCallback)(SCENE_TASK, 0xc80);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Engine_MapCopyCells, 23, 17, 1, 2, 30, 8);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Engine_MapCopyCells, 23, 17, 1, 2, 32, 10);
            break;
        case 8:
        case 9:
            FieldScene_PlaceAndPinSlots10And11();
            Object_GetById(10)->motion_flags = 0;
            Object_GetById(11)->motion_flags = 0;
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(10), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(11), 0);
            Object_GetById(10)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(11)->update = ACTOR_UPDATE_IDLE;
            ((void (*)())Engine_TaskAddCallback)(SCENE_TASK, 0xc80);
            break;
        case 10:
        case 11:
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(18), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(19), 0);
            ((void (*)())Object_SetModeById)(18, 2);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(20), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(21), 0);
            ((void (*)())Engine_ActorSetChildValue)(20, 15);
            ((void (*)())Engine_ActorSetChildValue)(21, 15);
            if (Engine_GameFlagIsSet(0x971)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 49, 8, 1, 3);
                ((void (*)())Map_CopyCellAttributeRect)(51, 8, 1, 1, 49, 8);
                Object_GetById(18)->priority_flags |= 2;
                ((void (*)())Object_SetModeById)(18, 3);
                ((void (*)())Map_CopyCellAttributeRect)(45, 4, 1, 1, 46, 8);
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 136 << 16);
                Object_GetById(18)->y.fixed = -0x100000;
                Call3((void (*)())Engine_ActorSetPosition, 20, 186 << 18, 136 << 16);
            }
            if (Engine_GameFlagIsSet(0x200)) {
                ((void (*)())Engine_ActorSetChildValue)(20, 0);
                ((void (*)())Object_SetModeById)(20, 5);
            }
            if (Value1(Engine_GameFlagIsSet, 0x202))
                ((void (*)())Object_SetModeById)(19, 2);
            if (Engine_GameFlagIsSet(0x972)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 45, 14, 1, 3);
                ((void (*)())Map_CopyCellAttributeRect)(51, 8, 1, 1, 45, 14);
                Object_GetById(19)->priority_flags |= 2;
                ((void (*)())Object_SetModeById)(19, 3);
                ((void (*)())Map_CopyCellAttributeRect)(45, 4, 1, 1, 48, 14);
                Call3((void (*)())Engine_ActorSetPosition, 19, 194 << 18, 232 << 16);
                Object_GetById(19)->y.fixed = -0x100000;
                ((void (*)())Engine_ActorSetPosition)(21, 194 << 18, 232 << 16);
                Engine_GameFlagSet(0x202);
            }
            if (Engine_GameFlagIsSet(0x201)) {
                ((void (*)())Engine_ActorSetChildValue)(21, 0);
                ((void (*)())Object_SetModeById)(21, 5);
            }
            break;
        case 12:
        case 13:
            FieldScene_RunScene3c4_02002480();
            Object_GetById(12)->motion_flags = 0;
            Object_GetById(13)->motion_flags = 0;
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(15), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(16), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(17), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(12), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(13), 0);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(14), 0);
            Object_GetById(12)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(13)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(14)->update = ACTOR_UPDATE_IDLE;
            ((void (*)())Engine_TaskAddCallback)(SCENE_TASK, 0xc80);
            break;
        }
    } else {
        switch (gGameState.entrance) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            gGameState.retreat_entrance = 1;
            gGameState.retreat_scene = (s32)&SceneId_BabiIriguchi3;
            Engine_GameFlagClear(0x12f);
            ((void (*)())Engine_ActorSetChildValue)(17, 6);
            ((void (*)())Engine_ActorSetChildValue)(18, 6);
            if (Engine_GameFlagIsSet(0x974))
                Call3((void (*)())Engine_ActorSetPosition, 17, 182 << 18, 156 << 17);
            if (Engine_GameFlagIsSet(0x975))
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 156 << 17);
            BabiChika_MarkActorCells();
            break;
        case 6:
        case 7:
            ((void (*)())Engine_ActorSetSpritePriority)(8, 1);
            Object_GetById(8)->motion_flags = 0;
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(8), 0);
            ((void (*)())Engine_ActorSetSpritePriority)(9, 1);
            ((void (*)())Engine_ActorSetChildValue)(9, 15);
            ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(9), 0);
            Object_GetById(9)->motion_flags = 0;
            if (!Engine_GameFlagIsSet(0x204))
                break;
            ((void (*)())Engine_ActorSetChildValue)(9, 0);
            ((void (*)())Object_SetModeById)(9, 5);
            ((void (*)())Map_CopyCellAttributeRect)(26, 8, 1, 1, Object_GetById(9)->x.fixed >> 20, Object_GetById(9)->z.fixed >> 20);
            Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
            break;
        case 4:
        case 5:
            if (!Engine_GameFlagIsSet(0x109)) {
                Object_GetById(10)->motion_flags = 0;
                Object_GetById(11)->motion_flags = 0;
                Object_GetById(10)->y.fixed = -0x300000;
                Object_GetById(11)->y.fixed = -0x300000;
                Object_GetById(10)->priority_flags |= 2;
                Object_GetById(11)->priority_flags |= 2;
                Object_GetById(10)->collision_flags &= 0xfe;
                Object_GetById(11)->collision_flags &= 0xfe;
                Object_GetById(10)->unknown_64 = 3;
                Object_GetById(11)->unknown_64 = 3;
                ((void (*)())Engine_ActorSetSpritePriority)(10, 1);
                ((void (*)())Engine_ActorSetSpritePriority)(11, 1);
                Object_GetById(12)->motion_flags = 0;
                Object_GetById(13)->motion_flags = 0;
                Object_GetById(14)->motion_flags = 0;
                ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(12), 0);
                ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(13), 0);
                ((void (*)())Engine_ActorSetSpriteFlags)(Object_GetById(14), 0);
                Object_GetById(12)->unknown_64 = 0;
                Object_GetById(13)->unknown_64 = 0;
                Object_GetById(14)->unknown_64 = 0;
                if (gGameState.entrance != 5)
                    break;
                Object_GetById(10)->y.fixed = -0x200000;
                Object_GetById(11)->y.fixed = -0x400000;
                Object_GetById(10)->unknown_64 = 2;
                Object_GetById(11)->unknown_64 = 4;
                Call3((void (*)())Engine_ActorSetPosition, 12, 200 << 16, 152 << 16);
                Object_GetById(12)->unknown_64 = 11;
                Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(12)->priority_flags |= 2;
                Call3((void (*)())Engine_ActorSetPosition, 13, 200 << 16, 152 << 16);
                Object_GetById(13)->unknown_64 = 12;
                Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(13)->priority_flags |= 2;
                ((void (*)())Engine_ActorSetPosition)(14, 136 << 16, 152 << 16);
                Object_GetById(14)->unknown_64 = 10;
                Object_GetById(14)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(14)->priority_flags |= 2;
                Battle_WaitMode0(2);
                Engine_GameFlagSet(0x200);
                Engine_GameFlagSet(0x201);
                Engine_GameFlagSet(0x202);
            }
            BabiChika_SettleSteps(0);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Engine_MapCopyCells, 10, 30, 1, 2, 16, 30);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Engine_MapCopyCells, 10, 30, 1, 2, 22, 30);
            Engine_GameFlagSet(0x973);
            break;
        case 12:
            Call6((void (*)())Map_CopyCellAttributeRect, 8, 49, 1, 1, 8, 113);
            FieldScene_RunLateSequenceHead();
            ((void (*)())Engine_TaskAddCallback)(SCENE_TASK, 0xc80);
            break;
        case 13:
        case 14:
            Battle_WaitMode0(1);
            if (Engine_GameFlagIsSet(0x984)) {
                Call6((void (*)())Engine_MapCopyCells, 24, 59, 1, 2, 32, 46);
                Call3((void (*)())Engine_ActorSetPosition, 19, 204 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 20, 188 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 21, 204 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 22, 188 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 23, 196 << 17, 194 << 18);
            }
            Object_GetById(19)->motion_flags &= 0xfe;
            Object_GetById(20)->motion_flags &= 0xfe;
            Object_GetById(21)->motion_flags &= 0xfe;
            Object_GetById(22)->motion_flags &= 0xfe;
            Object_GetById(23)->motion_flags &= 0xfe;
            ((void (*)())Engine_ActorSetChildValue)(19, 4);
            ((void (*)())Engine_ActorSetChildValue)(20, 1);
            ((void (*)())Engine_ActorSetChildValue)(21, 4);
            ((void (*)())Engine_ActorSetChildValue)(22, 10);
            ((void (*)())Engine_ActorSetChildValue)(23, 0);
            ((void (*)())Object_SetModeById)(19, 2);
            ((void (*)())Object_SetModeById)(23, 2);
            ((void (*)())Map_CopyCellAttributeRect)(20, 56, 1, 1, Object_GetById(19)->x.fixed >> 20, Object_GetById(19)->z.fixed >> 20);
            ((void (*)())Map_CopyCellAttributeRect)(20, 56, 1, 1, Object_GetById(20)->x.fixed >> 20, Object_GetById(20)->z.fixed >> 20);
            ((void (*)())Map_CopyCellAttributeRect)(20, 56, 1, 1, Object_GetById(21)->x.fixed >> 20, Object_GetById(21)->z.fixed >> 20);
            ((void (*)())Map_CopyCellAttributeRect)(20, 56, 1, 1, Object_GetById(22)->x.fixed >> 20, Object_GetById(22)->z.fixed >> 20);
            ((void (*)())Map_CopyCellAttributeRect)(20, 56, 1, 1, Object_GetById(23)->x.fixed >> 20, Object_GetById(23)->z.fixed >> 20);
            break;
        case 17:
            Call6((void (*)())Map_CopyCellAttributeRect, 49, 43, 1, 1, 49, 107);
            FieldScene_RunLateSequenceHead();
            break;
        }
    }
    return 0;
}
