.syntax unified
	.thumb
	.global Func_080d47b4
	.thumb_func
Func_080d47b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	movs r3, #255
	ands r3, r7
	mov r8, r0
	mov r10, r2
	cmp r3, #6
	bne .L_080d47d0
	movs r0, #110
	bl Audio_PlayCue
.L_080d47d0:
	mov r0, r8
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080d485e
	movs r0, #244
	lsls r0, r0, #1
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	bl Func_080200c0
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d4858
	ldr r1, .L_080d483c
	bl ObjectDispatch_InitializeFar
	movs r1, #15
	ands r1, r7
	adds r0, r5, #0
	bl Object_SetMode
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r3, #2
	mov r2, r8
	strh r2, [r3]
	ldr r3, .L_080d4840
	ldr r1, .L_080d4838
	str r3, [r5, #108]
	ldr r0, [r5, #80]
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r7
	str r6, [r5, #104]
	strb r1, [r0, #26]
	cmp r3, #0
	beq .L_080d4844
	ldrb r3, [r0, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r0, #9]
	b .L_080d4858
.L_080d4838:
	.4byte 0x00000000
.L_080d483c:
	.4byte Data_080f3310
.L_080d4840:
	.4byte BattleFx_CopyLinkedObjectPosition
.L_080d4844:
	ldr r3, [r6, #80]
	ldrb r1, [r0, #9]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
.L_080d4858:
	mov r0, r10
	bl EventRuntime_Wait
.L_080d485e:
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
