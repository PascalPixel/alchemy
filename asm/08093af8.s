@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08093af8
	.thumb_func
Func_08093af8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #40
	mov	r9, r3
	ldr	r3, [pc, #220]
	movs	r2, #0
	mov	sl, r2
	ldr	r5, [r3, #0]
	movs	r2, #63
	adds	r7, r0, #0
	mov	fp, r1
	mov	r8, r2
.L6:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L0
	cmp	r5, r7
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r1, [r5, #12]
	ldr	r3, [r7, #12]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L1
	ldr	r3, [pc, #176]
	cmp	r2, r3
	ble.n	.L2
	b.n	.L0
.L1:
	ldr	r2, [pc, #168]
	subs	r3, r3, r1
	cmp	r3, r2
	bgt.n	.L0
.L2:
	ldr	r2, [r5, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L3
	ldr	r3, [pc, #156]
	adds	r0, r0, r3
.L3:
	ldr	r2, [r5, #16]
	ldr	r3, [r7, #16]
	subs	r2, r2, r3
	asrs	r0, r0, #16
	cmp	r2, #0
	bge.n	.L4
	ldr	r3, [pc, #140]
	adds	r2, r2, r3
.L4:
	asrs	r3, r2, #16
	adds	r2, r0, #0
	muls	r2, r0
	adds	r0, r2, #0
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #124]
	bl	Func_080072f0
	adds	r6, r0, #0
	cmp	r6, r9
	bge.n	.L0
	ldr	r3, [r7, #16]
	ldr	r0, [r5, #16]
	ldr	r1, [r5, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r6, #23
	ble.n	.L5
	ldrh	r3, [r7, #6]
	subs	r3, r0, r3
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	ldr	r3, [pc, #84]
	cmp	r0, r3
	blt.n	.L0
	ldr	r2, [pc, #80]
	cmp	r0, r2
	bgt.n	.L0
.L5:
	mov	sl, r5
	mov	r9, r6
.L0:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r2, r8
	adds	r5, #112
	cmp	r2, #0
	bge.n	.L6
	mov	r3, sl
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L7
	mov	r2, sl
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, fp
	bne.n	.L7
	mov	r0, sl
.L7:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e64
	.4byte 0x002fffff
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0xffffd001
	.4byte 0x00002fff
