.syntax unified
	.thumb
	.global RunBattleEffect16
	.global Func_0809b698
	.thumb_func
RunBattleEffect16:
Func_0809b698:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #316]
	ldr	r3, [r3, #0]
	sub	sp, #8
	str	r3, [sp, #4]
	ldr	r1, [r3, #16]
	ldr	r6, [r1, #80]
	ldrh	r3, [r1, #6]
	ldr	r2, [r6, #40]
	mov	sl, r1
	str	r3, [sp, #0]
	mov	r9, r2
	bl	Resource_FindFreeEntry
	ldr	r2, [pc, #292]
	ldr	r1, [sp, #4]
	adds	r3, r1, r2
	movs	r1, #0
	mov	r8, r1
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	lsls	r1, r1, #1
	ldr	r2, [pc, #280]
	asrs	r0, r0, #16
	bl	VramBlock_LoadCached
	ldr	r5, [pc, #276]
	movs	r3, #145
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #150
	lsls	r3, r3, #20
	str	r3, [r2, #0]
	ldr	r0, [pc, #264]
	bl	GameFlag_IsSet
	movs	r1, #146
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strb	r0, [r3, #0]
	movs	r1, #0
	mov	r0, sl
	bl	Animation_ApplyChildValuesFar
	ldr	r3, [pc, #248]
	mov	r2, sl
	mov	r5, sl
	str	r3, [r2, #108]
	adds	r5, #100
	mov	r3, r8
	strh	r3, [r5, #0]
	mov	r3, sl
	mov	r1, r8
	adds	r3, #102
	strh	r1, [r3, #0]
	movs	r0, #140
	bl	Audio_PlayCue
	movs	r0, #15
	bl	WaitFrames
	movs	r3, #1
	strh	r3, [r5, #0]
	movs	r0, #10
	bl	WaitFrames
	movs	r2, #38
	adds	r2, r2, r6
	movs	r3, #7
	mov	r8, r2
	adds	r6, #37
	movs	r7, #1
	movs	r5, #19
	mov	fp, r3
.L_0809b73a:
	mov	r1, fp
	mov	r2, r9
	strb	r1, [r2, #5]
	movs	r0, #2
	strb	r7, [r6, #0]
	bl	WaitFrames
	movs	r3, #0
	mov	r1, r9
	mov	r2, r8
	strb	r7, [r6, #0]
	strb	r3, [r1, #5]
	strb	r7, [r2, #0]
	movs	r0, #3
	subs	r5, #1
	bl	WaitFrames
	cmp	r5, #0
	bge.n	.L_0809b73a
	mov	r2, sp
	ldrh	r2, [r2, #0]
	ldr	r5, [pc, #148]
	movs	r3, #0
	mov	r1, sl
	str	r3, [r1, #108]
	mov	r3, sl
	movs	r1, #200
	strh	r2, [r3, #6]
	lsls	r1, r1, #4
	adds	r0, r5, #0
	bl	Scheduler_AddOrUpdateCallback
	movs	r0, #15
	bl	WaitFrames
	movs	r0, #174
	bl	Audio_PlayCue
	movs	r0, #55
	bl	WaitFrames
	adds	r0, r5, #0
	bl	Scheduler_RemoveCallback
	ldr	r3, [pc, #92]
	movs	r1, #147
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_0809b7ac
	mov	r0, sl
	movs	r1, #2
	bl	ObjectDispatch_SetSingleChildField26Far
	b.n	.L_0809b7b4
.L_0809b7ac:
	mov	r0, sl
	movs	r1, #1
	bl	ObjectDispatch_SetSingleChildField26Far
.L_0809b7b4:
	mov	r0, sl
	movs	r1, #0
	bl	Animation_ApplyChildValuesFar
	ldr	r2, [pc, #40]
	ldr	r1, [sp, #4]
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	Resource_ResetEntry
	ldr	r0, [pc, #52]
	movs	r1, #1
	bl	UiText_DrawMessage
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x0000071a
	.4byte 0x0809c510
	.4byte 0x02000240
	.4byte 0x00000145
	.4byte 0x0809b5dd
	.4byte 0x0809b589
	.4byte 0x00000922
