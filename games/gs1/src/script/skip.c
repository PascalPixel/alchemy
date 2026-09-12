#include "scene.h"
#include "script_interpreter.h"
#include "script_object_entry.h"
#include "global_cells.h"
#include "object_commands.h"
#include "script_object_runtime.h"
#include "fixed_math.h"
#include "types.h"

/* script/interpreter/cmd/skip.c */
s32 Script_SkipCommand(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/cmd/audio.c */
s32 Audio_PlayCue(s32);

s32 Script_PlayAudioCue(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
