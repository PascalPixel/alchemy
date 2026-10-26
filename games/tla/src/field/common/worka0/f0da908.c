#include "worka0.h"
#include "types.h"
#include "input.h"
#include "object_runtime.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables reach these functions under the
// names of other entries.
#define Func_080da908 Func_080dbe44
#define Func_080da938 Func_080d3b28
#endif

extern s32 gData_080f3950;

void Func_080da060(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080da908(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 0;
  work->slots[index].head = NULL;
}

void Func_080da938(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 2;
  work->slots[index].tail = NULL;
}
