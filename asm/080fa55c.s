@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa55c
	.thumb_func
Func_080fa55c:
	push	{r4, r5, r6, lr}
	sub	sp, #4
	adds	r5, r0, #0
	ldr	r1, [pc, #192]
	movs	r0, #143
	strh	r0, [r1, #0]
	ldr	r3, [pc, #188]
	movs	r2, #0
	strh	r2, [r3, #0]
	ldr	r0, [pc, #188]
	movs	r1, #8
	strb	r1, [r0, #0]
	adds	r0, #6
	strb	r1, [r0, #0]
	adds	r0, #16
	strb	r1, [r0, #0]
	subs	r0, #20
	movs	r1, #128
	strb	r1, [r0, #0]
	adds	r0, #8
	strb	r1, [r0, #0]
	adds	r0, #16
	strb	r1, [r0, #0]
	subs	r0, #13
	strb	r2, [r0, #0]
	movs	r0, #119
	strb	r0, [r3, #0]
	ldr	r0, [pc, #156]
	ldr	r4, [r0, #0]
	ldr	r6, [r4, #0]
	ldr	r0, [pc, #152]
	cmp	r6, r0
	bne.n	.L0
	adds	r0, r6, #1
	str	r0, [r4, #0]
	ldr	r1, [pc, #148]
	ldr	r0, [pc, #148]
	str	r0, [r1, #32]
	ldr	r0, [pc, #148]
	str	r0, [r1, #68]
	ldr	r0, [pc, #148]
	str	r0, [r1, #76]
	ldr	r0, [pc, #148]
	str	r0, [r1, #112]
	ldr	r0, [pc, #148]
	str	r0, [r1, #116]
	ldr	r0, [pc, #148]
	str	r0, [r1, #120]
	ldr	r0, [pc, #148]
	str	r0, [r1, #124]
	adds	r2, r1, #0
	adds	r2, #128
	ldr	r0, [pc, #144]
	str	r0, [r2, #0]
	adds	r1, #132
	ldr	r0, [pc, #144]
	str	r0, [r1, #0]
	str	r5, [r4, #28]
	ldr	r0, [pc, #140]
	str	r0, [r4, #40]
	ldr	r0, [pc, #140]
	str	r0, [r4, #44]
	ldr	r0, [pc, #140]
	str	r0, [r4, #48]
	ldr	r0, [pc, #140]
	movs	r1, #0
	strb	r0, [r4, #12]
	str	r1, [sp, #0]
	ldr	r2, [pc, #136]
	mov	r0, sp
	adds	r1, r5, #0
	bl	Func_08006864
	movs	r0, #1
	strb	r0, [r5, #1]
	movs	r0, #17
	strb	r0, [r5, #28]
	adds	r1, r5, #0
	adds	r1, #65
	movs	r0, #2
	strb	r0, [r1, #0]
	adds	r1, #27
	movs	r0, #34
	strb	r0, [r1, #0]
	adds	r1, #37
	movs	r0, #3
	strb	r0, [r1, #0]
	adds	r1, #27
	movs	r0, #68
	strb	r0, [r1, #0]
	adds	r1, #36
	movs	r0, #4
	strb	r0, [r1, #1]
	movs	r0, #136
	strb	r0, [r1, #28]
	str	r6, [r4, #0]
.L0:
	add	sp, #4
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x04000084
	.4byte 0x04000080
	.4byte 0x04000063
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte 0x02004000
	.4byte 0x080fb519
	.4byte 0x080fa1d5
	.4byte 0x080fa1e9
	.4byte 0x080fb671
	.4byte 0x080fa16d
	.4byte 0x080fa799
	.4byte 0x080f9ef9
	.4byte 0x080fab7d
	.4byte 0x080fac45
	.4byte 0x080fae59
	.4byte 0x080fada1
	.4byte 0x080facf9
	.4byte 0x00000000
	.4byte 0x05000040
