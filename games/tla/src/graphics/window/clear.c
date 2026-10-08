#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"


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
