#ifndef ALCHEMY_FIELD_MODE_SCRIPT_H
#define ALCHEMY_FIELD_MODE_SCRIPT_H

// The field mode's script functions (field_mode/script.c and items.c).

#include "field/field_state.h"

// CUE (scene_tables.h), a row of the scene's cue table, as the Psynergy and item cues run it: its
// control word as key, its subject as flags, its condition unsigned, and its value as action (a
// message below 0x10000, else a function).
typedef struct PENDING_CUE
{
    Uint32 key;
    Uint16 flags;
    Uint16 condition;
    Sint32 action;
} PENDING_CUE;

void   fdScriptRunMessage(Sint32 actor, Sint32 msg);
Sint32 fdRunPsynergyEndScripts(Uint32 action);
Sint32 fdRunPsynergyScript(PENDING_CUE *event, Sint32 argument, Sint32 target);
void   fdScriptUpdateDebugSkip(void);
void   fdScriptNoOperationCallback(void);

#endif
