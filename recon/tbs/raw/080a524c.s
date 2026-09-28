.syntax unified
	.thumb
	.global Func_080a524c
	.thumb_func
Func_080a524c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #3
	movs	r2, #17
	adds	r5, r0, #0
	movs	r3, #10
	movs	r0, #13
	bl	UiWindow_CreateFar
	ldr	r3, [pc, #264]
	ands	r5, r3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	Item_Get
	ldr	r3, [pc, #256]
	adds	r5, r5, r3
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #0
	bl	UiText_DrawAt
	ldr	r5, [pc, #244]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	adds	r5, #1
	bl	UiText_DrawAt
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #24
	bl	UiText_DrawAt
	ldr	r5, [pc, #220]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #24
	movs	r3, #40
	adds	r5, #1
	bl	UiText_DrawAt
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #24
	movs	r3, #56
	bl	UiText_DrawAt
	movs	r6, #1
	movs	r0, #104
	movs	r1, #86
	mov	r8, r6
	bl	UiMenu_SlideCursor
	b.n	.L_080a5306
.L_080a52c8:
	lsls	r1, r6, #4
	adds	r1, #70
	movs	r0, #104
	bl	UiMenu_PositionCursor
	ldr	r5, [pc, #172]
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a52ea
	movs	r2, #1
	movs	r0, #111
	subs	r6, #1
	mov	r8, r2
	bl	Audio_PlayCue
.L_080a52ea:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a5300
	movs	r3, #1
	movs	r0, #111
	adds	r6, #1
	mov	r8, r3
	bl	Audio_PlayCue
.L_080a5300:
	movs	r0, #1
	bl	WaitFrames
.L_080a5306:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	GameFlag_IsSet
	cmp	r0, #0
	bne.n	.L_080a534c
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L_080a5326
	movs	r3, #0
	adds	r0, r6, #2
	movs	r1, #2
	mov	r8, r3
	bl	__modsi3
	adds	r6, r0, #0
.L_080a5326:
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a533a
	movs	r0, #112
	bl	Audio_PlayCue
	b.n	.L_080a534c
.L_080a533a:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a52c8
	movs	r0, #113
	bl	Audio_PlayCue
	movs	r6, #1
.L_080a534c:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	GameFlag_IsSet
	cmp	r0, #0
	beq.n	.L_080a535a
	movs	r6, #1
.L_080a535a:
	adds	r0, r7, #0
	movs	r1, #1
	bl	UiWork_FinalizeFar
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x000001ff
	.4byte 0x00000182
	.4byte 0x00000ad4
	.4byte 0x00000b2c
	.4byte 0x03001b04
	.4byte 0x03001c94
