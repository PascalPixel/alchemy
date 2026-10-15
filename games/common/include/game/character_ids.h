/*----------------------------------------------------------
    キャラクター番号
----------------------------------------------------------*/
#ifndef CHARACTER_IDS_H
#define CHARACTER_IDS_H

#define ISAAC 0	   /* ロビン */
#define GARET 1   /* ジェラルド */
#define IVAN 2	   /* イワン */
#define MIA 3	   /* メアリィ */
#define FELIX 4 /* ガルシア */
#define JENNA 5  /* ジャスミン */
#define SHEBA 6	   /* シバ */
#define PIERS 7  /* ピカード */

#define BATTLE_CHARACTER(no) (300 + (no)) /* 戦闘キャラ */

/* 子供のころの絵 (フラグ 32 のあいだ); 仲間 8 人の絵の番号はキャラ番号と同じ */
#define ISAAC_CHILD 18	 /* ロビン(こ) */
#define GARET_CHILD 19 /* ジェラルド(こ) */

/* 顔の番号 */
#define FACE_ISAAC_CHILD 56	 /* ロビン(こ) */
#define FACE_GARET_CHILD 57 /* ジェラルド(こ) */

#endif
