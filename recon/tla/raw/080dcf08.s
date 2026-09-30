.syntax unified
	.thumb
	.global Func_080dcf08
	.thumb_func
Func_080dcf08:
	push {lr}
	ldr r3, [r0, #56]
	movs r2, #128
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_080dcf26
	ldr r2, [r0, #60]
	cmp r2, r3
	bne .L_080dcf26
	ldr r3, [r0, #64]
	cmp r3, r2
	bne .L_080dcf26
	ldr r1, .L_080dcf28
	bl Object_SetCallback
.L_080dcf26:
	pop {pc}
.L_080dcf28:
	.4byte Data_080f3974
