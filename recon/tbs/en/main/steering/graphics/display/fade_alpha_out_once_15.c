/* NONMATCHING: 2026-10-01 brief Wave2 one-device attempt.
 * Graphics_FadeAlphaOut: removing the do-once at source line 15 changes
 * first changed instruction: mov r0, #1 => add r5, r5, #2; 29/29 assembly lines.
 * The production source retains and tags this scheduling boundary.
 * Other functions in this unit are unchanged from the current source.
 */
#include "TYPES.H"

s32 WaitFrames(s32);
void Graphics_FadeAlphaOut(void)
{
  s32 alpha_step;
  unsigned long long ctrl;
  long long alpha;
  ctrl = 0x04000050;
  *((s16 *)ctrl) = 0x2044;
  alpha = 0x04000052;
  alpha_step = 1;
  do
  {
    
  *((s16 *)alpha) = 0x1010 - alpha_step;
  alpha_step += 2;
  WaitFrames(1);

  }
  while (alpha_step <= 0x10);
}
