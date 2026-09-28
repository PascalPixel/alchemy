.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008108
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008180
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008194
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020081c4
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, lr}
	ldr r3, [pc, #124]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #68
	str r3, [r2]
	subs r3, #60
	adds r2, r1, r3
	movs r0, #192
	movs r3, #24
	str r3, [r2]
	lsls r0, r0, #2
	sub sp, #8
	bl 0x020080e8
	cmp r0, #0
	beq .L_02000054_0
	movs r1, #216
	movs r2, #136
	lsls r2, r2, #16
	movs r0, #8
	lsls r1, r1, #16
	bl 0x020080f8
	movs r1, #2
	movs r0, #8
	bl 0x02008100
	movs r0, #8
	bl 0x020080f0
	movs r1, #0
	bl 0x020080e0
	movs r0, #8
	bl 0x020080f0
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #8
	bl 0x020080f0
	movs r5, #0
	adds r0, #89
	movs r3, #11
	movs r2, #6
	strb r5, [r0]
	movs r1, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r2, #5
	movs r3, #5
	bl 0x020080d8
.L_02000054_0:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.include "games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x40000058
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0x40000158
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000088
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001a
	.4byte 0x0010401b
	.4byte 0x0020501b
	.4byte 0x0030301b
	.4byte 0x000001ff
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
