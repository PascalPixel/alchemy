#ifndef ALCHEMY_BATTLE_MSG_H
#define ALCHEMY_BATTLE_MSG_H

#include "types.h"

/* assets/text/message_archive.json の英文から付けた識別名。 */
/* 番号はリンク時定数。プールされる絶対シンボルとして参照する。 */
extern char Value_00000810;
extern char Value_00000811;
extern char Value_00000812;
extern char Value_00000813;
extern char Value_00000814;
extern char Value_00000816;
extern char Value_0000081d;
extern char Value_0000081e;
extern char Value_00000820;
extern char Value_00000821;
extern char Value_00000822;
extern char Value_00000823;
extern char Value_00000824;
extern char Value_00000825;
extern char Value_00000826;
extern char Value_00000827;
extern char Value_00000829;
extern char Value_0000082a;
extern char Value_00000831;
extern char Value_00000834;
extern char Value_00000843;
extern char Value_00000844;
extern char Value_0000084c;
extern char Value_0000084f;
extern char Value_00000850;
extern char Value_00000854;
extern char Value_0000085e;
extern char Value_0000085f;
extern char Value_00000860;
extern char Value_00000861;
extern char Value_00000862;
extern char Value_00000863;
extern char Value_00000864;
extern char Value_00000865;
extern char Value_00000866;
extern char Value_00000867;
extern char Value_00000868;
extern char Value_00000869;
extern char Value_0000086a;
extern char Value_0000086b;
extern char Value_0000086c;
extern char Value_0000086d;
extern char Value_0000086f;
extern char Value_00000870;
extern char Value_00000872;
extern char Value_00000873;
extern char Value_00000874;
extern char Value_00000875;
extern char Value_00000876;
extern char Value_00000877;
extern char Value_00000878;
extern char Value_0000087d;
extern char Value_0000087e;
extern char Value_0000087f;
extern char Value_00000881;
extern char Value_00000882;
extern char Value_00000883;
extern char Value_00000884;
extern char Value_0000088b;
extern char Value_0000088c;
extern char Value_0000088d;
extern char Value_0000088f;
extern char Value_00000894;
extern char Value_00000896;
extern char Value_000008f3;
extern char Value_000008f4;
extern char Value_000008f5;
extern char Value_000008f6;

