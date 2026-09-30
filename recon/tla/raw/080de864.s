.syntax unified
	.thumb
	.global Func_080de864
	.thumb_func
Func_080de864:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	sub sp, #12
	ldr r1, [r3]
	cmp r6, #0
	beq .L_080de8c6
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r2, r3, #16
	cmp r2, #0
	beq .L_080de8be
	ldr r3, [r1, #4]
	mov r5, sp
	str r3, [r5]
	movs r0, #160
	ldr r3, [r1, #8]
	lsls r0, r0, #12
	adds r3, r3, r0
	str r3, [r5, #4]
	lsls r0, r2, #16
	ldr r3, [r1, #12]
	str r3, [r5, #8]
	adds r3, r6, #0
	adds r3, #102
	movs r4, #0
	ldrsh r1, [r3, r4]
	lsls r3, r2, #11
	adds r1, r1, r3
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
	b .L_080de8c6
.L_080de8be:
	ldr r1, .L_080de8cc
	adds r0, r6, #0
	bl Object_SetCallback
.L_080de8c6:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080de8cc:
	.4byte BattleFx_CommonParticleScript
