#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunMapEntranceCoordinator:
 * A phase-two fast path, full-presentation and short-revisit branches, and
 * all 46 calls across the complete map-entrance owner. */

#define FieldScene_RunMapEntranceCoordinator Func_02001214

void Func_02002d86();
s32 Func_02002f9c();
void Func_020031cc();
void Func_02003246();
void Func_02003b8c();
void Func_02003e7a();
void Func_02003ece();
void Func_02003ed8();
void Func_02003f1a();
void Func_02003f24();
void Func_02004146();
void Func_02004150();
void Func_0200415c();
void Func_02004c14();
void Func_02004c1e();
void Func_02004c60();
void Func_02004c6a();
void Func_02004ed0();
void Func_02004f34();
void Func_02004f52();
void Func_02004f5a();
void Func_02004f76();
void Func_02004f7e();
void Func_02004fa4();
void Func_02004fb0();
void Func_02004fb4();
void Func_02004fc2();
void Func_02004fdc();
void Func_02004fe8();
void Func_02004ff0();
void Func_02005002();
void Func_02005008();
void Func_0200501e();
void Func_0200502e();
void Func_02005038();
void Func_0200503a();
void Func_02005042();
void Func_02005068();
void Func_02005076();
void Func_02005080();
void Func_0200509c();
void Func_020050b4();
void Func_020050c8();
void Func_020050e8();
void Func_020050fe();
void Func_02005100();

extern s16 Data_02000240[];

#define MapTransition_Phase Data_02000240[225]

void Func_02001214(s32 scene)
{
    s32 path;

    if (MapTransition_Phase == 2) {
        Func_02002d86();
        return;
    }
    Func_02004ed0();
    path = Func_02002f9c(scene, 1);
    if (path == 0) {
        Func_02004f76(8332);
        Func_02004fc2(196608, 24576);
        Func_02004fdc(21495808, -1, 11010048, 1);
        Func_02004fe8();
        Func_02004fb0(scene, 0);
        Func_02003b8c(0, 280, 200);
        Func_02004f52(0, 65536, 32768);
        Func_02004f7e(0, 360, 200);
        Func_02004f34(30);
        Func_02005008(0, 258, 60);
        Func_02004ff0(scene, 0);
        Func_02004fa4(0, 312, 200);
        Func_02004f5a(30);
        Func_0200501e(0, 49152, 10);
        Func_0200503a(0, 262, 60);
        Func_02004fb4(0, 98304, 49152);
        Func_02004146(0, 296, 184);
        Func_02004150(0, 296, 152);
        Func_0200415c(0, 312, 152);
        Func_02005068(0, 16384, 15);
        Func_02004c14();
        Func_02003ece(0);
        Func_02004c1e();
        Func_02003ed8(0);
        Func_02005002(0, 98304, 49152);
        Func_0200502e(0, 304, 184);
        Func_02005038(0, 296, 192);
        Func_02005042(0, 296, 200);
        Func_020050b4(0, 0, 15);
        Func_02004c60();
        Func_02003f1a(0);
        Func_02004c6a();
        Func_02003f24(0);
        Func_02005080(0, 1);
        Func_020050c8(scene, 0);
        Func_02003e7a(0);
        Func_020050fe(0, 0);
        Func_0200509c(9, 20447232, 11010048);
        Func_020031cc(scene, 1);
    } else if (path == 1) {
        Func_020050e8(8331);
        Func_02005100(scene, 0);
    }
    Func_02003246(path, scene, 1);
    Func_02005076();
}
