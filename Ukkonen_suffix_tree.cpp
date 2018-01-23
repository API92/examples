#include <bits/stdc++.h>
using namespace std;
 
const int MAXLEN = (int)2.5e5 + 100;
char s[2][MAXLEN];
 
const int ALPH = 26;
const char SC = 'a';
 
struct node {
    node *par;
    char *l, *r;
    node *next[ALPH];
    node *suflink;
};
 
node t[2 * MAXLEN + 1];
int tsz = 0;
 
node * new_node(node *par, char *l, char *r)
{
    node *v = &t[tsz++];
    v->par = par;
    v->l = l;
    v->r = r;
    memset(v->next, 0, sizeof(v->next));
    v->suflink = nullptr;
    return v;
}
 
node * create_root()
{
    node *v = new_node(nullptr, nullptr, nullptr);
    v->suflink = v;
    return v;
}

node *root = create_root();
 
struct state {
    node *v;
    char *pos;
    state(node *v = nullptr, char *pos = nullptr) : v(v), pos(pos) {}
};
 
inline state go(state st, char c)
{
    if (st.pos == st.v->r) {
        node *to = st.v->next[c - SC];
        return to ? state(to, to->l + 1) : state();
    }
    return *st.pos == c ? state(st.v, st.pos + 1) : state();
}
 
node * split(state st)
{
    node *v = st.v;
    if (st.pos == v->r)
        return st.v;
    if (st.pos == v->l)
        return v->par;
      
    node *mid = new_node(v->par, v->l, st.pos);
    mid->next[*st.pos - SC] = v;
    mid->par->next[*mid->l - SC] = mid;

    v->l = st.pos;
    v->par = mid;

    return mid;
}
 
state fastgo(node *v, char *l, char *r)
{
    if(l == r)
        return state(v, v->r);
    for (;;) {
        node *next = v->next[*l - SC];
        if (r - l <= next->r - next->l)
            return state(next, next->l + (r - l));
        l += next->r - next->l;
        v = next;
    }
}
 
node * suflink(node *v)
{
    if (!v->suflink)
        v->suflink = split(fastgo(
                    suflink(v->par), v->l + (v->par == root), v->r));
    return v->suflink;
}
 
state add_suffix(state ptr, char *l, char *r)
{
    for (;;) {
        state nptr = go(ptr, *l);
        if (nptr.v)
            return nptr;
        node *mid = split(ptr);
        mid->next[*l - SC] = new_node(mid, l, r);
        ptr = state(suflink(mid), suflink(mid)->r);
        if (mid == root)
            return ptr;
    }
}
 
void build_tree(char *l, char *r)
{
    state ptr(root, root->r);
    for (; l != r; ++l)
        ptr = add_suffix(ptr, l, r);
}

int lcs(char *s0, char *s1)
{
    build_tree(s0, s0 + strlen(s0));

    int res_lcs = 0;
    int cur_lcs = 0;
    state ptr(root, root->r);
    for (int i = 0, len = strlen(s1); i < len; ++i) {
        for (;;) {
            state nptr = go(ptr, s1[i]);
            if (nptr.v) {
                ptr = nptr;
                ++cur_lcs;
                break;
            }
            node *par = ptr.v->par;
            if (ptr.v == root || (par == root && ptr.pos == ptr.v->l))
                break;
            ptr = fastgo(suflink(par), ptr.v->l + (par == root), ptr.pos);
            --cur_lcs;
        }
        res_lcs = max(res_lcs, cur_lcs);
    }
    return res_lcs;
}
 
int main()
{
    scanf("%s %s", s[0], s[1]);
    printf("%d\n", lcs(s[0], s[1]));
    return 0;
}

