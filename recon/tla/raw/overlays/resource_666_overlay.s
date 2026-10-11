.syntax unified
	.thumb
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #0
	bl Func_02000bac
	movs r3, #128
	ldr r0, .L_02008720
	movs r1, #8
	movs r2, #15
	lsls r3, r3, #23
	bl Func_02000bb4
	ldr r5, .L_02008724
	movs r3, #128
	movs r1, #8
	movs r2, #15
	adds r0, r5, #0
	lsls r3, r3, #23
	bl Func_02000bb4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02008728
	str r3, [r0, #108]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200872c
	str r3, [r0, #108]
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d4
	ldr r3, .L_02008730
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #18
	cmp r3, r2
	blt .L_02008678
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008680
.L_02008678:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008680:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086c6
	movs r1, #188
	movs r2, #158
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	movs r1, #196
	movs r2, #172
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	b .L_0200871c
.L_020086c6:
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	movs r0, #32
	b .L_020086f2
.L_020086d4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086fc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200871c
	movs r0, #31
.L_020086f2:
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	b .L_0200871c
.L_020086fc:
	movs r0, #24
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200871c:
	movs r0, #0
	pop {r5, pc}
.L_02008720:
	.4byte Data_02001c90
.L_02008724:
	.4byte Data_02001c60
.L_02008728:
	.4byte Func_02000578
.L_0200872c:
	.4byte Func_02000598
.L_02008730:
	.4byte gPartyState
	.section .rodata.x02008be4,"a",%progbits
	.global Data_02000be4
Data_02000be4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
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
	.global gSceneExits
gSceneExits:
	.4byte 0x00000060
	.4byte 0x1010105f
	.4byte 0xffffffff
	.4byte 0x1020205f
	.4byte 0xffffffff
	.4byte 0x1050505f
	.4byte 0xffffffff
	.4byte 0x1060605f
	.4byte 0xffffffff
	.4byte 0x1070705f
	.4byte 0xffffffff
	.4byte 0x11414060
	.4byte 0xffffffff
	.4byte 0x11515060
	.4byte 0xffffffff
	.4byte 0x11616060
	.4byte 0xffffffff
	.4byte 0x11717060
	.4byte 0xffffffff
	.4byte 0x11818060
	.4byte 0xffffffff
	.4byte 0x11919060
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000cbc
Data_02000cbc:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00022000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00006000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00008000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f14
Data_02000f14:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200116c
Data_0200116c:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0000b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00002000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x00003000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013dc
Data_020013dc:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgScene666No121
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgScene666No122
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgScene666No123
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgScene666No124
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene666No125
	.2byte 0x0040
	.4byte 0x00000002
	.4byte 0x08b2001e
	.4byte Func_02000084
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene666No01
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene666No02
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene666No03
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene666No04
	.4byte 0x00000000
	.4byte 0x08b1000e
	.4byte Func_020002bc
	.4byte 0x00008d15
	.4byte 0x08b1040e
	.4byte Func_020002bc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene666No05
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000298
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000444
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene666No09
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene666No10
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene666No11
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene666No12
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene666No13
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene666No14
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene666No15
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene666No16
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene666No17
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene666No18
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene666No19
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene666No20
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene666No21
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgScene666No22
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene666No23
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene666No24
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgScene666No25
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgScene666No26
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgScene666No27
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene666No28
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene666No29
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene666No30
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgScene666No32
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene666No33
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene666No34
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene666No35
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene666No36
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene666No37
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene666No38
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene666No39
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene666No40
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene666No41
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgScene666No42
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgScene666No43
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene666No44
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene666No45
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene666No46
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene666No47
	.4byte 0x00008d15
	.4byte 0xffff041f
	.4byte Func_0200047c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene666No49
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene666No50
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001730
Data_02001730:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgScene666No121
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgScene666No122
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgScene666No123
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgScene666No124
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene666No125
	.2byte 0x0040
	.4byte 0x00000000
	.4byte 0x1971001e
	.4byte MsgScene666No84
	.4byte 0x00008d15
	.4byte 0x1971001e
	.4byte MsgScene666No85
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene666No51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene666No52
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene666No53
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene666No54
	.4byte 0x00000000
	.4byte 0x08b8000c
	.4byte Func_020004b8
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene666No56
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene666No57
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene666No58
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene666No59
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene666No60
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene666No61
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene666No62
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene666No63
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene666No64
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene666No65
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene666No66
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene666No67
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene666No68
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene666No69
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene666No70
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene666No71
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene666No72
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene666No73
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020004f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene666No75
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene666No76
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene666No77
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene666No78
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene666No79
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene666No80
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene666No81
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene666No82
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgScene666No83
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene666No86
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene666No87
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019dc
Data_020019dc:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgScene666No121
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgScene666No122
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgScene666No123
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgScene666No124
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene666No125
	.2byte 0x0040
	.4byte 0x00000002
	.4byte 0x1823001f
	.4byte Func_02000738
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene666No88
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene666No89
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene666No90
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene666No91
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene666No92
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene666No93
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene666No94
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene666No95
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene666No96
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene666No97
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene666No98
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene666No99
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene666No100
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene666No101
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene666No102
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene666No103
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene666No104
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene666No105
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene666No106
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene666No107
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene666No108
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene666No109
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene666No110
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene666No111
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene666No112
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene666No113
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene666No114
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene666No115
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene666No116
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene666No117
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene666No118
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene666No119
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global Data_02001c60
Data_02001c60:
	.space 0x00000030
	.global Data_02001c90
Data_02001c90:
