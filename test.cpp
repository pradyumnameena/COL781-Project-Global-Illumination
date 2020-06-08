#include <bits/stdc++.h>

using namespace std;

class A
{
public:
    int x;
    A() { x = 5; };
};

class B : public A
{
public:
    int y;
    B()
    {
        y = 10;
        x = 10;
    }
};

int main()
{
    vector<A> vec;
    A a;
    B b;
    vec.push_back(a);
    vec.push_back(b);
    cout << vec[0].x << endl;
    return 0;
}