#include "types.h"
#include "scene.h"
#include "abi/battle/draw_party_panels_with_empty_list.h"

extern void Battle_DrawPartyPanelsWithEmptyList(void)
    __attribute__((alias("Nested_080270ac.0")));

static __inline__ s32 Scope_080270ac(void)
{
    void Nested_080270ac(void)
    {
        u16 data[2];

        data[0] = 0xff;
        Battle_Do(data);
        Battle_Apply(data, 1);
    }

    return 0;
}
