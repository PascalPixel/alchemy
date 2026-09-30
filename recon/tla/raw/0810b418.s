.syntax unified
	.thumb
	.global Func_0810b418
	.thumb_func
Func_0810b418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r5, r0, #0
	mov r9, r1
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #6
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	mov r0, r9
	bl Object_GetByIdFar
	ldr r3, [r0, #80]
	movs r2, #128
	ldr r3, [r3, #40]
	lsls r2, r2, #3
	ldrh r3, [r3]
	adds r2, #250
	adds r7, r6, r2
	strh r3, [r7]
	movs r2, #0
	movs r3, #0
	movs r1, #0
	ldrh r0, [r7]
	bl Func_080380f8
	mov r8, r0
	adds r0, r5, #0
	bl Func_0810b378
	movs r1, #5
	adds r5, r0, #0
	bl UiText_DrawQuantity
	ldr r3, .L_0810b518
	mov r10, r3
	mov r0, r10
	bl Func_081084f4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #12
	movs r3, #4
	movs r0, #0
	bl UiWindow_CreateFar
	str r0, [r6, #12]
	bl Func_08109188
	movs r0, #0
	bl Func_08108630
	cmp r0, #0
	beq .L_0810b4a2
	mov r0, r10
	adds r0, #3
	b .L_0810b4ae
.L_0810b4a2:
	ldr r3, .L_0810b51c
	ldr r3, [r3, #16]
	cmp r5, r3
	bls .L_0810b4bc
	mov r0, r10
	adds r0, #2
.L_0810b4ae:
	bl Func_081084f4
	ldr r0, [r6, #12]
	movs r1, #2
	bl UiWork_FinalizeFar
	b .L_0810b4fe
.L_0810b4bc:
	movs r1, #2
	ldr r0, [r6, #12]
	bl UiWork_FinalizeFar
	mov r0, r10
	adds r0, #1
	bl Func_081084f4
	movs r1, #2
	mov r0, r8
	bl UiWork_FinalizeFar
	adds r0, r5, #0
	bl Func_0810b520
	mov r0, r9
	bl Object_GetByIdFar
	ldr r3, [r0, #80]
	movs r1, #0
	ldr r3, [r3, #40]
	movs r2, #0
	ldrh r3, [r3]
	strh r3, [r7]
	movs r3, #0
	ldrh r0, [r7]
	bl Func_080380f8
	mov r8, r0
	mov r0, r10
	adds r0, #4
	bl Func_081084f4
.L_0810b4fe:
	mov r0, r8
	movs r1, #2
	bl UiWork_FinalizeFar
	bl Func_0810824c
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0810b518:
	.4byte 0x000012cd
.L_0810b51c:
	.4byte gPartyState
