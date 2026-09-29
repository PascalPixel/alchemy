.syntax unified
	.thumb
	.global	BattleFx_UpdatePairedArcSpawner
	.thumb_func
BattleFx_UpdatePairedArcSpawner:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	adds	r1, r5, #0
	adds	r1, #102
	ldrh	r3, [r1]
	adds	r2, r3, #1
	lsls	r3, r3, #16
	strh	r2, [r1]
	asrs	r0, r3, #16
	movs	r2, #237
	ldr	r3, [pc, #68]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne	.L0_0
	movs	r1, #7
	bl	Math_Mod
	cmp	r0, #0
	bne	.L0_1
	adds	r0, r5, #0
	bl	BattleFx_SpawnDescendingArcParticles
	b	.L0_1
.L0_0:
	movs	r1, #5
	bl	Math_Mod
	cmp	r0, #0
	bne	.L0_1
	adds	r0, r5, #0
	bl	BattleFx_SpawnDescendingArcParticles
.L0_1:
	cmp	r6, #1
	bne	.L0_2
	ldrh	r3, [r5, #6]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L0_2:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte	0x0000
	.4byte	0x02000240
	.4byte	0x00000001
