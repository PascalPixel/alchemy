#ifndef ALCHEMY_SCENE_IDS_H
#define ALCHEMY_SCENE_IDS_H

/*
 * Scene numbers are rows of the scene table (FIELD/SCENES.S), which names
 * each row after the area whose code it loads. A name's value is its row
 * number, so it is a link-time value: take its address, and the compiler
 * loads the number from the literal pool as the game does. A switch cannot
 * use one as a case label.
 */
extern char SceneId_WorldMap;
extern char SceneId_ImiruMura1;
extern char SceneId_ImiruMura2;
extern char SceneId_MogoruMori1;
extern char SceneId_MogoruMori2;
extern char SceneId_MogoruMori3;
extern char SceneId_ArutinYama5;
extern char SceneId_RamakanSabaku1;
extern char SceneId_RamakanSabaku2;
extern char SceneId_RamakanSabaku3;
extern char SceneId_RamakanSabaku4;
extern char SceneId_KuupuappuDou1;
extern char SceneId_KuupuappuDou2;
extern char SceneId_KuupuappuDou3;
extern char SceneId_FuneHeya;
extern char SceneId_BabiChika1;
extern char SceneId_BabiChika2;
extern char SceneId_BabiIriguchi3;

#endif
