#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1010, MAXMASK = 1 << 10;
vector<int> g[MAXN];
int LBIT[MAXMASK], RBIT[MAXMASK];
int preorder[MAXN], I[MAXN], A[MAXN], par_I[MAXN], timer;

void dfs1(int v, int p) {
    I[v] = preorder[v] = ++timer;
    for (int to : g[v]) {
        dfs1(to, v);
        if (RBIT[I[to]] > RBIT[I[v]])
            I[v] = I[to];
    }
    par_I[I[v]] = p;
}

void dfs2(int v, int up) {
    A[v] = up | RBIT[I[v]];
    for (int to : g[v])
        dfs2(to, A[v]);
}

void preprocess(int root) {
    for (int i = 1; i < MAXMASK; i <<= 1)
        fill(LBIT + i, LBIT + i + i, i);
    for (int i = 1; i < MAXMASK; RBIT[i] = i, i <<= 1)
        copy(RBIT + 1, RBIT + i, RBIT + i + 1);
    dfs1(root, -1);
    dfs2(root, 0);
}

inline int enter_into_strip(int x, int hz) {
    if (RBIT[I[x]] == hz)
        return x;
    int hw = LBIT[A[x] & (hz - 1)];
    return par_I[(I[x] & (~hw + 1)) | hw];
}

inline int lca(int x, int y) {
    int hb = I[x] == I[y] ? RBIT[I[x]] : LBIT[I[x] ^ I[y]]; // Not a bug
    int hz = RBIT[A[x] & A[y] & (~hb + 1)];
    int ex = enter_into_strip(x, hz);
    int ey = enter_into_strip(y, hz);
    return preorder[ex] < preorder[ey] ? ex : ey;
}

int main()
{
    int n, m, q, x, y;
    scanf("%d", &n);
    for(int i = 1; i <= n; ++i)
    {
        scanf("%d", &m);
        g[i].resize(m);
        for(int j = 0; j < m; ++j)
            scanf("%d", &g[i][j]);
    }

    preprocess(1);

    scanf("%d", &q);
    for(int i = 0; i < q; ++i)
    {
        scanf("%d%d", &x, &y);
        printf("%d\n", lca(x, y));
    }
    
    return 0;
}
