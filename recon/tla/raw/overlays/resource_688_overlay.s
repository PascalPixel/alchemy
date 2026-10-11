.syntax unified
	.thumb
	.section .text.x02008060,"ax",%progbits
	.global Func_02000060
	.thumb_func
Func_02000060:
	push {r5, r6, lr}
	ldr r5, .L_020080ac
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	adds r2, r5, r3
	adds r6, r0, #0
	movs r3, #0
	movs r0, #150
	strh r3, [r2]
	lsls r0, r0, #4
	bl GameFlag_ClearBit
	movs r0, #123
	bl Func_020005dc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r3, #8
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	subs r2, #104
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r3, #4
	str r3, [r5]
	adds r0, r6, #0
	bl Func_020005ac
	pop {r5, r6, pc}
.L_020080ac:
	.4byte gPartyState
	.section .text.x020080b0,"ax",%progbits
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push {r5, r6, lr}
	ldr r5, .L_020080fc
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	adds r2, r5, r3
	adds r6, r0, #0
	movs r3, #0
	movs r0, #150
	strh r3, [r2]
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #123
	bl Func_020005dc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r3, #8
	str r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	subs r2, #104
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r3, #4
	str r3, [r5]
	adds r0, r6, #0
	bl Func_020005ac
	pop {r5, r6, pc}
.L_020080fc:
	.4byte gPartyState
	.section .text.x02008120,"ax",%progbits
	.global Func_02000120
	.thumb_func
Func_02000120:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008168
	bl Func_0200055c
	movs r0, #0
	bl Func_020005cc
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r2, .L_0200818c
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r1, r2, r3
	movs r3, #6
	strb r3, [r1]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r2, r2, r1
	movs r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #7
	bl Object_SetModeById
	bl Func_02000564
	b .L_02008188
.L_02008168:
	ldr r2, .L_0200818c
	movs r3, #133
	lsls r3, r3, #2
	adds r1, r2, r3
	movs r3, #4
	str r3, [r1]
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r2, r1
	strb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	adds r2, r2, r3
	strh r0, [r2]
.L_02008188:
	pop {pc}
	.2byte 0x0000
.L_0200818c:
	.4byte gPartyState
	.section .text.x02008190,"ax",%progbits
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	mov r8, r2
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, .L_02008304
	adds r2, #88
	str r2, [r3]
	movs r3, #133
	lsls r3, r3, #2
	adds r7, r6, r3
	ldr r0, [r7]
	mov r10, r2
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #7
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #180
	bl GameFlag_ClearBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #9
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081fc
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200058c
.L_020081fc:
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r6, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #90
	bne .L_02008296
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r6, r2
	movs r2, #1
	strh r2, [r3]
	ldr r0, .L_02008308
	movs r1, #1
	bl Func_020005a4
.L_02008296:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #3
	beq .L_020082a2
	cmp r3, #5
	bne .L_020082ce
.L_020082a2:
	movs r3, #8
	mov r2, r8
	str r3, [r7]
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r3, #4
	mov r2, r8
	str r3, [r7]
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r10
	str r2, [r3]
.L_020082ce:
	ldr r3, .L_02008304
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_020082fa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082fa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_ClearBit
	bl Func_020003cc
.L_020082fa:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008304:
	.4byte gPartyState
.L_02008308:
	.4byte 0x000000c7
	.section .text.x02008310,"ax",%progbits
	.global Func_02000310
	.thumb_func
Func_02000310:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #20
	ldr r7, [r3, #108]
	adds r5, r0, #0
	movs r3, #0
	mov r10, r1
	str r3, [sp, #16]
	str r3, [sp, #12]
	movs r6, #56
	cmp r5, #8
	beq .L_02008332
	adds r6, r5, #0
.L_02008332:
	adds r0, r6, #0
	bl BattleFx_GetResourceId
	mov r8, r0
	cmp r5, #7
	bne .L_02008366
	movs r3, #10
	str r3, [sp, #16]
	str r3, [sp, #12]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	add r4, sp, #4
	add r2, sp, #12
	add r3, sp, #8
	add r1, sp, #16
	str r4, [sp, #0]
	bl UiText_GetResourceDimensions
	ldr r3, [sp, #16]
	ldr r2, [sp, #8]
	adds r3, r3, r2
	subs r5, r3, #5
	b .L_02008370
.L_02008366:
	movs r3, #5
	str r3, [sp, #16]
	movs r3, #10
	str r3, [sp, #12]
	movs r5, #5
.L_02008370:
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r0, [r2]
	adds r3, r0, #1
	strh r3, [r2]
	mov r2, r8
	lsls r3, r2, #16
	lsls r0, r0, #16
	movs r2, #2
	orrs r3, r2
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	asrs r0, r0, #16
	bl UiText_OpenMessageWindow
	adds r2, r5, #0
	adds r0, r6, #0
	movs r1, #0
	movs r3, #5
	bl Func_0200053c
	adds r5, r0, #0
	b .L_020083a6
.L_020083a0:
	movs r0, #1
	bl WaitFrames
.L_020083a6:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_020083a0
	mov r0, r10
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #2
	bl UiWork_Finalize
	bl UiWork_FinalizePendingCore
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, r6, lr}
	ldr r5, .L_020084f0
	movs r3, #139
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r3, #2
	ldrb r6, [r5]
	strb r3, [r5]
	bl Func_0200055c
	movs r0, #0
	bl Func_020005cc
	movs r1, #138
	movs r2, #136
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r2, #143
	movs r0, #8
	adds r1, #30
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020084f4
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #80
	bl Battle_WaitMode0
	ldr r0, .L_020084f8
	bl Func_0200059c
	movs r0, #6
	movs r1, #90
	bl Func_02000310
	movs r0, #7
	movs r1, #90
	bl Func_02000310
	movs r0, #6
	movs r1, #180
	bl Func_02000310
	movs r0, #7
	movs r1, #90
	bl Func_02000310
	movs r0, #6
	movs r1, #250
	bl Func_02000310
	movs r0, #7
	movs r1, #180
	bl Func_02000310
	movs r0, #5
	movs r1, #250
	bl Func_02000310
	movs r0, #8
	movs r1, #90
	bl Func_02000310
	movs r0, #7
	movs r1, #180
	bl Func_02000310
	movs r0, #8
	movs r1, #180
	bl Func_02000310
	movs r0, #7
	movs r1, #180
	bl Func_02000310
	movs r0, #6
	movs r1, #90
	bl Func_02000310
	movs r0, #7
	movs r1, #250
	bl Func_02000310
	movs r0, #5
	movs r1, #250
	bl Func_02000310
	movs r0, #7
	movs r1, #250
	bl Func_02000310
	movs r0, #6
	movs r1, #90
	bl Func_02000310
	movs r0, #5
	movs r1, #90
	bl Func_02000310
	movs r0, #8
	movs r1, #90
	bl Func_02000310
	movs r0, #7
	movs r1, #250
	bl Func_02000310
	movs r0, #4
	movs r1, #90
	bl Func_02000310
	movs r1, #250
	movs r0, #7
	bl Func_02000310
	movs r0, #8
	bl Object_RefreshSelectorById
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #30
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #88
	str r3, [r2]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #1
	bl Func_020005ac
	strb r6, [r5]
	bl Func_02000564
	pop {r5, r6, pc}
.L_020084f0:
	.4byte gPartyState
.L_020084f4:
	.4byte Data_020005e4
.L_020084f8:
	.4byte MsgFieldAddressCompanion
