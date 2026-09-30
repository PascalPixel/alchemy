.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020000c4
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_020000f4
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000108
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020000bc
	movs r0, #48
	bl Func_020000b4
	pop {pc}
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {lr}
	movs r0, #9
	movs r1, #1
	movs r2, #0
	bl Func_020000bc
	movs r0, #68
	bl Func_020000b4
	pop {pc}
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #10
	movs r1, #2
	movs r2, #0
	bl Func_020000bc
	movs r0, #88
	bl Func_020000b4
	pop {pc}
	.section .text.x02008090,"ax",%progbits
	.global Func_02000090
	.thumb_func
Func_02000090:
	push {lr}
	movs r0, #11
	movs r1, #3
	movs r2, #0
	bl Func_020000bc
	movs r0, #108
	bl Func_020000b4
	pop {pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	ldr r0, .L_020080a8
	bx lr
.L_020080a8:
	.4byte Data_02000120
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	movs r0, #0
	bx lr
	.section .text.x020080b0,"ax",%progbits
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	movs r0, #0
	bx lr
	.section .rodata.x020080c4,"a",%progbits
	.global Data_020000c4
Data_020000c4:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020000f4
Data_020000f4:
	.4byte 0x00000139
	.4byte 0x0000013a
	.4byte 0x0000013b
	.4byte 0x0000013c
	.4byte 0x000001ff
	.global Data_02000108
Data_02000108:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000120
Data_02000120:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000068
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200007c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000090
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte GameFlagBytes + 0x1c0
	.4byte 0x03600400
	.4byte 0xffffffff
	.4byte 0xffffffff
