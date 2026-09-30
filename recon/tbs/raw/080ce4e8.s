.syntax unified
	.thumb
	.global Unnamed_080ce4e8
	.global Func_080ce4e8
	.thumb_func
Unnamed_080ce4e8:
Func_080ce4e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #816]
	mov	r8, r1
	mov	r3, r8
	ldmia	r3!, {r2}
	ldr	r3, [r3, #0]
	sub	sp, #72
	str	r3, [sp, #44]
	ldr	r3, [pc, #804]
	mov	sl, r2
	add	r3, sl
	str	r0, [r3, #0]
	movs	r0, #0
	bl	BattleFx_BeginCanvasLayer
	ldr	r0, [pc, #796]
	bl	Resource_GetTableEntry
	adds	r6, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r5, [pc, #788]
	adds	r1, r6, #0
	lsls	r0, r0, #19
	adds	r6, #128
	bl	_call_via_r5
	mov	r1, sl
	adds	r0, r6, #0
	bl	Resource_DecodeType01
	ldr	r0, [pc, #772]
	bl	Resource_GetTableEntry
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	_call_via_r5
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Unnamed_080ed408
	mov	r3, r8
	ldr	r3, [r3, #28]
	movs	r6, #3
	str	r3, [sp, #32]
	movs	r2, #7
	movs	r3, #3
	movs	r1, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Unnamed_080ed408
	mov	r5, r8
	ldr	r5, [r5, #32]
	str	r5, [sp, #36]
	movs	r5, #144
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r0, [pc, #704]
	bl	Scheduler_AddOrUpdateCallback
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	ldr	r2, [pc, #696]
	str	r6, [r3, #0]
	ldr	r3, [pc, #696]
	add	r2, sl
	adds	r1, r5, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #692]
	ldr	r5, [pc, #696]
	bl	Scheduler_AddOrUpdateCallback
	movs	r6, #255
	movs	r4, #0
.L_080ce59c:
	str	r4, [sp, #8]
	bl	Random16
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	Random16
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	Random16
	ands	r0, r6
	ldr	r4, [sp, #8]
	subs	r0, #127
	movs	r1, #128
	lsls	r0, r0, #15
	adds	r4, #1
	lsls	r1, r1, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	r4, r1
	bne.n	.L_080ce59c
	movs	r0, #142
	bl	Audio_PlayCue
	movs	r2, #0
	ldr	r3, [pc, #592]
	str	r2, [sp, #40]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r5, #96
	lsls	r3, r3, #5
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080ce5ee
	b.n	.L_080ce7fa
.L_080ce5ee:
	ldr	r3, [pc, #608]
	ldr	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #28]
	cmp	r1, #96
	bne.n	.L_080ce600
	movs	r0, #0
	bl	BattleEventRuntime_BeginPhaseFar
.L_080ce600:
	ldr	r3, [pc, #552]
	add	r3, sl
	ldr	r3, [r3, #0]
	movs	r6, #211
	ldr	r3, [r3, #4]
	lsls	r6, r6, #7
	add	r6, sl
	cmp	r3, #0
	bne.n	.L_080ce640
	ldr	r2, [sp, #40]
	movs	r4, #0
	lsls	r5, r2, #11
.L_080ce618:
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Trig_Sin
	lsls	r2, r0, #1
	adds	r2, r2, r0
	movs	r3, #192
	lsls	r2, r2, #1
	lsls	r3, r3, #11
	subs	r3, r3, r2
	asrs	r3, r3, #10
	ldr	r4, [sp, #8]
	stmia	r6!, {r3}
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r4, #1
	adds	r5, r5, r3
	cmp	r4, #160
	bne.n	.L_080ce618
	b.n	.L_080ce666
.L_080ce640:
	ldr	r1, [sp, #40]
	movs	r4, #0
	lsls	r5, r1, #11
.L_080ce646:
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Trig_Sin
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	movs	r2, #128
	asrs	r3, r3, #10
	lsls	r2, r2, #4
	adds	r4, #1
	stmia	r6!, {r3}
	adds	r5, r5, r2
	cmp	r4, #160
	bne.n	.L_080ce646
.L_080ce666:
	ldr	r2, [pc, #452]
	movs	r3, #0
	mov	r5, sl
	mov	fp, r3
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080ce678
	b.n	.L_080ce7d0
.L_080ce678:
	ldr	r1, [sp, #28]
	adds	r1, #12
	str	r1, [sp, #24]
	movs	r3, #48
	movs	r5, #36
	movs	r1, #0
	add	r3, sp
	str	r5, [sp, #16]
	str	r1, [sp, #12]
	mov	r9, r3
.L_080ce68c:
	mov	r3, sl
	adds	r6, r3, r2
	ldr	r5, [sp, #16]
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r5]
	bl	GetBattleObjectSlotFar
	ldr	r5, [r0, #0]
	bl	Render_ResetTransformState
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #24]
	bl	Graphics_PrepareTransferInIwramWork
	ldr	r3, [r5, #8]
	mov	r2, r9
	str	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r2, #4]
	ldr	r3, [r5, #16]
	mov	r0, r9
	str	r3, [r2, #8]
	bl	SceneTransform_ApplyPosition
	mov	r3, fp
	ldr	r1, [sp, #40]
	lsls	r5, r3, #5
	cmp	r1, r5
	ble.n	.L_080ce7ae
	lsls	r0, r1, #9
	bl	SceneTransform_ApplyPitch
	adds	r3, r5, #0
	ldr	r2, [sp, #40]
	adds	r3, #32
	cmp	r2, r3
	bne.n	.L_080ce6ec
	ldr	r3, [r6, #0]
	ldr	r5, [sp, #16]
	ldrsh	r0, [r3, r5]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, fp
	bl	ObjectGroup_UpdateMembers
.L_080ce6ec:
	mov	r2, fp
	lsls	r2, r2, #3
	movs	r3, #60
	ldr	r5, [sp, #12]
	ldr	r1, [pc, #340]
	add	r3, sp
	str	r2, [sp, #20]
	movs	r4, #0
	mov	r8, r3
	adds	r6, r5, r1
.L_080ce700:
	ldr	r2, [sp, #20]
	ldr	r5, [sp, #40]
	adds	r3, r2, r4
	lsls	r3, r3, #2
	cmp	r5, r3
	ble.n	.L_080ce7a6
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	str	r4, [sp, #8]
	ldr	r3, [pc, #292]
	bl	_call_via_r3
	asrs	r7, r0, #8
	ldr	r4, [sp, #8]
	cmp	r7, #0
	beq.n	.L_080ce7a6
	mov	r1, r8
	adds	r0, r6, #0
	bl	EffectPosition_ApplyBaseAndYOffset
	mov	r2, r8
	ldr	r5, [r2, #0]
	ldr	r4, [sp, #8]
	asrs	r5, r5, #1
	adds	r0, r4, #0
	str	r5, [r2, #0]
	movs	r1, #3
	bl	__modsi3
	mov	r2, r8
	lsls	r1, r0, #3
	ldr	r3, [r2, #4]
	adds	r1, r1, r0
	movs	r2, #24
	subs	r5, #12
	lsls	r1, r1, #6
	subs	r3, #12
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r2, r5, #0
	add	r1, sl
	ldr	r5, [sp, #32]
	bl	_call_via_r5
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	__divsi3
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	__divsi3
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	__divsi3
	ldr	r3, [r6, #24]
	subs	r5, r5, r0
	adds	r3, #1
	str	r5, [r6, #8]
	str	r3, [r6, #24]
	ldr	r4, [sp, #8]
.L_080ce7a6:
	adds	r4, #1
	adds	r6, #28
	cmp	r4, #8
	bne.n	.L_080ce700
.L_080ce7ae:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r2, r2, r3
	adds	r1, #2
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r2, [pc, #108]
	mov	r1, sl
	ldr	r3, [r1, r2]
	movs	r5, #1
	ldr	r3, [r3, #20]
	add	fp, r5
	cmp	fp, r3
	beq.n	.L_080ce7d0
	b.n	.L_080ce68c
.L_080ce7d0:
	bl	ObjectGroup_TickMemberTimers
	ldr	r2, [pc, #128]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	WaitFrames
	ldr	r2, [sp, #40]
	ldr	r3, [pc, #68]
	adds	r2, #1
	str	r2, [sp, #40]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #5
	adds	r3, #96
	cmp	r2, r3
	beq.n	.L_080ce7fa
	b.n	.L_080ce5ee
.L_080ce7fa:
	ldr	r0, [pc, #76]
	bl	Scheduler_RemoveCallback
	ldr	r0, [pc, #56]
	bl	Scheduler_RemoveCallback
	movs	r0, #47
	bl	Runtime_ReleaseHeapBlock
	movs	r0, #46
	bl	Runtime_ReleaseHeapBlock
	bl	BattleFx_EndCanvasLayer
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000a9
	.4byte 0x03001388
	.4byte 0x000000bb
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x03001e80
	.4byte 0x030001d8
	.4byte 0x00007824
