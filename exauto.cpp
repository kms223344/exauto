#include <cstdio>
#include <cassert>

#include <unistd.h>

#include <iostream>

#include "Automata"

using namespace std;
using namespace Auto;

#define BUFFER_SIZE 100

int main(int argc, char* argv[])
{
    freopen(NULL, "rb", stdin);
    assert(stdin != NULL);

    Stateconf Q(27);
    Q.setBegin(0);
    Q.setEnd(26);

    Inputconf S(26);
    S.setseq(0, "abcdefghijklmnopqrstuvwxyz");

    DFA M(Q, S);

    for(int i=0;i<26;i++) for(int j=0;j<26;j++)
    {
        if(i == j) M.config(i, i, i+1);
        else M.config(i, j, i);
    }

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
        int len = c.run(2);

        if(c.error())
        {
            cout<<"[!] Error on running... errno: "<<c.error()<<endl;
            cout<<"[!] While processing : "<<c.queued_next()<<endl;
            cout<<"[!] Left input length : "<<c.queued_length()<<"."<<endl;
            exit(-1);
        }

        c.show_status(cout);
    }

    auto cc = M.instance();
    cout<<examine_bytestring("안녛아세")<<endl;

    cc.recv("abcd");

    while(cc.queued_length() != 0)
    {
        int len = cc.run(1);

        if(cc.error())
        {
            cout<<"[!] Error on running... errno: "<<cc.error()<<endl;
            cout<<"[!] While processing : "<<cc.queued_next()<<endl;
            cout<<"[!] Left input length : "<<cc.queued_length()<<"."<<endl;
            exit(-1);
        }

        cc.show_status(cout);
    }

}