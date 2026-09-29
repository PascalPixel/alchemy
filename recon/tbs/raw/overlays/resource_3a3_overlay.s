.syntax unified
	.thumb
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {r5, lr}
	ldr r3, [pc, #116]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_0200007c_0
	ldr r0, [pc, #104]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_1
	ldr r3, [pc, #100]
	adds r1, r3, #0
	movs r2, #0
	adds r1, #142
	adds r3, #166
	strb r2, [r1]
	strb r2, [r3]
.L_0200007c_1:
	ldr r0, [pc, #84]
	b .L_0200007c_2
.L_0200007c_0:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200007c_3
	ldr r0, [pc, #84]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_4
	ldr r3, [pc, #76]
	movs r2, #1
	adds r3, #46
	strb r2, [r3]
.L_0200007c_4:
	ldr r0, [pc, #72]
	bl 0x02008e88
	cmp r0, #0
	bne .L_0200007c_5
	ldr r0, [pc, #68]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_6
.L_0200007c_5:
	ldr r3, [pc, #48]
	movs r2, #1
	adds r3, #94
	strb r2, [r3]
.L_0200007c_6:
	ldr r5, [pc, #40]
	adds r0, r5, #0
	bl 0x02008eb8
	adds r0, r5, #0
	b .L_0200007c_2
.L_0200007c_3:
	ldr r0, [pc, #40]
.L_0200007c_2:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x00000909
	.4byte 0x0200940c
	.4byte 0x0000004c
	.4byte 0x000008fd
	.4byte 0x020095bc
	.4byte 0x000008fe
	.4byte 0x00000907
	.4byte 0x020093f4
	.section .text.x02008874,"ax",%progbits
	.global Func_02000874
	.thumb_func
Func_02000874:
	push {lr}
	ldr r0, [pc, #108]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_0
	movs r0, #144
	lsls r0, r0, #2
	bl 0x02008e90
.L_02000874_0:
	ldr r0, [pc, #92]
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000874_1
	ldr r0, [pc, #88]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_2
.L_02000874_1:
	ldr r0, [pc, #80]
	bl 0x02008e90
.L_02000874_2:
	ldr r0, [pc, #68]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_3
	ldr r0, [pc, #60]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_3
	ldr r0, [pc, #60]
	bl 0x02008e90
.L_02000874_3:
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000874_4
	bl 0x02008904
	b .L_02000874_5
.L_02000874_4:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000874_5
	bl 0x02008b2c
.L_02000874_5:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x000008fd
	.4byte 0x000008fe
	.4byte 0x00000907
	.4byte 0x00000241
	.4byte 0x00000242
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x0000004c
	.section .text.x02008d58,"ax",%progbits
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #176]
	ldr r1, [pc, #176]
	bl 0x02008f70
	movs r0, #252
	movs r1, #1
	movs r2, #225
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #14
	bl 0x02008f78
	bl 0x02008f80
	movs r0, #30
	bl 0x02008ea0
	movs r1, #1
	movs r0, #18
	bl 0x02008f00
	movs r0, #1
	negs r0, r0
	bl 0x02008fa8
	ldr r0, [pc, #132]
	bl 0x02008e30
	movs r0, #20
	bl 0x02008ea0
	movs r0, #0
	movs r1, #18
	movs r2, #0
	bl 0x02008f18
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008f58
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x02008f58
	movs r1, #208
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #18
	bl 0x02008f58
	movs r0, #147
	bl 0x02008fc0
	movs r1, #2
	movs r0, #18
	bl 0x02008f10
	movs r0, #20
	bl 0x02008ea0
	movs r1, #176
	movs r2, #40
	movs r0, #18
	lsls r1, r1, #8
	bl 0x02008f58
	bl 0x020087b8
	movs r0, #0
	movs r1, #1
	bl 0x02008f68
	bl 0x02008f80
	movs r1, #4
	movs r0, #14
	bl 0x02008f08
	ldr r0, [pc, #24]
	bl 0x02008e90
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x02008d09
	.4byte 0x000008ff
	.section .rodata,"a",%progbits
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000080
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00026666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00013333
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01560000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01560000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gArutinMuraEntrancesOther
gArutinMuraEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinMuraEntrances1
gArutinMuraEntrances1:
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0x800000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000150
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000118
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001c8
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000a8
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000018
	.4byte 0x200001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000148
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000157
	.4byte 0x400000b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinMuraEntrances2
gArutinMuraEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000c8
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x0000009f
	.4byte 0xc0000102
	.4byte 0x00190000
	.4byte 0x00ff0010
	.4byte 0x0000012c
	.4byte 0xffff0002
	.4byte 0x00000150
	.4byte 0xc00000d5
	.4byte 0x010c0000
	.4byte 0x01f40028
	.4byte 0x000000fa
	.4byte 0xffff0003
	.4byte 0x000002b0
	.4byte 0xc00000cc
	.4byte 0x021e0000
	.4byte 0x03000028
	.4byte 0x000000fa
	.4byte 0xffff0004
	.4byte 0x000000f0
	.4byte 0xc00001f4
	.4byte 0x006e0000
	.4byte 0x015e0150
	.4byte 0x0000021c
	.4byte 0xffff0005
	.4byte 0x000001f0
	.4byte 0xc00001f8
	.4byte 0x01680000
	.4byte 0x02580150
	.4byte 0x0000021c
	.4byte 0xffff0006
	.4byte 0x000002f0
	.4byte 0xc0000204
	.4byte 0x027b0000
	.4byte 0x034d0118
	.4byte 0x0000022c
	.4byte 0xffff0007
	.4byte 0x00000050
	.4byte 0xc0000360
	.4byte 0x00100000
	.4byte 0x01b0027e
	.4byte 0x00000390
	.4byte 0xffff0008
	.4byte 0x00000044
	.4byte 0x400002d8
	.4byte 0x00100000
	.4byte 0x01b0027e
	.4byte 0x00000390
	.4byte 0xffff0009
	.4byte 0x000002b2
	.4byte 0x400002dc
	.4byte 0x01e00000
	.4byte 0x03800272
	.4byte 0x00000372
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x0000004b
	.4byte 0x00110002
	.4byte 0x0020704c
	.4byte 0x0031d009
	.4byte 0x0040104c
	.4byte 0x0050204c
	.4byte 0x0060304c
	.4byte 0x0070504c
	.4byte 0x0080604c
	.4byte 0x0090404c
	.4byte 0x00a0104d
	.4byte 0x00b01050
	.4byte 0x00c04054
	.4byte 0x00d03053
	.4byte 0x0000004c
	.4byte 0x0010404b
	.4byte 0x0020504b
	.4byte 0x0030604b
	.4byte 0x0040904b
	.4byte 0x0050704b
	.4byte 0x0060804b
	.4byte 0x0070204b
	.4byte 0x0080904c
	.4byte 0x0090804c
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00005000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x00d90000
	.4byte 0x00005000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x003a0000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00001000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00011000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00290000
	.4byte 0x00000000
	.4byte 0x01b90000
	.4byte 0x00013000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00003000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00001000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x015e0000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x0000c000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x003c0000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00002000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x018d0000
	.4byte 0x00001000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01a60000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00001000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x01c40000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x014c0000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x014c0000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00001000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00690000
	.4byte 0x00000000
	.4byte 0x00e90000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b60000
	.4byte 0x00039000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00007000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x02b20000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00017000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x029d0000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00011000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x00910000
	.4byte 0x00033000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x01b40000
	.4byte 0x00005000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01c60000
	.4byte 0x00005000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x01c60000
	.4byte 0x00003000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00670000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00003000
	.4byte 0x00000080
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00007000
	.4byte 0x0000007b
	.4byte 0x02008fc8
	.4byte 0x015e0000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00007000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x02860000
	.4byte 0x00000000
	.4byte 0x02de0000
	.4byte 0x00003000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinMuraEventsOther
gArutinMuraEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinMuraEvents1
gArutinMuraEvents1:
	.4byte 0x0000c402
	.4byte 0xffff000a
	.4byte 0x020086a5
	.4byte 0x0000c402
	.4byte 0xffff000b
	.4byte 0x020086a5
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte 0x020086a5
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0x09090008
	.4byte 0x0200815d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001917
	.4byte 0x00000000
	.4byte 0x09090009
	.4byte 0x000018c2
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020081b1
	.4byte 0x00000000
	.4byte 0x0909000a
	.4byte 0x000018c3
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000191b
	.4byte 0x00000000
	.4byte 0x0909000b
	.4byte 0x000018c4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000191c
	.4byte 0x00000000
	.4byte 0x0909000c
	.4byte 0x000018c5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000191d
	.4byte 0x00000000
	.4byte 0x0909000d
	.4byte 0x000018c6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000191e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020081d1
	.4byte 0x00000000
	.4byte 0x0909000f
	.4byte 0x000018c8
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001922
	.4byte 0x00000000
	.4byte 0x09090010
	.4byte 0x000018c9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001923
	.4byte 0x00000000
	.4byte 0x09090011
	.4byte 0x000018ca
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008299
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020083d5
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008449
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x000018cb
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001927
	.4byte 0x00008d15
	.4byte 0x09090009
	.4byte 0x000018cc
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001928
	.4byte 0x00008d15
	.4byte 0x0909000a
	.4byte 0x000018cd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001929
	.4byte 0x00008d15
	.4byte 0x0909000b
	.4byte 0x000018ce
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000192a
	.4byte 0x00008d15
	.4byte 0x0909000c
	.4byte 0x000018cf
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000192b
	.4byte 0x00008d15
	.4byte 0x0909000d
	.4byte 0x000018d0
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000192c
	.4byte 0x00008d15
	.4byte 0x0909000e
	.4byte 0x000018d1
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000192d
	.4byte 0x00008d15
	.4byte 0x0909000f
	.4byte 0x000018d2
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000192e
	.4byte 0x00008d15
	.4byte 0x09090010
	.4byte 0x000018d3
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000192f
	.4byte 0x00008d15
	.4byte 0x09090011
	.4byte 0x000018d4
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001930
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000018ef
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000018f3
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x0200884d
	.4byte 0x00000002
	.4byte 0x08ff0014
	.4byte 0x02008d59
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gArutinMuraEvents2
gArutinMuraEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x02400008
	.4byte 0x00001902
	.4byte 0x00000000
	.4byte 0x09090008
	.4byte 0x000018d5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001931
	.4byte 0x00000000
	.4byte 0x02400009
	.4byte 0x00001903
	.4byte 0x00000000
	.4byte 0x09090009
	.4byte 0x000018d6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020082b9
	.4byte 0x00000000
	.4byte 0x0241000a
	.4byte 0x00001906
	.4byte 0x00000000
	.4byte 0x0909000a
	.4byte 0x020082d9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001937
	.4byte 0x00000000
	.4byte 0x0241000b
	.4byte 0x00001907
	.4byte 0x00000000
	.4byte 0x0909000b
	.4byte 0x000018dc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001938
	.4byte 0x00000000
	.4byte 0x0909000c
	.4byte 0x000018df
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000193b
	.4byte 0x00000000
	.4byte 0x0909000d
	.4byte 0x000018e0
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000193c
	.4byte 0x00000000
	.4byte 0x0909000e
	.4byte 0x020082f9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000193d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008361
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020083d5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008449
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0x09090013
	.4byte 0x000018f8
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001948
	.4byte 0x00000000
	.4byte 0x09090014
	.4byte 0x000018f9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001949
	.4byte 0x00000000
	.4byte 0x02400015
	.4byte 0x000018fa
	.4byte 0x00000000
	.4byte 0x02410015
	.4byte 0x000018fb
	.4byte 0x00000000
	.4byte 0x09090015
	.4byte 0x000018fc
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008319
	.4byte 0x00008d15
	.4byte 0x02400008
	.4byte 0x00001904
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x000018d7
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001935
	.4byte 0x00008d15
	.4byte 0x02400009
	.4byte 0x00001905
	.4byte 0x00008d15
	.4byte 0x09090009
	.4byte 0x000018d8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001936
	.4byte 0x00008d15
	.4byte 0x0241000a
	.4byte 0x00001908
	.4byte 0x00008d15
	.4byte 0x0909000a
	.4byte 0x000018dd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001939
	.4byte 0x00008d15
	.4byte 0x0241000b
	.4byte 0x00001909
	.4byte 0x00008d15
	.4byte 0x0909000b
	.4byte 0x000018de
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000193a
	.4byte 0x00008d15
	.4byte 0x0909000c
	.4byte 0x000018e4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000193e
	.4byte 0x00008d15
	.4byte 0x0909000d
	.4byte 0x000018e5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000193f
	.4byte 0x00008d15
	.4byte 0x0909000e
	.4byte 0x000018e6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001940
	.4byte 0x00008d15
	.4byte 0x0242000f
	.4byte 0x000018eb
	.4byte 0x00008d15
	.4byte 0x0909000f
	.4byte 0x000018ec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001942
	.4byte 0x00008d15
	.4byte 0x09090011
	.4byte 0x000018f0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001944
	.4byte 0x00008d15
	.4byte 0x09090010
	.4byte 0x000018f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001946
	.4byte 0x00008d15
	.4byte 0x09090012
	.4byte 0x000018fd
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000194d
	.4byte 0x00008d15
	.4byte 0x09090013
	.4byte 0x000018fe
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000194e
	.4byte 0x00008d15
	.4byte 0x09090014
	.4byte 0x000018ff
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000194f
	.4byte 0x00008d15
	.4byte 0x02400015
	.4byte 0x00001900
	.4byte 0x00008d15
	.4byte 0x09090015
	.4byte 0x00001901
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001950
	.4byte 0x00000033
	.4byte 0x0f730064
	.4byte 0x001000b5
	.4byte 0x00000033
	.4byte 0x0f740065
	.4byte 0x00200009
	.4byte 0x000000e3
	.4byte 0x0f750066
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a4
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a5
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029a6
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x004029a7
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0021002a
	.4byte 0x00020002
	.4byte 0x002a0000
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00210028
	.4byte 0x00020002
	.4byte 0x00280000
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x00300021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00230023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x002e0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00320023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020032
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020021
	.4byte 0x00050002
	.4byte 0x00210021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00340023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020034
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global ArutinMura_TriggerTable
ArutinMura_TriggerTable:
	.4byte 0x02009dea
	.4byte 0x00050029
	.4byte 0x02009dc0
	.4byte 0x00060033
	.4byte 0x02009e2c
	.4byte 0x00110039
	.4byte 0x02009e42
	.4byte 0x00110027
	.4byte 0x02009e58
	.4byte 0x00190030
	.4byte 0x02009e00
	.4byte 0x0009003b
	.4byte 0x02009e16
	.4byte 0x0017002a
	.4byte 0x02009e42
	.4byte 0x001b0029
