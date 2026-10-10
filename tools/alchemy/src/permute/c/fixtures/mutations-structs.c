struct I { int member; };
struct O { struct I *ptr; struct I value; };
int target(struct I *p, struct I **pp, struct O *o, int i)
{
    return (p + i)->member + (*(p + i)).member + p[i].member
        + (&p[i])->member + pp[i]->member + (*pp[i]).member
        + o->ptr->member + o->value.member + (&o->value)->member
        + (*(p + (i - 1))).member + (*(p - (i + 1))).member
        + (*(&(p[i]))).member + (&(*(p + i)))->member
        + (*(*pp)).member + ((struct I *)p)->member;
}
