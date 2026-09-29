#include "STATUS.H"

void FieldScene_DispatchBySelector(void);
void SceneState_ApplyRectsByFlatla384And962(void);

/* The Colosso entrance's scene start: the first and third scenes run their
   own setup. */
s32 Scene_Initialize(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        FieldScene_DispatchBySelector();
    } else if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        SceneState_ApplyRectsByFlatla384And962();
    }
    return 0;
}
