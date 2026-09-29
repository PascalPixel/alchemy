#include "TYPES.H"
extern u8 gKeyState[];
extern u8 gKeysRepeat[];

extern s32 gGameState[][1];

void Engine_EventBegin();
void Engine_ActorFaceActor();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
s32 Engine_DebugCreateWindow();
void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber();
void Engine_TaskWait();
void UiWork_Finalize();
s32 DebugParty_LoadPreset();
s32 Engine_EventEnd();

/* Link lobby: ask for a choice in a small window, then answer with one of
 * three messages depending on the choice and its result. */
s32 LinkLobby_RunChoicePrompt(s32 id)
{
    s32 window;
    s32 choice;
    s32 shown;

    Engine_EventBegin();
    Engine_ActorFaceActor(id, gGameState[125][0], 0);
    Engine_EventSetMessage(0x989);
    Engine_EventShowMessage(id, 0);
    window = Engine_DebugCreateWindow(0, 0, 6, 4, 2);
    choice = 0;
    shown = -1;
    for (;;) {
        if (choice != shown) {
            RenderOutput_PrepareForRedraw(window);
            UiText_DrawNumber(choice, 3, window, 0, 0);
            shown = choice;
        }
        if (*(volatile u32 *)gKeysRepeat & 32) {
            choice--;
        }
        if (*(volatile u32 *)gKeysRepeat & 16) {
            choice++;
        }
        if (choice < 0) {
            choice = 0;
        }
        if (*(volatile u32 *)gKeyState & 1) {
            break;
        }
        if (*(volatile u32 *)gKeyState & 2) {
            choice = -1;
            break;
        }
        Engine_TaskWait(1);
    }
    UiWork_Finalize(window, 1);
    {
        s32 ok;
        s32 message;

        if (choice >= 0) {
            ok = DebugParty_LoadPreset(choice);
        } else {
            message = 0x98a;
            goto show;
        }
        /* FAKEMATCH: the refusal jumps into the 0x98b branch so the two share
         * one message call, as the reference lays them out. */
        if (ok != 0) {
            message = 0x98b;
        show:
            Engine_EventSetMessage(message);
            Engine_EventShowMessage(9, 0);
        } else {
            Engine_EventSetMessage(0x98c);
            Engine_EventShowMessage(9, 0);
        }
    }
    Engine_TaskWait(10);
    return Engine_EventEnd();
}
