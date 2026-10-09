@ C2: Nintendo MusicPlayer2000 RealClearChain and ply_fine routines, checked
@ against pret's public m4a_1.s. These ordinary Thumb routines are library
@ assembly, not claimed handwritten game code.
.syntax unified
	.thumb
	.global SoundNote_Detach
	.thumb_func
SoundNote_Detach:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L_080f9a4e
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L_080f9a42
	str	r1, [r2, #52]
	b.n	.L_080f9a44
.L_080f9a42:
	str	r1, [r3, #32]
.L_080f9a44:
	cmp	r1, #0
	beq.n	.L_080f9a4a
	str	r2, [r1, #48]
.L_080f9a4a:
	movs	r1, #0
	str	r1, [r0, #44]
.L_080f9a4e:
	bx	lr
	.global MusicTrack_FinishSequence
	.thumb_func
MusicTrack_FinishSequence:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9a74
.L_080f9a5a:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_080f9a68
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_080f9a68:
	adds	r0, r4, #0
	bl	SoundNote_Detach
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9a5a
.L_080f9a74:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
AlchemyUncredited_080f9a7e:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9a7e:
