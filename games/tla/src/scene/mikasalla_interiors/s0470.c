#include "types.h"

extern u8 MsgMikasallaInteriors20[];
extern u8 MsgMikasallaInteriors21[];

s32 GameFlag_Test(s32 flag);
void Func_02000604(s32 message);
void Func_02000614(s32 id, s32 flags);
void Func_020004dc(void);

void Func_02000470(void) {
  if (GameFlag_Test(0xa39)) {
    Func_02000604(GameFlag_Test(0x340) ? (s32)MsgMikasallaInteriors20
                                       : (s32)MsgMikasallaInteriors21);
    Func_02000614(18, 0);
  } else {
    Func_020004dc();
  }
}
