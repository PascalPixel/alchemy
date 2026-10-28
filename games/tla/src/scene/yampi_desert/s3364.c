#include "scene.h"
#include "task.h"

void Func_02003364(void) {
  Func_02004180(0);
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4) {
    if (Scene_FlagTest(0x889))
      Func_02004090(16, 0, 0);
    else
      Func_02002318();
  }
  Func_02003820(gData_020047d2);
  if (Scene_FlagTest(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    Task_Add(Func_020020a8, TASK_PASS_VBLANK);
}

void Func_020026a0(void);
void Func_02002b84(void);

/// Room 5's setup: from entrances other than 1 and 2 it places actors 11 to
/// 20 and copies the map cells flags 0x88c and 0x8ed select.
void Func_02003404(void) {
  Func_02004180(0);
  Func_02003820(gData_020047ec);
  Func_02003338(11);
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  Func_02003338(16);
  Func_02003338(17);
  Func_02003338(18);
  Func_02003338(19);
  Func_02003338(20);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    if (GameFlag_Test(0x889) && !GameFlag_Test(0x88a))
      Func_020026a0();
  } else {
    Scene_Call4090(11, 200 << 16, 248 << 16);
    Scene_Call4090(12, 168 << 16, 280 << 16);
    Scene_Call4090(13, 152 << 16, 344 << 16);
    Scene_Call4090(14, 184 << 16, 360 << 16);
    Scene_Call4090(15, 328 << 16, 264 << 16);
    Scene_Call4090(16, 296 << 16, 296 << 16);
    Scene_Call4090(17, 344 << 16, 312 << 16);
    Scene_Call4090(18, 312 << 16, 360 << 16);
    Scene_Call4090(19, 280 << 16, 376 << 16);
    Scene_Call4090(20, 328 << 16, 392 << 16);
    if (GameFlag_Test(0x88c)) {
      if (GameFlag_Test(0x8ed)) {
        Func_02003ff0(68, 77, 13, 9, 3, 3);
        Scene_Call4000(68, 77, 3, 3, 13, 9);
      } else {
        Func_02003ff0(74, 71, 77, 9, 3, 3);
        Scene_Call4000(74, 71, 3, 3, 13, 9);
      }
      Scene_Call4000(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Test(0x88a)) {
      Func_02002b84();
    }
  }
}
