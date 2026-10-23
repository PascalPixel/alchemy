#ifndef ALCHEMY_SCRIPT_H
#define ALCHEMY_SCRIPT_H

/// @file
/// Script commands: the all-caps macros the map scripts are written in. The
/// map README shows them at work.

#define WAIT(n) FieldScript_WaitFrames(n) ///< n frames, about 59.73 a second

#define MESSAGE_SET(m) FieldScript_SetMessageId(m)
/// Object event no speaks the chosen message; the next one is chosen after it.
#define MESSAGE_PUT(no, f) FieldScript_ShowMessageAndWait((no), (f))
#define MESSAGE_OPEN(no, f) FieldMessage_ShowAtObject((no), (f))

// Object events by number. Positions are whole pixels, speeds 16.16 pixels a
// frame, headings 1/65536 turns (enum Facing), modes enum ObjectEventAnimation,
// emotes enum Emote (with EMOTE_IN_FRONT), and t the frames waited after.
#define OBJECT_POSITION_SET(no, x, z)                                          \
  ObjectEvent_SetPosition((no), (x) << 16, (z) << 16)
#define OBJECT_WALK(no, x, z) ObjectEvent_WalkTo((no), (x), (z))
#define OBJECT_SPEED(no, s, a) ObjectEvent_SetSpeed((no), (s), (a))
#define OBJECT_DIR(no, d, t) ObjectEvent_FaceDirection((no), (d), (t))
#define OBJECT_FACE(no, to, t) ObjectMotion_SetAngleToward((no), (to), (t))
#define OBJECT_ANIMATION(no, m) Engine_ObjectEventSetAnimation((no), (m))
#define OBJECT_EMOTE(no, e, t) ObjectEvent_ShowEmote((no), (e), (t))

/// Gets an object by index as the same union FieldObject passed to update
/// hooks.
#define OBJECT_GET(no) ((union FieldObject *)ObjectEvent_GetById(no))

#define SE(n) Script_PlaySong(n)

#define SCRIPT_START() FieldScript_Start()
#define SCRIPT_END() FieldScript_End()

#endif
