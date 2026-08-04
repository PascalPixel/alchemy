#include "types.h"

/* Resource 378 sprite-flag synchronisation at 0x02000848 (68 bytes). */

extern u8 *Func_0808a080();

void Func_02000848(u8 *object)
{
    u8 *source = Func_0808a080(0);
    u8 *destination_sprite = *(u8 **)(object + 80);
    u8 *source_sprite = *(u8 **)(source + 80);

    destination_sprite[9] =
        (u8)((destination_sprite[9] & 0xf3) | (source_sprite[9] & 0x0c));

    source = Func_0808a080(0);
    source_sprite = *(u8 **)(source + 80);
    destination_sprite = *(u8 **)(object + 80);
    destination_sprite[21] =
        (u8)((destination_sprite[21] & 0xf3) | (source_sprite[9] & 0x0c));
}
