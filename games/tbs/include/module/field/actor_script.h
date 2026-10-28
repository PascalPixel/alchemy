#ifndef ALCHEMY_MODULE_FIELD_ACTOR_SCRIPT_H
#define ALCHEMY_MODULE_FIELD_ACTOR_SCRIPT_H

#include "types.h"
#include "script_operands.h"

/// Commands receive the actor's interpreter view. A nonzero result continues
/// its script this frame; zero waits until the next frame.
extern const ScriptCommand gActorScript_CommandTable[47];

#endif
