#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

void InitializeEscapeSceneActors(void);
void FieldScene_RunScene3b2_0200167c(void);
s32 FieldScene_RedrawActorFootprint(s32 id);
void InitializeSwayingSceneObject();
void FieldScene_RunScene3b2SequenceA(void);
void FieldScene_RunScene3b2_02001494(void);
void SetMapCellCollision();

/* The island's scene start. The screen opens through a window. The escape
   scene places its actors; the fifth scene clears two rocks until flag
   0xef7 is set and opens its passage from entrance 5 or once flag 0x8d1
   is set; the first scene hides the four statues whose flags 0x240 to
   0x243 are set and sways the tree until flag 0xfd7; the sixth to the
   fourteenth run the shared column sequence. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;
    s32 scene;
    s32 current;
    s32 first;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_TakaraShima3) {
        InitializeEscapeSceneActors();
        return 0;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        if (GameFlag_IsSet(0xef7) == 0) {
            Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
            Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
            MapObject_SetPosition(101, 0xd80000, 0x2880000);
        }
        if (gGameState.scene == scene) {
            if (gGameState.entrance != 5) {
                if (GameFlag_IsSet(0x8d1) == 0) {
                    return 0;
                }
            }
            GameFlag_Set(0x8d1);
            Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
            MapObject_SetPosition(100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    current = gGameState.scene;
    if (current == (s32)&SceneId_TakaraShima1) {
        FieldScene_RunScene3b2_0200167c();
        *(s32 *)((u8 *)Actor_Get(8) + 56) = 0x810000;
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        if (GameFlag_IsSet(0x240) != 0) {
            actor = Actor_Get(11);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetAnimation(actor, 4);
                Actor_SetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x1300000, 0x1700000, 253);
        }
        if (GameFlag_IsSet(0x241) != 0) {
            actor = Actor_Get(12);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetAnimation(actor, 4);
                Actor_SetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x500000, 0x1700000, 253);
        }
        if (GameFlag_IsSet(0x242) != 0) {
            actor = Actor_Get(13);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetAnimation(actor, 4);
                Actor_SetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x600000, 0x1500000, 253);
        }
        if (GameFlag_IsSet(0x243) != 0) {
            actor = Actor_Get(14);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetAnimation(actor, 4);
                Actor_SetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x900000, 0x1400000, 253);
            Call4(SetMapCellCollision, 0, 0x2f00000, 0x1400000, 253);
        }
        if (GameFlag_IsSet(0xfd7) != 0) {
            return 0;
        }
        InitializeSwayingSceneObject(8);
        return 0;
    }
    first = (s32)&SceneId_TakaraShima6;
    if (current == first) {
        if (GameFlag_IsSet(0xef4) == 0) {
            Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
            MapObject_SetPosition(100, 0x2580000, 0xa80000);
        }
    }
    current = gGameState.scene;
    if (current >= first) {
        if (current <= (s32)&SceneId_TakaraShima14) {
            FieldScene_RunScene3b2SequenceA();
            if (gGameState.entrance == 5) {
                FieldScene_RunScene3b2_02001494();
            }
        }
    }
    return 0;
}
