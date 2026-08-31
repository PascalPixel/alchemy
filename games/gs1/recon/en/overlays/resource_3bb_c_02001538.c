#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSceneThreeCoordinator:
 * A phase-two fast path, full and revisit branches, and all 37 calls across
 * the complete scene-three coordinator. */

#define FieldScene_RunSceneThreeCoordinator Func_02001538

void Func_02001930();
void Func_02003346();
s32 Func_0200355c();
void Func_02003750();
void Func_020037cc();
void Func_0200417a();
void Func_020043fe();
void Func_020046d8();
void Func_020046e2();
void Func_020046f6();
void Func_02005490();
void Func_020054c6();
void Func_02005508();
void Func_0200553e();
void Func_02005544();
void Func_02005546();
void Func_02005570();
void Func_02005590();
void Func_02005592();
void Func_020055a8();
void Func_020055ac();
void Func_020055ae();
void Func_020055b8();
void Func_020055ba();
void Func_020055d2();
void Func_020055d8();
void Func_020055da();
void Func_020055e2();
void Func_020055e6();
void Func_020055fc();
void Func_02005620();
void Func_02005652();
void Func_02005654();
void Func_0200565c();
void Func_02005676();
void Func_0200568e();
void Func_02005692();

extern s16 Data_02000240[];

#define SceneTransition_Phase Data_02000240[225]

void Func_02001538(s32 scene)
{
    s32 path;

    if (SceneTransition_Phase == 2) {
        Func_02003346();
        return;
    }
    Func_02005490();
    path = Func_0200355c(scene, 3);
    if (path == 0) {
        Func_0200553e(8358);
        Func_02005592(196608, 24576);
        Func_020055ac(49283072, -1, 12582912, 1);
        Func_020055b8();
        Func_020054c6(60);
        Func_020055ba(65536, 8192);
        Func_020055d8(49283072, -1, 14680064, 1);
        Func_020055e6();
        Func_020055a8(scene, 0);
        Func_0200417a(0, 856, 264);
        Func_02005508(10);
        Func_02005546(0, 98304, 49152);
        Func_020046d8(0, 856, 264);
        Func_020046e2(0, 856, 232);
        Func_020055e2(scene, 0);
        Func_020046f6(0, 840, 232);
        Func_02005544(10);
        Func_02001930(33, -64, 0);
        Func_02005652(49283072, -1, 14155776, 1);
        Func_020055d2(0, 1);
        Func_02005570(10);
        Func_020055ae(0, 65536, 32768);
        Func_020055da(0, 760, 232);
        Func_02005590(10);
        Func_0200565c(0, 16384, 30);
        Func_02005654(scene, 0);
        Func_020043fe(0);
        Func_02005692(0, 0);
        Func_02005620(33, 55050240, 15204352);
        Func_02003750(scene, 3);
    } else if (path == 1) {
        Func_02005676(8357);
        Func_0200568e(scene, 0);
    }
    Func_020037cc(path, scene, 3);
    Func_020055fc();
}
