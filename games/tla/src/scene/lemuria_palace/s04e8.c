#include "types.h"

extern s32 gData_02003020;
extern s32 gData_02003094;

void Task_RemoveFar(void (*callback)(void));
void Resource_ResetEntry(s32 entry);
void Func_020002b4(void);

void Func_020004e8(void) {
  Task_RemoveFar(Func_020002b4);
  Resource_ResetEntry(gData_02003020);
  Resource_ResetEntry(gData_02003094);
}
