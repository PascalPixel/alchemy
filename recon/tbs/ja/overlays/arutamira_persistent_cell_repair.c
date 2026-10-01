/* Draft: Altmiller Cave persistent cell restoration.
 * 2026-10-01: The international variant records flag530 when actor12
 * settles and restores cell(32,20) on reentry. Japanese has neither
 * persistence step; their complete owners shrink12+36 bytes.
 * Ordinary approved TBS flags, no output changes.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "SCENE_IDS.H"
#include "DMA.H"
#include "../../../../games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ARUTAMIRA.H"

void ArutamiraDou_MatchLeaderPriority(struct FieldActor *actor);

void ArutamiraDou_SettleActorOnCell(void)
{
    struct FieldActor *actor = Object_GetById(12);

    if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
        actor->motion_flags = 2;
        *(s32 *)actor->unknown_14 = 0;
        actor->priority_flags = 2;
        Engine_MapCopyCellAttributes(30, 20, 1, 1, 32, 20);
        Engine_GameFlagSet(0x212);
    }
}

s32 ArutamiraDou_ApplyEntryState(void)
{
    s8 *state;
    struct FieldActor *actor;
    volatile s32 zero;
    s32 i;

    if (gGameState.entrance == 0) {
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou2) {
            gGameState.entrance = 10;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou3) {
            gGameState.entrance = 20;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou4) {
            gGameState.entrance = 30;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou5) {
            gGameState.entrance = 40;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou6) {
            gGameState.entrance = 50;
        }
    }
    Engine_GameFlagSet(0x200);
    Call1(Engine_GameFlagClear, 0x201);
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou1) {
        if (gGameState.entrance == 1) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)((u32)gSceneState + 4) = 0;
            }
            Engine_GameFlagSet(0x201);
        }
        if (gGameState.entrance == 2) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)((u32)gSceneState + 4) = 5;
            }
            Engine_GameFlagSet(0x201);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou2) {
        if (Engine_GameFlagIsSet(0x962)) {
            Engine_ActorSetPosition(8, 0, 0);
        } else {
            actor = Object_GetById(8);
            ((s8 *)actor->sprite)[9] = (((s8 *)actor->sprite)[9] & ~0xc) | 4;
            ((u8 *)actor->sprite)[38] = 2;
            actor->sprite->rotation = 0x4000;
        }
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou4) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        if (Engine_GameFlagIsSet(0x211)) {
            Engine_ActorSetAnimation(11, 5);
            Call6(Engine_MapCopyCellAttributes, 76, 16, 1, 1, 73, 17);
        } else {
            Object_GetById(11)->priority_flags |= 2;
        }
        Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
        if (Engine_GameFlagIsSet(0x212)) {
            Call6(Engine_MapCopyCellAttributes, 30, 20, 1, 1, 32, 20);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou6) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        /* FAKEMATCH: the three callbacks are stored as s32, so the entrance read
         * after them may alias them and stays below the last store. */
        *(s32 *)&Object_GetById(8)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        *(s32 *)&Object_GetById(9)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        *(s32 *)&Object_GetById(10)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        if (gGameState.entrance == 52) {
            zero = 0;
            Dma_Set(&zero, ((void *)ArutamiraDou_ClearTarget), 0x85000003, (volatile u32 *)0x040000d4);
            if (!Engine_GameFlagIsSet(0x109)) {
                u8 *race = gSceneState;

                race[0] = 0;
                race[1] = 0;
                race[2] = 4;
            }
        }
        state = (s8 *)&gSceneState[1];
        if (state[0] == 99) {
            Call6((void (*)())Engine_MapCopyCellsTo, 41, 55, 3, 2, 30, 55);
            Call6(Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
        }
        if (state[0] == 2) {
            SceneActor_PlaceFiveActorsInRow((state[1] << 16) / 5 + 0x4000);
        }
        for (i = 0; i < 5; i++) {
            actor = Object_GetById(i + 11);
            actor->motion_flags = 0;
            actor->collision_flags = 0;
            actor->scale_x = 0x10000;
            actor->scale_y = 0x10000;
            Engine_ActorSetSpriteFlags(Object_GetById(i + 11), 0);
            Engine_ActorSetAnimation(i + 11, i + 1);
        }
        Engine_ActorSetChildValue(11, 1);
        Engine_ActorSetChildValue(12, 4);
        Engine_ActorSetChildValue(13, 11);
        Engine_ActorSetChildValue(14, 2);
        Engine_ActorSetChildValue(15, 3);
        Engine_ActorSetChildValue(16, 6);
        Engine_ActorSetChildValue(17, 6);
        Engine_ActorSetChildValue(18, 6);
        Engine_ActorSetChildValue(19, 6);
        Engine_ActorSetChildValue(20, 6);
        Object_GetById(16)->sprite->priority = 3;
        Object_GetById(20)->sprite->priority = 3;
        Object_GetById(16)->priority_flags = 2;
        Object_GetById(20)->priority_flags = 2;
        Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
    }
    if (Engine_GameFlagIsSet(0x200)) {
        SceneEffect_SetupBlendByFlag201();
    } else {
        gEventWork->start_transition = 0x204;
        gEventWork->transition_frames = 24;
    }
    return 0;
}
