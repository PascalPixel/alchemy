// Draft of overlay 64a's Func_02000e44, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02002208();
s32 Func_02002310();
s32 Func_020022b0();
s32 Func_02002408();
s32 Heap_AllocEwram();
s32 Func_02002320();
s32 __divsi3();
s32 Window_Create();
s32 Text_DrawCharacterAtOffset();
s32 Window_DrawDividerLine();
s32 __modsi3();
s32 Func_02002330();
s32 Resource_FindFreeEntry();
s32 Func_02002300();
s32 WindowSprite_Create();
s32 Func_02002400();
s32 Func_02002308();
s32 Func_02000db4();
s32 Func_020023f8();
s32 Func_020023f0();
s32 Func_02002068();
s32 Func_0200217c();
s32 wait();
s32 Func_02002350();
s32 Party_RemoveActiveOwner();
s32 Party_AddActiveOwner();
s32 Func_020018e0();
s32 Window_Close();
s32 Func_020022f8();
s32 Func_020023e8();
s32 WindowSprite_RedrawSavedRect();
s32 Window_SetTilemapEntry();
s32 Func_02000a70();
s32 Func_02002328();
s32 Text_DrawStringInWindow();
extern u8 gMainRoutineWork[];
extern u8 Data_0200274a[];
extern u8 IwramClearWords[];
extern u8 gInput[];
extern u8 gVBlankCount[];
extern u8 gData_02002438[];
extern u8 gData_0200265e[];
extern u8 IwramCopyWords[];
extern u8 Data_02012f94[];
extern u8 gGameFlagBytes[];
extern u8 gData_02002440[];
extern u8 gData_02002454[];
extern u8 gData_02002468[];
extern u8 gData_0200247c[];
extern u8 Data_0200288a[];

void Func_02000e44(s32 p0) {
  u8 frame[-124];
  s32 s12, s16, s20, s24, s28, s32, s36, s40, s44, s48, s52, s56, s60, s64, s8, v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r4, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  Call1((void (*)())Func_02002208, p0);
  v_r2 = (s32)gMainRoutineWork + 0x225;
  v_r3 = (s32)gMainRoutineWork + 0x226;
  v_r0 = FIELD_AT(v_r2, u8, 0);
  v_r1 = FIELD_AT(v_r3, u8, 0);
  Call4((void (*)())Func_02002310, v_r0, v_r1, v_r2, v_r3);
  /* XXX stmia r3!, {r0, r1, r2} */
  FIELD_AT(0x50001c0, u16, 8) = 0x6318;
  v_r0 = Func_020022b0(1, 0x50001c0, -0x7bfffff8, 0x6318);
  /* XXX stack argument */;
  if (v_r0 != -1)
    goto L_ja_02008e60;
L_ja_02008e60:
  ;
  Call1((void (*)())Func_02002408, 71);
  /* XXX stack argument */;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = Heap_AllocEwram(0x1dc);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call1((void (*)())Func_02002320, 0x6006000);
  v_r1 = s64;
  goto L_ja_02008e9c;
L_ja_02008e9c:
  ;
  /* XXX stack argument */;
  if (v_r1 == 0)
    goto L_ja_02008eb4;
  v_r3 = s64;
  /* XXX stack argument */;
  if (v_r3 == 1)
    goto L_ja_02008eb4;
  /* XXX stack argument */;
L_ja_02008eb4:
  ;
  v_r0 = s44;
  v_r0 = __divsi3(v_r0, 50, v_r2, v_r3);
  v_r0 = v_r0 + 1;
  /* XXX stack argument */;
  v_r5 = 2;
  /* XXX stack argument */;
  v_r0 = Window_Create(1, 13, 29, 6);
  v_r6 = 0xd80;
  v_r10 = v_r0;
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 144, 0);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 144, 8);
  /* XXX stack argument */;
  v_r6 = v_r6 + 1;
  v_r0 = Window_DrawDividerLine(v_r10, 24, 0, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 8);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 24);
  /* XXX stack argument */;
  v_r0 = Window_Create(5, 1, 20, 12);
  /* XXX stack argument */;
  v_r6 = 0;
L_ja_02008fe2:
  ;
  v_r2 = s36;
  v_r1 = v_r6 + 0x1c0;
  v_r2 = v_r2 + 2;
  v_r6 = v_r6 + 1;
  FIELD_AT(v_r2 + v_r1, u8, 0) = 0;
  if (v_r6 != 26)
    goto L_ja_02008fe2;
  v_r6 = 0;
L_ja_02008ffa:
  ;
  v_r6 = v_r6 + 1;
  FIELD_AT((s32)Data_0200274a, u8, 0) = 99;
  v_r2 = (s32)Data_0200274a + 1;
  if (v_r6 != 0x104)
    goto L_ja_02008ffa;
  v_r2 = s36;
  v_r6 = 0;
  v_r2 = v_r2 + 0x190;
  v_r9 = v_r2;
  v_r11 = v_r6;
L_ja_02009018:
  ;
  v_r0 = __modsi3(v_r6, 10, v_r2, v_r3);
  v_r5 = v_r0;
  v_r3 = v_r5 << 1;
  v_r3 = v_r3 + v_r5;
  v_r7 = v_r3 << 2;
  v_r4 = 8 + v_r7;
  v_r8 = v_r4;
  v_r0 = __divsi3(v_r6, 10);
  v_r0 = v_r0 << 4;
  v_r3 = v_r0 + 2;
  if (v_r5 <= 4)
    goto L_ja_02009042;
  v_r7 = v_r7 + 18;
  v_r8 = v_r7;
L_ja_02009042:
  ;
  v_r2 = s36;
  v_r1 = FIELD_AT(v_r11 + v_r2, s32, 0);
  v_r0 = s56;
  v_r0 = Func_02002330(v_r0, v_r1, v_r8, v_r3);
  v_r3 = s36;
  v_r2 = v_r11 + 200;
  FIELD_AT(v_r2 + v_r3, s32, 0) = v_r0;
  v_r4 = v_r9;
  FIELD_AT(v_r4, u8, 0) = 99;
  v_r1 = s36;
  v_r9 = v_r9 + 1;
  v_r0 = FIELD_AT(v_r2 + v_r1, s32, 0);
  v_r6 = v_r6 + 1;
  v_r11 = v_r11 + 4;
  if (v_r6 != 50)
    goto L_ja_02009018;
  v_r0 = Resource_FindFreeEntry(v_r0, 128, 4, (s32)IwramClearWords);
  v_r5 = v_r0;
  if (v_r5 > 95)
    goto L_ja_020090b2;
  v_r0 = Func_02002300();
  /* XXX stack argument */;
  v_r0 = WindowSprite_Create(v_r5, 0x40000000, v_r10, 0);
  v_r0 = (s32)frame + 108;
  FIELD_AT(v_r0, s32, 0) = v_r0;
  v_r4 = v_r10;
  v_r1 = FIELD_AT(v_r4, s16, 12);
  v_r2 = FIELD_AT(v_r4, s16, 14);
  v_r1 = v_r1 << 3;
  v_r2 = v_r2 << 3;
  v_r1 = v_r1 - 2;
  v_r2 = v_r2 + 12;
  Call4((void (*)())Func_02002400, v_r0, v_r1, v_r2, 14);
L_ja_020090b2:
  ;
  v_r0 = Resource_FindFreeEntry();
  v_r5 = v_r0;
  if (v_r5 > 95)
    goto L_ja_020090fa;
  v_r0 = Func_02002308();
  /* XXX stack argument */;
  v_r0 = WindowSprite_Create(v_r5, 0x40000000, v_r10, 0);
  v_r0 = (s32)frame + 92;
  FIELD_AT(v_r0, s32, 0) = v_r0;
  v_r2 = FIELD_AT(v_r0, u8, 25);
  FIELD_AT(v_r0, u8, 15) = 255;
  v_r3 = -13 & v_r2;
  FIELD_AT(v_r0, u8, 25) = v_r3;
  v_r2 = s56;
  v_r1 = FIELD_AT(v_r2, s16, 12);
  v_r2 = FIELD_AT(v_r2, s16, 14);
  v_r1 = v_r1 << 3;
  v_r2 = v_r2 << 3;
  v_r1 = v_r1 - 4;
  v_r2 = v_r2 + 12;
  Call4((void (*)())Func_02002400, v_r0, v_r1, v_r2, 14);
