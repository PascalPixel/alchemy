.syntax unified
	.thumb
	.global RunBattleEffect07
	.global Func_08098954
	.thumb_func
RunBattleEffect07:
Func_08098954:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #272]
	ldr	r3, [r3, #0]
	sub	sp, #20
	mov	sl, r3
	bl	BattleEffect_InitializeSharedScene
	mov	r3, sl
	ldr	r0, [r3, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	ldr	r1, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r1, r1, r3
	str	r1, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	BattleFx_SpawnItemBreakMode3
	ldr	r3, [pc, #224]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	ldr	r2, [r5, #8]
	movs	r3, #0
	bl	BattleFx_SpawnItemBreakMode3
	str	r0, [sp, #4]
	movs	r0, #15
	mov	fp, sp
	bl	WaitFrames
	movs	r0, #1
	mov	r7, fp
	mov	r8, r0
.L_080989b6:
	ldmia	r7!, {r6}
	cmp	r6, #0
	beq.n	.L_080989c8
	movs	r1, #192
	ldrh	r2, [r6, #6]
	adds	r0, r6, #0
	lsls	r1, r1, #13
	bl	Motion_SetTargetPositionFromMagnitudeAngle
.L_080989c8:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080989b6
	ldr	r0, [sp, #0]
	bl	Object_CommitPosition
	movs	r0, #134
	bl	Audio_PlayCue
	movs	r0, #128
	movs	r3, #23
	lsls	r0, r0, #10
	adds	r7, r5, #0
	mov	r8, r3
	mov	r9, r0
.L_080989ec:
	mov	r3, sl
	ldr	r1, [r3, #4]
	str	r1, [r7, #0]
	movs	r0, #128
	ldr	r2, [r3, #8]
	lsls	r0, r0, #13
	adds	r2, r2, r0
	str	r2, [r7, #4]
	ldr	r3, [r3, #12]
	ldr	r0, [pc, #124]
	str	r3, [r7, #8]
	bl	Object_Spawn
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_08098a44
	ldr	r1, [pc, #112]
	bl	Engine_ObjectSetScript
	bl	Random16
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	Random16
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	Random16
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	Motion_SetTargetPositionFromMagnitudeAngle
.L_08098a44:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080989ec
	ldr	r0, [sp, #0]
	bl	Object_Destroy
	mov	r3, fp
	ldr	r0, [r3, #4]
	bl	Object_Destroy
	bl	BattleFx_PrepareBufferInterpolation
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0xffe00000
	.4byte 0x0000011d
	.4byte 0x0809f0d4