#define MSG_HP_RECOVER ((s32)&Value_0000081d) /* 「HPが N かいふくした！」 */
#define MSG_PP_RECOVER ((s32)&Value_0000081e) /* 「PPが N かいふくした！」 */
#define MSG_HP_FULL ((s32)&Value_00000820) /* 「HPがぜんかいふくした！」 */
#define MSG_PP_FULL ((s32)&Value_00000821) /* 「PPがぜんかいふくした！」 */
#define MSG_CRITICAL ((s32)&Value_00000822) /* 「かいしんのいちげき！」 */
#define MSG_BITTER_BLOW ((s32)&Value_00000823) /* 「つうこんのいちげき！」 */
#define MSG_FELLED ((s32)&Value_00000824) /* 「あいてを たおした！」 */
#define MSG_GOES_DOWN ((s32)&Value_00000825) /* 「たおれてしまった…」 */
#define MSG_DMG_E ((s32)&Value_00000826) /* 「Nの ダメージ！」敵側 */
#define MSG_DMG_P ((s32)&Value_00000827) /* 「Nの ダメージ！」味方側 */
#define MSG_PP_LOSS_E ((s32)&Value_00000829) /* 「PPを Nうしなった！」敵側 */
#define MSG_PP_LOSS_P ((s32)&Value_0000082a) /* 「PPを Nうしなった！」味方側 */
#define MSG_DMG_EMPH_E ((s32)&Value_00000831) /* 相性で句読点が変わる・敵側 */
#define MSG_DMG_EMPH_P ((s32)&Value_00000834) /* 相性で句読点が変わる・味方側 */
#define MSG_SPIRIT_DRAIN ((s32)&Value_0000084c) /* 「せいしんを すいとられた！」 */
#define MSG_KO_DOWN ((s32)&Value_0000084f) /* 「たおれた！」 */
#define MSG_SUFFOCATE ((s32)&Value_00000850) /* 「いきができない！」 */
#define MSG_NO_EFFECT ((s32)&Value_00000854) /* 「しかし こうかがなかった！」 */
#define MSG_LEECH_GAIN ((s32)&Value_0000085e) /* 「さらに PPを Nかいふく！」 */
#define MSG_LEECH_TAKE ((s32)&Value_0000085f) /* 「PPを Nすいとられた！」 */
#define MSG_ATK_DOWN ((s32)&Value_00000860) /* 「こうげきが Nさがった！」 */
#define MSG_ATK_UP ((s32)&Value_00000861) /* 「こうげきが Nあがった！」 */
#define MSG_DEF_DOWN ((s32)&Value_00000862) /* 「ぼうぎょが Nさがった！」 */
#define MSG_DEF_UP ((s32)&Value_00000863) /* 「ぼうぎょが Nあがった！」 */
#define MSG_REVIVED ((s32)&Value_00000864) /* 「いきかえった！」 */
#define MSG_RES_DOWN ((s32)&Value_00000865) /* 「たいせいが Nさがった！」 */
#define MSG_RES_UP ((s32)&Value_00000866) /* 「たいせいが Nあがった！」 */
#define MSG_POISONED ((s32)&Value_00000867) /* 「どくに おかされた！」 */
#define MSG_DELUSION ((s32)&Value_00000868) /* 「げんわくに つつまれた！」 */
#define MSG_CONFUSED ((s32)&Value_00000869) /* 「こんらんした！」 */
#define MSG_CHARMED ((s32)&Value_0000086a) /* 「こころを うばわれた！」 */
#define MSG_STUNNED ((s32)&Value_0000086b) /* 「スタンした！」 */
#define MSG_ASLEEP ((s32)&Value_0000086c) /* 「ねむってしまった！」 */
#define MSG_PSY_BLOCK ((s32)&Value_0000086d) /* 「エナジーを ふうじられた！」 */
#define MSG_REFRAIN ((s32)&Value_0000086f) /* 「リフレインに はばまれた！」 */
#define MSG_REFLECT ((s32)&Value_00000870) /* 「リフレクの こうか！」 */
#define MSG_EVIL_SPIRIT ((s32)&Value_00000872) /* 「あくりょうに とりつかれた！」 */
#define MSG_DEATH_CURSE ((s32)&Value_00000873) /* 「しのせんこくを うけた！」 */
#define MSG_VENOM ((s32)&Value_00000874) /* 「もうどくに おかされた！」 */
#define MSG_DEATH_COUNT ((s32)&Value_00000875) /* 「しにがみが てまねく… N」 */
#define MSG_PSY_SEAL ((s32)&Value_00000876) /* 「エナジーを シールされた！」 */
#define MSG_AGI_UP ((s32)&Value_00000877) /* 「すばやさが Nあがった！」 */
#define MSG_AGI_DOWN ((s32)&Value_00000878) /* 「すばやさが Nさがった！」 */
#define MSG_READIES ((s32)&Value_0000087d) /* 「みがまえた！」 */
#define MSG_CHALLENGE ((s32)&Value_0000087e) /* 「たたかいに もえてきた！」 */
#define MSG_IMMOBILE ((s32)&Value_0000087f) /* 「うごけなくなった！」 */
#define MSG_AURA ((s32)&Value_00000881) /* 「まもりのオーラに つつまれた！」 */
#define MSG_AURA_2 ((s32)&Value_00000882) /* 同文面・強 */
#define MSG_WAKES ((s32)&Value_00000883) /* 「めをさました！」 */
#define MSG_CURE_POISON ((s32)&Value_00000884) /* 「どくが きえた！」 */
#define MSG_CURE_DELUSION ((s32)&Value_0000088b) /* 「めが みえるようになった！」 */
#define MSG_CURE_SEAL ((s32)&Value_0000088c) /* 「シールが とけた！」 */
#define MSG_CURE_STUN ((s32)&Value_0000088d) /* 「スタンが なおった！」 */
#define MSG_CURE_SPIRIT ((s32)&Value_0000088f) /* 「しにがみが はなれた！」 */
#define MSG_CURE_CURSE ((s32)&Value_00000894) /* 「しのせんこくを ふりはらった！」 */
#define MSG_BUFFS_RESET ((s32)&Value_00000896) /* 「のうりょくが もとにもどった！」 */
#define MSG_SPLIT_OFF ((s32)&Value_000008f3) /* 「ぶんれつした！」 */
#define MSG_SPLIT_FAIL ((s32)&Value_000008f4) /* 「ぶんれつに しっぱいした！」 */
#define MSG_APPEARS ((s32)&Value_000008f5) /* 「あらわれた！！」 */
#define MSG_NO_ONE_CAME ((s32)&Value_000008f6) /* 「しかし だれもこなかった！」 */

#endif
