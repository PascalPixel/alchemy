@ 桁移動の繰上がりを分岐へ直結する固定小数点補助処理。
@ 呼出し規約と命令配置を保つため速度優先アセンブリとして保持する。
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
