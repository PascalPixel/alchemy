.syntax unified
	.thumb
	.global	BattleFx_UpdateDescendingParticlePositiveArc
	.thumb_func
BattleFx_UpdateDescendingParticlePositiveArc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #100]
	movs	r5, #237
	lsls	r5, r5, #1
	ldr	r1, [r0, #20]
	movs	r2, #160
	lsls	r2, r2, #12
	adds	r3, r3, r5
	adds	r4, r1, r2
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	ldr	r3, [pc, #84]
	ldr	r6, [r0, #104]
	cmp	r2, r3
	bne	.L0_0
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r4, r1, r2
.L0_0:
	ldr	r5, [r0, #12]
	cmp	r5, r4
	bgt	.L0_1
	bl	Object_Destroy
	b	.L0_2
.L0_1:
	ldr	r3, [r0, #24]
	movs	r4, #192
	lsls	r4, r4, #4
	movs	r1, #128
	adds	r2, r3, r4
	lsls	r1, r1, #9
	cmp	r2, r1
	ble	.L0_3
	adds	r2, r1, #0
.L0_3:
	str	r2, [r0, #24]
	str	r2, [r0, #28]
	ldr	r4, [pc, #40]
	ldr	r3, [r6, #8]
	str	r3, [r0, #8]
	adds	r3, r5, r4
	str	r3, [r0, #12]
	subs	r3, r1, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r6, #16]
	movs	r5, #144
	adds	r3, r3, r2
	lsls	r5, r5, #12
	adds	r3, r3, r5
	str	r3, [r0, #16]
.L0_2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte	0x02000240
	.4byte	0x00000001
	.4byte	0xfffe0000
	.global	BattleFx_UpdateDescendingParticleNegativeArc
	.thumb_func
BattleFx_UpdateDescendingParticleNegativeArc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #104]
	movs	r5, #237
	lsls	r5, r5, #1
	ldr	r1, [r0, #20]
	movs	r2, #160
	lsls	r2, r2, #12
	adds	r3, r3, r5
	adds	r4, r1, r2
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	ldr	r3, [pc, #88]
	ldr	r6, [r0, #104]
	cmp	r2, r3
	bne	.L1_0
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r4, r1, r2
.L1_0:
	ldr	r5, [r0, #12]
	cmp	r5, r4
	bgt	.L1_1
	bl	Object_Destroy
	b	.L1_2
.L1_1:
	ldr	r3, [r0, #24]
	movs	r4, #192
	lsls	r4, r4, #4
	movs	r1, #128
	adds	r2, r3, r4
	lsls	r1, r1, #9
	cmp	r2, r1
	ble	.L1_3
	adds	r2, r1, #0
.L1_3:
	negs	r3, r2
	str	r2, [r0, #24]
	str	r3, [r0, #28]
	ldr	r4, [pc, #40]
	ldr	r3, [r6, #8]
	str	r3, [r0, #8]
	adds	r3, r5, r4
	str	r3, [r0, #12]
	subs	r3, r1, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r6, #16]
	movs	r5, #128
	subs	r3, r3, r2
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #16]
.L1_2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte	0x0000
	.4byte	0x02000240
	.4byte	0x00000001
	.4byte	0xfffe0000
