.syntax unified
	.thumb
	.global Func_081051a8
	.thumb_func
Func_081051a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r5, #136
	sub sp, #8
	mov r8, r3
	movs r6, #0
	lsls r5, r5, #2
	movs r4, #3
.L_081051c2:
	mov r1, r8
	ldr r0, [r5, r1]
	cmp r0, #0
	beq .L_081051d6
	str r4, [sp, #0]
	bl Func_08020040 + 0x8
	mov r2, r8
	str r6, [r5, r2]
	ldr r4, [sp, #0]
.L_081051d6:
	subs r4, #1
	adds r5, #4
	cmp r4, #0
	bge .L_081051c2
	ldr r3, .L_0810525c
	movs r6, #140
	movs r7, #136
	str r3, [sp, #4]
	lsls r6, r6, #2
	lsls r7, r7, #2
	movs r4, #0
	add r6, r8
	add r7, r8
.L_081051f0:
	ldr r2, [sp, #4]
	str r4, [sp, #0]
	ldmia r2!, {r0}
	adds r1, r2, #0
	str r1, [sp, #4]
	bl Func_08020040
	adds r5, r0, #0
	ldr r4, [sp, #0]
	cmp r5, #0
	beq .L_0810521a
	movs r1, #2
	bl Animation_ApplyChildArgumentFar
	ldrb r3, [r5, #9]
	movs r1, #13
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	ldr r4, [sp, #0]
.L_0810521a:
	ldr r3, .L_08105250
	str r5, [r7]
	strh r3, [r6]
	ldr r3, .L_08105254
	adds r4, #1
	strh r3, [r6, #8]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #32]
	ldr r3, .L_08105258
	adds r7, #4
	strh r3, [r6, #32]
	adds r6, #2
	cmp r4, #3
	ble .L_081051f0
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #10
	add r2, r8
	movs r3, #1
	movs r1, #144
	strb r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_08105260
	bl Func_080145a8
	b .L_08105264
.L_08105250:
	.4byte 0x00000010
.L_08105254:
	.4byte 0x000000c8
.L_08105258:
	.4byte 0x00004000
.L_0810525c:
	.4byte Data_08105a40
.L_08105260:
	.4byte Func_081050cc
.L_08105264:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
