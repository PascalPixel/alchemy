.syntax unified
	.thumb
	.global Func_080de5a4
	.thumb_func
Func_080de5a4:
	push {r5, r6, lr}
	adds r6, r0, #0
	sub sp, #12
	cmp r6, #0
	beq .L_080de5f4
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r2, r3, #16
	cmp r2, #0
	beq .L_080de5f0
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #60]
	lsls r0, r2, #17
	str r3, [r5, #4]
	ldr r3, [r6, #64]
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
	b .L_080de5f4
.L_080de5f0:
	ldr r3, .L_080de5f8
	str r3, [r6, #108]
.L_080de5f4:
	add sp, #12
	pop {r5, r6, pc}
.L_080de5f8:
	.4byte BattleFx_UpdateOrbitingParticleFade
