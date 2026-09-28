/* NONMATCHING: resource_3bc at 0x0200c57c..0x0200c5e0 (100 bytes with their
 * pools), the scene-control pair, between
 * FIELD/KOROSSEO_MARUTA/SCENE_DESCRIPTOR.C and PUSH.C, stay listing.
 *
 * Remaining difference: they reach the word at 0x02001000 and the resident
 * pointer at 0x03001f3c, which the main image does not name yet.
 */
#include "SITES.H"

void ColossoLogRollingStage_InitializeSceneControl(void)
{
    extern SceneControl Data_02001000;

    u8 *scene_state = Data_03001f3c;
    SceneControl *control = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(scene_state + 240));
    if (GameFlag_IsSet(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, event_id);
    }
}

void ColossoLogRollingStage_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}
