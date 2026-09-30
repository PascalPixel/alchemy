.syntax unified
	.thumb
	.global Func_081a8264
	.thumb_func
Func_081a8264:
	push {lr}
	cmp r0, #31
	ble .L_081a826e
	movs r0, #31
	b .L_081a8274
.L_081a826e:
	cmp r0, #0
	bge .L_081a8274
	movs r0, #0
.L_081a8274:
	pop {pc}
	.2byte 0x0000
