#ifndef ALCHEMY_ACTOR_SCRIPT_OPERANDS_H
#define ALCHEMY_ACTOR_SCRIPT_OPERANDS_H

/* @file */
/* The handlers of the script state values that SC_SET, SC_ADD and SC_CMP */
/* reach (actor_script_operands.c). */

#include "types.h"

struct SCRIPT_OPERANDS;

/* A state value's handler: by operation (SCRIPT_OPERAND_OPERATION in */
/* field_script_commands.c) it sets the actor field it names to value, adds */
/* value to it, or stores whether it equals value in conditionResult. */
typedef void (*OPERAND_FUNC)(struct SCRIPT_OPERANDS *state, s32 operation,
							 s32 value);

extern const OPERAND_FUNC ActorScript_OperandHandlerTable[41];

#endif
