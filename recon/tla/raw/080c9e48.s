.syntax unified
	.thumb
	.global Func_080c9e48
	.thumb_func
Func_080c9e48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080c9f20
	movs r3, #192
	movs r1, #240
	lsls r1, r1, #1
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	adds r3, r2, r1
	movs r1, #0
	ldrsh r4, [r3, r1]
	sub sp, #8
	mov r10, r4
	movs r4, #241
	lsls r4, r4, #1
	adds r3, r2, r4
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #198
	movs r4, #0
	lsls r3, r3, #1
	mov r8, r1
	adds r6, r7, r3
	str r4, [sp, #4]
	str r4, [sp, #0]
	ldr r5, .L_080c9f24
	cmp r0, #0
	beq .L_080c9eda
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldr r1, .L_080c9f28
	ldrh r2, [r5]
	asrs r0, r1, #16
	cmp r3, r0
	beq .L_080c9eda
	mov r11, r1
	mov r9, r0
.L_080c9e9c:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, r10
	bne .L_080c9ece
	movs r2, #2
	ldrsh r3, [r5, r2]
	mov r4, r11
	asrs r2, r4, #16
	cmp r3, r2
	beq .L_080c9eb4
	cmp r3, r8
	bne .L_080c9ece
.L_080c9eb4:
	movs r1, #4
	ldrsh r0, [r5, r1]
	cmp r0, r2
	beq .L_080c9ec4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080c9ece
.L_080c9ec4:
	ldrb r2, [r5, #6]
	str r2, [sp, #4]
	ldrb r5, [r5, #7]
	str r5, [sp, #0]
	b .L_080c9eda
.L_080c9ece:
	adds r5, #8
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r2, [r5]
	cmp r3, r9
	bne .L_080c9e9c
.L_080c9eda:
	add r1, sp, #4
	ldrb r1, [r1]
	mov r2, sp
	strb r1, [r6]
	adds r6, #1
	ldrb r2, [r2]
	movs r3, #0
	strb r2, [r6]
	adds r6, #1
	movs r2, #0
.L_080c9eee:
	adds r3, #1
	strb r2, [r6]
	adds r6, #1
	cmp r3, #5
	bls .L_080c9eee
	movs r3, #202
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r4, #204
	movs r3, #0
	str r3, [r2]
	lsls r4, r4, #1
	movs r3, #128
	adds r2, r7, r4
	lsls r3, r3, #13
	str r3, [r2]
	bl Func_080ca4a8
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080c9f20:
	.4byte gPartyState
.L_080c9f24:
	.4byte Data_080ee6d4
.L_080c9f28:
	.4byte 0xffff0000
