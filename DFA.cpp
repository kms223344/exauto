#include "DFA.hpp"

Auto::ExecuterDFA Auto::DFA::instance() const
{
    return Auto::ExecuterDFA(*this);
}