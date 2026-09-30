.syntax unified
	.thumb
	.global Func_0810abf0
	.thumb_func
Func_0810abf0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	sub sp, #16
	ldr r7, [r3]
	movs r6, #0
	movs r0, #0
	movs r2, #129
	str r0, [sp, #12]
	str r6, [sp, #4]
	lsls r2, r2, #3
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r1, #1
	ldr r0, .L_0810ae94
	mov r9, r1
	mov r10, r3
	bl Func_0810a9ac
	movs r5, #2
	movs r1, #12
	movs r2, #13
	movs r3, #3
	movs r0, #1
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	mov r11, r0
	movs r0, #128
	lsls r0, r0, #3
	adds r0, #220
	adds r3, r7, r0
	ldr r2, [r3]
	movs r1, #160
	lsls r1, r1, #3
	movs r3, #4
	adds r1, #5
	strb r3, [r2, #5]
	adds r3, r7, r1
	mov r2, r9
	strb r2, [r3]
	mov r0, r11
	movs r1, #2
	movs r2, #0
	bl Func_080f8060
	movs r0, #1
	movs r1, #16
	movs r2, #23
	movs r3, #3
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	mov r8, r6
	str r0, [sp, #8]
	str r6, [sp, #12]
	b .L_0810ac7c
.L_0810ac76:
	ldr r3, [sp, #12]
	adds r3, #1
	str r3, [sp, #12]
.L_0810ac7c:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #4
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, [sp, #12]
	cmp r2, r3
	bge .L_0810aca8
	movs r1, #153
	lsls r1, r1, #3
	lsls r3, r2, #1
	adds r3, r3, r1
	adds r2, r7, #2
	ldrsh r0, [r2, r3]
	mov r1, r10
	mov r8, r0
	bl Shop_CanServe
	cmp r0, #0
	beq .L_0810ac76
.L_0810aca8:
	movs r2, #2
	mov r9, r2
.L_0810acac:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0810acf8
	movs r0, #0
	str r0, [sp, #4]
	ldr r0, .L_0810ae94
	bl Func_0810a9ac
	ldr r2, [sp, #4]
	movs r1, #2
	mov r9, r1
	str r2, [sp, #12]
	b .L_0810accc
.L_0810acc6:
	ldr r3, [sp, #12]
	adds r3, #1
	str r3, [sp, #12]
.L_0810accc:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #4
	adds r3, r7, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r2, [sp, #12]
	cmp r2, r3
	bge .L_0810acf8
	movs r1, #153
	lsls r1, r1, #3
	lsls r3, r2, #1
	adds r3, r3, r1
	adds r2, r7, #2
	ldrsh r0, [r2, r3]
	mov r1, r10
	mov r8, r0
	bl Shop_CanServe
	cmp r0, #0
	beq .L_0810acc6
.L_0810acf8:
	mov r2, r9
	cmp r2, #0
	beq .L_0810ad68
	ldr r4, [sp, #12]
	movs r3, #153
	lsls r3, r3, #3
	lsls r2, r4, #1
	adds r2, r2, r3
	adds r3, r7, #2
	ldrsh r0, [r3, r2]
	adds r3, r4, #0
	mov r8, r0
	cmp r4, #0
	bge .L_0810ad16
	adds r3, r4, #3
.L_0810ad16:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r4, r3
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	mov r0, r11
	movs r2, #0
	subs r1, #12
	bl Func_08108af0
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #5
	adds r2, r7, r3
	mov r0, r9
	movs r3, #3
	strb r3, [r2]
	cmp r0, #2
	bne .L_0810ad54
	ldr r0, [sp, #12]
	cmp r0, #0
	bge .L_0810ad46
	adds r0, #3
.L_0810ad46:
	asrs r0, r0, #2
	lsls r0, r0, #2
	bl Func_080f8058
	movs r0, #1
	bl WaitFrames
.L_0810ad54:
	ldr r1, [sp, #12]
	mov r0, r11
	bl Func_0810af24
	mov r1, r8
	ldr r0, [sp, #8]
	bl Func_0810b04c
	movs r1, #0
	mov r9, r1
.L_0810ad68:
	ldr r1, .L_0810ae98
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	beq .L_0810ae3a
	movs r0, #1
	bl WaitFrames
	mov r1, r10
	mov r0, r8
	bl Shop_ServicePrice
	mov r1, r10
	adds r5, r0, #0
	mov r0, r8
	bl Shop_CanServe
	cmp r0, #0
	bne .L_0810ad98
	movs r0, #113
	bl Audio_PlayCue
	b .L_0810acac
.L_0810ad98:
	mov r0, r11
	bl RenderOutput_PrepareForRedrawFar
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r5, #0
	movs r1, #5
	bl UiText_DrawQuantity
	ldr r6, .L_0810ae9c
	adds r0, r6, #0
	bl Func_0810a9ac
	movs r0, #0
	bl Func_08108660
	cmp r0, #0
	beq .L_0810adcc
	adds r0, r6, #2
	bl Func_0810a9fc
	movs r2, #1
	str r2, [sp, #4]
	b .L_0810acac
.L_0810adcc:
	ldr r3, .L_0810aea0
	ldr r3, [r3, #16]
	cmp r5, r3
	bls .L_0810ade6
	movs r0, #113
	bl Audio_PlayCue
	adds r0, r6, #1
	bl Func_0810a9fc
	movs r3, #1
	str r3, [sp, #4]
	b .L_0810acac
.L_0810ade6:
	movs r1, #1
	mov r0, r8
	bl UiText_DrawQuantity
	adds r0, r6, #3
	bl Func_0810a9ac
	bl UiWork_FinalizePendingCoreFar
	mov r0, r8
	mov r1, r10
	bl Func_0810aea4
	ldr r3, [sp, #12]
	adds r0, r3, #0
	cmp r3, #0
	bge .L_0810ae0a
	adds r0, r3, #3
.L_0810ae0a:
	asrs r0, r0, #2
	lsls r0, r0, #2
	subs r0, r3, r0
	bl Func_0810b1b4
	negs r0, r5
	bl Party_AdjustSixDigitCounterAFar
	bl Func_08109188
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	adds r0, r6, #4
	bl Func_0810a9ac
	bl Func_0810a8ec
	cmp r0, #0
	beq .L_0810ae6a
	movs r0, #1
	str r0, [sp, #4]
	b .L_0810acac
.L_0810ae3a:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0810ae4c
	movs r0, #113
	bl Audio_PlayCue
	b .L_0810ae6a
.L_0810ae4c:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #4
	adds r3, r7, r1
	movs r1, #0
	ldrsb r1, [r3, r1]
	add r0, sp, #12
	movs r2, #4
	bl Func_08108690
	mov r9, r0
	movs r0, #1
	bl WaitFrames
	b .L_0810acac
.L_0810ae6a:
	bl Func_080f8068
	movs r1, #2
	ldr r0, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r11
	movs r1, #2
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810ae94:
	.4byte 0x000012d7
.L_0810ae98:
	.4byte gInput
.L_0810ae9c:
	.4byte 0x000012d8
.L_0810aea0:
	.4byte gPartyState
