#include "unit/runtime/savedata.h"
#include "types.h"
#include "save_state.h"

// セーブ領域の署名
const u8 Save_Signature[] = "CAMELOT";
const u8 Save_HeaderTemplate[8] = "CAMELOTT";

const u32 Link_TestWord = 0;

// 通信テストの送信データ
const u8 Link_TestDataA[] = "Trans Data A: This is a pen.";
const u8 Link_TestDataB[] = "Trans Data B: Hello world.";
