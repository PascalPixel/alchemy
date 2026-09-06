@ Command slot 35 wrapper; reference call uses the handler in r1.
.syntax unified
.thumb
.global AudioCommand_InvokeSlot35
.global Func_080fa68c
.thumb_func
AudioCommand_InvokeSlot35:
Func_080fa68c:
    push {lr}
    ldr r1, .Lcommand_slot
    ldr r1, [r1]
    bl Func_080072e8
    pop {r0}
    bx r0
.balign 4, 0
.Lcommand_slot:
    .4byte 0x0200408c
