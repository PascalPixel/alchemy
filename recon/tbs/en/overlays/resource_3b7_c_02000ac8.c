/* NONMATCHING: 680 bytes, candidate 680, 212 differing halfwords (2026-09-25).
 * TorebiIzumi_RunSpringRide, meant for FIELD/TOREBI_IZUMI/SPRING_RIDE.C as a
 * single-overlay unit binding Engine_* and Main_* at their import veneers
 * (runtime = listing offset + 0x8000) and Func_020004bc at 0x020084bc.
 * Block layout matches with the goto spelling of the coin/ticket prompt, the
 * Call2 wrappers keep -1 out of a shared register and the Value_ bases give
 * the r9/r5 message and text bases. Remaining: choice and message swap sl
 * and r9; the poll loop hoists both key masks (the reference only the 1);
 * the weight sum loop is reversed into a count-down where the reference
 * keeps i < n in ip and loads each weight with ldrb plus sign extension. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 Main_080772e0(s32 item);
void Main_0808a460(void);
s32 Main_08015010(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Main_08015080(s32 text, s32 window, s32 x, s32 y);
void Main_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
s32 Main_08015398(s32 choice);
void Main_08015018(s32 window, s32 mode);
void Main_08015140(void);
s32 Main_08077308(void);
void Main_08077230(s32 delta);
void Main_08077048(s32 item);
void Main_08015120(s32 value, s32 digits);
s32 TorebiIzumi_Func0200173c(s32 side);
void Func_020004bc(s32 item);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u16 Data_0200a00c[];
extern s32 Data_02009fd0[];
extern volatile u32 Data_03001c94;
/* Message 3651, as a link-time value; the prompts that follow it are derived from it. */
extern u8 Value_00000e43[];
extern u8 Value_00000e49[];
extern u8 Value_00000e4c[];

s32 TorebiIzumi_RunSpringRide(void)
{
    s32 choice = 0;
    s32 message;
    union GameStateRows *state;
    s32 window;
    s32 text;
    s32 coins;
    s32 tickets;
    s32 result;
    s32 sum;
    s32 n;
    s32 i;
    s32 roll;
    s8 *weights;

    Engine_EventBegin();
    Main_0808a460();
    message = (s32)Value_00000e43;
    state = &Data_02000240_t;
ask:
    coins = state->words[4];
    tickets = Main_080772e0(229);
    Engine_EventSetMessage(message);
    Call2((void (*)())Engine_EventOpenMessage, -1, 0);
    window = Main_08015010(0, 0, 17, 4, 2);
    text = (s32)Value_00000e49;
    Main_08015080(text, window, 0, 0);
    Main_080150b0(coins, 6, window, 72, 0);
    Main_08015080(text + 1, window, 0, 8);
    Main_080150b0(tickets, 6, window, 72, 8);
    choice = Main_08015398(choice);
    Main_08015018(window, 2);
    Main_08015140();
    if (choice == -1) {
        goto end;
    }
    if (choice == 0) {
        if (coins != 0) {
            goto play;
        }
        n = message + 1;
    } else {
        if (choice != 1) {
            goto play;
        }
        if (tickets != 0) {
            goto check_room;
        }
        n = message + 2;
    }
    Engine_EventSetMessage(n);
    Call2((void (*)())Engine_EventShowMessage, -1, 0);
    Engine_TaskWait(1);
    goto ask;
pressed:
    Engine_AudioPlayCue(112);
    result = 0;
    goto close;
check_room:
    if (Main_08077308() == 0) {
        Engine_EventSetMessage(message + 4);
        Call2((void (*)())Engine_EventOpenMessage, -1, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
            goto end;
        }
    }
play:
    window = Main_08015010(20, 15, 9, 4, 2);
    text = (s32)Value_00000e4c;
    Main_08015080(text, window, 0, 0);
    Main_08015080(text + 1, window, 0, 8);
    Engine_TaskWait(5);
    Engine_AudioPlayCue(116);
    for (;;) {
        if (Data_03001c94 & 1) {
            goto pressed;
        }
        if (Data_03001c94 & 2) {
            break;
        }
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(113);
    result = -1;
close:
    Main_08015018(window, 2);
    if (result == -1) {
        goto end;
    }
    if (choice == 0) {
        Main_08077230(-1);
    } else if (choice == 1) {
        Main_08077048(229);
    }
    result = TorebiIzumi_Func0200173c(choice);
    if (choice == 0) {
        if (result != 4) {
            Main_08077230(Data_0200a00c[result]);
            Engine_AudioPlayCue(91);
            Main_08015120(Data_0200a00c[result], 5);
            Engine_EventSetMessage(3654);
            Call2((void (*)())Engine_EventShowMessage, -1, 0);
        } else {
            Engine_AudioPlayCue(113);
            Engine_EventWait(10);
        }
    } else {
        weights = (s8 *)&state->bytes[0][1];
        n = result * 3 + 3;
        sum = 0;
        for (i = 0; i < n; i++) {
            sum += weights[i + 284];
        }
        roll = (u32)(sum * Engine_RandomNext()) >> 16;
        for (i = 0; i < 15; i++) {
            roll -= weights[i + 284];
            if (roll < 0) {
                break;
            }
        }
        if (i == 15) {
            i = 14;
        }
        Func_020004bc(Data_02009fd0[i]);
        if (weights[i + 284] > 1) {
            weights[i + 284] /= 2;
        }
    }
end:
    Engine_EventEnd();
    return 0;
}
