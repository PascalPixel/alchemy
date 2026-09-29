#include "ARUTIN.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_RunMiddleSequence(void);
void FieldScene_RunScene3a3SequenceD(void);

/*
 * Altin's scene start: mirror three progress flags into three scene flags,
 * then run the continuation of the area the party enters. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 Scene_Initialize(void)
{
    s16 scene;

    if (GameFlag_IsSet(0x8fd) != 0) {
        GameFlag_Set(0x240);
    }

    if (GameFlag_IsSet(0x8fe) != 0 || GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x241);
    }

    if (GameFlag_IsSet(0x8fe) != 0 && GameFlag_IsSet(0x907) != 0) {
        GameFlag_Set(0x242);
    }

    scene = gGameState.scene;
    if (scene == (s32)&SceneId_ArutinMura1) {
        FieldScene_RunMiddleSequence();
    } else if (scene == (s32)&SceneId_ArutinMura2) {
        FieldScene_RunScene3a3SequenceD();
    }

    return 0;
}
