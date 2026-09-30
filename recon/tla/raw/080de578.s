.syntax unified
	.thumb
	.global Func_080de578
	.thumb_func
Func_080de578:
	push {lr}
	cmp r0, #0
	beq .L_080de59a
	ldr r1, .L_080de59c
	ldr r2, [r0, #28]
	ldr r3, [r0, #24]
	adds r2, r2, r1
	str r2, [r0, #28]
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #5
	str r3, [r0, #24]
	cmp r3, r2
	bgt .L_080de59a
	ldr r1, .L_080de5a0
	bl Object_SetCallback
.L_080de59a:
	pop {pc}
.L_080de59c:
	.4byte 0xfffff000
.L_080de5a0:
	.4byte BattleFx_CommonParticleScript
