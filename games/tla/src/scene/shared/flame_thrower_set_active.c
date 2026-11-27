// Turns flame thrower `index` on or off.
UNUSED void scFlameThrowerSetActive(Sint32 index, Sint16 value)
{
    FLAME_HIT_STATE *state           = FLAME_THROWERS_STATE;

    (state->entries + index)->active = value;
}
