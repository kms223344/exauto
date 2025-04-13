#include "DFA.hpp"

#include <iostream>
#include <fstream>

namespace Auto
{

// DFATransition
DFATransition::DFATransition(const Stateconf& Q, const Inputconf& S):Q(Q), S(S), transition() {}

int DFATransition::config(size_t q1, size_t s, size_t q2)
{
    if(q1<Q.size() & q2 < Q.size() && s < S.size())
    {
        transition[q1][s] = q2;
        return 0;
    }
    return -1;
}

std::pair<size_t, bool> DFATransition::operator()(size_t q, size_t s) const
{
    if(q < Q.size() && transition.count(q) && transition.at(q).count(s))
        return {transition.at(q).at(s), false};
    else return {q, true};
}

void DFATransition::write(std::ostream& os) const
{
    size_t n = transition.size();
    os.write(reinterpret_cast<const char*> (&n), sizeof(n));

    for(auto& [c, mp] : transition)
    {
        os.write(reinterpret_cast<const char*> (&c), sizeof(c));
        size_t m = mp.size();

        os.write(reinterpret_cast<const char*> (&m), sizeof(m));
        for(auto t:mp)
            os.write(reinterpret_cast<const char*>(&t), sizeof(t));
    }
}

// DFA
DFA::DFA(Stateconf Q, Inputconf S):Q(Q), S(S), transition(Q, S) {}

int DFA::config(size_t q1, size_t s, size_t q2)
{
    return transition.config(q1, s, q2);
}

int DFA::save(std::string path) const
{
    std::ofstream fout(path, std::ios::out|std::ios::binary);
    if(!fout) return -1;

    S.write(fout);
    Q.write(fout);
    transition.write(fout);
    
    fout.close();
    return 0;
}

ExecuterDFA DFA::instance() const
{
    return Auto::ExecuterDFA(*this);
}

// ExecuterDFA
ExecuterDFA::ExecuterDFA(const DFA& ref):ref(ref), Executer(ref.Q, ref.S) {}
size_t ExecuterDFA::run(size_t step) // return the number of step it did.
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

void ExecuterDFA::show_status(std::ostream& os) const
{
    os<<"[status] State: "<<q_now<<", Error: "<<_errno<<std::endl;
}

} // namespace Auto