.syntax unified
	.thumb
	.global Func_080f89a4
	.thumb_func
Func_080f89a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	sub sp, #32
	ldr r5, [r3]
	bl Party_CountActiveOwnersFar
	lsls r0, r0, #16
	lsrs r0, r0, #16
	mov r10, r0
	movs r4, #0
	cmp r4, r10
	bge .L_080f8a38
	movs r2, #8
	movs r3, #140
	add r2, sp
	lsls r3, r3, #1
	adds r7, r5, r3
	mov r8, r2
	add r6, sp, #16
	adds r5, #248
.L_080f89d6:
	movs r2, #16
	ldrsh r3, [r7, r2]
	ldr r0, [r5]
	movs r2, #229
	lsls r3, r3, #15
	lsls r2, r2, #15
	subs r1, r2, r3
	cmp r0, #0
	beq .L_080f8a2e
	ldrb r3, [r0, #9]
	str r4, [sp, #4]
	mov r12, r3
	movs r3, #13
	negs r3, r3
	adds r2, r3, #0
	mov r3, r12
	ands r3, r2
	strb r3, [r0, #9]
	mov r2, r8
	ldr r3, [r5, #64]
	str r3, [sp, #8]
	ldr r3, [r5, #64]
	str r3, [r2, #4]
	movs r2, #0
	ldrsh r3, [r7, r2]
	str r1, [r6, #4]
	lsls r3, r3, #16
	str r3, [r6]
	movs r2, #16
	ldrsh r3, [r7, r2]
	mov r2, r8
	lsls r3, r3, #16
	adds r3, r3, r1
	str r3, [r6, #8]
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #250
	str r3, [sp, #0]
	movs r3, #128
	adds r1, r6, #0
	lsls r3, r3, #7
	bl Func_08020018
	ldr r4, [sp, #4]
.L_080f8a2e:
	adds r4, #1
	adds r7, #2
	adds r5, #4
	cmp r4, r10
	blt .L_080f89d6
.L_080f8a38:
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
