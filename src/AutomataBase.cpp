#include "AutomataBase.hpp"

#include <format>

namespace Auto
{

std::string examine_bytestring(std::string s)
{
    std::string ret;
    for(auto c : s)
    {
        if(32 <= c && c <= 126 && c != 92)
            ret += c;
        else if(c == 92) ret += "\\\\";
        else ret += "\\"+std::format("{:02X}", c); 
        // else ret += "{"+std::to_string(uint8_t(c))+"}";
    }
    return ret;
}

// Inputconf
Inputconf::Inputconf(size_t nInput) : nInput(nInput)
{
    assert(nInput != 0);
    for(int i=0;i<256;i++) mapping[i] = -1;
}

int Inputconf::set(size_t idx, unsigned char c)
{
    if(idx >= nInput) return -1;
    if(mapping[c] != -1 || alias.count(idx)) return -1;

    mapping[c] = idx;
    alias[idx] = c;

    return 0;
}

int Inputconf::setseq(size_t idx, std::string s)
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

        if(alias.count(idx+i))
            return -1;
    }

    for(int i=0;i<n;i++)
    {
        mapping[s[i]] = idx + i;
        alias[idx+i] = s[i];
    }

    return 0;
}
size_t Inputconf::size() const {return nInput;}

size_t Inputconf::convert(unsigned char c) const
{
    return mapping[c];
}

std::vector<size_t> Inputconf::convert(std::string s) const
{
    std::vector<size_t> cvt;
    for(auto c:s) cvt.push_back(mapping[c]);
    return cvt;
}

void Inputconf::write(std::ostream& os) const
{
    const char* HEAD = "AUTO.DFA";
    os.write(HEAD, 8*sizeof(char));

    os.write(reinterpret_cast<const char*> (&nInput), sizeof(nInput));
    for(int i=0;i<256;i++)
        os.write(reinterpret_cast<const char*> (&mapping[i]), sizeof(mapping[i]));
}

std::optional<Inputconf> Inputconf::read(std::istream& is)
{
    const char* HEAD = "AUTO.DFA";
    size_t dummy;
    is.read(reinterpret_cast<char*>(&dummy), sizeof(dummy));
    assert(dummy == *reinterpret_cast<const size_t*>(HEAD));

    size_t nInput;
    is.read(reinterpret_cast<char*>(&nInput), sizeof(nInput));

    Inputconf S(nInput);

    for(int i=0;i<256;i++)
    {
        size_t tmp;
        is.read(reinterpret_cast<char*> (&tmp), sizeof(tmp));

        if(tmp != -1 && S.set(tmp, i)) return std::nullopt;
    }

    return S;
}

// Stateconf
Stateconf::Stateconf(size_t nState):nState(nState)
{
    assert(nState != 0);
    q0 = -1;
}

int Stateconf::setBegin(size_t q0)
{
    if(q0 >= nState) return -1;
    this->q0 = q0;
    
    return 0;
}

int Stateconf::setEnd(size_t qf)
{
    this->qf.insert(qf);
    return 0;
}

int Stateconf::unsetEnd(size_t qf)
{
    this->qf.erase(qf);
    return 0;
}

size_t Stateconf::start_state() const {return q0;}
size_t Stateconf::size() const {return nState;}
bool Stateconf::isfin(size_t q) const {return qf.count(q);}


void Stateconf::write(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*> (&nState), sizeof(nState));
    os.write(reinterpret_cast<const char*> (&q0), sizeof(q0));

    size_t n = qf.size();
    os.write(reinterpret_cast<const char*> (&n), sizeof(n));
    for(auto i:qf)
        os.write(reinterpret_cast<const char*>(&i), sizeof(i));    
}

std::optional<Stateconf> Stateconf::read(std::istream& is)
{
    size_t nState;
    is.read(reinterpret_cast<char*>(&nState), sizeof(nState));

    Stateconf Q(nState);

    // Todo: Before setBegin, setEnd, we have to check the consistency of given value.
    size_t q0;
    is.read(reinterpret_cast<char*>(&q0), sizeof(q0));
    Q.setBegin(q0);

    size_t n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));

    for(int i=0;i<n;i++)
    {
        size_t qf;
        is.read(reinterpret_cast<char*>(&qf), sizeof(qf));
        Q.setEnd(qf);
    }

    return Q;
}

// Executer
Executer::Executer(Stateconf Q, Inputconf S):Q(Q), S(S), q_now(Q.start_state()), _errno(0) {}
int Executer::recv(const std::vector<size_t>& vec)
{
    for(auto i : vec)
    {
        if(i < S.size()) stream.push(i);
        else return -1;
    }
    return 0;
}
int Executer::recv(const std::string& s){ return recv(S.convert(s)); }


bool Executer::isfin() const {return Q.isfin(q_now);}
size_t Executer::queued_length() const {return stream.size();}
size_t Executer::queued_next() const {return stream.front();}
bool Executer::error() const {return _errno;}

size_t Executer::now(){return q_now;}
} // namespace Auto