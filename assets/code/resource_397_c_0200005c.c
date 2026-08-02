typedef unsigned char u8;
typedef signed int s32;

u8 *Func_020003ac();
s32 Func_020003b4();
s32 Func_020003da();
extern s32 Data_02000240[];

/* The two tracked scene objects share this coordinate/terrain prefix. */
struct SceneObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 settled_y;
    u8 unknown_18[10];
    u8 layer;
};

void Func_0200005c(s32 block_x, s32 block_z)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    struct SceneObject *object;
    s32 dx = block_x;
    s32 dz = block_z;
    s32 height;

    /* Block coordinates become 16.16 fixed-point shifts of sixteen tiles. */
    object = (struct SceneObject *)Func_020003ac(Data_02000240[125]);
    dx <<= 20;
    dz <<= 20;

    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_020003b4((s32)object->layer, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }

    /* Apply the same shift to the workspace's independently optional object. */
    object = *(struct SceneObject **)(workspace + 480);
    if (object != 0) {
        object->x += dx;
        object->z += dz;
        height = Func_020003da((s32)object->layer, object->x, object->z);
        object->y = height;
        object->settled_y = height;
    }
}
