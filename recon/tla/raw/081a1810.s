.syntax unified
	.thumb
	.global Func_081a1810
	.thumb_func
Func_081a1810:
	push {r5, r6, lr}
	ldr r3, .L_081a182c
	movs r6, #23
	ldr r5, [r3]
.L_081a1818:
	adds r0, r5, #0
	movs r1, #0
	subs r6, #1
	adds r5, #12
	bl Func_080140d8
	cmp r6, #0
	bge .L_081a1818
	pop {r5, r6, pc}
	.2byte 0x0000
.L_081a182c:
	.4byte Data_0200752c
