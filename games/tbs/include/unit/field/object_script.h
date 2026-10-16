#ifndef ALCHEMY_UNIT_FIELD_OBJECT_SCRIPT_H
#define ALCHEMY_UNIT_FIELD_OBJECT_SCRIPT_H

#include "types.h"
#include "script.h"

// Commands receive the object's interpreter view. A nonzero result continues
// its script this frame; zero yields until the next frame.
extern const ScriptCommand gScriptObject_CommandTable[47];

#endif
