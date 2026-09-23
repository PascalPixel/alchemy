.syntax unified
	.thumb
	.global MusicPlayer_SetPitchAndUpdateFrequency
	.global Func_080fb2a4
	.thumb_func
MusicPlayer_SetPitchAndUpdateFrequency:
Func_080fb2a4:
	push	{r4, lr}
	adds	r2, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	ldr	r3, [r2, #52]
	ldr	r0, [pc, #24]
	cmp	r3, r0
	bne.n	.L_080fb2c0
	strh	r1, [r2, #30]
	ldrh	r4, [r2, #28]
	adds	r0, r1, #0
	muls	r0, r4
	asrs	r0, r0, #8
	strh	r0, [r2, #32]
.L_080fb2c0:
	pop	{r4}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x68736d53
