.syntax unified
	.thumb
	.global Func_080c9934
	.thumb_func
Func_080c9934:
	ldr r3, .L_080c9940
	lsls r0, r0, #3
	adds r0, r0, r3
	movs r3, #6
	ldrsh r0, [r0, r3]
	bx lr
.L_080c9940:
	.4byte Data_080f17a8
