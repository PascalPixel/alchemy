@ 通常関数の再構築サム逆アセンブル。高級言語による一致は未達。
.syntax unified
	.thumb
	.global Func_0800230c
	.thumb_func
Func_0800230c:
	lsrs	r2, r0, #14
	movs	r0, #1
	ands	r0, r2
	lsls	r0, r0, #16
	lsrs	r2, r2, #2
	bcc.n	.L_return
	negs	r0, r0
.L_return:
	bx	lr
