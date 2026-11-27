// 1 when `other` is nearer `self` than `range` pixels and lies in the sixteenth of a turn `self`
// faces or in one either side of it.
Sint32 scActorIsFacingNear(ACTOR *self, ACTOR *other, Sint32 range)
{
    Bool    result = FALSE;
    Sint32 *to;
    Sint32 *from;

    to   = &other->x.fixed;
    from = &self->x.fixed;
    if (scPosDistance(to, from) < range)
    {
        Uint32 angle  = (Uint16)syAtan(other->z.fixed - self->z.fixed, *to - *from);
        Uint32 left   = FACING_SIXTEENTH(angle - FACING_STEP);
        Uint32 right  = FACING_SIXTEENTH(angle + FACING_STEP);
        Uint32 ahead  = FACING_SIXTEENTH(angle);
        Uint32 facing = FACING_SIXTEENTH(self->facing);

        if (ahead == facing || right == facing || left == facing)
            result = TRUE;
    }
    return result;
}
