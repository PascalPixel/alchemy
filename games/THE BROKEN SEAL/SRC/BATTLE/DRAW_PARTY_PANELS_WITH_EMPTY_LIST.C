#include "TYPES.H"

void Func_0802281c(u16 *);
void BattlePres_SetActorModesFar(u16 *, s32);

extern void Battle_DrawPartyPanelsWithEmptyList(void)
    __attribute__((alias("Nested_080270ac.0")));

static __inline__ s32 Scope_080270ac(void)
{
    void Nested_080270ac(void)
    {
        u16 data[2];

        data[0] = 0xff;
        Func_0802281c(data);
        BattlePres_SetActorModesFar(data, 1);
    }

    return 0;
}
