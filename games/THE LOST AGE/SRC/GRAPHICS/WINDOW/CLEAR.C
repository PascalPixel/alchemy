#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RENDER_INPUT.H"


void RenderOutput_ClearList(void *work)
{
    struct RenderOutputList *list = work;
    struct RenderOutput *node;
    struct RenderOutput *next;

    next = NULL;
    if (list != NULL) {
        node = list->head;
        list->tail_link = &list->head;
        list->head = next;
        while (node != NULL) {
            next = node->next;
            RenderOutput_Release(node);
            node = next;
        }
    }
}
