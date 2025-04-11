#ifndef AUTOMATA_BASE
#define AUTOMATA_BASE

#include <string>
#include <map>
#include <set>
#include <vector>
#include <format>
#include <queue>
#include <ostream>

#include <cassert>
#include <cstdint>

namespace Auto
{

std::string examine_bytestring(std::string);

class Inputconf
{
private:
    const size_t nInput; // the number of inputs; character : 0~nInput-1
    size_t mapping[256];
    std::map<size_t, unsigned char> alias;

public:
    Inputconf(size_t nInput) : nInput(nInput)
    {
        assert(nInput != 0);
        for(int i=0;i<256;i++) mapping[i] = -1;
    }


    int set(size_t idx, unsigned char c)
    {
        if(idx >= nInput) return -1;

        mapping[c] = idx;
        alias[idx] = c;
    }

    int setseq(size_t idx, std::string s)
    {
        const size_t n = s.size();

        if(idx+n-1 >= nInput) // Todo: may have vulnerable!! (overflow)
            return -1;

        bool chkdup[256];
        for(int i=0;i<256;i++) chkdup[i] = (mapping[i] != -1);

        for(int i=0;i<n;i++)
        {
            if(chkdup[s[i]] != -1)
                chkdup[s[i]] = true;
            else return -1;
        }

        for(int i=0;i<n;i++)
        {
            mapping[s[i]] = idx + i;
            alias[idx+i] = s[i];
        }

        return 0;
    }

    size_t size() const {return nInput;}
    size_t convert(unsigned char c) const
    {
        return mapping[c];
    }
    std::vector<size_t> convert(std::string s) const
    {
        std::vector<size_t> cvt;
        for(auto c:s) cvt.push_back(mapping[c]);
        return cvt;
    }
};

class Stateconf
{
private:
    const size_t nState;
    size_t q0;
    std::set<size_t> qf;


public:
    Stateconf(size_t nState):nState(nState)
    {
        assert(nState != 0);
        q0 = -1;
    }

    int setBegin(size_t q0)
    {
        if(q0 >= nState) return -1;
        this->q0 = q0;
        
        return 0;
    }

    int setEnd(size_t qf)
    {
        this->qf.insert(qf);
        return 0;
    }
    int unsetEnd(size_t qf)
    {
        this->qf.erase(qf);
        return 0;
    }

    size_t start_state() const {return q0;}
    size_t size() const {return nState;}
    bool isfin(size_t q) const {return qf.count(q);}
};

class Executer
{
private:
    const Stateconf Q;
    const Inputconf S;

protected:
    size_t q_now;
    int _errno = 0;
    std::queue<size_t> stream;
    Executer(Stateconf Q, Inputconf S):Q(Q), S(S), q_now(Q.start_state()) {}

public:

    int recv(const std::vector<size_t>& vec)
    {
        for(auto i : vec)
        {
            if(i < S.size()) stream.push(i);
            else return -1;
        }
        return 0;
    }
    int recv(const std::string& s){ return recv(S.convert(s)); }

    bool isfin() const {return Q.isfin(q_now);}
    size_t queued_length() const {return stream.size();}
    size_t queued_next() const {return stream.front();}
    bool error() const {return _errno;}

    virtual size_t run(size_t step) {};
    virtual void show_status(std::ostream& os) const {};
};

}

#endif