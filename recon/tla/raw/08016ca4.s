.syntax unified
	.thumb
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	push {lr}
	cmp r0, #7
	bhi .L_08016cb6
	movs r3, #166
	lsls r3, r3, #1
	ldr r2, .L_08016cdc
	muls r3, r0
	adds r0, r3, r2
	b .L_08016cda
.L_08016cb6:
	adds r3, r0, #0
	subs r3, #128
	cmp r3, #5
	bhi .L_08016cd8
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #216
	ldr r2, [r3]
	cmp r2, #0
	beq .L_08016cd8
	movs r3, #166
	lsls r3, r3, #1
	muls r3, r0
	adds r3, r2, r3
	ldr r2, .L_08016ce0
	adds r0, r3, r2
	b .L_08016cda
.L_08016cd8:
	movs r0, #0
.L_08016cda:
	pop {pc}
.L_08016cdc:
	.4byte Data_02000520
.L_08016ce0:
	.4byte 0xffff5a00
