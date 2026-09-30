.syntax unified
	.thumb
	.global Func_0801601c
	.thumb_func
Func_0801601c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #204
	ldr r3, [r3]
	adds r5, r0, #0
	adds r1, r3, #0
	movs r0, #0
	movs r4, #0
	adds r1, #30
	adds r2, r3, #0
.L_08016032:
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08016046
	ldrb r3, [r2, #15]
	cmp r5, r3
	bne .L_08016046
	ldrh r3, [r1]
	cmp r0, r3
	bcs .L_08016046
	adds r0, r3, #0
.L_08016046:
	adds r4, #3
	adds r1, #6
	adds r2, #3
	cmp r4, #14
	bls .L_08016032
	pop {r5, pc}
	.2byte 0x0000
