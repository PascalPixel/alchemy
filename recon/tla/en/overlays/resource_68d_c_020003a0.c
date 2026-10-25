// Draft, differ 163 of 126: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Data_02002620[];
extern u8 gData_02003020[];
extern u8 Data_02003088[];
extern u8 Data_02003060[];
extern u8 Data_02002af8[];
extern u8 gData_02003094[];
extern u8 Data_02003058[];
extern u8 Data_02003030[];
extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003098[];
extern u8 Func_020002b4[];
extern u8 gData_0200308c[];
s32 MemAlloc();
void Func_02001f6c();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
void Func_02002104();
s32 Scheduler_AddOrUpdateCallback();
void MemFree();

s32 Func_020003a0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r5_2, r6_1, r6_2, r6_3;
  s32 v0, r9, r7, r5, v1, v2, r6, r8, r10, r11, v3, v4, v5, r0;
  s32 stack[16];

  /* sub (s32)stack, #4 */
  v0 = MemAlloc(0x1000);
  r9 = v0;
  FIELD_AT(67108944, u16, 0) = 0x3f40;
  FIELD_AT(67108946, u16, 0) = 0x1000;
  r7 = 32;
  Func_02001f6c((s32)Data_02002620, r9);
  r5 = (s32)gData_02003020;
  v1 = Resource_FindFreeEntry();
  FIELD_AT(r5, s32, 0) = v1;
  r5_1 = (s32)Data_02003088;
  v2 = VramCache_Load(v1, 0x1000, r9);
  r6 = (s32)Data_02003060;
  FIELD_AT(r5_1, s32, 0) = v2;
  Func_02002104(r6, 0, 0, -0x40000000, v2);
  FIELD_AT(r6, u16, 30) = 1;
  r5_2 = 13;
  r5_2 = -r5_2;
  r8 = 8;
  r10 = 4;
  FIELD_AT(r6, u8, 5) = ((r5_2 & (FIELD_AT(r6, u8, 5))) | r10) | r7;
  FIELD_AT(r6, u8, 9) = ((r5_2 & (FIELD_AT(r6, u8, 9))) | r8) & 15;
  r11 = 15;
  Func_02001f6c((s32)Data_02002af8, r9);
  r6_1 = (s32)gData_02003094;
  v3 = Resource_FindFreeEntry();
  FIELD_AT(r6_1, s32, 0) = v3;
  r6_2 = (s32)Data_02003058;
  v4 = VramCache_Load(v3, 0x1000, r9);
  FIELD_AT(r6_2, s32, 0) = v4;
  r6_3 = (s32)Data_02003030;
  Func_02002104(r6_3, 0, 0, -0x40000000, v4);
  FIELD_AT(r6_3, u16, 30) = 2;
  FIELD_AT(r6_3, u8, 9) = ((r5_2 & (FIELD_AT(r6_3, u8, 9))) | r8) & r11;
  r5_2 = r5_2 & (FIELD_AT(r6_3, u8, 5));
  r5_2 = r5_2 | r10;
  r5_2 = r5_2 | r7;
  FIELD_AT(r6_3, u8, 5) = r5_2;
  FIELD_AT((s32)Data_02003090, s32, 0) = 0;
  FIELD_AT((s32)Data_02003024, s32, 0) = 0;
  FIELD_AT((s32)Data_02003098, s32, 0) = 0;
  FIELD_AT((s32)gData_0200308c, u8, 0) = 0;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020002b4, 0x480);
  MemFree(r9);
  /* add (s32)stack, #4 */
  return v5;
}
