.syntax unified
	.thumb
	.global Func_080cce84
	.thumb_func
Func_080cce84:
	push {lr}
	ldr r3, .L_080cce90
	ldr r0, [r3, #4]
	mov lr, r0
	.2byte 0xf800
	pop {pc}
.L_080cce90:
	.4byte Data_02008000
