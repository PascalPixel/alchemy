@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b5534
	.thumb_func
Func_080b5534:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #0
	movs	r0, #0
	sub	sp, #128
	mov	sl, r2
	bl	Func_08077008
	mov	r5, sp
	adds	r6, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #356]
	bl	Func_08015020
	mov	r2, sl
	ldrh	r3, [r5, r2]
	strb	r3, [r6, #0]
	lsls	r3, r3, #16
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r2, r6, #0
	adds	r1, r5, #0
.L1:
	adds	r0, #1
	cmp	r0, #13
	bgt.n	.L0
	adds	r1, #2
	ldrh	r3, [r1, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L1
.L0:
	movs	r3, #0
	strb	r3, [r6, #14]
	bl	Func_08015000
	movs	r0, #71
	bl	Func_080f9010
	movs	r5, #0
	ldr	r2, [pc, #300]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	ldr	r3, [pc, #296]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #2
.L18:
	strb	r2, [r3, #0]
	bl	Func_08015118
	ldr	r0, [pc, #284]
	movs	r1, #5
	bl	Func_08015120
	movs	r0, #0
	movs	r1, #3
	bl	Func_08015120
	movs	r0, #1
	movs	r1, #1
	bl	Func_08015120
	movs	r0, #1
	movs	r1, #2
	bl	Func_08015120
	movs	r0, #2
	movs	r1, #4
	bl	Func_08015120
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L2
	ldr	r3, [pc, #240]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #10
	b.n	.L3
.L2:
	ldr	r3, [pc, #236]
	movs	r1, #2
	adds	r0, r5, r3
	movs	r2, #2
.L3:
	movs	r3, #4
	bl	Func_08015038
	adds	r7, r0, #0
	movs	r0, #10
	bl	Func_080030f8
	ldr	r2, [pc, #216]
	ldr	r3, [pc, #208]
	ldr	r6, [pc, #216]
	subs	r2, r2, r3
	mov	r8, r2
	b.n	.L4
.L17:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L5
	ldr	r3, [r6, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L6
.L5:
	movs	r0, #1
	bl	Func_080030f8
.L4:
	ldr	r3, [r6, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L8
	movs	r3, #0
	mov	sl, r3
	b.n	.L7
.L8:
	movs	r2, #1
	adds	r5, #1
	mov	sl, r2
.L7:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	adds	r5, #1
.L9:
	ldr	r3, [r6, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	subs	r5, #2
.L10:
	ldr	r3, [r6, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	movs	r3, #1
	mov	sl, r3
.L11:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L12
	movs	r2, #0
	mov	sl, r2
.L12:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	adds	r5, #10
.L13:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	subs	r5, #10
.L14:
	cmp	r5, #0
	bge.n	.L15
	movs	r5, #0
.L15:
	mov	r3, r8
	adds	r3, #5
	cmp	r5, r3
	bcc.n	.L16
	adds	r5, r3, #0
.L16:
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #64]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L17
.L6:
	movs	r0, #1
	bl	Func_08015148
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08015018
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #0
	b.n	.L18
	.4byte 0x00000903
	.4byte 0x00001341
	.4byte 0x02000240
	.4byte 0x000003e7
	.4byte 0x000026fa
	.4byte 0x00000ad0
	.4byte 0x00002850
	.4byte 0x03001b04
	.4byte 0x000003f2
	.4byte 0x03001e8c
	.4byte 0x000012f8
