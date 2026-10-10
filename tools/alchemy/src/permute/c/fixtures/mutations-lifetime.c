struct S { int member; };
extern int new_var;
int called(int a);
int copied(struct S s);
int target(int a, int b, struct S *p)
{
    int x = 1;
    int new_var2;
    int y = 2;
    int arr[2];
    int *ptr;
    struct S s;
    int spare;
    x = a + b;
    y = a + b;
    arr[x] = x + y;
    if (x) {
        int z = x;
        arr[z] = a + b;
    }
    for (x = 0; x < b; x++) { arr[x] = x; }
    p->member += x;
    s = *p;
    ptr = &x;
    *ptr = y;
    new_var2 = copied(s);
    spare = sizeof(called(a));
    return (x = a) + x + !(y = called(b)) + y + p->member + new_var + spare + new_var2;
}
