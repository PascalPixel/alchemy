.syntax unified
	.thumb
	.global Func_0803dd98
	.thumb_func
Func_0803dd98:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #72]
	sub sp, #8
	mov r9, r3
	movs r3, #229
	lsls r3, r3, #2
	add r3, r9
	ldrh r3, [r3]
	movs r2, #213
	str r3, [sp, #4]
	movs r3, #231
	lsls r3, r3, #2
	add r3, r9
	ldrh r3, [r3]
	movs r6, #0
	mov r10, r3
	lsls r3, r3, #1
	add r3, r9
	lsls r2, r2, #2
	mov r11, r6
	adds r4, r3, r2
	b .L_0803de0e
.L_0803ddd4:
	adds r2, r5, #0
	movs r3, #0
	adds r0, r7, #0
	mov r1, r8
	str r4, [sp, #0]
	bl MenuSelection_SetupEntry
	movs r3, #210
	lsls r3, r3, #2
	add r3, r9
	ldr r2, [r3]
	ldr r4, [sp, #0]
	cmp r2, #0
	bne .L_0803ddf6
	str r5, [r3]
	str r2, [r5]
	b .L_0803ddfa
.L_0803ddf6:
	str r5, [r6, #4]
	str r6, [r5]
.L_0803ddfa:
	movs r3, #0
	str r3, [r5, #4]
	movs r3, #1
	add r11, r3
	mov r2, r11
	adds r6, r5, #0
	cmp r2, #5
	beq .L_0803de2a
	adds r4, #2
	add r10, r3
.L_0803de0e:
	ldr r3, [sp, #4]
	cmp r10, r3
	bcs .L_0803de2a
	ldrh r2, [r4, #32]
	movs r0, #0
	ldrh r7, [r4]
	str r4, [sp, #0]
	mov r8, r2
	bl Func_0803deac
	adds r5, r0, #0
	ldr r4, [sp, #0]
	cmp r5, #0
	bne .L_0803ddd4
.L_0803de2a:
	mov r3, r11
	lsls r2, r3, #3
	ldr r3, .L_0803de64
	movs r1, #192
	lsls r1, r1, #2
	subs r3, r3, r2
	adds r1, #150
	movs r2, #230
	add r1, r9
	lsls r2, r2, #2
	strh r3, [r1]
	add r2, r9
	movs r3, #140
	strh r3, [r2]
	movs r3, #210
	lsls r3, r3, #2
	add r3, r9
	ldr r6, [r3]
	movs r3, #0
	mov r11, r3
	cmp r6, #0
	beq .L_0803de94
	movs r0, #238
	lsls r0, r0, #2
	adds r5, r1, #0
	adds r4, r2, #0
	add r0, r9
	movs r1, #0
	b .L_0803de68
.L_0803de64:
	.4byte 0x00000064
.L_0803de68:
	ldrh r3, [r5]
	add r3, r11
	strh r3, [r6, #16]
	ldrh r2, [r4]
	strh r3, [r6, #24]
	strh r2, [r6, #18]
	strh r2, [r6, #26]
	ldrh r2, [r6, #10]
	cmp r2, #6
	bne .L_0803de86
	ldrh r3, [r0]
	cmp r3, #0
	bne .L_0803de86
	strh r2, [r6, #18]
	strh r2, [r6, #26]
.L_0803de86:
	strh r1, [r6, #20]
	strh r1, [r6, #22]
	ldr r6, [r6, #4]
	movs r2, #16
	add r11, r2
	cmp r6, #0
	bne .L_0803de68
.L_0803de94:
	bl Menu_LoadSelectedResource
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
