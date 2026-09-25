#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

void Main_0808a5e0(s32 cue);

#define RubbleFall_Callback ((void (*)(void))0x0200c601)

/* The ceiling gives way: rubble rains down in thirteen rows, the fourth row
 * four times, while the map fills in behind it. */
void Scene_RunEastParticleWaveSequence(void)
{
    struct EffectOptions options;
    u32 repeat;
    /* The spawn velocities, held in one register for the stack arguments. */
    s32 zero;
    u32 row;
    s32 offset;
    s32 z;
    u32 i;
    struct EffectOptions *opts;

    gEventWork->start_transition = 0x202;
    Engine_EventBegin();
    Engine_ActorSetSpriteFlags(Engine_ActorGet(0), 0);
    Engine_ActorSetChildValue(0, 15);
    Main_0808a5e0(170);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(162);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraMoveTo(0x2380000, -1, 0x1680000, 1);
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0xc00000;
        for (i = 0, zero = 0, z += offset; i <= 3; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x2200000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat <= 2) {
            repeat++;
            goto again;
        }
        Engine_TaskAddCallback(RubbleFall_Callback, 3200);
        Map_CopyCellsTo(58, row + 12, 34, row + 12, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 12);
    Map_CopyCellsTo(86, 41, 97, 14, 5, 2);
    Engine_CameraWaitForMove();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventWait(60);
    Engine_GameFlagSet(0x307);
    Engine_EventRequestExit(20);
    Engine_EventEnd();
}
