#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "mem.h"
#include "iwram_call.h"

extern u8 MsgNoBackupMemory;
extern u8 MsgSaveFailed;

extern u8 gData_02000000[];
extern u8 gData_02000504[];
extern s16 gData_020036d0;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);
s32 Save_ReadPart(s32 slot, void *buffer);
s32 SaveProgress_Open(s32 arg0);
void SaveProgress_Close(s32 arg0);
s32 Func_08042f10(s32 slot, void *buffer);
s32 Func_08043cd8(s32 slot, s32 mode);
void FieldMessage_Show(s32 message, s32 position);

s32 Save_WriteFile(s32 slot) {
  s32 result = 0;

  if (Func_0801596c() != 0) {
    FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
    result = -9;
  } else if (Func_08042f10(slot, gData_02000000) != 0) {
    FieldMessage_Show((s32)&MsgSaveFailed, 1);
    result = -3;
  }
  Save_ReleaseWork();
  return result;
}

s32 Func_08043534(void) {
  void *buffer;
  s32 value;
  s32 result;
  s32 found;
  s32 token;

  result = 0;
  buffer = Runtime_BumpAllocateAlternatePool(0x3000);
  value = gData_020036d0;
  if (value != -1) {
    found = Func_0801596c();
    if (found != 0) {
      FieldMessage_Show((s32)&MsgNoBackupMemory, 1);
      result = -9;
    } else {
      u8 *dst;

      found = Save_ReadPart(gData_020036d0, buffer);
      if (found != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, 1);
        result = -2;
      }
      dst = (u8 *)buffer + (s32)gData_02000504;
      dst -= (s32)gData_02000000;
      Iwram_CopyWords(dst, gData_02000504, 16);
      token = SaveProgress_Open(0);
      found = Func_08042f10(gData_020036d0, buffer);
      SaveProgress_Close(token);
      if (found != 0) {
        FieldMessage_Show((s32)&MsgSaveFailed, 1);
        result = -3;
      }
    }
    Save_ReleaseWork();
    MemFree(buffer);
    value = result;
  }
  return value;
}
