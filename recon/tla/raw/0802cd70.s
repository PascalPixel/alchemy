.syntax unified
	.thumb
	.global Func_0802cd70
	.thumb_func
Func_0802cd70:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0802cd8e
	ldr r0, .L_0802cd90
	bl Func_0801475c
.L_0802cd8e:
	pop {pc}
.L_0802cd90:
	.4byte Func_0802cb64