L_ja_020090fa:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r3 = FIELD_AT(v_r10, s16, 12);
  v_r3 = v_r3 << 3;
  v_r3 = v_r3 - 8;
  /* XXX stack argument */;
  v_r3 = FIELD_AT(v_r10, s16, 14);
  /* XXX stack argument */;
  v_r3 = v_r3 << 3;
  v_r3 = v_r3 + 11;
  /* XXX stack argument */;
L_ja_0200911e:
  ;
  v_r2 = s12;
  v_r9 = -1;
  if (v_r2 != 2)
    goto L_ja_0200912e;
  /* XXX stack argument */;
L_ja_0200912e:
  ;
  /* XXX stack argument */;
  v_r1 = s32;
  v_r2 = s28;
  v_r3 = s24;
  /* XXX stack argument */;
  Call4((void (*)())Func_02000db4, v_r10, v_r1, v_r2, v_r3);
  v_r0 = s12;
  if (v_r0 != 0)
    goto L_ja_0200921c;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 16;
  if (v_r3 == 0)
    goto L_ja_02009170;
  Call1((void (*)())Func_02002408, 111);
  v_r0 = s52;
  v_r1 = s48;
  v_r0 = v_r0 + 1;
  if (v_r1 != 2)
    goto L_ja_02009168;
  goto L_ja_0200916a;
L_ja_02009168:
  ;
L_ja_0200916a:
  ;
  v_r0 = __modsi3(v_r0, v_r1);
  /* XXX stack argument */;
L_ja_02009170:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 32;
  if (v_r3 == 0)
    goto L_ja_020091ac;
  Call1((void (*)())Func_02002408, 111);
  v_r2 = s48;
  if (v_r2 != 2)
    goto L_ja_020091a0;
  v_r0 = s52;
  v_r0 = v_r0 + 19;
  goto L_ja_020091a6;
L_ja_020091a0:
  ;
  v_r0 = s52;
  v_r0 = v_r0 + 20;
L_ja_020091a6:
  ;
  v_r0 = __modsi3(v_r0, v_r1, v_r2);
  /* XXX stack argument */;
L_ja_020091ac:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 128;
  if (v_r3 == 0)
    goto L_ja_020091e4;
  Call1((void (*)())Func_02002408, 111);
  v_r1 = s48;
  v_r1 = v_r1 + 1;
  if (v_r1 >= 0)
    goto L_ja_020091cc;
  v_r3 = s48;
  v_r3 = v_r3 + 4;
L_ja_020091cc:
  ;
  v_r3 = v_r3 >> 2;
  /* XXX stack argument */;
  v_r3 = v_r3 << 2;
  v_r1 = v_r1 - v_r3;
  v_r3 = s52;
  /* XXX stack argument */;
  if (v_r3 != 20)
    goto L_ja_020091e4;
  if (v_r1 != 2)
    goto L_ja_020091e4;
  /* XXX stack argument */;
L_ja_020091e4:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 64;
  if (v_r3 == 0)
    goto L_ja_0200921c;
  Call4((void (*)())Func_02002408, 111, v_r1, 64, v_r3);
  v_r1 = s48;
  v_r1 = v_r1 + 3;
  if (v_r1 >= 0)
    goto L_ja_02009204;
  v_r3 = s48;
  v_r3 = v_r3 + 6;
L_ja_02009204:
  ;
  v_r3 = v_r3 >> 2;
  v_r0 = s52;
  /* XXX stack argument */;
  v_r3 = v_r3 << 2;
  v_r1 = v_r1 - v_r3;
  /* XXX stack argument */;
  if (v_r0 != 20)
    goto L_ja_02009226;
  if (v_r1 != 2)
    goto L_ja_0200921c;
  /* XXX stack argument */;
L_ja_0200921c:
  ;
  v_r3 = s52;
  /* XXX stack argument */;
  if (v_r3 == 20)
    goto L_ja_0200922a;
L_ja_02009226:
  ;
  /* XXX stack argument */;
L_ja_0200922a:
  ;
  v_r0 = s52;
  v_r1 = s48;
  /* XXX stack argument */;
  /* XXX stack argument */;
  if (v_r0 <= 4)
    goto L_ja_0200923c;
  v_r2 = v_r0 + 1;
  /* XXX stack argument */;
L_ja_0200923c:
  ;
  v_r3 = s52;
  if (v_r3 <= 9)
    goto L_ja_02009248;
  v_r4 = s32;
  v_r4 = v_r4 + 1;
  /* XXX stack argument */;
L_ja_02009248:
  ;
  v_r0 = s52;
  if (v_r0 <= 14)
    goto L_ja_02009254;
  v_r1 = s32;
  v_r1 = v_r1 + 1;
  /* XXX stack argument */;
L_ja_02009254:
  ;
  v_r2 = s52;
  if (v_r2 <= 19)
    goto L_ja_02009260;
  v_r3 = s32;
  v_r3 = v_r3 + 1;
  /* XXX stack argument */;
L_ja_02009260:
  ;
  /* XXX stack argument */;
  v_r1 = s32;
  v_r2 = s48;
  v_r3 = s24;
  /* XXX stack argument */;
  Call4((void (*)())Func_02000db4, v_r10, v_r1, v_r2, v_r3);
  v_r3 = FIELD_AT(v_r10, s16, 12);
  v_r2 = s52;
  v_r3 = v_r3 + v_r2;
  v_r1 = v_r3 << 3;
  v_r3 = FIELD_AT(v_r10, s16, 14);
  v_r4 = s48;
  v_r3 = v_r3 + v_r4;
  v_r3 = v_r3 << 3;
  v_r3 = s52;
  v_r2 = v_r1 - 8;
  v_r0 = v_r3 + 11;
  if (v_r3 <= 4)
    goto L_ja_0200929c;
L_ja_0200929c:
  ;
  v_r4 = s52;
  if (v_r4 <= 9)
    goto L_ja_020092a4;
  v_r2 = v_r1 + 8;
L_ja_020092a4:
  ;
  v_r1 = s52;
  if (v_r1 <= 14)
    goto L_ja_020092ac;
  v_r2 = v_r2 + 8;
L_ja_020092ac:
  ;
  v_r3 = s52;
  if (v_r3 != 20)
    goto L_ja_020092b4;
  v_r2 = v_r2 + 8;
L_ja_020092b4:
  ;
  v_r4 = s20;
  v_r1 = s16;
  v_r3 = v_r2 - v_r4;
  v_r2 = (u32)v_r3 >> 31;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 >> 1;
  v_r4 = v_r4 + v_r3;
  v_r3 = v_r0 - v_r1;
  v_r2 = (u32)v_r3 >> 31;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 >> 1;
  v_r1 = v_r1 + v_r3;
  /* XXX stack argument */;
  v_r2 = s16;
  v_r0 = (s32)frame + 108;
  /* XXX stack argument */;
  v_r0 = Func_020023f8(v_r0, v_r4, v_r2, 1);
  v_r0 = (s32)frame + 108;
  Call1((void (*)())Func_020023f0, v_r0);
  v_r0 = s60;
  v_r0 = __modsi3(v_r0, 50);
  v_r5 = v_r0;
  v_r0 = __divsi3();
  v_r4 = s56;
  v_r0 = v_r0 << 1;
  v_r3 = FIELD_AT(v_r4, s16, 14);
  v_r0 = v_r0 + v_r3;
  v_r0 = v_r0 << 3;
  v_r3 = FIELD_AT((s32)gVBlankCount, s32, 0);
  v_r7 = v_r0 - 2;
  v_r6 = (u32)v_r3 >> 1;
  v_r8 = 7;
  v_r6 = v_r6 & 7;
  v_r0 = __modsi3(v_r5, 10, 14, v_r3);
  v_r4 = s56;
  v_r2 = v_r0 << 1;
  v_r3 = FIELD_AT(v_r4, s16, 12);
  v_r2 = v_r2 + v_r0;
  v_r2 = v_r2 << 2;
  v_r3 = v_r3 << 3;
  v_r2 = v_r2 + v_r3;
  v_r1 = v_r2 + 16;
  if (v_r0 <= 4)
    goto L_ja_0200932e;
  v_r1 = v_r1 + 10;
