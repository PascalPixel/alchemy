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
extern char SceneId_ArutinYama5;
extern char SceneId_KuupuappuDou1;
extern char SceneId_KuupuappuDou2;
extern char SceneId_KuupuappuDou3;
extern char SceneId_FuneHeya;
extern char SceneId_BabiChika1;

#endif
