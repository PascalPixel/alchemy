#include "unit/runtime/savedata.h"
#include "types.h"
#include "save_state.h"

// セーブ領域の署名
const u8 Save_Signature[] = "CAMELOT";
const u8 Save_HeaderTemplate[8] = "CAMELOTT";
