#include "EFFECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"

s32 Func_0808e5d8(s32);
#define BattleFx_ExecutePackedAbilityEffect Func_0808e5d8
void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **)ADDR_03001EBC;
  runtime = *cell;
  counter = ((u8 *)runtime) + 0xCB6;
  zero = 0;
  *((s16 *)counter) = zero;
  if ((*((s16 *)(((u8 *)runtime) + 0xCB8))) != 0)
  {
    BattleFx_ExecutePackedAbilityEffect(0x2090);
  }
}