L_ja_0200932e:
  ;
  v_r3 = FIELD_AT((s32)gData_02002438 + v_r6, s8, 0);
  v_r1 = v_r1 + v_r3;
  v_r3 = v_r6 + 5;
  v_r3 = v_r3 & v_r8;
  v_r3 = FIELD_AT((s32)gData_02002438 + v_r3, s8, 0);
  v_r0 = (s32)frame + 92;
  v_r7 = v_r7 + v_r3;
  v_r0 = Func_020023f8(v_r0, v_r1, v_r7, 1);
  v_r0 = (s32)frame + 92;
  Call1((void (*)())Func_020023f0, v_r0);
  v_r1 = s8;
  if (v_r1 <= 0)
    goto L_ja_02009366;
  v_r1 = v_r1 + 1;
  /* XXX stack argument */;
  if (v_r1 != 8)
    goto L_ja_02009366;
  v_r3 = s60;
  v_r3 = v_r3 + 1;
  /* XXX stack argument */;
  /* XXX stack argument */;
L_ja_02009366:
  ;
  v_r4 = s8;
  if (v_r4 == 0)
    goto L_ja_0200936e;
  goto L_ja_0200974a;
L_ja_0200936e:
  ;
  v_r0 = s52;
  if (v_r0 != 20)
    goto L_ja_0200938a;
  v_r1 = s48;
  if (v_r1 != 0)
    goto L_ja_02009380;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  goto L_ja_0200938c;
L_ja_02009380:
  ;
  v_r2 = s48;
  if (v_r2 != 1)
    goto L_ja_0200938a;
  v_r3 = FIELD_AT(v_r3, s32, 12);
  goto L_ja_0200938c;
L_ja_0200938a:
  ;
  v_r3 = FIELD_AT(v_r3, s32, 4);
L_ja_0200938c:
  ;
  v_r3 = v_r3 << 16;
  v_r2 = v_r3 >> 16;
  v_r2 = v_r2 << 16;
  v_r3 = 0x10000 & v_r2;
  if (v_r3 == 0)
    goto L_ja_0200943c;
  v_r3 = s12;
  if (v_r3 != 0)
    goto L_ja_0200943c;
  v_r0 = s52;
  v_r9 = 0;
  if (v_r0 != 20)
    goto L_ja_020093d0;
  v_r1 = s48;
  if (v_r1 != 0)
    goto L_ja_020093b8;
  v_r9 = 1;
  goto L_ja_0200943c;
L_ja_020093b8:
  ;
  v_r3 = s48;
  if (v_r3 != 1)
    goto L_ja_020093c4;
  v_r9 = 2;
  goto L_ja_0200943c;
L_ja_020093c4:
  ;
  v_r0 = s48;
  if (v_r0 != 3)
    goto L_ja_0200943c;
  v_r9 = 3;
  goto L_ja_0200943c;
L_ja_020093d0:
  ;
  v_r2 = s60;
  v_r3 = s44;
  if (v_r2 >= v_r3)
    goto L_ja_0200943c;
  v_r4 = s48;
  v_r0 = s52;
  v_r3 = v_r4 << 2;
  v_r3 = v_r3 + v_r4;
  v_r3 = v_r3 << 2;
  v_r3 = v_r0 + v_r3;
  v_r5 = FIELD_AT((s32)gData_0200265e + v_r3, u8, 0);
  if (v_r5 > 63)
    goto L_ja_0200943c;
  Call4((void (*)())Func_02002408, 112, v_r1, (s32)gData_0200265e, v_r3);
  v_r1 = s60;
  FIELD_AT((s32)Data_0200274a + v_r1, u8, 0) = v_r5;
  v_r0 = s60;
  v_r0 = __modsi3(v_r0, 10);
  v_r3 = s44;
  v_r2 = s60;
  v_r3 = v_r3 - 1;
  if (v_r2 >= v_r3)
    goto L_ja_02009438;
  v_r0 = __modsi3(v_r2, 50, v_r2, v_r3);
  if (v_r0 != 49)
    goto L_ja_0200941c;
  /* XXX stack argument */;
  goto L_ja_0200943c;
L_ja_0200941c:
  ;
  v_r4 = s60;
  v_r4 = v_r4 + 1;
  /* XXX stack argument */;
  goto L_ja_0200943c;
L_ja_02009438:
  ;
  /* XXX stack argument */;
L_ja_0200943c:
  ;
  if (v_r9 != 0)
    goto L_ja_0200944a;
  v_r2 = s12;
  if (v_r2 == 1)
    goto L_ja_0200944a;
  goto L_ja_0200974a;
L_ja_0200944a:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 8;
  if (v_r3 != 0)
    goto L_ja_02009464;
  if (v_r9 == 3)
    goto L_ja_02009464;
  v_r4 = s12;
  if (v_r4 == 1)
    goto L_ja_02009464;
  goto L_ja_020096ba;
L_ja_02009464:
  ;
  v_r7 = (s32)Data_0200274a;
  v_r1 = s44;
  v_r0 = Func_02002068((s32)Data_0200274a, v_r1, v_r7, v_r9);
  v_r9 = v_r0;
  v_r6 = v_r9;
  v_r6 = v_r6 - 2;
  v_r0 = Func_0200217c(v_r6, v_r7);
  v_r5 = v_r0;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = Window_Create(8, 12, 14, 3);
  v_r5 = v_r5 << 16;
  v_r5 = v_r5 >> 16;
  v_r3 = FIELD_AT(v_r7 + v_r6, u8, 0);
  v_r5 = v_r5 << 16;
  v_r1 = (u32)v_r5 >> 16;
  v_r5 = (u32)v_r5 >> 24;
  v_r8 = v_r0;
  if (v_r3 != v_r5)
    goto L_ja_0200957e;
  v_r3 = v_r9 - 1;
  v_r3 = FIELD_AT(v_r7 + v_r3, u8, 0);
  v_r2 = 255 & v_r1;
  if (v_r3 != v_r2)
    goto L_ja_0200957e;
  v_r0 = Text_DrawCharacterAtOffset(0xd92, v_r8, 0, 0);
  Call1((void (*)())Func_02002408, 112);
L_ja_020094c6:
  ;
  Call1((void (*)())wait, 1);
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 1;
  if (v_r3 == 0)
    goto L_ja_020094c6;
  Call1((void (*)())Func_02002408, 112);
  Func_02002350();
  v_r5 = (s32)gMainRoutineWork;
  v_r2 = v_r5 + 0x214;
  FIELD_AT(v_r2, s32, 0) = 0;
  Call4((void (*)())Party_RemoveActiveOwner, 4, 0x214, v_r2, 0);
  Call1((void (*)())Party_RemoveActiveOwner, 5);
  Call1((void (*)())Party_RemoveActiveOwner, 6);
  Call1((void (*)())Party_RemoveActiveOwner, 7);
  Call1((void (*)())Party_AddActiveOwner, 0);
  Call1((void (*)())Party_AddActiveOwner, 1);
  Call1((void (*)())Party_AddActiveOwner, 2);
  Call1((void (*)())Party_AddActiveOwner, 3);
  v_r0 = s64;
  Call2((void (*)())Func_020018e0, v_r0, (s32)Data_0200274a);
  Call2((void (*)())Window_Close, v_r8, 2);
  v_r0 = s56;
  Call2((void (*)())Window_Close, v_r0, 2);
  Call2((void (*)())Window_Close, v_r10, 2);
  v_r3 = v_r5 + 0x225;
  v_r0 = FIELD_AT(v_r3, u8, 0);
  v_r5 = v_r5 + 0x226;
  v_r1 = FIELD_AT(v_r5, u8, 0);
  Call4((void (*)())Func_02002310, v_r0, v_r1, 0x225, 0x226);
  Call1((void (*)())Func_02002408, 74);
  Call1((void (*)())wait, 6);
  Call1((void (*)())Func_020022f8, 0);
  Func_020023e8();
  goto L_ja_02009894;
