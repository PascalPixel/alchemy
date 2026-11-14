#ifndef ALCHEMY_ACTOR_SCRIPT_OPERANDS_H
#define ALCHEMY_ACTOR_SCRIPT_OPERANDS_H

#include "types.h"

struct ScriptOperands;

/// A work value's handler: by operation (enum ScriptOperandOperation in
/// field_script_commands.c) it sets the actor field it names to value, adds
/// value to it, or stores whether it equals value in conditionResult.
typedef void (*OperandFunc)(struct ScriptOperands *state, s32 operation,
                            s32 value);

extern const OperandFunc gActorScript_OperandHandlerTable[41];

#endif
