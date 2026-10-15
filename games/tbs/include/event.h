/*----------------------------------------------------------
    イベント コマンド
----------------------------------------------------------*/
// Scene commands: the all-caps macros the scene scripts are written in.
#ifndef EVENT_H
#define EVENT_H

#define WAIT(n) Engine_EventWait(n)

#define MESSAGE_SET(m) Engine_EventSetMessage(m)
#define MESSAGE_PUT(no, f) Engine_EventShowMessage((no), (f))
#define MESSAGE_OPEN(no, f) Engine_EventOpenMessage((no), (f))

#define OBJ_POS_SET(no, x, z) Actor_SetPosition((no), (x) << 16, (z) << 16)
#define OBJ_WALK(no, x, z) Actor_WalkTo((no), (x), (z))
#define OBJ_SPEED(no, s, a) Actor_SetSpeed((no), (s), (a))
#define OBJ_DIR(no, d, t) Actor_FaceDirection((no), (d), (t))
#define OBJ_FACE(no, to, t) Engine_ActorFaceActor((no), (to), (t))
#define OBJ_MODE(no, m) Engine_ActorSetAnimation((no), (m))
#define OBJ_EMOTE(no, e, t) Actor_ShowEmote((no), (e), (t))

// 物体を番号で得る。型の update が受け取るのと同じ union FieldObject として
#define OBJ_GET(no) ((union FieldObject *)Object_GetById(no))

#define FLAG_CHECK(f) GameFlag_IsSet(f)
#define FLAG_SET(f) GameFlag_Set(f)

#define SE(n) Event_PlayCue(n)

#define EV_BEGIN() Engine_EventBegin()
#define EV_END() Engine_EventEnd()

#endif
