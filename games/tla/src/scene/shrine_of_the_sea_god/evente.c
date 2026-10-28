#include "shrine_of_the_sea_god.h"

struct ShrineOfTheSeaGodSpot {
  s32 x;
  s32 z;
  s32 field18;
  s32 rotation;
};

extern const struct ShrineOfTheSeaGodSpot gData_02003cc0[];
extern const u8 gData_02003cbc[];
extern const u8 gData_02003c10[];

void Object_SetPartAttribute(struct ShrineOfTheSeaGodObject *object, s32 value);
void Func_02003b00(s32 value);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003b30(const void *script);
void Func_02003854(s32 actor);
void Func_0200123c(void);
void Func_020010e8(void);
void Func_02001088(void);
void Func_02001128(void);
void Func_020011a0(void);
void Func_02000f18(s32 actor);
void Func_020027f8(void);
void Func_02002830(void);
void Func_0200291c(void);
void Func_0200296c(void);
void Func_020029a4(void);
void Func_020029dc(void);
void Func_02002a2c(void);
void Func_02002b18(void);
void Func_02002b50(void);
void Func_02002c18(void);
void Func_02002c50(void);
void Func_02002c88(void);

void Func_02002e70(void) {
  Object_SetModeById(8, 2);
  Object_SetModeById(9, 0);
  Func_02003aa0(9);
}

void Func_02002e8c(void) {
  Object_GetById(gAdventure.currentOwner)->layer = 2;
}

void Func_02002ea8(void) {
  Object_GetById(gAdventure.currentOwner)->layer = 0;
}
