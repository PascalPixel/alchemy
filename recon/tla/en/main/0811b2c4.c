#include "TYPES.H"
#include "OBJDISP.H"
#include "RAM_BUFFER.H"
#include "MOTOBJ.H"
#include "BTLWORK.H"
#include "BTLSYS.H"
#include "INVENTORY.H"

s32 EnmUpd(void)
{
    struct BtlWork *work = BtlWk;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;

    for (i = 0; i < 6 && work->enemy_units[i] != 0xff; i++)
        unit_ids[i] = work->enemy_units[i];
    count = i;
    Summon_LayoutPositions(unit_ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = work->enemy_units[i];

        if (id != 0xfe) {
            struct BtlObj *object = BtlObjGet(id);

            object->anchor_x = x[i] << 16;
            object->anchor_z = z[i] << 16;
        }
    }
}
