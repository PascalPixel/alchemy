@ 符号なし乗算の上位32ビットを返すThumb・ARM混在補助関数。
@ 承認済みThumbコンパイラではumullを直接生成できないため保持する。
.syntax unified
	.thumb
	.global Func_080f95e0
	.thumb_func
Func_080f95e0:
	add	r2, pc, #0
	bx	r2
	.arm
	umull	r2, r3, r0, r1
	add	r0, r3, #0
	bx	lr
