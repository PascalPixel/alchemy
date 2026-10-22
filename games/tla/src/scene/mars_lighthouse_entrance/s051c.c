#include "room.h"

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_02005af4[];
extern const u8 gData_02005b0c[];
extern const u8 gData_02005b54[];
extern const u8 gData_02005bcc[];
extern const u8 gData_02005d4c[];
extern const u8 gData_02005eb4[];
extern const u8 gData_02005f74[];
extern const u8 gData_020060c4[];

void Func_02000414(s32 id);
void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004bf8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004d58(s32 a0, s32 a1);
void Func_02004d60(s32 frames);
void Func_02004dc8(void);
void Func_02004dd0(void);
void BattleEffect_PrepareBufferInterpolation(void);

void Func_0200051c(void) {
  s32 x;
  s32 count;

  Func_02004de0(0x23c);
  x = 32;
  count = 6;
  /* FAKEMATCH: a goto loop, which GCC does not treat as a loop: ⚓️ builds the
   * stack arguments inside it */
step:
  Func_02004c30_(x, 125, 3, 3, 39, 85);
  Func_02000414(10);
  count--;
  WaitFrames(10);
  x += 3;
  if (count >= 0)
    goto step;
  Func_02004db0();
  Func_02004d30(20);
}