L_ja_0200957e:
  ;
  Call4((void (*)())Func_02002408, 113, (s32)gGameFlagBytes, v_r2, (s32)IwramCopyWords);
  v_r0 = Text_DrawCharacterAtOffset(0xd91, v_r8, 0, 0);
L_ja_02009590:
  ;
  Call1((void (*)())wait, 1);
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 1;
  if (v_r3 != 0)
    goto L_ja_020095ac;
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 2;
  if (v_r3 == 0)
    goto L_ja_02009590;
L_ja_020095ac:
  ;
  Call4((void (*)())Window_Close, v_r8, 2, v_r2, v_r3);
  v_r0 = WindowSprite_RedrawSavedRect(v_r10);
  v_r6 = 0xd80;
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 0, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 48, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 8);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 16);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 96, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 144, 0);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 144, 8);
  /* XXX stack argument */;
  v_r6 = v_r6 + 1;
  v_r0 = Window_DrawDividerLine(v_r10, 24, 0, 24);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 8);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r10, 192, 24);
  goto L_ja_0200974a;
L_ja_020096ba:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 12);
  v_r3 = v_r3 & 0x200;
  if (v_r3 != 0)
    goto L_ja_020096cc;
  v_r4 = v_r9;
  if (v_r4 != 2)
    goto L_ja_020096e0;
L_ja_020096cc:
  ;
  v_r0 = s60;
  if (v_r0 <= 0)
    goto L_ja_0200974a;
  Call1((void (*)())Func_02002408, 111);
  v_r1 = s60;
  v_r1 = v_r1 - 1;
  /* XXX stack argument */;
  goto L_ja_0200974a;
L_ja_020096e0:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 12);
  v_r3 = v_r3 & 0x100;
  if (v_r3 != 0)
    goto L_ja_020096f2;
  if (v_r9 != 1)
    goto L_ja_0200970a;
L_ja_020096f2:
  ;
  v_r3 = s44;
  v_r4 = s60;
  v_r3 = v_r3 - 1;
  if (v_r4 >= v_r3)
    goto L_ja_0200974a;
  Call4((void (*)())Func_02002408, 111, v_r1, v_r9, v_r3);
  v_r0 = s60;
  v_r0 = v_r0 + 1;
  /* XXX stack argument */;
  goto L_ja_0200974a;
L_ja_0200970a:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 4);
  v_r3 = v_r3 & 2;
  if (v_r3 == 0)
    goto L_ja_0200974a;
  v_r1 = s60;
  if (v_r1 <= 0)
    goto L_ja_02009728;
  Call4((void (*)())Func_02002408, 111, v_r1, 2, v_r3);
  v_r2 = s60;
  v_r2 = v_r2 - 1;
  /* XXX stack argument */;
  goto L_ja_0200974a;
L_ja_02009728:
  ;
  Call1((void (*)())Func_02002408, 113);
  v_r0 = s56;
  Call2((void (*)())Window_Close, v_r0, 2);
  Call2((void (*)())Window_Close, v_r10, 2);
  Call1((void (*)())Func_02002408, 74);
  goto L_ja_02009894;
L_ja_0200974a:
  ;
  v_r0 = s60;
  v_r0 = __divsi3(v_r0, 50);
  v_r11 = v_r0;
  v_r0 = s56;
  v_r0 = WindowSprite_RedrawSavedRect(v_r0);
  v_r6 = 0;
  v_r5 = 2;
L_ja_0200975e:
  ;
  v_r0 = s56;
  v_r6 = v_r6 + 1;
  /* XXX stack argument */;
  v_r0 = Window_DrawDividerLine(v_r0, 0, v_r5, 18);
  v_r5 = v_r5 + 2;
  if (v_r6 != 4)
    goto L_ja_0200975e;
  v_r3 = s40;
  v_r6 = 0;
  if (v_r3 == 0)
    goto L_ja_020097ae;
  v_r3 = -v_r3;
  v_r5 = v_r3;
  v_r5 = v_r5 + 18;
L_ja_02009782:
  ;
  v_r1 = v_r6 + 0xf301;
  if (v_r6 != v_r11)
    goto L_ja_02009796;
  v_r1 = v_r6 + 0xf30b;
L_ja_02009796:
  ;
  /* XXX stack argument */;
  v_r0 = s56;
  Call4((void (*)())Window_SetTilemapEntry, v_r0, v_r1, v_r5, 10);
  v_r1 = s40;
  v_r6 = v_r6 + 1;
  v_r5 = v_r5 + 1;
  if (v_r6 != v_r1)
    goto L_ja_02009782;
L_ja_020097ae:
  ;
  v_r2 = s44;
  v_r9 = v_r2;
  if (v_r2 <= 49)
    goto L_ja_020097f8;
  v_r3 = s40;
  v_r3 = v_r3 - 1;
  if (v_r3 != v_r11)
    goto L_ja_020097f4;
  v_r0 = s44;
  v_r0 = __modsi3(v_r0, 50, v_r2, v_r3);
  v_r9 = v_r0;
  goto L_ja_020097f8;
L_ja_020097f4:
  ;
  v_r9 = 50;
L_ja_020097f8:
  ;
  v_r4 = v_r9;
  v_r6 = 0;
  if (v_r4 == 0)
    goto L_ja_02009858;
  v_r0 = s36;
  v_r1 = s36;
  v_r0 = v_r0 + 200;
  v_r8 = v_r0;
  v_r7 = v_r1 + 0x190;
L_ja_0200980e:
  ;
  v_r0 = v_r11 * 50;
  v_r0 = v_r6 + v_r0;
  v_r2 = FIELD_AT((s32)Data_0200274a + v_r0, u8, 0);
  v_r3 = FIELD_AT(v_r7, u8, 0);
  if (v_r2 == v_r3)
    goto L_ja_0200984c;
  FIELD_AT(v_r7, u8, 0) = v_r2;
  if (v_r2 != 99)
    goto L_ja_02009838;
  v_r0 = FIELD_AT(v_r8, s32, 0);
  goto L_ja_0200984c;
L_ja_02009838:
  ;
  v_r5 = (s32)frame + 68;
  v_r0 = FIELD_AT((s32)Data_0200274a + v_r0, u8, 0);
  v_r0 = Func_02000a70(v_r0, v_r5, v_r2, (s32)IwramClearWords);
  v_r0 = FIELD_AT(v_r5, u16, 0);
  v_r1 = FIELD_AT(v_r8, s32, 0);
  Call3((void (*)())Func_02002328, v_r0, v_r1, v_r8);
L_ja_0200984c:
  ;
  v_r6 = v_r6 + 1;
  v_r8 = v_r8 + 4;
  v_r7 = v_r7 + 1;
  if (v_r6 != v_r9)
    goto L_ja_0200980e;
L_ja_02009858:
  ;
  if (v_r6 == 50)
    goto L_ja_0200988c;
  v_r4 = s36;
  v_r3 = v_r6 << 2;
  v_r3 = v_r3 + v_r4;
  v_r7 = v_r3;
  v_r3 = v_r6 + 0x190;
  v_r7 = v_r7 + 200;
  v_r5 = v_r3 + v_r4;
L_ja_0200986e:
  ;
  v_r3 = FIELD_AT(v_r5, u8, 0);
  if (v_r3 == 99)
    goto L_ja_02009882;
  FIELD_AT(v_r5, u8, 0) = 99;
  v_r0 = FIELD_AT(v_r7, s32, 0);
