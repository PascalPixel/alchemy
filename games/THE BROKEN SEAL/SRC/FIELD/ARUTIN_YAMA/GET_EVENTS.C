#include "YAMA.H"

extern const struct SceneEvent gArutinYamaEventsOther[];
extern const struct SceneEvent gArutinYamaEvents1[];
extern const struct SceneEvent gArutinYamaEvents2[];
extern const struct SceneEvent gArutinYamaEvents3[];
extern const struct SceneEvent gArutinYamaEvents4[];
extern const struct SceneEvent gArutinYamaEvents5[];
extern const struct SceneEvent gArutinYamaEvents6[];
extern const struct SceneEvent gArutinYamaEvents7[];
extern const struct SceneEvent gArutinYamaEvents8[];
extern const struct SceneEvent gArutinYamaEvents9[];
extern const struct SceneEvent gArutinYamaEvents10[];
extern const struct SceneEvent gArutinYamaEvents11[];

/* What each of Altin Peak's areas answers; the first area answers
   differently once flag 0x8fd is set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    const struct SceneEvent *table;

    if (gGameState.scene == (s32)&SceneId_ArutinYama1) {
        if (Value1(Engine_GameFlagIsSet, 0x8fd) != 0) {
            table = (const struct SceneEvent *)ArutinYama_OpenedAreaScript;
        } else {
            table = gArutinYamaEvents1;
        }
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama2) {
        table = gArutinYamaEvents2;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama3) {
        table = gArutinYamaEvents3;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama4) {
        table = gArutinYamaEvents4;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama5) {
        table = gArutinYamaEvents5;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama6) {
        table = gArutinYamaEvents6;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama7) {
        table = gArutinYamaEvents7;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama8) {
        table = gArutinYamaEvents8;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama9) {
        table = gArutinYamaEvents9;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama10) {
        table = gArutinYamaEvents10;
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama11) {
        table = gArutinYamaEvents11;
    } else {
        table = gArutinYamaEventsOther;
    }
    return table;
}
