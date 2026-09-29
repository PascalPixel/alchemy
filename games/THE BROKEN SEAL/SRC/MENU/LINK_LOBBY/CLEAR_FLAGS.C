/* The link lobby: the scene teardown that clears the lobby flags. */
#include "LOBBY.H"

/*
 * Scene teardown: reset one workspace field, clear three flags, play cue
 * 0x2927 and return the last call's result -- the epilogue pops the return
 * address into r1, so r0 survives and is the result. The 88-byte owner
 * includes its alignment bytes and four pool words, one of which is
 * 0x03001ebc -- the IWRAM workspace-pointer cell, not an in-image address.
 */
s32 Scene_ClearFlagsAndPlayCue2927(void)
{
    u16 *work = *(u16 **)&gEventWork;

    LinkLobby_WriteSlotValue(4);
    Engine_GameFlagClear(512);
    Engine_GameFlagClear(0x203);
    Engine_EventBegin();

    {
        /*
         * The halfword store goes through a pointer local and then a value
         * local, in that order. Storing the literal straight into the
         * halfword builds the constant in HImode and fetches it from the
         * literal pool, costing a pool word the reference does not have;
         * splitting the address out first also fixes which register holds
         * the address.
         */
        u16 *p = (u16 *)((u32)work + 386);
        s32 val = 0;
        *p = (u16)val;
    }

    Engine_EventSetMessage(0x2927);
    Engine_EventOpenMessage(8, 0);
    Engine_GameFlagClear(0x205);
    return Engine_EventEnd();
}
