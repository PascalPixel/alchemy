#include "TYPES.H"
extern struct GameState gGameState;
extern u8 Data_03001cb4[];

/* runtime/blank_display_load_value_and_run.c */
s32 Audio_PlayCue(s32);
s32 Unnamed_080f7460(void);

s32 Runtime_BlankDisplayLoadValueAndRun(void)
{
  s32 *p;
  u8 *src;
  if (1)
  {
    *((s16 *) 0x04000000) = 0x40;
    src = (u8 *)((void *) &gGameState);
    p = (s32 *)((u32)&Data_03001cb4);
    *p = *((s32 *)(src + 4));
  }
  Audio_PlayCue(9);
  Unnamed_080f7460();
  return 0;
}
