.syntax unified
	.thumb
	.global Func_080d8174
	.thumb_func
Func_080d8174:
	push {r5, r6, r7, lr}
	ldr r3, .L_080d81e4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	adds r3, r7, #0
	adds r3, #100
	ldrh r1, [r3]
	adds r6, r0, #0
	subs r1, #1
	strh r1, [r3]
	mov r5, sp
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	str r3, [r5]
	asrs r1, r1, #16
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	adds r0, r1, #0
	muls r0, r3
	adds r3, r7, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r1, r1, #11
	adds r1, r1, r3
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	ldr r2, [r7, #12]
	str r3, [r7, #8]
	movs r1, #160
	ldr r3, [r5, #8]
	lsls r1, r1, #13
	str r3, [r7, #16]
	ldr r3, .L_080d81e8
	adds r2, r2, r3
	str r2, [r7, #12]
	ldr r3, [r6, #12]
	adds r3, r3, r1
	cmp r2, r3
	bge .L_080d81e0
	adds r0, r7, #0
	bl Object_Destroy
.L_080d81e0:
	add sp, #12
	pop {r5, r6, r7, pc}
.L_080d81e4:
	.4byte gPartyState
.L_080d81e8:
	.4byte 0xffff0000
