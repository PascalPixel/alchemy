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
	.4byte Data_02000124
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #225
	adds r1, r1, r3
	adds r0, r1, #0
	bl Func_02000088
	pop {pc}
	.2byte 0x0000
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	ldr r0, .L_0200806c
	bx lr
.L_0200806c:
	.4byte Data_0200019c
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	movs r0, #0
	bx lr
	.section .text.x02008084,"ax",%progbits
	.global Func_02000084
	.thumb_func
Func_02000084:
	movs r0, #0
	bx lr
	.section .rodata.x02008090,"a",%progbits
.L_02008090:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00040000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000011
	.global Data_020000c4
Data_020000c4:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
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
	.4byte 0x0000009d
	.4byte 0x1010209d
	.4byte 0xffffffff
	.4byte 0x1020109d
	.4byte 0xffffffff
	.4byte 0x1030309b
	.4byte 0xffffffff
	.4byte 0x1040409b
	.4byte 0xffffffff
	.4byte 0x1050509b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000124
Data_02000124:
	.4byte Tileset_Set114TilesD + 0x4e3
	.4byte .L_02008090
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x0002c000
	.4byte Tileset_Set116TilesD + 0x693
	.4byte .L_02008090
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0002c000
	.4byte Field_Map290 + 0x577
	.4byte .L_02008090
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x0002c000
	.4byte Field_Map295 + 0x31b
	.4byte .L_02008090
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200019c
Data_0200019c:
	.4byte 0x00000001
	.4byte 0x18eb0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004e15
	.4byte Tileset_Set114TilesD + 0x3b8
	.4byte Func_02000054
	.4byte 0x00004e15
	.4byte Tileset_Set116TilesD + 0x569
	.4byte Func_02000054
	.4byte 0x00004e15
	.4byte Field_Map290 + 0x44e
	.4byte Func_02000054
	.4byte 0x00004e15
	.4byte Field_Map295 + 0x1f3
	.4byte Func_02000054
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
