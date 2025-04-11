#ifndef DFA_HPP
#define DFA_HPP

#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

#include "AutomataBase.hpp"

namespace Auto
{

class DFA;
class DFATransition
{
private:
    const Stateconf Q;
    const Inputconf S;

    std::vector<std::map<size_t, size_t> > transition;
    DFATransition(const Stateconf& Q, const Inputconf& S):Q(Q), S(S), transition(Q.size()) {}

public:

    int config(size_t q1, size_t s, size_t q2)
    {
        if(q1<Q.size() & q2 < Q.size() && s < S.size())
        {
            transition[q1][s] = q2;
            return 0;
        }
        return -1;
    }
    std::pair<size_t, bool> operator()(size_t q, size_t s) const
    {
        if(q < Q.size() && transition[q].count(s))
            return {transition[q].at(s), false};
        else return {q, true};
    }
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
    DFA(Stateconf Q, Inputconf S):Q(Q), S(S), transition(Q, S) {}
    
    int config(size_t q1, size_t s, size_t q2)
    {
        return transition.config(q1, s, q2);
    }

    ExecuterDFA instance() const;

    friend class ExecuterDFA;
};

class ExecuterDFA : public Executer
{
private:
    const DFA& ref;
    ExecuterDFA(const DFA& ref):ref(ref), Executer(ref.Q, ref.S) {}
public:
    size_t run(size_t step) // return the number of step it did.
    {
        size_t st = std::min(stream.size(), step);

        size_t t;
        for(t = 0; t < st; t++)
        {
            auto [q, error] = ref.transition(q_now, stream.front());

            if(error)
            {
                _errno = 1;
                break;
            }

            q_now = q;
            stream.pop();
        }

        return t;
    }

    void show_status(std::ostream& os) const
    {
        os<<"[status] State: "<<q_now<<", Error: "<<_errno<<std::endl;
    }

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