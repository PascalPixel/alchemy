@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08077cb8
	.thumb_func
Func_08077cb8:
	push	{r5, lr}
	ldr	r0, [pc, #108]
	bl	Func_08002f40
	ldrb	r2, [r0, #0]
	lsls	r3, r2, #2
	ldr	r1, [pc, #100]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r5, r3, r1
	ldrb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r5, r3
	adds	r5, r3, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, #1
	adds	r4, r3, r1
	ldrb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r2, [r0, #0]
	adds	r3, r4, r3
	adds	r4, r3, #0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r2, r3, r1
	ldrb	r3, [r0, #1]
	subs	r5, #48
	adds	r3, r2, r3
	adds	r2, r3, #0
	subs	r4, #48
	lsls	r3, r5, #4
	adds	r3, r3, r4
	subs	r2, #48
	lsls	r3, r3, #6
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #21
	orrs	r2, r3
	ldr	r3, [pc, #28]
	ldrb	r3, [r3, #0]
	asrs	r0, r2, #16
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #24]
	orrs	r0, r3
.L0:
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x00000002
	.4byte 0xfffffe20
	.4byte 0x03001f54
	.4byte 0xffff8000
