#include "types.h"

/*
 * resource_39c lead-actor presentation at 0x02001340
 * (200-byte whole-owner span, 14 static call sites).
 *
 * The 190-byte body ends at 0x020013fe.  Alignment and the two referenced
 * runtime/script pool words extend the owner to 0x02001408.  A runtime mode
 * selects one of three direct marker positions; other modes use the two-step
 * fallback and a 30-frame pause.  The actor's presentation flag is cleared
 * while the marker is active and restored before the closing script call.
 */

typedef struct LeadActor_02001340 {
    u8 reserved00[90];
    u8 presentationFlags;
} LeadActor_02001340;

typedef struct Runtime_02001340 {
    u8 reserved00[364];
    s16 presentationMode;
} Runtime_02001340;

extern void Func_0808a018(void);
extern void Func_080000d0(const void *script, s32 duration);
extern void Func_0808a090(s32 index, s32 x, s32 z);
extern void Func_0808a100(s32 index, s32 mode);
extern LeadActor_02001340 *Func_0808a080(s32 index);
extern void Func_080f9010(s32 sound);
extern void Func_0808a0b8(s32 index, s32 x, s32 z);
extern void Func_0808a0c0(s32 index, s32 x, s32 z);
extern void Func_0808a010(s32 frames);
extern void Func_0808a0e8(s32 index);
extern void Func_080000d8(const void *script);
extern void Func_0808a020(void);

void Func_02001340(void)
{
    Runtime_02001340 *runtime = *(Runtime_02001340 **)0x03001ebc;
    LeadActor_02001340 *actor;
    s32 markerX = 0;
    s32 markerZ = 0;
    s32 directMarker = 1;

    Func_0808a018();
    Func_080000d0((const void *)0x0200ba65, 3200);
    Func_0808a090(0, 0x28000, 0x14000);
    Func_0808a100(0, 1);

    actor = Func_0808a080(0);
    actor->presentationFlags &= 0xfe;
    Func_080f9010(0xe4);

    if (runtime->presentationMode == 2) {
        markerX = 232;
        markerZ = 616;
    } else if (runtime->presentationMode == 3) {
        markerX = 360;
        markerZ = 728;
        markerZ = 792;
        markerX = 248;
    } else if (runtime->presentationMode == 4) {
    } else {
        directMarker = 0;
    }

    if (directMarker != 0) {
        Func_0808a0b8(0, markerX, markerZ);
    } else {
        Func_0808a0c0(0, 696, 592);
        Func_0808a0b8(0, 696, 600);
        Func_0808a010(30);
    }

    Func_0808a0e8(0);
    actor = Func_0808a080(0);
    actor->presentationFlags |= 1;
    Func_080000d8((const void *)0x0200ba65);
    Func_0808a020();
}
