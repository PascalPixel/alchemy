#ifndef ALCHEMY_BATTLE_MSG_H
#define ALCHEMY_BATTLE_MSG_H

#include "TYPES.H"

/* 名前は各版の TEXT/*.PO の msgctxt。番号は版ごとに異なり、ビルドが */
/* カタログからリンク時定数として定義する。プールされる絶対シンボルとして参照する。 */
extern char MsgEnemyAppeared;
extern char MsgLastEnemyAppeared;
extern char MsgPartyStrikesFirst;
extern char MsgPartySurprised;
extern char MsgActorAttacks;
extern char MsgActorDefends;
extern char MsgHpRecover; /* 「HPが N かいふくした！」 */
extern char MsgPpRecover; /* 「PPが N かいふくした！」 */
extern char MsgHpFull; /* 「HPがぜんかいふくした！」 */
extern char MsgPpFull; /* 「PPがぜんかいふくした！」 */
extern char MsgCritical; /* 「かいしんのいちげき！」 */
extern char MsgBitterBlow; /* 「つうこんのいちげき！」 */
extern char MsgFelled; /* 「あいてを たおした！」 */
extern char MsgGoesDown; /* 「たおれてしまった…」 */
extern char MsgDmgE; /* 「Nの ダメージ！」敵側 */
extern char MsgDmgP; /* 「Nの ダメージ！」味方側 */
extern char MsgPpLossE; /* 「PPを Nうしなった！」敵側 */
extern char MsgPpLossP; /* 「PPを Nうしなった！」味方側 */
extern char MsgDmgEmphE; /* 相性で句読点が変わる・敵側 */
extern char MsgDmgEmphP; /* 相性で句読点が変わる・味方側 */
extern char MsgPartyFlees;
extern char MsgCannotEscape;
extern char MsgCurseDamage; /* A curse does N damage (English text) */
extern char MsgSpiritDrain; /* 「せいしんを すいとられた！」 */
extern char MsgKoDown; /* 「たおれた！」 */
extern char MsgSuffocate; /* 「いきができない！」 */
extern char MsgPoisonDamage; /* The poison does N damage (English text) */
extern char MsgNoEffect; /* 「しかし こうかがなかった！」 */
extern char MsgLeechGain; /* 「さらに PPを Nかいふく！」 */
extern char MsgLeechTake; /* 「PPを Nすいとられた！」 */
extern char MsgAtkDown; /* 「こうげきが Nさがった！」 */
extern char MsgAtkUp; /* 「こうげきが Nあがった！」 */
extern char MsgDefDown; /* 「ぼうぎょが Nさがった！」 */
extern char MsgDefUp; /* 「ぼうぎょが Nあがった！」 */
extern char MsgRevived; /* 「いきかえった！」 */
extern char MsgResDown; /* 「たいせいが Nさがった！」 */
extern char MsgResUp; /* 「たいせいが Nあがった！」 */
extern char MsgPoisoned; /* 「どくに おかされた！」 */
extern char MsgDelusion; /* 「げんわくに つつまれた！」 */
extern char MsgConfused; /* 「こんらんした！」 */
extern char MsgCharmed; /* 「こころを うばわれた！」 */
extern char MsgStunned; /* 「スタンした！」 */
extern char MsgAsleep; /* 「ねむってしまった！」 */
extern char MsgPsyBlock; /* 「エナジーを ふうじられた！」 */
extern char MsgRefrain; /* 「リフレインに はばまれた！」 */
extern char MsgReflect; /* 「リフレクの こうか！」 */
extern char MsgEvilSpirit; /* 「あくりょうに とりつかれた！」 */
extern char MsgDeathCurse; /* 「しのせんこくを うけた！」 */
extern char MsgVenom; /* 「もうどくに おかされた！」 */
extern char MsgDeathCount; /* 「しにがみが てまねく… N」 */
extern char MsgPsySeal; /* 「エナジーを シールされた！」 */
extern char MsgAgiUp; /* 「すばやさが Nあがった！」 */
extern char MsgAgiDown; /* 「すばやさが Nさがった！」 */
extern char MsgNimblyDodgesBlow; /* 「すばやくかわした!」 */
extern char MsgNoEscape; /* 「しかし にげられなかった!」 */
extern char MsgEarthPowerUp; /* Earth Power rises by N; the other elements follow (English text) */
extern char MsgReadies; /* 「みがまえた！」 */
extern char MsgChallenge; /* 「たたかいに もえてきた！」 */
extern char MsgImmobile; /* 「うごけなくなった！」 */
extern char MsgAura; /* 「まもりのオーラに つつまれた！」 */
extern char MsgAura2; /* 同文面・強 */
extern char MsgWakes; /* 「めをさました！」 */
extern char MsgCurePoison; /* 「どくが きえた！」 */
extern char MsgResNormal; /* Resistances return to normal (English text) */
extern char MsgAtkNormal; /* Attack returns to normal (English text) */
extern char MsgDefNormal; /* Defense returns to normal (English text) */
extern char MsgAgiNormal; /* Agility returns to normal (English text) */
extern char MsgConscious; /* regains consciousness (English text) */
extern char MsgCureDelusion; /* 「めが みえるようになった！」 */
extern char MsgCureSeal; /* 「シールが とけた！」 */
extern char MsgCureStun; /* 「スタンが なおった！」 */
extern char MsgConscious2; /* same English text as MsgConscious (English text) */
extern char MsgCureSpirit; /* 「しにがみが はなれた！」 */
extern char MsgRefrainEnd; /* Refrain no longer affects (English text) */
extern char MsgReflectEnd; /* Reflect no longer affects (English text) */
extern char MsgCureCurse; /* 「しのせんこくを ふりはらった！」 */
extern char MsgBuffsReset; /* 「のうりょくが もとにもどった！」 */
extern char MsgDjinnSet; /* Djinni is set to its owner again (English text) */
extern char MsgSplitOff; /* 「ぶんれつした！」 */
extern char MsgSplitFail; /* 「ぶんれつに しっぱいした！」 */
extern char MsgAppears; /* 「あらわれた！！」 */
extern char MsgNoOneCame; /* 「しかし だれもこなかった！」 */

#endif
