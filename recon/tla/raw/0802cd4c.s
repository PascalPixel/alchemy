.syntax unified
	.thumb
	.global Func_0802cd4c
	.thumb_func
Func_0802cd4c:
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
	bne .L_0802cd6a
	ldr r0, .L_0802cd6c
	bl Func_08014694
.L_0802cd6a:
	pop {pc}
.L_0802cd6c:
	.4byte Func_0802cb64
