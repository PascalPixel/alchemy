/* resource_3ce: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/COMMANDS.o"(.text)
        "*/resource_3ce_overlay.o"(.text.x02008180)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/BLOCK_D4C.o"(.text)
        "*/resource_3ce_overlay.o"(.text.x020081b8)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/SELECT_ITEM.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/COMMAND_LIST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/DEBUG/MENU_TEST/IMPORT.o"(.text)
        "*/resource_3ce_overlay.o"(.rodata)
    }
    .bss : { "*/resource_3ce_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
