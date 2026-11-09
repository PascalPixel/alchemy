#include "tundaria_tower.h"
#include "call.h"

void Func_02000f0c(s32 step) {
  struct TundariaTowerPosition position;

  switch (step) {
  case 1:
    Map_CopyCellPictureIndicesRect(18, 97, 6, 97, 3, 4);
    Map_CopyCellPictureIndicesRect(82, 34, 70, 34, 3, 2);
    Call6((void (*)())Map_CopyCellAttributeRect, 7, 36, 1, 1, 7, 35);
    GameFlag_SetBit(0x94b);
    break;
  case 2:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 1, 0xcccc);
    break;
  case 3:
    position.x = 0x780000;
    position.y = 0;
    position.z = 0x2380000;
    Func_02000708(&position, 8, 0x18000);
    break;
  }
}
