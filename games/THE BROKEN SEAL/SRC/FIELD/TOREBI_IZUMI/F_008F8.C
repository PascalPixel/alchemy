#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080772e0(s32 item);
void Main_0808a5b0(void);
void Main_08015120(s32 value, s32 digits);
s32 Main_08077308(void);
void Main_0808a270(s32 a, s32 b);
void Engine_GameStateSetReturn(s32 scene, s32 entrance);

/* Message 3619, "Step right up and try your hand at lucky wheels!", as a
 * link-time value; the offers that follow it are derived from it. */
extern u8 Value_00000e23[];
/* The lucky wheels scene. */
extern u8 Value_00000089[];

/* The lucky wheels barker: counts the game tickets (item 228), offers a
 * game, warns when the bag is nearly full (six or fewer free slots) and
 * starts the lucky wheels when the player agrees. */
void TorebiIzumi_Func020008f8(s32 spoken)
{
    s32 tickets = Main_080772e0(228);
    s32 room;

    Main_0808a5b0();
    if (spoken == 0) {
        s32 message = (s32)Value_00000e23;

        Engine_EventSetMessage(message);
        Engine_EventShowMessage(8, 0);
        if (tickets == 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        Engine_EventSetMessage(message + 2);
        Main_08015120(tickets, 5);
        Engine_EventOpenMessage(8, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        room = Main_08077308();
        if (room == 0) {
            Engine_EventSetMessage(message + 4);
            Engine_EventOpenMessage(8, 0);
        } else {
            if (room > 6) {
                goto play;
            }
            Engine_EventSetMessage(message + 5);
            Engine_EventOpenMessage(8, 0);
        }
        if (room > 6 || Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
            goto play;
        }
        Engine_EventSetMessage(3625);
        Engine_EventShowMessage(8, 0);
        return;
    }
    if (tickets == 0) {
        Engine_EventSetMessage(3634);
        Engine_EventShowMessage(8, 0);
        return;
    }
    Engine_EventSetMessage(3635);
    Engine_EventOpenMessage(8, 0);
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
        Engine_EventSetMessage(3633);
        Engine_EventShowMessage(8, 0);
        return;
    }
play:
    Engine_EventSetMessage(3626);
    Engine_EventShowMessage(8, 0);
    Main_0808a270(508, 0);
    Engine_GameStateSetReturn((s32)Value_00000089, 12);
}