L_ja_02009882:
  ;
  v_r6 = v_r6 + 1;
  v_r7 = v_r7 + 4;
  v_r5 = v_r5 + 1;
  if (v_r6 != 50)
    goto L_ja_0200986e;
L_ja_0200988c:
  ;
  Call2((void (*)())wait, 1, v_r1);
  goto L_ja_0200911e;
L_ja_02009894:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  Func_02002208();
  v_r2 = (s32)gMainRoutineWork + 0x225;
  v_r3 = (s32)gMainRoutineWork + 0x226;
  v_r0 = FIELD_AT(v_r2, u8, 0);
  v_r1 = FIELD_AT(v_r3, u8, 0);
  Call4((void (*)())Func_02002310, v_r0, v_r1, v_r2, v_r3);
  /* XXX stmia r3!, {r0, r1, r2} */
  FIELD_AT(0x50001c0, u16, 8) = 0x6318;
  v_r0 = Func_020022b0(1, 0x50001c0, -0x7bfffff8, 0x6318);
  /* XXX stack argument */;
  if (v_r0 != -1)
    goto L_fr_02008e98;
L_fr_02008e98:
  ;
  Call1((void (*)())Func_02002408, 71);
  /* XXX stack argument */;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = Heap_AllocEwram(0x1dc);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call1((void (*)())Func_02002320, 0x6006000);
  v_r1 = s64;
  goto L_fr_02008ed4;
L_fr_02008ed4:
  ;
  /* XXX stack argument */;
  if (v_r1 == 0)
    goto L_fr_02008eec;
  v_r3 = s64;
  /* XXX stack argument */;
  if (v_r3 == 1)
    goto L_fr_02008eec;
  /* XXX stack argument */;
L_fr_02008eec:
  ;
  v_r0 = s44;
  v_r0 = __divsi3(v_r0, 50, v_r2, v_r3);
  v_r0 = v_r0 + 1;
  /* XXX stack argument */;
  v_r5 = 2;
  /* XXX stack argument */;
  v_r0 = Window_Create(1, 13, 29, 6);
  v_r9 = v_r0;
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002440, v_r9, 0, 0);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002454, v_r9, 0, 8);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002468, v_r9, 0, 16);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_0200247c, v_r9, 0, 24);
  v_r6 = 0xf5a;
  /* XXX stack argument */;
  v_r0 = Window_DrawDividerLine(v_r9, 20, 0, 20);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 8);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 24);
  /* XXX stack argument */;
  v_r0 = Window_Create(5, 1, 20, 12);
  /* XXX stack argument */;
  v_r6 = 0;
L_fr_02008f86:
  ;
  v_r2 = s36;
  v_r1 = v_r6 + 0x1c0;
  v_r2 = v_r2 + 2;
  v_r6 = v_r6 + 1;
  FIELD_AT(v_r2 + v_r1, u8, 0) = 0;
  if (v_r6 != 26)
    goto L_fr_02008f86;
  v_r6 = 0;
L_fr_02008f9e:
  ;
  v_r6 = v_r6 + 1;
  FIELD_AT((s32)Data_0200274a, u8, 0) = 99;
  v_r2 = (s32)Data_0200274a + 1;
  if (v_r6 != 0x104)
    goto L_fr_02008f9e;
  v_r2 = s36;
  v_r6 = 0;
  v_r2 = v_r2 + 0x190;
  v_r10 = v_r2;
  v_r11 = v_r6;
L_fr_02008fbc:
  ;
  v_r0 = __modsi3(v_r6, 10, v_r2, v_r3);
  v_r5 = v_r0;
  v_r3 = v_r5 << 1;
  v_r3 = v_r3 + v_r5;
  v_r7 = v_r3 << 2;
  v_r4 = 8 + v_r7;
  v_r8 = v_r4;
  v_r0 = __divsi3(v_r6, 10);
  v_r0 = v_r0 << 4;
  v_r3 = v_r0 + 2;
  if (v_r5 <= 4)
    goto L_fr_02008fe6;
  v_r7 = v_r7 + 18;
  v_r8 = v_r7;
L_fr_02008fe6:
  ;
  v_r2 = s36;
  v_r1 = FIELD_AT(v_r11 + v_r2, s32, 0);
  v_r0 = s56;
  v_r0 = Func_02002330(v_r0, v_r1, v_r8, v_r3);
  v_r3 = s36;
  v_r2 = v_r11 + 200;
  FIELD_AT(v_r2 + v_r3, s32, 0) = v_r0;
  v_r4 = v_r10;
  FIELD_AT(v_r4, u8, 0) = 99;
  v_r1 = s36;
  v_r10 = v_r10 + 1;
  v_r0 = FIELD_AT(v_r2 + v_r1, s32, 0);
  v_r6 = v_r6 + 1;
  v_r11 = v_r11 + 4;
  if (v_r6 != 50)
    goto L_fr_02008fbc;
  v_r0 = Resource_FindFreeEntry(v_r0, 128, 4, (s32)IwramClearWords);
  v_r5 = v_r0;
  if (v_r5 > 95)
    goto L_fr_02009056;
  v_r0 = Func_02002300();
  /* XXX stack argument */;
  v_r0 = WindowSprite_Create(v_r5, 0x40000000, v_r9, 0);
  v_r0 = (s32)frame + 108;
  FIELD_AT(v_r0, s32, 0) = v_r0;
  v_r4 = v_r9;
  v_r1 = FIELD_AT(v_r4, s16, 12);
  v_r2 = FIELD_AT(v_r4, s16, 14);
  v_r1 = v_r1 << 3;
  v_r2 = v_r2 << 3;
  v_r1 = v_r1 - 2;
  v_r2 = v_r2 + 12;
  Call4((void (*)())Func_02002400, v_r0, v_r1, v_r2, 14);
L_fr_02009056:
  ;
  v_r0 = Resource_FindFreeEntry();
  v_r5 = v_r0;
  if (v_r5 > 95)
    goto L_fr_0200909e;
  v_r0 = Func_02002308();
  /* XXX stack argument */;
  v_r0 = WindowSprite_Create(v_r5, 0x40000000, v_r9, 0);
  v_r0 = (s32)frame + 92;
  FIELD_AT(v_r0, s32, 0) = v_r0;
  v_r2 = FIELD_AT(v_r0, u8, 25);
  FIELD_AT(v_r0, u8, 15) = 255;
  v_r3 = -13 & v_r2;
  FIELD_AT(v_r0, u8, 25) = v_r3;
  v_r2 = s56;
  v_r1 = FIELD_AT(v_r2, s16, 12);
  v_r2 = FIELD_AT(v_r2, s16, 14);
  v_r1 = v_r1 << 3;
  v_r2 = v_r2 << 3;
  v_r1 = v_r1 - 4;
  v_r2 = v_r2 + 12;
  Call4((void (*)())Func_02002400, v_r0, v_r1, v_r2, 14);
L_fr_0200909e:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r3 = FIELD_AT(v_r9, s16, 12);
  v_r3 = v_r3 << 3;
  v_r3 = v_r3 - 8;
  /* XXX stack argument */;
  v_r3 = FIELD_AT(v_r9, s16, 14);
  /* XXX stack argument */;
  v_r3 = v_r3 << 3;
  v_r3 = v_r3 + 11;
  /* XXX stack argument */;
L_fr_020090c2:
  ;
  v_r2 = s12;
  v_r10 = -1;
  if (v_r2 != 2)
    goto L_fr_020090d2;
  /* XXX stack argument */;
L_fr_020090d2:
  ;
  /* XXX stack argument */;
  v_r1 = s32;
  v_r2 = s28;
  v_r3 = s24;
  /* XXX stack argument */;
  Call4((void (*)())Func_02000db4, v_r9, v_r1, v_r2, v_r3);
  v_r0 = s12;
  if (v_r0 == 0)
    goto L_fr_020090ee;
  goto L_fr_020091f4;
