#include "TYPES.H"

/* event/get_special_value.c */
struct Fields_0808b248 {
    u8 filler[0x1d6];
    s16 value;
};

extern struct Fields_0808b248 gGameState;

s16 Event_GetSpecialValue(void)
{
    /* 作業領域0x1d6の半語を返す。 */
    return gGameState.value;
}
