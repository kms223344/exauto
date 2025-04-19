#include <iostream>
#include <fstream>

#include <cstdio>
#include <cassert>
#include <unistd.h>

#include <DFA.hpp>

using namespace std;
using namespace Auto;

#define BUFFER_SIZE 100

int main(int argc, char* argv[])
{
    freopen(NULL, "rb", stdin);
    assert(stdin != NULL);

    Stateconf Q(4);
    Q.setBegin(0);
    Q.setEnd(3);

    Inputconf S(2);
    S.set(0, '0');
    S.set(1, '1');

    DFA M(Q, S);

    for(int i=0;i<26;i++) for(int j=0;j<26;j++)
    {
        if(i == j) M.config(i, i, i+1);
        else M.config(i, j, i);
    }

    M.config(0, 0, 0);
    M.config(0, 1, 1);

    M.config(1, 0, 0);
    M.config(1, 1, 2);

    M.config(2, 0, 0);
    M.config(2, 1, 3);

    M.config(3, 0, 0);
    M.config(3, 1, 3);

    M.save("end_with_three_1s.dfa");

    auto c = M.instance();

    while(!feof(stdin))
    {
        unsigned char buf[BUFFER_SIZE];
        size_t len = fread(buf, sizeof(unsigned char), sizeof(buf), stdin);

        string s;
        for(int i=0;i<len;i++) s += buf[i];

        cout<<"[@]"<<examine_bytestring(s)<<endl;

        c.recv(s);
    }

    cout<<c.queued_length()<<' '<<c.queued_next()<<endl;

    while(c.queued_length() != 0)
    {
        int len = c.run(1);

        if(c.error())
        {
            cout<<"[!] Error on running... errno: "<<c.error()<<endl;
            cout<<"[!] While processing : "<<c.queued_next()<<endl;
            cout<<"[!] Left input length : "<<c.queued_length()<<"."<<endl;
            exit(-1);
        }

        c.show_status(cout);
    }

    // c.run(-1);
    // srand(time(0));
    // return c.now()+!(rand()%10);
    return c.now();
}