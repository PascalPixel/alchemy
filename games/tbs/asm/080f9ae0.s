@ 音響列位置を三段まで退避して四バイト値を読む補助関数。
@ 満杯時は音響状態解放処理へ末尾分岐する。
.syntax unified
	.thumb
	.global Func_080f9ae0
	.thumb_func
Func_080f9ae0:
	ldrb	r2, [r1, #2]
	cmp	r2, #3
	bcs.n	.Lfull
	lsls	r2, r2, #2
	adds	r3, r1, r2
	ldr	r2, [r1, #64]
	adds	r2, #4
	str	r2, [r3, #68]
	ldrb	r2, [r1, #2]
	adds	r2, #1
	strb	r2, [r1, #2]
	b.n	Func_080f9ac0
.Lfull:
	b.n	Func_080f9a50
	.2byte 0
