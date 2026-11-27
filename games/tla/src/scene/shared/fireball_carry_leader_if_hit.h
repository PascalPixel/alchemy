// Once a flame or a rolling fireball has hit the leader (the flames' state active), it carries him
// along (scFireballCarryLeader); 1 when it did.
Bool scFireballCarryLeaderIfHit(void)
{
    if ((FLAME_THROWERS_STATE)->active)
    {
        scFireballCarryLeader();
        return TRUE;
    }
    return FALSE;
}
