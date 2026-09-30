.syntax unified
	.thumb
	.global Func_080d45d8
	.thumb_func
Func_080d45d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	movs r1, #213
	lsls r3, r3, #18
	lsls r1, r1, #4
	movs r0, #108
	ldr r7, [r3, #32]
	bl Runtime_AllocateBlock
	movs r1, #230
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r3, [r0]
	adds r3, #91
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_080d468e
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #108
	adds r2, r2, r7
	movs r0, #0
	ldrsh r3, [r2, r0]
	mov r8, r2
	cmp r3, #0
	beq .L_080d468e
	movs r1, #144
	movs r2, #144
	lsls r1, r1, #4
	lsls r2, r2, #4
	adds r1, #100
	adds r2, #104
	adds r6, r7, r1
	adds r3, r7, r2
	ldr r2, [r3]
	ldr r3, [r6]
	subs r2, r2, r3
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #110
	adds r5, r7, r3
	ldrh r3, [r5]
	adds r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r0, r3, #0
	muls r0, r2
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Math_Div
	ldr r1, [r6]
	adds r1, r1, r0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #92
	adds r3, r7, r0
	ldr r0, [r3]
	ldr r3, .L_080d4698
	mov lr, r3
	.2byte 0xf800
	movs r1, #150
	lsls r1, r1, #4
	adds r3, r7, r1
	str r0, [r3]
	movs r0, #142
	lsls r0, r0, #1
	adds r3, r7, r0
	ldrh r3, [r3]
	ldr r2, .L_080d469c
	adds r3, #1
	str r3, [r2]
	movs r1, #0
	ldrsh r2, [r5, r1]
	mov r1, r8
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r2, r3
	bne .L_080d468e
	mov r2, r10
	mov r3, r8
	strh r2, [r3]
	ldr r0, .L_080d46a0
	bl Scheduler_RemoveCallback
.L_080d468e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d4698:
	.4byte IwramMulQ16
.L_080d469c:
	.4byte Data_03001144
.L_080d46a0:
	.4byte Func_080d45d8
