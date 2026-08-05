#include "types.h"




/* Play one of thirteen actor-8 pose sequences, then hold for twelve frames. */
extern void Func_02002f8e(s32 actor, s32 mode);
extern void Func_02002e94(s32 frames);
extern void Func_02002f9c(s32 actor, s32 mode);
extern void Func_02002fa6(s32 actor, s32 mode);
extern void Func_02002eac(s32 frames);
extern void Func_02002fb4(s32 actor, s32 mode);
extern void Func_02002fbe(s32 actor, s32 mode);
extern void Func_02002ec4(s32 frames);
extern void Func_02002fcc(s32 actor, s32 mode);
extern void Func_02002fd6(s32 actor, s32 mode);
extern void Func_02002edc(s32 frames);
extern void Func_02002fe4(s32 actor, s32 mode);
extern void Func_02002eea(s32 frames);
extern void Func_02002ff2(s32 actor, s32 mode);
extern void Func_02002ef8(s32 frames);
extern void Func_02003000(s32 actor, s32 mode);
extern void Func_02002f06(s32 frames);
extern void Func_0200300e(s32 actor, s32 mode);
extern void Func_02003018(s32 actor, s32 mode);
extern void Func_02002f1e(s32 frames);
extern void Func_02003026(s32 actor, s32 mode);
extern void Func_02003030(s32 actor, s32 mode);
extern void Func_02002f36(s32 frames);
extern void Func_0200303e(s32 actor, s32 mode);
extern void Func_02003048(s32 actor, s32 mode);
extern void Func_02002f4e(s32 frames);
extern void Func_02003056(s32 actor, s32 mode);
extern void Func_02003060(s32 actor, s32 mode);
extern void Func_02002f66(s32 frames);
extern void Func_0200306e(s32 actor, s32 mode);
extern void Func_02003078(s32 actor, s32 mode);
extern void Func_02002f7e(s32 frames);
extern void Func_02003086(s32 actor, s32 mode);
extern void Func_02002f8c(s32 frames);
extern void Func_02003094(s32 actor, s32 mode);
extern void Func_02002f9a(s32 frames);
extern void Func_020030a2(s32 actor, s32 mode);
extern void Func_02002fa8(s32 frames);
extern void Func_020030b0(s32 actor, s32 mode);
extern void Func_020030ba(s32 actor, s32 mode);
extern void Func_02002fc0(s32 frames);
void Func_02001424(s32 sequence)
{
    if (sequence == 0) {
        Func_02002f8e(8, 1);
        Func_02002e94(6);
        Func_02002f9c(8, 3);
        goto done;
    }
    if (sequence == 2) {
        Func_02002fa6(8, 1);
        Func_02002eac(6);
        Func_02002fb4(8, 5);
        goto done;
    }
    if (sequence == 3) {
        Func_02002fbe(8, 1);
        Func_02002ec4(6);
        Func_02002fcc(8, 4);
        goto done;
    }
    if (sequence == 4) {
        Func_02002fd6(8, 1);
        Func_02002edc(6);
        Func_02002fe4(8, 3);
        Func_02002eea(6);
        Func_02002ff2(8, 1);
        Func_02002ef8(6);
        Func_02003000(8, 3);
        Func_02002f06(6);
    }
    if (sequence == 1 || sequence == 4) {
        Func_0200300e(8, 1);
        goto done;
    }
    if (sequence == 5) {
        Func_02003018(8, 1);
        Func_02002f1e(6);
        goto done;
        Func_02003026(8, 2);
    }
    if (sequence == 7) {
        Func_02003030(8, 6);
        Func_02002f36(6);
        Func_0200303e(8, 8);
        goto done;
    }
    if (sequence == 9) {
        Func_02003048(8, 6);
        Func_02002f4e(6);
        Func_02003056(8, 9);
        goto done;
    }
    if (sequence == 10) {
        Func_02003060(8, 6);
        Func_02002f66(6);
        Func_0200306e(8, 10);
        goto done;
    }
    if (sequence == 11) {
        Func_02003078(8, 6);
        Func_02002f7e(6);
        Func_02003086(8, 8);
        Func_02002f8c(6);
        Func_02003094(8, 6);
        Func_02002f9a(6);
        Func_020030a2(8, 8);
        Func_02002fa8(6);
    }
    if (sequence == 8 || sequence == 11) {
        Func_020030b0(8, 6);
        goto done;
    }
    if (sequence == 12)
        Func_020030ba(8, 6);

    Func_02002fc0(12);
done:
}
