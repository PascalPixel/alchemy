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

void WindowSprite_ClearList(void *work) {
  struct WindowSpriteList *list = work;
  struct WindowSprite *node;
  struct WindowSprite *next;

  next = NULL;
  if (list != NULL) {
    node = list->head;
    list->tailLink = &list->head;
    list->head = next;
    while (node != NULL) {
      next = node->next;
      WindowSprite_Release(node);
      node = next;
    }
  }
}
