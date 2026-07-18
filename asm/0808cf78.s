@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808cf78
	.thumb_func
Func_0808cf78:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #300]
	movs	r0, #225
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #300]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #292]
	ldr	r0, [r3, #12]
	mov	r8, r1
	bl	Func_080072e4
	movs	r6, #1
	movs	r3, #0
	adds	r5, r0, #0
	mov	sl, r3
	negs	r6, r6
	b.n	.L0
.L2:
	adds	r5, #24
.L0:
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, r6
	beq.n	.L1
	cmp	r3, r8
	bne.n	.L2
	movs	r1, #2
	ldrsh	r0, [r5, r1]
	cmp	r0, r6
	beq.n	.L3
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
.L3:
	movs	r2, #1
	mov	sl, r2
.L1:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L4
	ldr	r3, [pc, #232]
	ldr	r0, [r3, #12]
	bl	Func_080072e4
	adds	r5, r0, #0
.L4:
	ldr	r0, [pc, #224]
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L5
	ldr	r1, [pc, #208]
	movs	r4, #238
	lsls	r4, r4, #1
	adds	r2, r1, r4
	movs	r4, #4
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #242
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #8
	ldrsh	r3, [r5, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #244
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r4, #246
	ldrh	r3, [r5, #10]
	lsls	r4, r4, #1
	str	r3, [r2, #0]
	adds	r3, r1, r4
	strh	r0, [r3, #0]
.L5:
	movs	r1, #1
	movs	r0, #14
	ldrsh	r3, [r5, r0]
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L6
	adds	r6, r7, #0
	adds	r6, #236
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	b.n	.L7
.L6:
	adds	r6, r7, #0
	adds	r6, #236
.L7:
	movs	r2, #16
	ldrsh	r3, [r5, r2]
	cmp	r3, r1
	beq.n	.L8
	adds	r4, r7, #0
	adds	r4, #240
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	b.n	.L9
.L8:
	adds	r4, r7, #0
	adds	r4, #240
.L9:
	movs	r0, #18
	ldrsh	r3, [r5, r0]
	cmp	r3, r1
	beq.n	.L10
	adds	r2, r7, #0
	adds	r2, #244
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	b.n	.L11
.L10:
	adds	r2, r7, #0
	adds	r2, #244
.L11:
	movs	r3, #20
	ldrsh	r0, [r5, r3]
	cmp	r0, r1
	beq.n	.L12
	adds	r1, r7, #0
	adds	r1, #248
	lsls	r3, r0, #16
	str	r3, [r1, #0]
	b.n	.L13
.L12:
	adds	r1, r7, #0
	adds	r1, #248
.L13:
	ldr	r3, [r6, #0]
	movs	r0, #240
	lsls	r0, r0, #16
	ldr	r2, [r2, #0]
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L14
	ldr	r0, [pc, #52]
	adds	r3, r2, r0
	str	r3, [r6, #0]
.L14:
	ldr	r3, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [r1, #0]
	cmp	r3, r2
	ble.n	.L15
	ldr	r0, [pc, #36]
	adds	r3, r2, r0
	str	r3, [r4, #0]
.L15:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x02000240
	.4byte 0x02008000
	.4byte 0x00000109
	.4byte 0xff100000
	.4byte 0xff600000
