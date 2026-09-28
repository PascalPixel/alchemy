.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #8
	movs r1, #61
	bl 0x02008344
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000040
	.thumb_func
Func_02000040:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200835c
	.global Func_02000048
	.thumb_func
Func_02000048:
	movs r0, #0
	bx lr
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200844c
	.global Func_02000054
	.thumb_func
Func_02000054:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008474
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #108]
	ldr r3, [r3]
	movs r2, #250
	mov r8, r3
	ldr r3, [pc, #104]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	adds r7, r1, #0
	bl 0x02008334
	adds r5, r0, #0
	lsls r6, r6, #20
	lsls r7, r7, #20
	cmp r5, #0
	beq .L_0200005c_0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	adds r1, r1, r6
	adds r2, r2, r7
	str r1, [r5, #8]
	str r2, [r5, #16]
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	bl 0x0200831c
	str r0, [r5, #12]
	str r0, [r5, #20]
.L_0200005c_0:
	movs r3, #240
	lsls r3, r3, #1
	add r3, r8
	ldr r5, [r3]
	cmp r5, #0
	beq .L_0200005c_1
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	adds r1, r1, r6
	adds r2, r2, r7
	str r1, [r5, #8]
	str r2, [r5, #16]
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	bl 0x0200831c
	str r0, [r5, #12]
	str r0, [r5, #20]
.L_0200005c_1:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {lr}
	movs r0, #0
	movs r1, #5
	bl 0x0200805c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	push {lr}
	movs r1, #5
	negs r1, r1
	movs r0, #0
	bl 0x0200805c
	pop {r0}
	bx r0
	.global Func_020000f8
	.thumb_func
Func_020000f8:
	push {lr}
	movs r0, #0
	movs r1, #5
	bl 0x0200805c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000108
	.thumb_func
Func_02000108:
	push {lr}
	movs r1, #5
	negs r1, r1
	movs r0, #0
	bl 0x0200805c
	pop {r0}
	bx r0
	.global Func_02000118
	.thumb_func
Func_02000118:
	push {lr}
	movs r0, #0
	movs r1, #6
	bl 0x0200805c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000128
	.thumb_func
Func_02000128:
	push {lr}
	movs r1, #6
	negs r1, r1
	movs r0, #0
	bl 0x0200805c
	pop {r0}
	bx r0
	.global Func_02000138
	.thumb_func
Func_02000138:
	push {r5, lr}
	ldr r3, [pc, #28]
.L_0200013c:
	movs r0, #123
	ldr r5, [r3]
	bl 0x02008354
	movs r3, #182
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
.L_0200014e:
	bl 0x0200833c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
.L_0200015c:
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {lr}
	movs r0, #9
	bl 0x0200834c
	ldr r2, [pc, #48]
	ldr r3, [pc, #40]
	strh r3, [r2]
	ldr r3, [pc, #40]
	adds r2, #2
.L_0200016e:
	strh r3, [r2]
	ldr r3, [pc, #40]
	ldr r2, [r3]
	ldr r3, [pc, #40]
	adds r1, r2, r3
	ldr r3, [pc, #40]
	strh r3, [r1]
.L_0200017c:
	ldr r3, [pc, #40]
	adds r1, r2, r3
	movs r3, #31
	strh r3, [r1]
	ldr r3, [pc, #36]
	adds r2, r2, r3
	movs r3, #10
	strh r3, [r2]
	b .L_0200017c_0
	.2byte 0x0000
	.2byte 0x3f42
	.2byte 0x0000
	.2byte 0x0c04
	.2byte 0x0000
	.2byte 0x0050
	.2byte 0x0400
	.2byte 0x1ecc
	.2byte 0x0300
	.2byte 0x0534
	.2byte 0x0000
	.2byte 0x3f3f
	.2byte 0x0000
	.4byte 0x00000536
	.4byte 0x0000052a
.L_0200017c_0:
	pop {r0}
	bx r0
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020084a4
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {lr}
	bl 0x02008324
	bl 0x0200832c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00004770
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	bx lr
	.2byte 0x0000
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r3, [pc, #24]
	ldr r0, [pc, #24]
	ldr r3, [r3]
	strh r3, [r0]
	ldr r3, [pc, #24]
	ldr r3, [r3]
	ldr r1, [pc, #24]
	strh r3, [r0, #2]
	bl 0x02008314
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ae8
	.4byte 0x02008590
	.4byte 0x03001b04
	.4byte 0x020085b0
	.global Func_02000200
	.thumb_func
Func_02000200:
	push {r5, lr}
	ldr r5, [pc, #72]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	movs r0, #9
	bl 0x0200834c
	ldr r2, [pc, #56]
	ldr r3, [pc, #40]
	strh r3, [r2]
	ldr r3, [pc, #40]
	adds r2, #2
	strh r3, [r2]
	ldr r2, [r5, #16]
	ldr r3, [pc, #44]
	adds r1, r2, r3
	ldr r3, [pc, #44]
	strh r3, [r1]
	ldr r3, [pc, #44]
	adds r1, r2, r3
	movs r3, #31
	strh r3, [r1]
	ldr r3, [pc, #40]
	adds r2, r2, r3
	movs r3, #10
	strh r3, [r2]
	bl 0x020082e0
	movs r0, #0
	b .L_02000200_0
	.4byte 0x00003f42
	.4byte 0x00000c04
	.4byte 0x03001ebc
	.4byte 0x04000050
	.4byte 0x00000534
	.4byte 0x00003f3f
	.4byte 0x00000536
	.4byte 0x0000052a
.L_02000200_0:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_0200026c
	.thumb_func
Func_0200026c:
	push {lr}
	ldr r3, [pc, #28]
	ldrh r2, [r3]
	ldr r3, [pc, #28]
	ldr r3, [r3]
	cmp r2, r3
	blt .L_0200026c_0
	ldr r3, [pc, #24]
	b .L_0200026c_1
.L_0200026c_0:
	ldr r3, [pc, #24]
.L_0200026c_1:
	ldrh r2, [r3]
	ldr r3, [pc, #24]
	strh r2, [r3]
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x04000006
	.4byte 0x02008610
	.4byte 0x02008614
	.4byte 0x02008616
	.4byte 0x0400001c
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	ldr r3, [pc, #40]
	movs r1, #130
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	movs r3, #6
	ldrsh r1, [r2, r3]
	ldr r0, [pc, #32]
	movs r3, #192
	subs r3, r3, r1
	str r3, [r0]
	ldr r3, [pc, #28]
	movs r1, #2
	ldrsh r2, [r2, r1]
	strh r2, [r3]
	ldr r3, [pc, #24]
	ldr r3, [r3]
	ldr r1, [pc, #24]
	lsrs r3, r3, #2
	subs r2, r2, r3
	strh r2, [r1]
	bx lr
	.4byte 0x03001e70
	.4byte 0x02008610
	.4byte 0x02008614
	.4byte 0x03001e40
	.4byte 0x02008616
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {lr}
	ldr r2, [pc, #24]
	movs r0, #1
	movs r1, #0
	bl 0x0200830c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #12]
	bl 0x02008304
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200826d
	.4byte 0x020082a1
	.include "games/THE BROKEN SEAL/SRC/FIELD/TORETO_EDA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x00000064
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0x800001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0x000001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000178
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x00000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000148
	.4byte 0x800000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000148
	.4byte 0x80000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000001a8
	.4byte 0x00000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x0010202d
	.4byte 0x0020302d
	.4byte 0x0030402d
	.4byte 0x0040502d
	.4byte 0x0050702d
	.4byte 0x0060602d
	.4byte 0x0070802d
	.4byte 0x0080902d
	.4byte 0x000001ff
	.4byte 0x006d005d
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000a02
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0005
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0006
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0007
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0008
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020080d9
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020080e9
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020080f9
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008109
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008119
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008129
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008031
	.4byte 0x20009085
	.4byte 0xffff0000
	.4byte 0x0200815d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020060
	.4byte 0x00020002
	.2byte 0xffff
