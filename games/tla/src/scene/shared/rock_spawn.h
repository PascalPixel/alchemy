// Throws `count` rocks from a 16.16 point, as Burst breaks stone: each starts 8 to 16 pixels out at
// a random angle and flies up at 5 pixels a tick and out along that angle at 2 to 4.
void scRockSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 count)
{
    ROCK_STATE *state;
    Sint32      i;

    state = RamHeapSlots->menuState;
    for (i = 0; i < count; i++)
    {
        Sint32 angle = syRandom();
        ROCK  *rock  = &state->rocks[state->next];

        rock->timer  = 0;
        rock->pos[0] = x;
        rock->pos[1] = y;
        rock->pos[2] = z;
        syFieldVectorAddPolarOffsetFX((syRandom() << 3) + toFIXED(8), angle, rock->pos);
        rock->velocity[0] = 0;
        rock->velocity[1] = toFIXED(5);
        rock->velocity[2] = 0;
        syFieldVectorAddPolarOffsetFX((syRandom() << 1) + toFIXED(2), angle, rock->velocity);
        state->next++;
        state->next &= ARRAY_COUNT(state->rocks) - 1;
    }
}
