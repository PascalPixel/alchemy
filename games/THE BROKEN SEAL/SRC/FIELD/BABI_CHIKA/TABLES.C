/* Scene tables. */
#include "BABI.H"

u8 *SceneData_GetTableB85c(void) { return Data_0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_0808b868(tbl);
    return tbl;
}
