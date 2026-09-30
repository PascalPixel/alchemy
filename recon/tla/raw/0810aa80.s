.syntax unified
	.thumb
	.global Func_0810aa80
	.thumb_func
Func_0810aa80:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	sub sp, #8
	mov r8, r1
	adds r5, r0, #0
	mov r10, r1
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #129
	lsls r2, r2, #3
	adds r2, #255
	adds r3, r7, r2
	mov r1, r8
	strb r1, [r3]
	adds r0, r5, #0
	bl Object_GetByIdFar
	ldr r3, [r0, #80]
	movs r1, #128
	ldr r3, [r3, #40]
	lsls r1, r1, #3
	ldrh r2, [r3]
	adds r1, #250
	adds r3, r7, r1
	strh r2, [r3]
	movs r1, #0
	ldrh r0, [r3]
	movs r2, #0
	movs r3, #0
	bl Func_080380f8
	mov r8, r0
	cmp r0, #0
	bne .L_0810ab0e
	movs r0, #5
	negs r0, r0
	movs r5, #2
	movs r1, #0
	movs r2, #5
	movs r3, #5
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	mov r8, r0
	cmp r0, #0
	bne .L_0810ab0e
	movs r1, #0
	movs r2, #5
	movs r3, #5
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_CreateFar
	movs r3, #4
	negs r3, r3
	mov r8, r0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #0
	movs r2, #0
	mov r3, r8
	bl RenderOutput_CreateFar + 0x10
.L_0810ab0e:
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r7, r2
	movs r1, #128
	ldrh r0, [r3]
	movs r6, #0
	lsls r1, r1, #23
	mov r2, r8
	movs r3, #0
	str r6, [sp, #0]
	bl RenderOutput_CreateFar
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r5, #5]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	strb r6, [r5, #4]
	movs r1, #32
	adds r6, r7, r3
	negs r1, r1
	adds r0, r6, #0
	movs r2, #112
	bl Func_08108aa8
	str r5, [r6]
	ldr r0, .L_0810abe0
	bl Func_0810a9ac
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #11
	movs r2, #12
	movs r3, #4
	movs r0, #16
	bl UiWindow_CreateFar
	str r0, [r7, #12]
	bl Func_08109188
	movs r1, #129
	lsls r1, r1, #3
	adds r1, #255
	adds r6, r7, r1
	b .L_0810aba4
.L_0810ab6c:
	ldr r5, .L_0810abe4
	adds r0, r5, #0
	bl Func_0810a9ac
	bl Func_0810a8ec
	cmp r0, #0
	bne .L_0810ab84
	adds r0, r5, #1
	bl Func_0810a9ac
	b .L_0810ab88
.L_0810ab84:
	bl Func_0810abf0
.L_0810ab88:
	movs r2, #128
	lsls r2, r2, #3
	movs r3, #0
	adds r2, #220
	movs r1, #32
	adds r0, r7, r2
	strb r3, [r6]
	negs r1, r1
	movs r2, #112
	bl Func_08108aa8
	ldr r0, .L_0810abe8
	bl Func_0810a9ac
.L_0810aba4:
	mov r0, r10
	bl Func_08038368 + 0x8
	movs r1, #1
	mov r10, r0
	mov r3, r10
	negs r1, r1
	strb r3, [r6]
	cmp r10, r1
	bne .L_0810ab6c
	ldr r0, .L_0810abec
	bl Func_0810a9ac
	ldr r0, [r7, #12]
	movs r1, #2
	bl UiWork_FinalizeFar
	mov r0, r8
	movs r1, #2
	bl UiWork_FinalizeFar
	bl Func_0810824c
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810abe0:
	.4byte 0x000012d2
.L_0810abe4:
	.4byte 0x000012d5
.L_0810abe8:
	.4byte 0x000012d3
.L_0810abec:
	.4byte 0x000012d4
