@ 音響状態初期化補助関数二個。
@ 承認済みコンパイラではレジスタ割当と定数読出し幅が一致しないため暫定保持する。
.syntax unified
	.thumb
	.global Func_080fa4cc
	.thumb_func
Func_080fa4cc:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, .Lmagic0
	cmp	r3, r0
	bne.n	.Lreturn0
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	ldr	r0, .Lvalue
	strh	r0, [r2, #40]
.Lreturn0:
	bx	lr
	.align 2
.Lmagic0:
	.4byte 0x68736d53
.Lvalue:
	.4byte 0x00000101

	.global Func_080fa4ec
	.thumb_func
Func_080fa4ec:
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, .Lmagic1
	cmp	r3, r0
	bne.n	.Lreturn1
	strh	r1, [r2, #38]
	strh	r1, [r2, #36]
	movs	r0, #2
	strh	r0, [r2, #40]
	ldr	r0, [r2, #4]
	ldr	r1, .Lmask
	ands	r0, r1
	str	r0, [r2, #4]
.Lreturn1:
	bx	lr
	.align 2
.Lmagic1:
	.4byte 0x68736d53
.Lmask:
	.4byte 0x7fffffff
