.syntax unified
	.thumb
	.global Func_081050cc
	.thumb_func
Func_081050cc:
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
	ldr r7, [r3]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #10
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #32
	cmp r3, #0
	beq .L_08105198
	movs r1, #140
	lsls r1, r1, #2
	movs r3, #8
	adds r6, r7, r1
	subs r1, #16
	movs r2, #0
	add r3, sp
	adds r1, r7, r1
	mov r11, r2
	mov r10, r3
	movs r2, #144
	movs r3, #148
	str r1, [sp, #4]
	lsls r2, r2, #2
	lsls r3, r3, #1
	add r5, sp, #16
	mov r8, r2
	mov r9, r3
.L_0810511a:
	ldr r3, [sp, #4]
	ldmia r3!, {r0}
	adds r2, r3, #0
	str r2, [sp, #4]
	cmp r0, #0
	beq .L_08105184
	mov r1, r9
	ldrsh r3, [r1, r7]
	movs r2, #229
	lsls r3, r3, #15
	lsls r2, r2, #15
	subs r2, r2, r3
	mov r3, r8
	ldr r1, [r3, r7]
	cmp r1, #0
	bge .L_08105144
	negs r3, r1
	mov r4, r10
	str r3, [sp, #8]
	str r3, [r4, #4]
	b .L_0810515e
.L_08105144:
	movs r3, #128
	lsls r3, r3, #9
	subs r3, r3, r1
	cmp r3, #0
	bge .L_08105150
	adds r3, #3
.L_08105150:
	asrs r3, r3, #2
	adds r3, r1, r3
	mov r4, r10
	mov r1, r8
	str r3, [sp, #8]
	str r3, [r4, #4]
	str r3, [r7, r1]
.L_0810515e:
	movs r1, #0
	ldrsh r3, [r6, r1]
	str r2, [r5, #4]
	lsls r3, r3, #16
	str r3, [r5]
	movs r1, #8
	ldrsh r3, [r6, r1]
	adds r1, r5, #0
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r3, #0
	str r3, [r5, #12]
	movs r2, #250
	ldrh r3, [r6, #32]
	str r2, [sp, #0]
	adds r2, r4, #0
	bl Func_08020018
.L_08105184:
	movs r1, #1
	movs r2, #4
	add r11, r1
	add r8, r2
	movs r3, #2
	mov r2, r11
	adds r6, #2
	add r9, r3
	cmp r2, #3
	ble .L_0810511a
.L_08105198:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
