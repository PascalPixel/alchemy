/* The pillar rooms' setup: redraw the pillars' footprints, restore the map
 * the switches changed, and in the fourth room raise the pillars and watch
 * the actors near them. */
#include "HASHIRA.H"
#include "CALL.H"

extern u8 TakaraHashira_PillarSlots[];

void SceneActor_ApplyPlacementQueryAndTag();
void BattleFx_StartFadeOverlay();
void SceneState_ClearWord24AndObjectByte62();
void OverlayObject_SetCallbackAndMode2();
void SceneActor_CheckActors8To11NearSlotZero(void);
void TakaraHashira_DropActorTen();
void TakaraHashira_PrepLoweredActor();
void FieldScene_RunScene3b3_0200263c();
s32 TakaraHashira_SetupArea(void)
{
    u32 i;
    u8 *record;
    s32 v1;
    s32 v2;
    s32 base5_8;
    s32 v5;
    s32 v0;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraHashira2) {
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        FieldScene_RedrawActorFootprint(11);
        FieldScene_RedrawActorFootprint(12);
    } else {
        if (gGameState.scene == (s32)&SceneId_TakaraHashira3) {
            Call6(Engine_MapCopyCells, 32, 0, 64, 32, 0, 64);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            FieldScene_RedrawActorFootprint(10);
            FieldScene_RedrawActorFootprint(11);
            FieldScene_RedrawActorFootprint(12);
            FieldScene_RedrawActorFootprint(13);
            FieldScene_RedrawActorFootprint(14);
            FieldScene_RedrawActorFootprint(15);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            if (Engine_GameFlagIsSet(0x200) == 0) {
                goto L_020029fa;
            }
            Call6(Engine_MapCopyCellsTo, 79, 34, 84, 24, 1, 2);
            Engine_MapCopyCellsTo(0, 32, 32, 0, 32, 32);
            Engine_MapCopyCellsTo(32, 32, 64, 0, 32, 32);
            SceneActor_ApplyPlacementQueryAndTag(9);
            SceneActor_ApplyPlacementQueryAndTag(10);
            SceneActor_ApplyPlacementQueryAndTag(11);
            SceneActor_ApplyPlacementQueryAndTag(12);
            SceneActor_ApplyPlacementQueryAndTag(13);
            SceneActor_ApplyPlacementQueryAndTag(14);
            SceneActor_ApplyPlacementQueryAndTag(15);
            Call6(Engine_MapCopyCellAttributes, 24, 3, 1, 1, 24, 8);
            goto L_020029fa;
        } else {
            if (gGameState.scene != (s32)&SceneId_TakaraHashira4) {
                goto L_02002922;
            }
            OverlayObject_CreateConfiguredObject(0x2480000, 0, 0xc80000, 223);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                *((u8 *)Engine_ActorGet(0) + 98) = 1;
            }
            BattleFx_StartFadeOverlay(0);
            if (*((u8 *)Engine_ActorGet(0) + 98) == 0) {
                SceneState_ClearWord24AndObjectByte62();
            }
            OverlayObject_SetCallbackAndMode2(8);
            OverlayObject_SetCallbackAndMode2(9);
            OverlayObject_SetCallbackAndMode2(10);
            OverlayObject_SetCallbackAndMode2(11);
            {
                s32 *entry = (s32 *)TakaraHashira_PillarSlots;

                for (v1 = 0; (u32)v1 <= 3; v1++) {
                    entry[0] = 0;
                    entry[1] = 0;
                    entry[2] = 0;
                    entry[4] = v1 + 0x200;
                    entry += 5;
                }
            }
            TakaraHashira_UpdatePillarActors();
            Engine_EventWait(1);
            ((void (*)())Engine_TaskAddCallback)((s32)SceneActor_CheckActors8To11NearSlotZero, 0xc80);
            if (Engine_GameFlagIsSet(0x109) == 0) {
                goto L_020029fa;
            }
            for (base5_8 = 8; (u32)base5_8 <= 11; base5_8++) {
                record = (u8 *)((s32 (*)())Engine_ActorGet)(base5_8);
                v2 = *(s32 *)(record + 8) >> 20;
                if (v2 == 37) {
                    v0 = *(s32 *)(record + 16) >> 20;
                    if (v0 == 9) {
                        Engine_MapCopyCellAttributes(27, 8, 1, 1, v2, v0);
                        break;
                    }
                }
            }
        }
        goto L_020029fa;
        L_02002922:;
        if (gGameState.scene == (s32)&SceneId_TakaraHashira5) {
            Engine_ActorSetAnimation(10, 2);
            Engine_ActorSetChildValue(10, 6);
            FieldScene_RedrawActorFootprint(8);
            FieldScene_RedrawActorFootprint(9);
            v5 = 0;
            *((u8 *)Engine_ActorGet(8) + 85) = v5;
            *((u8 *)Engine_ActorGet(9) + 85) = v5;
            TakaraHashira_DropActorTen();
            TakaraHashira_PrepLoweredActor(11);
            TakaraHashira_PrepLoweredActor(12);
            TakaraHashira_PrepLoweredActor(13);
            FieldScene_RunScene3b3_0200263c(11);
            FieldScene_RunScene3b3_0200263c(12);
            FieldScene_RunScene3b3_0200263c(13);
            record = (u8 *)((s32 (*)())Engine_ActorGet)(13);
            *(s32 *)((s32)record + 108) = v5;
            TakaraHashira_PrepLoweredActor(14);
            {
                u8 *record = (u8 *)Engine_ActorGet(14);
                /* FAKEMATCH: retain the flag read before its merge. */
                u8 value = *(volatile u8 *)&record[89];

                record[89] = (u8)(value | 8);
            }
            if (Engine_GameFlagIsSet(0x202) == 0) {
                v5 = 192;
                record = (u8 *)Engine_ActorGet(13);
                *(s32 *)((s32)record + 24) = (v5 << 9);
                record = (u8 *)((s32 (*)())Engine_ActorGet)(13);
                *(s32 *)((s32)record + 28) = (v5 << 9);
                record = (u8 *)((s32 (*)())Engine_ActorGet)(13);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                record = (u8 *)((s32 (*)())Engine_ActorGet)(14);
                *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
                Call6(Engine_MapCopyCellAttributes, 26, 12, 1, 1, 22, 16);
            }
        }
    }
    L_020029fa:;
    return 0;
}
