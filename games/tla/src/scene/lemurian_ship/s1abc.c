#include "lemurian_ship.h"

void Func_02001c88(void);
void Func_020027e4(void);
void Func_02001e0c(void);
void Func_0200241c(void);
void Func_02002498(void);
void Func_02002820(void);
void Func_02002970(void);
void Func_02002b98(void);

s32 Func_02001abc(void)
{
    ((struct LemurianShipEventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x209;
    if (GameFlag_Test(0x9b4))
        GameFlag_SetBit(0x144);
    if (gPartyState.scene == (s32)&SceneId_Ov656Room1) {
        switch (gPartyState.entrance) {
        case 3:
        case 4:
        case 5:
            Func_02001c88();
            break;
        case 6:
        case 7:
        case 8:
            Func_020027e4();
            break;
        case 25:
        case 26:
        case 27:
        case 28:
            Func_02001e0c();
            break;
        case 15:
        case 16:
        case 17:
        case 18:
            Func_0200241c();
            break;
        case 29:
        case 30:
            Func_02002498();
            break;
        }
    } else if (gPartyState.scene == (s32)&SceneId_Ov656Room2) {
        switch (gPartyState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 20:
            Func_02002820();
            break;
        case 6:
        case 10:
            Func_02002970();
            break;
        case 9:
            Func_02002b98();
            break;
        }
    }
    return 0;
}
