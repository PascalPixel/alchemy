#include "types.h"
#include "adventure.h"

extern const u8 MsgFixingThisWill[];
extern const u8 MsgFortuneTeller2[];
extern const u8 MsgConfirmGraphic2[];
extern const u8 MsgPiers2[];
void Func_020006e4(void);
void Func_020006ec(void);
void Func_0200072c(s32 value);
void Func_02000714(s32 message);
void Func_0200071c(s32 index, s32 flags);

void Func_02000098(s32 index) {
  Func_020006e4();
  Func_0200072c(0);
  if (gAdventure.entrance == 99)
    Func_02000714(index + (s32)MsgFixingThisWill);
  else if (gAdventure.entrance == 98)
    Func_02000714(index + (s32)MsgFortuneTeller2);
  else if (gAdventure.entrance == 97)
    Func_02000714(index + (s32)MsgConfirmGraphic2);
  else
    Func_02000714(index + (s32)MsgPiers2);
  Func_0200071c(index, 0);
  Func_020006ec();
}
