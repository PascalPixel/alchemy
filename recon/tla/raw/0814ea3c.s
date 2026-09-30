.syntax unified
	.thumb
	.global Func_0814ea3c
	.thumb_func
Func_0814ea3c:
	push {lr}
	ldr r3, [r0, #24]
	negs r1, r3
	orrs r1, r3
	lsrs r1, r1, #31
	bl Func_0814ea58
	pop {pc}