L_fr_020090ee:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 16;
  if (v_r3 == 0)
    goto L_fr_02009126;
  Call1((void (*)())Func_02002408, 111);
  v_r0 = s52;
  v_r1 = s48;
  v_r0 = v_r0 + 1;
  if (v_r1 != 2)
    goto L_fr_0200911e;
  if (v_r0 >= 0)
    goto L_fr_02009114;
  v_r3 = s52;
  v_r3 = v_r3 + 16;
L_fr_02009114:
  ;
  v_r3 = v_r3 >> 4;
  /* XXX stack argument */;
  v_r3 = v_r3 << 4;
  v_r0 = v_r0 - v_r3;
  goto L_fr_02009124;
L_fr_0200911e:
  ;
  v_r0 = __modsi3(v_r0, 17);
L_fr_02009124:
  ;
  /* XXX stack argument */;
L_fr_02009126:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 32;
  if (v_r3 == 0)
    goto L_fr_02009184;
  Call1((void (*)())Func_02002408, 111);
  v_r2 = s48;
  if (v_r2 != 2)
    goto L_fr_02009178;
  v_r2 = s52;
  v_r2 = v_r2 + 15;
  if (v_r2 >= 0)
    goto L_fr_0200914c;
  v_r3 = s52;
  v_r3 = v_r3 + 30;
L_fr_0200914c:
  ;
  v_r3 = v_r3 >> 4;
  /* XXX stack argument */;
  v_r3 = v_r3 << 4;
  v_r2 = v_r2 - v_r3;
  /* XXX stack argument */;
  goto L_fr_02009184;
L_fr_02009178:
  ;
  v_r0 = s52;
  v_r0 = v_r0 + 16;
  v_r0 = __modsi3(v_r0, 17, v_r2, v_r3);
  /* XXX stack argument */;
L_fr_02009184:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 128;
  if (v_r3 == 0)
    goto L_fr_020091bc;
  Call1((void (*)())Func_02002408, 111);
  v_r1 = s48;
  v_r1 = v_r1 + 1;
  if (v_r1 >= 0)
    goto L_fr_020091a4;
  v_r3 = s48;
  v_r3 = v_r3 + 4;
L_fr_020091a4:
  ;
  v_r3 = v_r3 >> 2;
  /* XXX stack argument */;
  v_r3 = v_r3 << 2;
  v_r1 = v_r1 - v_r3;
  v_r3 = s52;
  /* XXX stack argument */;
  if (v_r3 != 16)
    goto L_fr_020091bc;
  if (v_r1 != 2)
    goto L_fr_020091bc;
  /* XXX stack argument */;
L_fr_020091bc:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  v_r3 = v_r3 & 64;
  if (v_r3 == 0)
    goto L_fr_020091f4;
  Call4((void (*)())Func_02002408, 111, v_r1, 64, v_r3);
  v_r1 = s48;
  v_r1 = v_r1 + 3;
  if (v_r1 >= 0)
    goto L_fr_020091dc;
  v_r3 = s48;
  v_r3 = v_r3 + 6;
L_fr_020091dc:
  ;
  v_r3 = v_r3 >> 2;
  v_r0 = s52;
  /* XXX stack argument */;
  v_r3 = v_r3 << 2;
  v_r1 = v_r1 - v_r3;
  /* XXX stack argument */;
  if (v_r0 != 16)
    goto L_fr_020091fe;
  if (v_r1 != 2)
    goto L_fr_020091f4;
  /* XXX stack argument */;
L_fr_020091f4:
  ;
  v_r3 = s52;
  /* XXX stack argument */;
  if (v_r3 == 16)
    goto L_fr_02009202;
L_fr_020091fe:
  ;
  /* XXX stack argument */;
L_fr_02009202:
  ;
  v_r0 = s52;
  v_r1 = s48;
  /* XXX stack argument */;
  /* XXX stack argument */;
  if (v_r0 <= 4)
    goto L_fr_02009214;
  v_r2 = v_r0 + 1;
  /* XXX stack argument */;
L_fr_02009214:
  ;
  v_r3 = s52;
  if (v_r3 <= 9)
    goto L_fr_02009220;
  v_r4 = s32;
  v_r4 = v_r4 + 1;
  /* XXX stack argument */;
L_fr_02009220:
  ;
  v_r0 = s52;
  if (v_r0 <= 14)
    goto L_fr_0200922c;
  v_r1 = s32;
  v_r1 = v_r1 + 1;
  /* XXX stack argument */;
L_fr_0200922c:
  ;
  v_r2 = s52;
  if (v_r2 <= 15)
    goto L_fr_02009238;
  v_r3 = s32;
  v_r3 = v_r3 + 1;
  /* XXX stack argument */;
L_fr_02009238:
  ;
  /* XXX stack argument */;
  v_r1 = s32;
  v_r2 = s48;
  v_r3 = s24;
  /* XXX stack argument */;
  Call4((void (*)())Func_02000db4, v_r9, v_r1, v_r2, v_r3);
  v_r3 = FIELD_AT(v_r9, s16, 12);
  v_r2 = s52;
  v_r3 = v_r3 + v_r2;
  v_r1 = v_r3 << 3;
  v_r3 = FIELD_AT(v_r9, s16, 14);
  v_r4 = s48;
  v_r3 = v_r3 + v_r4;
  v_r3 = v_r3 << 3;
  v_r3 = s52;
  v_r2 = v_r1 - 8;
  v_r0 = v_r3 + 11;
  if (v_r3 <= 4)
    goto L_fr_02009274;
L_fr_02009274:
  ;
  v_r4 = s52;
  if (v_r4 <= 9)
    goto L_fr_0200927c;
  v_r2 = v_r1 + 8;
L_fr_0200927c:
  ;
  v_r1 = s52;
  if (v_r1 <= 14)
    goto L_fr_02009284;
  v_r2 = v_r2 + 8;
L_fr_02009284:
  ;
  v_r3 = s52;
  if (v_r3 <= 15)
    goto L_fr_0200928c;
  v_r2 = v_r2 + 8;
L_fr_0200928c:
  ;
  v_r4 = s20;
  v_r1 = s16;
  v_r3 = v_r2 - v_r4;
  v_r2 = (u32)v_r3 >> 31;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 >> 1;
  v_r4 = v_r4 + v_r3;
  v_r3 = v_r0 - v_r1;
  v_r2 = (u32)v_r3 >> 31;
  v_r3 = v_r3 + v_r2;
  v_r3 = v_r3 >> 1;
  v_r1 = v_r1 + v_r3;
  /* XXX stack argument */;
  v_r2 = s16;
  v_r0 = (s32)frame + 108;
  /* XXX stack argument */;
  v_r0 = Func_020023f8(v_r0, v_r4, v_r2, 1);
  v_r0 = (s32)frame + 108;
  Call1((void (*)())Func_020023f0, v_r0);
  v_r0 = s60;
  v_r0 = __modsi3(v_r0, 50);
  v_r5 = v_r0;
  v_r0 = __divsi3();
  v_r4 = s56;
  v_r0 = v_r0 << 1;
  v_r3 = FIELD_AT(v_r4, s16, 14);
  v_r0 = v_r0 + v_r3;
  v_r0 = v_r0 << 3;
  v_r3 = FIELD_AT((s32)gVBlankCount, s32, 0);
  v_r7 = v_r0 - 2;
  v_r6 = (u32)v_r3 >> 1;
  v_r8 = 7;
  v_r6 = v_r6 & 7;
  v_r0 = __modsi3(v_r5, 10, 14, v_r3);
  v_r4 = s56;
  v_r2 = v_r0 << 1;
  v_r3 = FIELD_AT(v_r4, s16, 12);
  v_r2 = v_r2 + v_r0;
  v_r2 = v_r2 << 2;
  v_r3 = v_r3 << 3;
  v_r2 = v_r2 + v_r3;
  v_r1 = v_r2 + 16;
  if (v_r0 <= 4)
    goto L_fr_02009306;
  v_r1 = v_r1 + 10;
