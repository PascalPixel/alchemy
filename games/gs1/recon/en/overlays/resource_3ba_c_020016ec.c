#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

#define FieldScene_RunSceneFourCoordinator Func_020016ec

void Func_02001b18();
void Func_0200325e();
s32 Func_02003474();
void Func_02003680();
void Func_020036fa();
void Func_02004080();
void Func_0200432e();
void Func_0200447a();
void Func_02004546();
void Func_02004606();
void Func_02004610();
void Func_0200461c();
void Func_020053a8();
void Func_020053fc();
void Func_0200544e();
void Func_0200546a();
void Func_02005474();
void Func_02005488();
void Func_02005494();
void Func_0200549a();
void Func_020054b4();
void Func_020054c0();
void Func_020054c2();
void Func_020054ca();
void Func_020054d2();
void Func_020054de();
void Func_020054f6();
void Func_020054fa();
void Func_020054fe();
void Func_0200552a();
void Func_02005550();
void Func_02005566();
void Func_02005578();
void Func_0200557c();
void Func_02005594();
void Func_0200559c();
void Func_020055b2();
void Func_020055b4();

extern s16 Data_02000240[];

#define SceneTransition_Phase Data_02000240[225]

void Func_020016ec(s32 scene)
{
    s32 path;

    if (SceneTransition_Phase == 2) {
        Func_0200325e();
        return;
    }
    Func_020053a8();
    path = Func_02003474(scene, 4);
    if (path == 0) {
        Func_0200544e(8345);
        Func_0200549a(196608, 24576);
        Func_020054b4(71303168, -1, 11010048, 1);
        Func_020054c0();
        Func_02005488(scene, 0);
        Func_0200447a(120, 72, 0);
        Func_0200549a(scene, 0);
        Func_02004546();
        Func_020053fc(15);
        Func_02004080(0, 984, 200);
        Func_020054ca(0, 0, 10);
        Func_020054c2(scene, 0);
        Func_020054de(0, 16384, 30);
        Func_020054fa(0, 262, 60);
        Func_02005474(0, 98304, 49152);
        Func_02004606(0, 1000, 192);
        Func_02004610(0, 1000, 176);
        Func_0200461c(0, 1016, 168);
        Func_0200546a(15);
        Func_02001b18(18, 160, 0);
        Func_02005566(71303168, -1, 11010048, 1);
        Func_020054f6(0, 1);
        Func_02005494(10);
        Func_020054d2(0, 65536, 32768);
        Func_020054fe(0, 1192, 168);
        Func_020054b4(10);
        Func_02005578(0, 32768, 30);
        Func_02005594(0, 258, 60);
        Func_0200557c(scene, 0);
        Func_0200432e(0);
        Func_020055b2(0, 0);
        Func_02005550(18, 66584576, 11010048);
        Func_02003680(scene, 4);
    } else if (path == 1) {
        Func_0200559c(8344);
        Func_020055b4(scene, 0);
    }
    Func_020036fa(path, scene, 4);
    Func_0200552a();
}
