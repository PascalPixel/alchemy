#include "types.h"

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actorId, s32 width, s32 height);
extern void Func_0808a0d0(s32 actorId, s32 x, s32 y);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a100(s32 actorId, s32 direction);
extern void Func_0808a170(s32 messageId);
extern void Func_0808a180(s32 actorId, s32 mode);
extern void Func_0808a1b8(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a1e8(s32 actorId, s32 animationId, s32 frames);
extern void Func_0808a200(s32 mode, s32 value);
extern void Func_0808a208(s32 left, s32 right);
extern void Func_0808a210(s32 value, s32 direction, s32 distance, s32 enabled);
extern void Func_0808a218(void);
extern void Func_02001b5c(s32 sceneId);
extern s32 Func_02001d64(s32 sceneId, s32 routeCount);
extern void Func_02001e20(s32 sceneId, s32 routeCount);
extern void Func_02001e7c(s32 route, s32 sceneId, s32 routeCount);
extern void Func_02002910(s32 actorId, s32 x, s32 y);
extern void Func_02002aec(s32 actorId);
extern void Func_02002bac(s32 actorId);
extern void Func_02002e50(s32 actorId, s32 x, s32 y);
extern void Func_020038f8(void);

/* Run the chosen version of this map entrance. Route zero performs the full
 * camera-and-actor presentation, route one acknowledges the shorter revisit,
 * and phase two delegates to the already-active scene handler. */
void Func_02001214(s32 sceneId)
{
    s32 route;
    volatile s16 *const scenePhase = (volatile s16 *)0x02000402;

    if (*scenePhase == 2) {
        return;
        Func_02001b5c(sceneId);
    }

    Func_0808a018();
    route = Func_02001d64(sceneId, 1);

    if (route == 0) {
        Func_0808a170(0x208c);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x1480000, -1, 0xa80000, 1);
        Func_0808a218();
        Func_0808a180(sceneId, 0);

        Func_02002910(0, 280, 200);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 360, 200);
        Func_0808a010(30);
        Func_0808a1e8(0, 0x102, 60);
        Func_0808a180(sceneId, 0);
        Func_0808a0d0(0, 312, 200);
        Func_0808a010(30);
        Func_0808a1b8(0, 0xc000, 10);
        Func_0808a1e8(0, 0x106, 60);

        Func_0808a090(0, 0x18000, 0xc000);
        Func_02002e50(0, 296, 184);
        Func_02002e50(0, 296, 152);
        Func_02002e50(0, 312, 152);
        Func_0808a1b8(0, 0x4000, 15);
        Func_020038f8();
        Func_02002bac(0);
        Func_020038f8();
        Func_02002bac(0);

        Func_0808a090(0, 0x18000, 0xc000);
        Func_0808a0d0(0, 304, 184);
        Func_0808a0d0(0, 296, 192);
        Func_0808a0d0(0, 296, 200);
        Func_0808a1b8(0, 0, 15);
        Func_020038f8();
        Func_02002bac(0);
        Func_020038f8();
        Func_02002bac(0);

        Func_0808a100(0, 1);
        Func_0808a180(sceneId, 0);
        Func_02002aec(0);
        Func_0808a200(0, 0);
        Func_0808a0f0(9, 0x1380000, 0xa80000);
        Func_02001e20(sceneId, 1);
    } else if (route == 1) {
        Func_0808a170(0x208b);
        Func_0808a180(sceneId, 0);
    }

    Func_02001e7c(route, sceneId, 1);
    Func_0808a020();
}
