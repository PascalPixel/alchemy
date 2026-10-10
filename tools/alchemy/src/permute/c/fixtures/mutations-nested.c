struct S { int member; };
int target(int a, struct S *p)
{
    int x;
    int pad;
    float f = .5f;
    int arr[2] = {0, 1};
    x = p->member + arr[a];
    {
        int y = 0;
        y = x;
        x = y + a;
    }
    switch (a) {
        case 0: x = 0; break;
        default: x = p->member; break;
    }
    done: x += a;
    return x;
}
