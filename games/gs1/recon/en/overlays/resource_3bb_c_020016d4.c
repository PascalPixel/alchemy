#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunLiftedActorCoordinator:
 * A phase-two fast path, full and revisit branches, the lifted actor-state
 * transfer, and all 40 calls across the complete scene coordinator. */

#define FieldScene_RunLiftedActorCoordinator Func_020016d4

void Func_020034de();
s32 Func_020036f4();
void Func_02003918();
void Func_02003992();
void Func_02004310();
void Func_020045d4();
void Func_02004902();
void Func_02005628();
void Func_02005654();
void Func_0200565e();
void Func_020056d6();
void Func_02005702();
void *Func_0200571c();
void Func_0200571e();
void Func_0200572a();
void Func_0200573a();
void Func_02005744();
void Func_02005750();
void Func_02005752();
void Func_0200575e();
void Func_0200576a();
void Func_0200576c();
void Func_0200576e();
void Func_0200577a();
void Func_0200577e();
void Func_020057b6();
void Func_020057c2();
void Func_020057d0();
void Func_020057e0();
void Func_020057ea();
void Func_02005816();
void Func_02005818();
void Func_0200582a();
void Func_0200583c();
void Func_0200583e();
void Func_02005854();
void Func_02005862();
void Func_02005868();

extern s16 Data_02000240[];

#define SceneTransition_Phase Data_02000240[225]

void Func_020016d4(s32 scene)
{
    void *actor;
    s32 path;

    if (SceneTransition_Phase == 2) {
        Func_020034de();
        return;
    }
    Func_02005628();
    path = Func_020036f4(scene, 4);
    if (path == 0) {
        Func_020056d6(8362);
        Func_0200572a(196608, 24576);
        Func_02005744(35127296, -1, 15728640, 1);
        Func_02005750();
        Func_0200565e(45);
        Func_02005752(65536, 8192);
        Func_0200576c(35127296, -1, 12582912, 1);
        Func_0200577a();
        Func_0200573a(scene, 0);
        Func_02004310(0, 632, 264);
        Func_020056d6(0, 65536, 32768);
        Func_02005702(0, 616, 264);
        Func_0200577e(0, 49152, 20);
        Func_020057ea();
        Func_020057b6(16384, 2048);
        Func_020057d0(35127296, -1, 10485760, 1);
        Func_0200571e(0, 32768, 16384);
        Func_0200575e(0, 10);
        actor = Func_0200571c(0);
        Func_02005654(actor, *(s32 *)((u8 *)actor + 8),
            *(s32 *)((u8 *)actor + 12) + 4194304,
            *(s32 *)((u8 *)actor + 16));
        Func_0200576a(0);
        Func_0200583e();
        Func_02005818(-1, -1, -1, 0);
        Func_020057e0(scene, 0);
        Func_0200576e(0, 98304, 49152);
        Func_02004902(0, 488, 248);
        Func_02005816(0, 16384, 20);
        Func_020057e0(0, 6, 0);
        Func_02005862(35127296, -1, 10485760, 1);
        Func_0200582a(scene, 0);
        Func_020045d4(0);
        Func_02005868(0, 0);
        Func_02003918(scene, 4);
    } else if (path == 1) {
        Func_0200583c(8361);
        Func_02005854(scene, 0);
    }
    Func_02003992(path, scene, 4);
    Func_020057c2();
}
