/*----------------------------------------------------------
    イベント コマンド
----------------------------------------------------------*/
#ifndef EVENT_H
#define EVENT_H

#define WAIT(n)                 Engine_EventWait(n)

#define MES_SET(m)              Engine_EventSetMessage(m)
#define MES_PUT(no, f)          Engine_EventShowMessage((no), (f))
#define MES_OPEN(no, f)         Engine_EventOpenMessage((no), (f))

#define OBJ_POS_SET(no, x, z)   Actor_SetPosition((no), (x) << 16, (z) << 16)
#define OBJ_WALK(no, x, z)      Actor_WalkTo((no), (x), (z))
#define OBJ_SPD(no, s, a)       Actor_SetSpeed((no), (s), (a))
#define OBJ_DIR(no, d, t)       Actor_FaceDirection((no), (d), (t))
#define OBJ_FACE(no, to, t)     Engine_ActorFaceActor((no), (to), (t))
#define OBJ_MODE(no, m)         Engine_ActorSetAnimation((no), (m))
#define OBJ_EMO(no, e, t)       Actor_ShowEmote((no), (e), (t))

#define FLAG_CHK(f)             GameFlag_IsSet(f)
#define FLAG_SET(f)             GameFlag_Set(f)

#define SE(n)                   Audio_PlayCue(n)

#define EV_BEGIN()              Engine_EventBegin()
#define EV_END()                Engine_EventEnd()

#endif
