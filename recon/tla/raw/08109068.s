.syntax unified
	.thumb
	.global Func_08109068
	.thumb_func
Func_08109068:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #2
	movs r1, #156
	adds r3, r5, r2
	lsls r1, r1, #2
	adds r7, r5, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	mov r9, r0
	str r1, [sp, #4]
	ldr r0, [sp, #8]
	movs r1, #7
	bl __divsi3
	lsls r3, r0, #3
	subs r6, r3, r0
	mov r3, r9
	cmp r3, #0
	beq .L_08109178
	mov r0, r9
	bl RenderOutput_PrepareForRedrawFar
	cmp r6, #0
	beq .L_081090da
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #238
	adds r3, r5, r1
	ldrh r0, [r3]
	movs r3, #16
	negs r3, r3
	movs r1, #128
	str r3, [sp, #0]
	mov r2, r9
	movs r3, #216
	lsls r1, r1, #23
	bl RenderOutput_CreateFar
	movs r2, #0
	movs r3, #17
	strb r2, [r0, #4]
	strb r3, [r0, #5]
	strh r2, [r0, #12]
.L_081090da:
	ldr r2, [sp, #4]
	adds r3, r6, #7
	cmp r3, r2
	bge .L_08109104
	movs r1, #158
	lsls r1, r1, #3
	adds r3, r5, r1
	ldrh r0, [r3]
	movs r1, #128
	movs r3, #24
	str r3, [sp, #0]
	mov r2, r9
	movs r3, #216
	lsls r1, r1, #23
	bl RenderOutput_CreateFar
	movs r2, #0
	movs r3, #15
	strb r2, [r0, #4]
	strb r3, [r0, #5]
	strh r2, [r0, #12]
.L_08109104:
	ldr r3, [sp, #4]
	movs r2, #0
	mov r10, r2
	cmp r6, r3
	bcs .L_08109178
	lsls r3, r6, #1
	adds r3, r3, r7
	movs r1, #16
	mov r8, r3
	mov r11, r1
.L_08109118:
	mov r3, r8
	movs r2, #0
	ldrsh r5, [r3, r2]
	adds r0, r5, #0
	bl Item_Get
	mov r1, r10
	movs r2, #0
	lsls r3, r1, #5
	str r2, [sp, #0]
	adds r7, r0, #0
	mov r2, r9
	adds r0, r5, #0
	movs r1, #1
	bl RenderOutput_CreateLoadedFar
	movs r3, #252
	strb r3, [r0, #15]
	ldr r2, [sp, #8]
	cmp r6, r2
	bne .L_0810914e
	movs r3, #9
	strb r3, [r0, #5]
	movs r3, #10
	strh r3, [r0, #12]
	movs r3, #253
	strb r3, [r0, #15]
.L_0810914e:
	mov r2, r11
	mov r1, r9
	movs r3, #0
	ldrh r0, [r7]
	bl Func_081087e0
	movs r3, #251
	movs r1, #1
	strb r3, [r0, #15]
	add r10, r1
	movs r3, #32
	add r11, r3
	movs r2, #2
	mov r3, r10
	add r8, r2
	adds r6, #1
	cmp r3, #6
	bhi .L_08109178
	ldr r1, [sp, #4]
	cmp r6, r1
	bcc .L_08109118
.L_08109178:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