L_fr_02009306:
  ;
  v_r3 = FIELD_AT((s32)gData_02002438 + v_r6, s8, 0);
  v_r1 = v_r1 + v_r3;
  v_r3 = v_r6 + 5;
  v_r3 = v_r3 & v_r8;
  v_r3 = FIELD_AT((s32)gData_02002438 + v_r3, s8, 0);
  v_r0 = (s32)frame + 92;
  v_r7 = v_r7 + v_r3;
  v_r0 = Func_020023f8(v_r0, v_r1, v_r7, 1);
  v_r0 = (s32)frame + 92;
  Call1((void (*)())Func_020023f0, v_r0);
  v_r1 = s8;
  if (v_r1 <= 0)
    goto L_fr_0200933e;
  v_r1 = v_r1 + 1;
  /* XXX stack argument */;
  if (v_r1 != 8)
    goto L_fr_0200933e;
  v_r3 = s60;
  v_r3 = v_r3 + 1;
  /* XXX stack argument */;
  /* XXX stack argument */;
L_fr_0200933e:
  ;
  v_r4 = s8;
  if (v_r4 == 0)
    goto L_fr_02009346;
  goto L_fr_0200968a;
L_fr_02009346:
  ;
  v_r0 = s52;
  if (v_r0 != 16)
    goto L_fr_02009362;
  v_r1 = s48;
  if (v_r1 != 0)
    goto L_fr_02009358;
  v_r3 = FIELD_AT((s32)gInput, s32, 12);
  goto L_fr_02009364;
L_fr_02009358:
  ;
  v_r2 = s48;
  if (v_r2 != 1)
    goto L_fr_02009362;
  v_r3 = FIELD_AT(v_r3, s32, 12);
  goto L_fr_02009364;
L_fr_02009362:
  ;
  v_r3 = FIELD_AT(v_r3, s32, 4);
L_fr_02009364:
  ;
  v_r3 = v_r3 << 16;
  v_r2 = v_r3 >> 16;
  v_r2 = v_r2 << 16;
  v_r3 = 0x10000 & v_r2;
  if (v_r3 == 0)
    goto L_fr_02009410;
  v_r3 = s12;
  if (v_r3 != 0)
    goto L_fr_02009410;
  v_r0 = s52;
  v_r10 = 0;
  if (v_r0 != 16)
    goto L_fr_020093a8;
  v_r1 = s48;
  if (v_r1 != 0)
    goto L_fr_02009390;
  v_r10 = 1;
  goto L_fr_02009410;
L_fr_02009390:
  ;
  v_r3 = s48;
  if (v_r3 != 1)
    goto L_fr_0200939c;
  v_r10 = 2;
  goto L_fr_02009410;
L_fr_0200939c:
  ;
  v_r0 = s48;
  if (v_r0 != 3)
    goto L_fr_02009410;
  v_r10 = 3;
  goto L_fr_02009410;
L_fr_020093a8:
  ;
  v_r2 = s60;
  v_r3 = s44;
  if (v_r2 >= v_r3)
    goto L_fr_02009410;
  v_r4 = s48;
  v_r0 = s52;
  v_r3 = v_r4 << 4;
  v_r3 = v_r0 + v_r3;
  v_r5 = FIELD_AT((s32)gData_0200265e + v_r3, u8, 0);
  if (v_r5 > 63)
    goto L_fr_02009410;
  Call4((void (*)())Func_02002408, 112, v_r1, (s32)gData_0200265e, v_r3);
  v_r1 = s60;
  FIELD_AT((s32)Data_0200274a + v_r1, u8, 0) = v_r5;
  v_r0 = s60;
  v_r0 = __modsi3(v_r0, 10);
  v_r3 = s44;
  v_r2 = s60;
  v_r3 = v_r3 - 1;
  if (v_r2 >= v_r3)
    goto L_fr_0200940c;
  v_r0 = __modsi3(v_r2, 50, v_r2, v_r3);
  if (v_r0 != 49)
    goto L_fr_020093f0;
  /* XXX stack argument */;
  goto L_fr_02009410;
L_fr_020093f0:
  ;
  v_r4 = s60;
  v_r4 = v_r4 + 1;
  /* XXX stack argument */;
  goto L_fr_02009410;
L_fr_0200940c:
  ;
  /* XXX stack argument */;
L_fr_02009410:
  ;
  if (v_r10 != 0)
    goto L_fr_0200941e;
  v_r2 = s12;
  if (v_r2 == 1)
    goto L_fr_0200941e;
  goto L_fr_0200968a;
L_fr_0200941e:
  ;
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 8;
  if (v_r3 != 0)
    goto L_fr_02009438;
  if (v_r10 == 3)
    goto L_fr_02009438;
  v_r4 = s12;
  if (v_r4 == 1)
    goto L_fr_02009438;
  goto L_fr_020095fa;
L_fr_02009438:
  ;
  v_r7 = (s32)Data_0200288a;
  v_r1 = s44;
  v_r0 = Func_02002068((s32)Data_0200274a, v_r1, v_r7, v_r10);
  v_r10 = v_r0;
  v_r6 = v_r10;
  v_r6 = v_r6 - 2;
  v_r0 = Func_0200217c(v_r6, v_r7);
  v_r5 = v_r0;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = Window_Create(5, 12, 20, 3);
  v_r5 = v_r5 << 16;
  v_r5 = v_r5 >> 16;
  v_r3 = FIELD_AT(v_r7 + v_r6, u8, 0);
  v_r5 = v_r5 << 16;
  v_r1 = (u32)v_r5 >> 16;
  v_r5 = (u32)v_r5 >> 24;
  v_r8 = v_r0;
  if (v_r3 != v_r5)
    goto L_fr_02009552;
  v_r3 = v_r10 - 1;
  v_r3 = FIELD_AT(v_r7 + v_r3, u8, 0);
  v_r2 = 255 & v_r1;
  if (v_r3 != v_r2)
    goto L_fr_02009552;
  v_r0 = Text_DrawCharacterAtOffset(0xf5e, v_r8, 0, 0);
  Call1((void (*)())Func_02002408, 112);
L_fr_0200949a:
  ;
  Call1((void (*)())wait, 1);
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 1;
  if (v_r3 == 0)
    goto L_fr_0200949a;
  Call1((void (*)())Func_02002408, 112);
  Func_02002350();
  v_r5 = (s32)gMainRoutineWork;
  v_r2 = v_r5 + 0x214;
  FIELD_AT(v_r2, s32, 0) = 0;
  Call4((void (*)())Party_RemoveActiveOwner, 4, 0x214, v_r2, 0);
  Call1((void (*)())Party_RemoveActiveOwner, 5);
  Call1((void (*)())Party_RemoveActiveOwner, 6);
  Call1((void (*)())Party_RemoveActiveOwner, 7);
  Call1((void (*)())Party_AddActiveOwner, 0);
  Call1((void (*)())Party_AddActiveOwner, 1);
  Call1((void (*)())Party_AddActiveOwner, 2);
  Call1((void (*)())Party_AddActiveOwner, 3);
  v_r0 = s64;
  Call2((void (*)())Func_020018e0, v_r0, (s32)Data_0200288a);
  Call2((void (*)())Window_Close, v_r8, 2);
  v_r0 = s56;
  Call2((void (*)())Window_Close, v_r0, 2);
  Call2((void (*)())Window_Close, v_r9, 2);
  v_r3 = v_r5 + 0x225;
  v_r0 = FIELD_AT(v_r3, u8, 0);
  v_r5 = v_r5 + 0x226;
  v_r1 = FIELD_AT(v_r5, u8, 0);
  Call4((void (*)())Func_02002310, v_r0, v_r1, 0x225, 0x226);
  Call1((void (*)())Func_02002408, 74);
  Call1((void (*)())wait, 6);
  Call1((void (*)())Func_020022f8, 0);
  Func_020023e8();
  goto L_fr_020097e8;
L_fr_02009552:
  ;
  Call4((void (*)())Func_02002408, 113, (s32)gGameFlagBytes, v_r2, (s32)IwramCopyWords);
  v_r0 = Text_DrawCharacterAtOffset(0xf5d, v_r8, 0, 0);
