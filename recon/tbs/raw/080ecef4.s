.syntax unified
	.thumb
	.global BattleFx_RunRevealColumn
	.global Func_080ecef4
	.thumb_func
BattleFx_RunRevealColumn:
Func_080ecef4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #56]
	ldmia	r3!, {r7}
	ldr	r2, [pc, #56]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	mov	r8, r3
	adds	r3, r7, r2
	str	r5, [r3, #0]
	movs	r0, #0
	sub	sp, #16
	adds	r6, r1, #0
	bl	BattleFx_BeginCanvasLayer
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	cmp	r6, #0
	bne.n	.L_080ecf48
	ldr	r0, [pc, #32]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Resource_LoadAndDecompress
	ldr	r0, [pc, #24]
	b.n	.L_080ecf5a
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000050
	.4byte 0x0000004f
	.2byte 0x0050
	.2byte 0x0000
.L_080ecf48:
	cmp	r6, #1
	bne.n	.L_080ecf66
	ldr	r0, [pc, #380]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Resource_LoadAndDecompress
	ldr	r0, [pc, #372]
.L_080ecf5a:
	ldr	r1, [pc, #376]
	movs	r2, #1
	movs	r3, #1
	bl	Resource_LoadAndDecompress
	b.n	.L_080ecf7e
.L_080ecf66:
	ldr	r0, [pc, #368]
	adds	r1, r7, #0
	movs	r2, #1
	movs	r3, #0
	bl	Resource_LoadAndDecompress
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #348]
	movs	r2, #1
	movs	r3, #1
	bl	Resource_LoadAndDecompress
.L_080ecf7e:
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r4, [pc, #348]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r1, #144
	adds	r2, r7, r4
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #336]
	lsls	r1, r1, #3
	bl	Engine_ScheduleCallback
	cmp	r6, #1
	bne.n	.L_080ecfb0
	movs	r3, #128
	movs	r2, #36
	ldrsh	r1, [r5, r2]
	ldr	r0, [r5, #8]
	lsls	r3, r3, #12
	movs	r2, #16
	bl	BattleMotion_ApproachTargetFar
	b.n	.L_080ecfbe
.L_080ecfb0:
	movs	r3, #36
	ldrsh	r1, [r5, r3]
	ldr	r0, [r5, #8]
	movs	r2, #16
	movs	r3, #0
	bl	BattleMotion_ApproachTargetFar
.L_080ecfbe:
	movs	r0, #16
	bl	WaitFrames
	ldr	r4, [pc, #288]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080ecfe2
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Unnamed_080ed408
	b.n	.L_080ecff2
.L_080ecfe2:
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Unnamed_080ed408
.L_080ecff2:
	ldr	r3, [pc, #248]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r0, #212
	str	r3, [sp, #8]
	bl	Audio_PlayCue
	movs	r5, #0
	movs	r6, #120
.L_080ed004:
	cmp	r5, #3
	bgt.n	.L_080ed012
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	adds	r1, r7, #0
	b.n	.L_080ed030
.L_080ed012:
	cmp	r5, #7
	bgt.n	.L_080ed024
	movs	r2, #225
	lsls	r2, r2, #6
	adds	r1, r7, r2
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	b.n	.L_080ed030
.L_080ed024:
	cmp	r5, #11
	bgt.n	.L_080ed03c
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #164]
.L_080ed030:
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	_call_via_r4
	b.n	.L_080ed052
.L_080ed03c:
	cmp	r5, #15
	bgt.n	.L_080ed052
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	mov	r0, r8
	ldr	r1, [pc, #168]
	movs	r2, #0
	movs	r3, #0
	ldr	r4, [sp, #8]
	bl	_call_via_r4
.L_080ed052:
	adds	r3, r5, #0
	subs	r3, #16
	cmp	r3, #3
	bhi.n	.L_080ed068
	movs	r1, #128
	ldr	r3, [pc, #148]
	mov	r0, r8
	lsls	r1, r1, #7
	ldr	r2, [pc, #148]
	bl	_call_via_r3
.L_080ed068:
	cmp	r5, #18
	bne.n	.L_080ed072
	movs	r0, #134
	bl	BattleEventRuntime_BeginPhaseFar
.L_080ed072:
	cmp	r5, #20
	bne.n	.L_080ed08e
	ldr	r3, [pc, #132]
	ldr	r4, [pc, #108]
	adds	r2, r7, r3
	movs	r3, #8
	str	r3, [r2, #0]
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	BattleMotion_ApplyVariantMotionFar
.L_080ed08e:
	movs	r0, #16
	movs	r1, #16
	bl	Camera_ApplyShake
	bl	ObjectGroup_TickMemberTimers
	ldr	r3, [pc, #100]
	adds	r2, r7, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r5, #1
	bl	WaitFrames
	cmp	r5, #21
	bne.n	.L_080ed004
	movs	r0, #46
	bl	Runtime_ReleaseHeapBlock
	ldr	r0, [pc, #44]
	bl	Scheduler_RemoveCallback
	bl	BattleFx_EndCanvasLayer
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000004d
	.4byte 0x0000004e
	.4byte 0x02010000
	.4byte 0x0000004b
	.4byte 0x0000004c
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x02013840
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x000077a8
	.4byte 0x00007824
