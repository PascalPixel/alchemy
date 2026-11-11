/// @file
/// The event command object, as The Broken Seal's event_commands.c begins: the
/// commands events run (waits, received items, questions and destinations),
/// the battles and destinations an event requests, and the commands on
/// actors. event_commands_internal.h holds the includes and the declarations
/// the parts share; the code is in the files under event_commands/, included
/// here in ROM order. The ROM shows one object; the parts are ours, cut where
/// The Broken Seal's are. Here the ROM breaks The Broken Seal's object in
/// three: object/step_angle.c, which an overlay links too, follows this
/// object, and the rest of the commands are still separate files.

#include "event_commands_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "event_commands/event.c"
#include "event_commands/requests.c"
#include "event_commands/actor_commands.c"
