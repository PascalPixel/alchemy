.syntax unified
	.thumb
	.global Func_0810a748
	.thumb_func
Func_0810a748:
	push {lr}
	bl Func_0810a70c
	cmp r0, #0
	beq .L_0810a75a
	movs r3, #2
	ldrsh r0, [r0, r3]
	adds r0, #1
	b .L_0810a75c
.L_0810a75a:
	movs r0, #0
.L_0810a75c:
	pop {pc}
	.2byte 0x0000
