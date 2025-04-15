#ifndef AUTOMATA_BASE
#define AUTOMATA_BASE

#include <cassert>
#include <map>
#include <iostream>
#include <queue>
#include <string>
#include <set>
#include <vector>
#include <optional>


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
    Inputconf(size_t nInput);

    int set(size_t idx, unsigned char c);
    int setseq(size_t idx, std::string s);


    size_t size() const;
    size_t convert(unsigned char c) const;

    std::vector<size_t> convert(std::string s) const;
    
    void write(std::ostream& os) const;
    static std::optional<Inputconf> read(std::istream& is);
};

class Stateconf
{
private:
    const size_t nState;
    size_t q0;
    std::set<size_t> qf;

public:
    Stateconf(size_t nState);
    int setBegin(size_t q0);

    int setEnd(size_t qf);
    int unsetEnd(size_t qf);

    size_t start_state() const;
    size_t size() const;
    bool isfin(size_t q) const;

    void write(std::ostream& os) const;
    static std::optional<Stateconf> read(std::istream& is);
};

class Executer
{
private:
    const Stateconf Q;
    const Inputconf S;

protected:
    size_t q_now;
    int _errno;
    std::queue<size_t> stream;
    Executer(Stateconf Q, Inputconf S);

public:
    int recv(const std::vector<size_t>& vec);
    int recv(const std::string& s);

    bool isfin() const;
    size_t queued_length() const;
    size_t queued_next() const;
    bool error() const;

    size_t now();

    virtual size_t run(size_t step) = 0;
    virtual void show_status(std::ostream& os) const = 0;
};

}

#endif