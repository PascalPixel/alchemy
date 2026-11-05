.syntax unified
	.thumb
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r1, .L_020081ec
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_020081f0
.L_020081e0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020081e0
	b .L_020081f0
.L_020081ec:
	.4byte 0x0000ffff
.L_020081f0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020083f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200842e,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02006000
gData_02006000:
	.space 0x00000004
	.global gData_02006004
gData_02006004:
	.space 0x00000004
	.global gData_02006008
gData_02006008:
	.space 0x00000004
	.global gData_0200600c
gData_0200600c:
	.space 0x00000004
	.global gData_02006010
gData_02006010:
	.space 0x00000004
	.global gData_02006014
gData_02006014:
	.space 0x00000004
	.global Data_02006018
Data_02006018:
	.space 0x00000004
	.global gData_0200601c
gData_0200601c:
	.space 0x00000004
	.global gData_02006020
gData_02006020:
	.space 0x00000010
	.global gData_02006030
gData_02006030:
