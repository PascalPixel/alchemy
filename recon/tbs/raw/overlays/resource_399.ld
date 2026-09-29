/* resource_399: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/FACING.o"(.text)
        "*/resource_399_overlay.o"(.text.x02008130)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/SCENE_DATA.o"(.text)
        "*/resource_399_overlay.o"(.text.x0200816c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/FORWARD.o"(.text)
        "*/resource_399_overlay.o"(.text.x020081ec)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/EXIT_DOOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/WEAPON_SHOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ARMOR_SHOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ITEM_SHOP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/PRIMARY.o"(.text)
        "*/resource_399_overlay.o"(.text.x020095b4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/CHOREOGRAPHY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMPORT.o"(.text)
        "*/resource_399_overlay.o"(.rodata)
    }
    .bss : { "*/resource_399_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
