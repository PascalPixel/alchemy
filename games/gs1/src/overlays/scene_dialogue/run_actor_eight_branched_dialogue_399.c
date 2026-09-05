#include "types.h"

void Func_0200283c();
extern s32 Func_02002822();
void Func_0200291c();
extern s32 Func_02002834();
void Func_0200292e();
void Func_02002936();
void Func_02002946();
void Func_0200287a();

void SceneDialogue_RunActorEightBranchedDialogue(void) {
    Func_0200283c();
    if (Func_02002822(0x82b) != 0) {
        Func_0200291c(0x156f);
    } else if (Func_02002834(0x82c) != 0) {
        Func_0200292e(0x153b);
    } else {
        Func_02002936(0x1533);
    }
    Func_02002946(8, 0);
    Func_0200287a();
}
