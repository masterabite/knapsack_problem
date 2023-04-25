/*Задача о рюкзаке. Дано n предметов, их веса, и ценности. 
Ценность каждого предмета находится в пределах от l до r.
В ответ нужно вывести кол-во предметов и их номера*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
 
using namespace std;
 
struct backpack {
    long long w = 0, x = 0, mask = 0;
};
 
long long i, n, l, r, mx = 0, mxmsk, msk, pwr[33], sum, len1, len2, curx;
backpack a[33], d[1048576], null;
vector <int> ans;
vector <backpack> d_new;
 
 
bool cmp(backpack q, backpack w) {
    if (q.w > w.w) {
        return true;
    } else if (q.w == w.w) {
        return (q.x > w.x);
    } else {
        return false;
    }
}
 
int binpos(long long q) {
    int ll = -1,
        rr = int(d_new.size());
    int md;
    while (rr - ll > 1) {
        md = (ll + rr) / 2;
        if (d_new[md].w > q) {
            ll = md;
        } else {
            rr = md;
        }
    }
    return rr;
}
 
void out() {
    for (int i = 0; i < n; ++i) {
        cout << a[i].w << ' ' << a[i].x << ' ' << a[i].mask << '\n';
    }
    cout << '\n';
}
 
int main() {
    freopen("dowry.in", "r", stdin);
    freopen("dowry.out", "w", stdout);
 
    pwr[0] = 1;
    for (int i = 1; i <= 32; ++i) {
        pwr[i] = pwr[i - 1] * 2;
    }
 
    cin >> n >> l >> r;
    r -= l;
    for (int i = 0; i < n; ++i) {
        cin >> a[i].w >> a[i].x;
    }
 
    len1 = int(n / 2);
    len2 = n - len1;
 
    for (int i = 0; i < pwr[len1]; ++i) {
        msk = i;
        d[i].mask = msk;
        for (int k = 0; msk > 0; msk /= 2, ++k) {
            if (msk % 2 == 1) {
                d[i].w += a[k].w;
                d[i].x += a[k].x;
            }
        }
        d[i].w -= l;
    }
 
    sort(d, d + pwr[len1], cmp);
 
    for (int i = pwr[len1] - 1; i >= 0; --i) {
        while (d[i].w == d[i - 1].w && i >= 0) {
            --i;
        }
        if (d_new.empty() || d_new.back().x < d[i].x) {
            d_new.push_back(d[i]);
        }
    }
    reverse(d_new.begin(), d_new.end());
 
    /*for (int i = 0; i < int(d_new.size()); ++i) {
        cout << d_new[i].w << ' '<< d_new[i].x << ' '<< d_new[i].mask << '\n';
    }*/
 
    for (int i = 0; i < pwr[len2]; ++i) {
        msk = i;
        msk;
        d[i].mask = msk;
        d[i].w = 0;
        d[i].x = 0;
        for (int k = 0; msk > 0; msk /= 2, ++k) {
            if (msk % 2 == 1) {
                d[i].w += a[k + len1].w;
                d[i].x += a[k + len1].x;
            }
        }
 
        d[i].mask <<= len1;
        int cur = binpos(r - d[i].w);
        while (cur < int(d_new.size()) && d[i].w + d_new[cur].w >= 0 && d[i].w + d_new[cur].w <= r) {
            if (d_new[cur].x + d[i].x > mx) {
                mx = d_new[cur].x + d[i].x;
                mxmsk = d[i].mask + d_new[cur].mask;
            }
        ++cur;
        }
    }
 
    for (int k = 0; mxmsk > 0; mxmsk /= 2, ++k) {
        if (mxmsk % 2 == 1) {
            ans.push_back(k + 1);
        }
    }
    cout << int(ans.size()) << '\n';
    for (int i = 0; i < int(ans.size()); ++i) {
        cout << ans[i] << ' ';
    }
 
    return 0;
}
