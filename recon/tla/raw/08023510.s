.syntax unified
	.thumb
	.global Func_08023510
	.thumb_func
Func_08023510:
	push {r5, r6, lr}
	adds r6, r1, #0
	ldr r1, .L_08023520
	adds r5, r0, #0
	bl ObjectDispatch_Initialize
	str r6, [r5, #104]
	pop {r5, r6, pc}
.L_08023520:
	.4byte Data_0802f1e8
