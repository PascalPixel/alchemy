.syntax unified
	.thumb
	.global Func_080eab70
	.thumb_func
Func_080eab70:
	push {lr}
	adds r3, r0, #0
	ldr r0, [r3, #8]
	cmp r0, #0
	bge .L_080eab7e
	ldr r2, .L_080eab94
	adds r0, r0, r2
.L_080eab7e:
	asrs r2, r0, #20
	ldr r0, [r3, #16]
	cmp r0, #0
	bge .L_080eab8a
	ldr r3, .L_080eab94
	adds r0, r0, r3
.L_080eab8a:
	asrs r0, r0, #20
	lsls r0, r0, #7
	adds r0, r2, r0
	pop {pc}
	.2byte 0x0000
.L_080eab94:
	.4byte 0x000fffff
