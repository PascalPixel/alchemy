#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 gVinasuHeyaEvents1[];
extern u8 gVinasuHeyaEvents2[];
extern u8 gVinasuHeyaEvents3[];
extern u8 gVinasuHeyaEvents4[];
extern u8 gVinasuHeyaEvents5[];
extern u8 gVinasuHeyaEvents6[];

/* What each room answers; other scenes take the second room's events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    extern u8 Data_02000240[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == (s32)&SceneId_VinasuHeya1) {
        return (const struct SceneEvent *)gVinasuHeyaEvents1;
    }
    if (v == (s32)&SceneId_VinasuHeya2) {
        return (const struct SceneEvent *)gVinasuHeyaEvents2;
    }
    if (v == (s32)&SceneId_VinasuHeya3) {
        return (const struct SceneEvent *)gVinasuHeyaEvents3;
    }
    if (v == (s32)&SceneId_VinasuHeya4) {
        return (const struct SceneEvent *)gVinasuHeyaEvents4;
    }
    if (v == (s32)&SceneId_VinasuHeya5) {
        return (const struct SceneEvent *)gVinasuHeyaEvents5;
    }
    if (v == (s32)&SceneId_VinasuHeya6) {
        return (const struct SceneEvent *)gVinasuHeyaEvents6;
    }
    return (const struct SceneEvent *)gVinasuHeyaEvents2;
}
