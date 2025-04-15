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

    DFA M = DFA::load("test01.dfa").value();

    auto c = M.instance();

    while(!feof(stdin))
    {
        unsigned char buf[BUFFER_SIZE];
        size_t len = fread(buf, sizeof(unsigned char), sizeof(buf), stdin);

        string s;
        for(int i=0;i<len;i++) s += buf[i];

        // cout<<"[@]"<<examine_bytestring(s)<<endl;

        c.recv(s);
    }

    // cout<<c.queued_length()<<' '<<c.queued_next()<<endl;

    // while(c.queued_length() != 0)
    // {
    //     int len = c.run(1);

    //     if(c.error())
    //     {
    //         cout<<"[!] Error on running... errno: "<<c.error()<<endl;
    //         cout<<"[!] While processing : "<<c.queued_next()<<endl;
    //         cout<<"[!] Left input length : "<<c.queued_length()<<"."<<endl;
    //         exit(-1);
    //     }

    //     c.show_status(cout);
    // }

    c.run(-1);
    return c.now();
}