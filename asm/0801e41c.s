@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e41c
	.thumb_func
Func_0801e41c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r3
	ldr	r3, [pc, #676]
	ldr	r3, [r3, #0]
	adds	r7, r1, #0
	adds	r5, r0, #0
	mov	r8, r2
	ldr	r6, [sp, #28]
	mov	r9, r3
	cmp	r7, sl
	beq.n	.L0
	b.n	.L1
.L0:
	cmp	r8, r6
	bne.n	.L2
	b.n	.L3
.L2:
	cmp	r8, r6
	bls.n	.L4
	mov	r4, r8
	mov	r8, r6
	adds	r6, r4, #0
.L4:
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	mov	r2, r8
	subs	r3, r6, r2
	add	r0, sl
	movs	r2, #1
	add	r1, r8
	bl	Func_0801e260
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	add	r3, r8
	lsls	r3, r3, #5
	adds	r3, r3, r2
	add	r3, sl
	lsls	r3, r3, #1
	mov	r2, r9
	mov	r4, r8
	adds	r0, r3, r2
	cmp	r4, r6
	bls.n	.L5
	b.n	.L3
.L5:
	ldrh	r1, [r0, #0]
	cmp	r4, r8
	bne.n	.L6
	ldr	r2, [pc, #596]
	adds	r3, r1, r2
	cmp	r3, #15
	bhi.n	.L7
	ldr	r2, [pc, #592]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801e566
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e4d4
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e4d0
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e566
	ldr	r1, [pc, #520]
	b.n	.L8
	ldr	r1, [pc, #520]
	b.n	.L8
.L6:
	cmp	r4, r6
	bne.n	.L9
	ldr	r2, [pc, #500]
	adds	r3, r1, r2
	cmp	r3, #16
	bhi.n	.L7
	ldr	r2, [pc, #508]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e534
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e530
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e564
	.4byte 0x0801e566
	ldr	r1, [pc, #436]
	b.n	.L8
	ldr	r1, [pc, #436]
	b.n	.L8
.L9:
	ldr	r2, [pc, #408]
	adds	r3, r1, r2
	cmp	r3, #5
	bhi.n	.L7
	ldr	r2, [pc, #428]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e560
	.4byte 0x0801e560
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e566
	.4byte 0x0801e560
	ldr	r1, [pc, #400]
	b.n	.L8
.L7:
	ldr	r1, [pc, #400]
.L8:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #64
	cmp	r4, r6
	bls.n	.L5
	b.n	.L3
.L1:
	cmp	r8, r6
	beq.n	.L10
	b.n	.L3
.L10:
	cmp	r7, sl
	bne.n	.L11
	b.n	.L3
.L11:
	cmp	r7, sl
	bls.n	.L12
	adds	r4, r7, #0
	mov	r7, sl
	mov	sl, r4
.L12:
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	mov	r3, sl
	subs	r2, r3, r7
	adds	r0, r0, r7
	movs	r3, #1
	add	r1, r8
	bl	Func_0801e260
	ldrh	r3, [r5, #14]
	ldrh	r2, [r5, #12]
	add	r3, r8
	lsls	r3, r3, #5
	adds	r3, r3, r2
	adds	r3, r3, r7
	lsls	r3, r3, #1
	mov	r2, r9
	adds	r4, r7, #0
	adds	r0, r3, r2
	cmp	r4, sl
	bls.n	.L13
	b.n	.L3
.L13:
	ldrh	r1, [r0, #0]
	cmp	r4, r7
	bne.n	.L14
	ldr	r2, [pc, #280]
	adds	r3, r1, r2
	cmp	r3, #17
	bhi.n	.L15
	ldr	r2, [pc, #312]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.4byte 0x0801e618
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e614
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b6
	ldr	r1, [pc, #232]
	b.n	.L16
	ldr	r1, [pc, #232]
	b.n	.L16
.L14:
	cmp	r4, sl
	bne.n	.L17
	ldr	r2, [pc, #176]
	adds	r3, r1, r2
	cmp	r3, #18
	bhi.n	.L15
	ldr	r2, [pc, #220]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.4byte 0x0801e680
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e67c
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b4
	.4byte 0x0801e6b6
	ldr	r1, [pc, #140]
	b.n	.L16
	ldr	r1, [pc, #140]
	b.n	.L16
.L17:
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	cmp	r3, #6
	bhi.n	.L15
	ldr	r2, [pc, #132]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0801e6b6
	.4byte 0x0801e6b6
	.4byte 0x0801e6b0
	.4byte 0x0801e6b0
	.4byte 0x0801e6b6
	.4byte 0x0801e6b4
	.4byte 0x0801e6b0
	ldr	r1, [pc, #64]
	b.n	.L16
.L15:
	ldr	r1, [pc, #96]
.L16:
	adds	r4, #1
	strh	r1, [r0, #0]
	adds	r0, #2
	cmp	r4, sl
	bhi.n	.L3
	b.n	.L13
.L3:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0xffff0ff7
	.4byte 0x0801e490
	.4byte 0x0000f018
	.4byte 0x0000f009
	.4byte 0x0801e4ec
	.4byte 0x0000f019
	.4byte 0x0000f00a
	.4byte 0x0801e548
	.4byte 0x0000f00d
	.4byte 0x0000f00f
	.4byte 0x0801e5cc
	.4byte 0x0000f01a
	.4byte 0x0000f00b
	.4byte 0x0801e630
	.4byte 0x0000f01b
	.4byte 0x0000f00c
	.4byte 0x0801e694
	.4byte 0x0000f00e
