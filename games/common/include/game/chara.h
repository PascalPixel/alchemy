/*----------------------------------------------------------
    キャラクター番号
----------------------------------------------------------*/
#ifndef CHARA_H
#define CHARA_H

#define ROBIN       0           /* ロビン */
#define GERALD      1           /* ジェラルド */
#define IWAN        2           /* イワン */
#define MEARI       3           /* メアリィ */
#define GARUSHIA    4           /* ガルシア */
#define JASMINE     5           /* ジャスミン */
#define SHIBA       6           /* シバ */
#define PIKAADO     7           /* ピカード */

#define BTL_CHARA(no)   (300 + (no))    /* 戦闘キャラ */

/* 子供のころの絵 (フラグ 32 のあいだ); 仲間 8 人の絵の番号はキャラ番号と同じ */
#define ROBIN_KO        18      /* ロビン(こ) */
#define GERALD_KO       19      /* ジェラルド(こ) */

/* 顔の番号 */
#define KAO_ROBIN_KO    56      /* ロビン(こ) */
#define KAO_GERALD_KO   57      /* ジェラルド(こ) */

#endif
