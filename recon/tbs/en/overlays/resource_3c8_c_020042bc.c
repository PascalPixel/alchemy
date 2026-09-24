/* NONMATCHING: 592 of 612 bytes, 229 halfword edits (2026-09-24). Same shape
 * as the matched rubble event F_047C0.C, run twice. Remaining: the reference
 * tests row <= 7 before it sets up each spawn loop (z and the zero after the
 * test) and keeps row in r8; ours sets z first and allocates row to r7, 20
 * bytes short. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

void Main_0808a5e0(s32 cue);

void Local_020042bc(void)
{
    struct EffectOptions options;
    u32 repeat;
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
    repeat = 0;
    row = 0;
    opts = &options;
    offset = 0;
    do {
        options.start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    again:
        z = 0x300000;
        for (i = 0, zero = 0, z += offset; i <= 3 && row <= 7; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3600000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        if (row == 3 && repeat <= 2) {
            repeat++;
            goto again;
        }
        Map_CopyCellsTo(48, row + 3, 54, row + 3, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 9);
    Engine_MapCopyCellsTo(111, 5, 117, 5, 5, 2);
    Engine_MapCopyCellsTo(111, 10, 117, 10, 5, 2);
    Engine_MapCopyCellsTo(111, 7, 111, 5, 5, 2);
    Engine_MapCopyCellsTo(111, 7, 111, 10, 5, 2);
    row = 0;
    offset = 0;
    do {
        opts->start_scale_x = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        opts->start_scale_y = ((u32)(Engine_RandomNext() << 1) >> 16) * 0x4ccc + 0x17ffc;
        opts->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        z = 0x300000;
        for (i = 0, zero = 0, z += offset; i <= 3 && row <= 7; i++) {
            Effect_Spawn((((u32)(Engine_RandomNext() * 7) >> 16) << 19) + 0x3000000, 0, z, 0, zero, zero, 0x880000, opts);
            z += 0x40000;
        }
        Engine_TaskWait(3);
        Map_CopyCellsTo(55, row + 26, 48, row + 3, 3, 1);
        offset += 0x100000;
        row++;
    } while (row <= 9);
    Engine_AudioPlayCue(289);
    Engine_EventWait(60);
    Engine_EventRequestExit(21);
    Engine_EventEnd();
}
