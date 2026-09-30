.syntax unified
	.thumb
	.global Func_0818f5cc
	.thumb_func
Func_0818f5cc:
	push {lr}
	ldr r3, [r0]
	cmp r3, #199
	ble .L_0818f5dc
	movs r1, #2
	bl Func_0818f620
	b .L_0818f5e2
.L_0818f5dc:
	movs r1, #1
	bl Func_0818f620
.L_0818f5e2:
	pop {pc}
