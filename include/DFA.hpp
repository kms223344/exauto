#ifndef DFA_HPP
#define DFA_HPP

#include "AutomataBase.hpp"

#include <cassert>
#include <map>
#include <ostream>
#include <string>
#include <vector>


namespace Auto
{

class DFA;
class DFATransition
{
private:
    const Stateconf Q;
    const Inputconf S;

    std::map<size_t, std::map<size_t, size_t> > transition;
    DFATransition(const Stateconf& Q, const Inputconf& S);

public:

    int config(size_t q1, size_t s, size_t q2);
    std::pair<size_t, bool> operator()(size_t q, size_t s) const;

    void write(std::ostream& os) const;
    static std::optional<DFATransition> read(std::istream& is);

    friend class DFA;
};

class ExecuterDFA;
class DFA
{
private:
    const Stateconf Q;
    const Inputconf S;

    DFATransition transition;

public:
    DFA(Stateconf Q, Inputconf S);
    //DFA(std::string path);
    
    int config(size_t q1, size_t s, size_t q2);
    ExecuterDFA instance() const;

    int save(std::string path) const;
    friend class ExecuterDFA;
};

class ExecuterDFA : public Executer
{
private:
    const DFA& ref;
    ExecuterDFA(const DFA& ref);
    
public:
    size_t run(size_t step); // return the number of step it did.
    void show_status(std::ostream& os) const;
    friend class DFA;
};

} // namespace Auto

#endif // DFA_HPP

/*
DFA A;

A.import("/path/to/명세");

Inputconf T(2352);
T.setseq(9, "sdfb");
T.set(3, 'a');
T.set({(3, 'a'), (7, 'c'), });

Stateconf S(2552);
S.setseq(3, {"sfafs", "sgsdfg", "sgdg"});
S.set(2, "sgasd");

A.setinputN
A.addinputN

A.setInput(T);
A.setState(S);


A.addInput();

A.config(c, s, g); // (c, s) => g

A.export("/path/to/명세"); // json?

DFA A("/path/to/명세")

auto c = A.instance();

c.send("abcd");
c.send({'a', 'b', 'c', 'd'});
c.send({97, 'b', 99, 'd'});

c.send('a');
c.send({'a', 2000, 'b', 0, });
c.send({1, 32, 2, 11304});

c.send("abcd", 1223, s, 'a', "asfsf");

c.state();          // "HErEIa"
c.state_idx();      // 0

c.input({1, 2, 1, 1, 101010, 234, 0});
c.input("abas\'dkfwd\\");


DFA A(명세);
A.input(input);

A.step();
A.run();

A.input(); A.step();
*/