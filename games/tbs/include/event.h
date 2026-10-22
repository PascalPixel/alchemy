#ifndef EVENT_H
#define EVENT_H

/// @file
/// Scene commands: the all-caps macros the scene scripts are written in. The
/// scene README shows them at work.

#define WAIT(n) Engine_EventWait(n) ///< n frames, about 59.73 a second

#define MESSAGE_SET(m) Engine_EventSetMessage(m)
/// Actor no speaks the chosen message; the next one is chosen after it.
#define MESSAGE_PUT(no, f) Engine_EventShowMessage((no), (f))
#define MESSAGE_OPEN(no, f) Engine_EventOpenMessage((no), (f))

// Actors by number. Positions are whole pixels, speeds 16.16 pixels a frame,
// headings 1/65536 turns (enum Facing), modes enum ActorAnimation, emotes enum
// Emote (with EMOTE_IN_FRONT), and t the frames waited after.
#define OBJ_POS_SET(no, x, z) Actor_SetPosition((no), (x) << 16, (z) << 16)
#define OBJ_WALK(no, x, z) Actor_WalkTo((no), (x), (z))
#define OBJ_SPEED(no, s, a) Actor_SetSpeed((no), (s), (a))
#define OBJ_DIR(no, d, t) Actor_FaceDirection((no), (d), (t))
#define OBJ_FACE(no, to, t) Engine_ActorFaceActor((no), (to), (t))
#define OBJ_MODE(no, m) Engine_ActorSetAnimation((no), (m))
#define OBJ_EMOTE(no, e, t) Actor_ShowEmote((no), (e), (t))

/// Gets an object by index as the same union FieldObject passed to update
/// hooks.
#define OBJ_GET(no) ((union FieldObject *)Object_GetById(no))

#define FLAG_CHECK(f) GameFlag_IsSet(f)
#define FLAG_SET(f) GameFlag_Set(f)

#define SE(n) Event_PlayCue(n)

#define EV_BEGIN() Engine_EventBegin()
#define EV_END() Engine_EventEnd()

#endif
