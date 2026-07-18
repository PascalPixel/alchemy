@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f9438
	.thumb_func
Func_080f9438:
	push	{lr}
	bl	Func_080fa2a0
	ldr	r2, [pc, #72]
	movs	r3, #255
	strb	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r1, #0
	strb	r1, [r3, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #64]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #44]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #64]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #64]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #64]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #64]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #64]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #64]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #64]
	strb	r1, [r3, #0]
	ldr	r1, [pc, #12]
	movs	r3, #7
	b.n	.L0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0200303c
	.4byte 0x02003000
	.4byte 0x02003034
	.4byte 0x02003008
	.4byte 0x02003010
	.4byte 0x02003030
	.4byte 0x02003038
	.4byte 0x0200300c
	.4byte 0x02003014
	.4byte 0x02003040
	.4byte 0x02003004
	.4byte 0x02003020
.L0:
	subs	r3, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
