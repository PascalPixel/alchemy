extern void Func_020049cc(int arg0, int arg1);
extern void Func_020049d4(int arg0, int arg1);

#define SetSceneActorModes Func_02001904

void SetSceneActorModes(int actor_id) {
    Func_020049cc(actor_id, 1);
    Func_020049d4(actor_id, 2);
}
