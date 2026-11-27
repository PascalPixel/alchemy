// Vault, before and after the thieves are caught: the two villagers who walk about together
// (ACTOR_BOY_ROUND_THE_WELL and ACTOR_GIRL_ROUND_THE_WELL, as both scenes name them).

// The bits of a child round the well's timer.
enum
{
    // Set for ACTOR_BOY_ROUND_THE_WELL, whose partner is ACTOR_GIRL_ROUND_THE_WELL; clear for the
    // girl.
    VILLAGER_IS_BOY    = 1,
    // Set while the party talks to it, so that it stops whichever way it faces.
    VILLAGER_TALKED_TO = 2
};

// The update function of the two children round the well, by their timer's VILLAGER_IS_BOY and
// VILLAGER_TALKED_TO. A villager stops when close to and facing its partner, and otherwise when the
// leader is within STOP_RANGE in front of it. While a talk is pending (FIELD_STATE's talk) or Mind
// Read is on the range is TALK_STOP_RANGE, and a villager being talked to stops whichever way it
// faces.
static UNREAD_RESULT scVaultUpdateWalkingVillager(ACTOR *actor)
{
    Uint8       *windowState = WindowStateWork[0];
    FIELD_STATE *state       = FieldStateWork;
    Sint32       force       = 0;
    Sint32       range       = STOP_RANGE;
    ACTOR       *partner;
    ACTOR       *player;

    if ((actor->timer & VILLAGER_IS_BOY) != 0)
    {
        partner = fdActorGetById(ACTOR_GIRL_ROUND_THE_WELL);
    }
    else
    {
        partner = fdActorGetById(ACTOR_BOY_ROUND_THE_WELL);
    }
    if (scVaultStopIfFacingNear(actor, partner, PARTNER_STOP_RANGE, 0) != 0)
    {
        RETURN_UNREAD(0);
    }

    player = fdActorGetById(ACTOR_PARTY_LEADER);

    if ((Sint16)state->talk != 0 || windowState[WINDOW_STATE_MIND_READ_OFFSET] != 0)
    {
        range = TALK_STOP_RANGE;
        if ((actor->timer & VILLAGER_TALKED_TO) != 0)
        {
            force = 1;
        }
    }

    scVaultStopIfFacingNear(actor, player, range, force);
    RETURN_UNREAD(0);
}
