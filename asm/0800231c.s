@ 後続表と0800230cを共有する固定小数点変換処理。
@ 命令位置相対表と繰上がり分岐を保つ速度優先アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_0800231c
	.thumb_func
Func_0800231c:
	movs	r1, #64
	lsls	r1, r1, #8
	adds	r0, r0, r1
	adds	r0, #32
	lsls	r1, r0, #18
	lsrs	r1, r1, #24
	beq.n	Func_0800230c
	lsrs	r2, r0, #15
	bcc.n	.L_index
	movs	r3, #128
	lsls	r3, r3, #1
	subs	r1, r3, r1
.L_index:
	lsls	r1, r1, #1
	add	r3, pc, #12
	ldrh	r0, [r3, r1]
	lsrs	r2, r2, #1
	bcc.n	.L_return
	negs	r0, r0
	bx	lr
.L_return:
	bx	lr
