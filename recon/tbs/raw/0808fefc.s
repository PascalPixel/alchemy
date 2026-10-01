.syntax unified
	.thumb
	.global DisplayTransition_Start
	.thumb_func
DisplayTransition_Start:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #255
	mov r10, r1
	asrs r2, r0, #8
	ldr r1, .L_080900fc
	adds r6, r3, #0
	ands r2, r3
	ldr r7, [r1]
	ands r6, r0
	cmp r2, #4
	bls .L_0808ff1c
	b .L_08090168
.L_0808ff1c:
	lsls r3, r2, #2
	ldr r2, .L_08090100
	ldr r3, [r3, r2]
	mov pc, r3
.L_0808ff24:
	.4byte .L_0808ff38
	.4byte .L_0808ff4c
	.4byte .L_0808ffa2
	.4byte .L_0809003c
	.4byte .L_080900c0
.L_0808ff38:
	movs r0, #0
	bl Blend_SetDarkenTarget16
	mov r0, r10
	bl Blend_SetDarkenTarget0
	movs r0, #1
	bl WaitFrames
	b .L_08090168
.L_0808ff4c:
	movs r3, #160
	lsls r3, r3, #19
	movs r0, #128
	ldrh r1, [r3]
	lsls r0, r0, #8
	bl BattleFx_ApplyColorToSourceBuffer
	mov r0, r10
	bl BattleFx_StartBufferInterpolation
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_08090104
	ldr r4, .L_08090108
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_0808ff98
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_0808ff98:
	strh r5, [r4]
	movs r0, #0
	bl BattleFx_SetPrimaryBufferValue
	b .L_0809019c
.L_0808ffa2:
	bl DisplayTransition_AllocateAndClearState
	movs r1, #165
	adds r5, r0, #0
	lsls r1, r1, #3
	movs r2, #0
	adds r3, r5, r1
	mov r8, r2
	adds r1, #2
	strh r6, [r3]
	mov r2, r8
	adds r3, r5, r1
	strh r2, [r3]
	ldr r3, .L_0809010c
	adds r1, #12
	adds r2, r5, r3
	movs r3, #63
	strh r3, [r2]
	adds r2, r5, r1
	movs r3, #1
	movs r1, #200
	strh r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_08090110
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_08090114
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_08090104
	ldr r4, .L_08090108
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_08090018
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_08090018:
	strh r6, [r4]
	ldr r2, .L_08090118
	adds r3, r5, r2
	mov r1, r8
	strb r1, [r3]
	ldr r3, .L_0809011c
	ldr r1, .L_08090120
	adds r2, r5, r3
	movs r3, #32
	strb r3, [r2]
	adds r3, r5, r1
	mov r2, r10
	adds r1, #1
	strb r2, [r3]
	adds r3, r5, r1
	mov r2, r8
	strb r2, [r3]
	b .L_0809019c
.L_0809003c:
	bl DisplayTransition_AllocateAndClearState
	movs r1, #165
	adds r5, r0, #0
	lsls r1, r1, #3
	adds r3, r5, r1
	ldr r2, .L_08090124
	movs r1, #32
	mov r8, r1
	strh r6, [r3]
	adds r3, r5, r2
	mov r2, r8
	strh r2, [r3]
	movs r0, #15
	bl DisplayTransition_FillTilemapAndSolidTile
	movs r0, #1
	bl WaitFrames
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08090128
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_08090104
	ldr r4, .L_08090108
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_0809009e
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_0809009e:
	strh r6, [r4]
	ldr r1, .L_08090118
	movs r2, #0
	adds r3, r5, r1
	adds r1, #1
	strb r2, [r3]
	adds r3, r5, r1
	mov r1, r8
	strb r1, [r3]
	ldr r1, .L_08090120
	adds r3, r5, r1
	mov r1, r10
	strb r1, [r3]
	ldr r1, .L_0809012c
	adds r3, r5, r1
	strb r2, [r3]
	b .L_0809019c
.L_080900c0:
	ldr r7, [r1]
	bl DisplayTransition_AllocateAndClearState
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, .L_080900f4
	adds r2, r7, r3
	ldr r3, .L_080900f8
	mov r8, r1
	movs r1, #129
	mov r9, r3
	lsls r1, r1, #1
	movs r3, #80
	strh r3, [r2]
	adds r2, r7, r1
	adds r5, r0, #0
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	cmp r6, #0
	bne .L_08090134
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_08090130
	b .L_0809013a
.L_080900f4:
	.4byte 0x00000000
.L_080900f8:
	.4byte 0x00000050
.L_080900fc:
	.4byte gMapWork
.L_08090100:
	.4byte .L_0808ff24
.L_08090104:
	.4byte gIoWriteQueue
.L_08090108:
	.4byte 0x04000208
.L_0809010c:
	.4byte 0x00000534
.L_08090110:
	.4byte DisplayTransition_UpdateScanlineTable
.L_08090114:
	.4byte BattleFx_StartWindowHBlankDma
.L_08090118:
	.4byte 0x0000053a
.L_0809011c:
	.4byte 0x0000053b
.L_08090120:
	.4byte 0x0000053c
.L_08090124:
	.4byte 0x0000052a
.L_08090128:
	.4byte DisplayTransition_UpdateFrame
.L_0809012c:
	.4byte 0x0000053d
.L_08090130:
	.4byte DisplayTransition_Update
.L_08090134:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080901ac
.L_0809013a:
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080901b0
	movs r1, #0
	movs r0, #1
	bl Runtime_SetIrqHandler
	ldr r2, .L_080901b4
	mov r1, r9
	adds r3, r5, r2
	adds r2, #1
	strb r1, [r3]
	adds r3, r5, r2
	mov r1, r8
	adds r2, #1
	strb r1, [r3]
	adds r3, r5, r2
	mov r1, r10
	adds r2, #1
	strb r1, [r3]
	adds r3, r5, r2
	mov r1, r8
	strb r1, [r3]
.L_08090168:
	ldr r1, .L_080901b8
	ldr r4, .L_080901bc
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_0809019a
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	lsls r2, r2, #2
	ldrh r3, [r7, #20]
	adds r2, r2, r1
	ldrh r1, [r0]
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_0809019a:
	strh r5, [r4]
.L_0809019c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080901ac:
	.4byte DisplayTransition_UpdateFromCentre
.L_080901b0:
	.4byte DisplayTransition_UpdateScanline
.L_080901b4:
	.4byte 0x0000053a
.L_080901b8:
	.4byte gIoWriteQueue
.L_080901bc:
	.4byte 0x04000208
