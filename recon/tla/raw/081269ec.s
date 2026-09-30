.syntax unified
	.thumb
	.global Func_081269ec
	.thumb_func
Func_081269ec:
	push {lr}
	cmp r0, #0
	bge .L_081269f4
	adds r0, #15
.L_081269f4:
	asrs r0, r0, #4
	pop {pc}
