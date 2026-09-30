.syntax unified
	.thumb
	.global Menu_UpdateEntryObjectTransforms
	.thumb_func
Menu_UpdateEntryObjectTransforms:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r2, [r3]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #9
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #32
	cmp r3, #0
	beq .L_08104e30
	movs r1, #140
	movs r3, #0
	lsls r1, r1, #1
	adds r5, r2, #0
	mov r8, r3
	add r4, sp, #8
	add r6, sp, #16
	adds r7, r2, r1
	adds r5, #248
.L_08104ddc:
	movs r2, #16
	ldrsh r3, [r7, r2]
	ldr r0, [r5]
	movs r2, #229
	lsls r3, r3, #15
	lsls r2, r2, #15
	subs r2, r2, r3
	cmp r0, #0
	beq .L_08104e22
	ldr r3, [r5, #64]
	str r4, [sp, #4]
	str r3, [sp, #8]
	ldr r3, [r5, #64]
	str r3, [r4, #4]
	movs r1, #0
	ldrsh r3, [r7, r1]
	str r2, [r6, #4]
	lsls r3, r3, #16
	str r3, [r6]
	movs r1, #16
	ldrsh r3, [r7, r1]
	adds r1, r6, #0
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r3, #0
	str r3, [r6, #12]
	movs r3, #250
	str r3, [sp, #0]
	movs r3, #128
	adds r2, r4, #0
	lsls r3, r3, #7
	bl Func_08020018
	ldr r4, [sp, #4]
.L_08104e22:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #2
	adds r5, #4
	cmp r3, #7
	ble .L_08104ddc
.L_08104e30:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