L_fr_02009564:
  ;
  Call1((void (*)())wait, 1);
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 1;
  if (v_r3 != 0)
    goto L_fr_02009580;
  v_r3 = FIELD_AT((s32)gInput, s32, 4);
  v_r3 = v_r3 & 2;
  if (v_r3 == 0)
    goto L_fr_02009564;
L_fr_02009580:
  ;
  Call4((void (*)())Window_Close, v_r8, 2, v_r2, v_r3);
  v_r0 = WindowSprite_RedrawSavedRect(v_r9);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002440, v_r9, 0, 0);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002454, v_r9, 0, 8);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_02002468, v_r9, 0, 16);
  Call4((void (*)())Text_DrawStringInWindow, (s32)gData_0200247c, v_r9, 0, 24);
  v_r6 = 0xf5a;
  /* XXX stack argument */;
  v_r0 = Window_DrawDividerLine(v_r9, 20, 0, 20);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 0);
  v_r6 = v_r6 + 1;
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 8);
  v_r0 = Text_DrawCharacterAtOffset(v_r6, v_r9, 160, 24);
  goto L_fr_0200968a;
L_fr_020095fa:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 12);
  v_r3 = v_r3 & 0x200;
  if (v_r3 != 0)
    goto L_fr_0200960c;
  v_r4 = v_r10;
  if (v_r4 != 2)
    goto L_fr_02009620;
L_fr_0200960c:
  ;
  v_r0 = s60;
  if (v_r0 <= 0)
    goto L_fr_0200968a;
  Call1((void (*)())Func_02002408, 111);
  v_r1 = s60;
  v_r1 = v_r1 - 1;
  /* XXX stack argument */;
  goto L_fr_0200968a;
L_fr_02009620:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 12);
  v_r3 = v_r3 & 0x100;
  if (v_r3 != 0)
    goto L_fr_02009632;
  if (v_r10 != 1)
    goto L_fr_0200964a;
L_fr_02009632:
  ;
  v_r3 = s44;
  v_r4 = s60;
  v_r3 = v_r3 - 1;
  if (v_r4 >= v_r3)
    goto L_fr_0200968a;
  Call4((void (*)())Func_02002408, 111, v_r1, v_r10, v_r3);
  v_r0 = s60;
  v_r0 = v_r0 + 1;
  /* XXX stack argument */;
  goto L_fr_0200968a;
L_fr_0200964a:
  ;
  v_r3 = FIELD_AT(v_r1, s32, 4);
  v_r3 = v_r3 & 2;
  if (v_r3 == 0)
    goto L_fr_0200968a;
  v_r1 = s60;
  if (v_r1 <= 0)
    goto L_fr_02009668;
  Call4((void (*)())Func_02002408, 111, v_r1, 2, v_r3);
  v_r2 = s60;
  v_r2 = v_r2 - 1;
  /* XXX stack argument */;
  goto L_fr_0200968a;
L_fr_02009668:
  ;
  Call1((void (*)())Func_02002408, 113);
  v_r0 = s56;
  Call2((void (*)())Window_Close, v_r0, 2);
  Call2((void (*)())Window_Close, v_r9, 2);
  Call1((void (*)())Func_02002408, 74);
  goto L_fr_020097e8;
L_fr_0200968a:
  ;
  v_r0 = s60;
  v_r0 = __divsi3(v_r0, 50);
  v_r11 = v_r0;
  v_r0 = s56;
  v_r0 = WindowSprite_RedrawSavedRect(v_r0);
  v_r6 = 0;
  v_r5 = 2;
L_fr_0200969e:
  ;
  v_r0 = s56;
  v_r6 = v_r6 + 1;
  /* XXX stack argument */;
  v_r0 = Window_DrawDividerLine(v_r0, 0, v_r5, 18);
  v_r5 = v_r5 + 2;
  if (v_r6 != 4)
    goto L_fr_0200969e;
  v_r3 = s40;
  v_r6 = 0;
  if (v_r3 == 0)
    goto L_fr_020096ee;
  v_r3 = -v_r3;
  v_r5 = v_r3;
  v_r5 = v_r5 + 18;
L_fr_020096c2:
  ;
  v_r1 = v_r6 + 0xf301;
  if (v_r6 != v_r11)
    goto L_fr_020096d6;
  v_r1 = v_r6 + 0xf30b;
L_fr_020096d6:
  ;
  /* XXX stack argument */;
  v_r0 = s56;
  Call4((void (*)())Window_SetTilemapEntry, v_r0, v_r1, v_r5, 10);
  v_r1 = s40;
  v_r6 = v_r6 + 1;
  v_r5 = v_r5 + 1;
  if (v_r6 != v_r1)
    goto L_fr_020096c2;
L_fr_020096ee:
  ;
  v_r2 = s44;
  v_r10 = v_r2;
  if (v_r2 <= 49)
    goto L_fr_0200970e;
  v_r3 = s40;
  v_r3 = v_r3 - 1;
  if (v_r3 != v_r11)
    goto L_fr_0200970a;
  v_r0 = s44;
  v_r0 = __modsi3(v_r0, 50, v_r2, v_r3);
  v_r10 = v_r0;
  goto L_fr_0200970e;
L_fr_0200970a:
  ;
  v_r10 = 50;
L_fr_0200970e:
  ;
  v_r4 = v_r10;
  v_r6 = 0;
  if (v_r4 == 0)
    goto L_fr_020097ac;
  v_r0 = s36;
  v_r1 = s36;
  v_r0 = v_r0 + 200;
  v_r8 = v_r0;
  v_r7 = v_r1 + 0x190;
L_fr_02009724:
  ;
  v_r0 = v_r11 * 50;
  v_r0 = v_r6 + v_r0;
  v_r2 = FIELD_AT((s32)Data_0200274a + v_r0, u8, 0);
  v_r3 = FIELD_AT(v_r7, u8, 0);
  if (v_r2 == v_r3)
    goto L_fr_020097a0;
  FIELD_AT(v_r7, u8, 0) = v_r2;
  if (v_r2 != 99)
    goto L_fr_0200978c;
  v_r0 = FIELD_AT(v_r8, s32, 0);
  goto L_fr_020097a0;
L_fr_0200978c:
  ;
  v_r5 = (s32)frame + 68;
  v_r0 = FIELD_AT((s32)Data_0200274a + v_r0, u8, 0);
  v_r0 = Func_02000a70(v_r0, v_r5, v_r2, (s32)IwramClearWords);
  v_r0 = FIELD_AT(v_r5, u16, 0);
  v_r1 = FIELD_AT(v_r8, s32, 0);
  Call3((void (*)())Func_02002328, v_r0, v_r1, v_r8);
L_fr_020097a0:
  ;
  v_r6 = v_r6 + 1;
  v_r8 = v_r8 + 4;
  v_r7 = v_r7 + 1;
  if (v_r6 != v_r10)
    goto L_fr_02009724;
L_fr_020097ac:
  ;
  if (v_r6 == 50)
    goto L_fr_020097e0;
  v_r4 = s36;
  v_r3 = v_r6 << 2;
  v_r3 = v_r3 + v_r4;
  v_r7 = v_r3;
  v_r3 = v_r6 + 0x190;
  v_r7 = v_r7 + 200;
  v_r5 = v_r3 + v_r4;
L_fr_020097c2:
  ;
  v_r3 = FIELD_AT(v_r5, u8, 0);
  if (v_r3 == 99)
    goto L_fr_020097d6;
  FIELD_AT(v_r5, u8, 0) = 99;
  v_r0 = FIELD_AT(v_r7, s32, 0);
L_fr_020097d6:
  ;
  v_r6 = v_r6 + 1;
  v_r7 = v_r7 + 4;
  v_r5 = v_r5 + 1;
  if (v_r6 != 50)
    goto L_fr_020097c2;
L_fr_020097e0:
  ;
  Call2((void (*)())wait, 1, v_r1);
  goto L_fr_020090c2;
L_fr_020097e8:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}
