#include "src.hpp"
#include <iostream>
#include <cstdio>

int main() {
    ESet<long long> s[25];
    ESet<long long>::iterator it;
    int op, lst = 0, it_a = -1, valid = 0;

    while (scanf("%d", &op) != EOF) {
        long long a, b, c;

        switch (op) {
            case 0: {
                // emplace(a, b) - insert b into set s[a]
                scanf("%lld%lld", &a, &b);
                auto p = s[a].emplace(b);
                if (p.second) {
                    it_a = a;
                    it = p.first;
                    valid = 1;
                }
                break;
            }
            case 1: {
                // erase(a, b) - erase b from set s[a]
                scanf("%lld%lld", &a, &b);
                if (valid && it_a == a && *it == b) valid = 0;
                s[a].erase(b);
                break;
            }
            case 2: {
                // copy set s[a] to s[++lst]
                scanf("%lld", &a);
                s[++lst] = s[a];
                break;
            }
            case 3: {
                // find(a, b) - find b in set s[a]
                scanf("%lld%lld", &a, &b);
                auto it2 = s[a].find(b);
                if (it2 != s[a].end()) {
                    printf("true\n");
                    it_a = a;
                    it = it2;
                    valid = 1;
                } else {
                    printf("false\n");
                }
                break;
            }
            case 4: {
                // range(a, b, c) - count elements in range [b, c] in set s[a]
                scanf("%lld%lld%lld", &a, &b, &c);
                printf("%d\n", (int)s[a].range(b, c));
                break;
            }
            case 5: {
                // iterator decrement
                if (valid) {
                    auto it2 = it;
                    if (it == --it2) valid = 0;
                }
                if (valid)
                    printf("%lld\n", *(--it));
                else
                    printf("-1\n");
                break;
            }
            case 6: {
                // iterator increment
                if (valid) {
                    auto it2 = ++it;
                    if (it == ++it2) valid = 0;
                    else printf("%lld\n", *it);
                }
                if (!valid)
                    printf("-1\n");
                break;
            }
        }
    }

    return 0;
}
