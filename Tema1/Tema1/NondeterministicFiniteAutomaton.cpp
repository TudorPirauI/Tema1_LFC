﻿#include "NondeterministicFiniteAutomaton.h"

typedef NondeterministicFiniteAutomaton nfa;

int NondeterministicFiniteAutomaton::m_stateCounter = 0;

std::set<int>& NondeterministicFiniteAutomaton::getStates()
{
    return m_states;
}

std::set<char>& NondeterministicFiniteAutomaton::getAlphabet()
{
    return m_alphabet;
}

std::unordered_map<std::pair<int, char>, std::vector<int>, NondeterministicFiniteAutomaton::PairHash>& NondeterministicFiniteAutomaton::getTransitions()
{
    return m_transitions;
}

int NondeterministicFiniteAutomaton::getFinalState()
{
    return m_final_state;
}

int NondeterministicFiniteAutomaton::getInitState() const
{
    return m_init_state;
}

void NondeterministicFiniteAutomaton::setStates(std::set<int> states)
{
    m_states = states;
}

void NondeterministicFiniteAutomaton::setAlphabet(std::set<char> alphabet)
{
    m_alphabet = alphabet;
}

void NondeterministicFiniteAutomaton::setTransitions(std::unordered_map<std::pair<int, char>, std::vector<int>, NondeterministicFiniteAutomaton::PairHash> transitions)
{
    m_transitions = transitions;
}

void NondeterministicFiniteAutomaton::setFinalState(int finalState)
{
    m_final_state = finalState;
}

void NondeterministicFiniteAutomaton::setInitState(int initState)
{
    m_init_state = initState;
}

void NondeterministicFiniteAutomaton::addTransition(int initialState, char symbol, int finalState)
{
    auto key = std::make_pair(initialState, symbol);
    m_states.insert(initialState);
    m_states.insert(finalState);
    m_alphabet.insert(symbol);
    if (m_transitions.find(key) == m_transitions.end())
    {
        m_transitions[key] = { finalState };
    }
    else
    {
        m_transitions[key].push_back(finalState);
    }
}

void NondeterministicFiniteAutomaton::copyTransitions(const nfa& a)
{
    for (const auto& transition : a.m_transitions)
    {
        const auto& rule = transition.first;
        const auto& destinations = transition.second;
        for (const auto& state : destinations)
        {
            addTransition(rule.first, rule.second, state);
        }
    }
}

nfa NondeterministicFiniteAutomaton::returnAFNfromPolishForm(std::vector<char> polishForm)
{
    std::stack<nfa> automatonStack;
    for (auto i = 0; i < polishForm.size(); i++)
    {
        nfa result;
        if (polishForm[i] == '.')
        {
            nfa a = automatonStack.top();
            automatonStack.pop();
            nfa b = automatonStack.top();
            automatonStack.pop();
            result = Concatenate(a, b);
        }
        else if(polishForm[i] == '|')
        {
            nfa a = automatonStack.top();
            automatonStack.pop();
            nfa b = automatonStack.top();
            automatonStack.pop();
            result = Alternate(a, b);
        }
        else if (polishForm[i] == '*')
        {
            result = KleeneStar(automatonStack.top());
            automatonStack.pop();
        }
        else
        {
            result.m_init_state = m_stateCounter;
            m_stateCounter++;
            result.m_final_state = m_stateCounter;
            m_stateCounter++;
            result.addTransition(result.m_init_state, polishForm[i], result.m_final_state);
        }
        automatonStack.push(result);
    }

    return automatonStack.top();
}

void NondeterministicFiniteAutomaton::PrintAutomation(std::ostream& os)
{
    os << "\nStates:\n";
    for (int state : m_states)
        os << state << ", ";

    os <<std::endl<< "\nAlphabet:\n";
    for (char symbol : m_alphabet)
        os << symbol << ", ";

    os << "\n\nTransitions:\n";
    for (const auto& transition : m_transitions)
    {
        for (int destination : transition.second)
        {
            os << transition.first.first << " --" << transition.first.second << "--> " << destination << "; ";
        }
        os << std::endl;
    }

    os << "\nInitial state:\n" << m_init_state << std::endl;

    os << "\nFinal state:\n";
    os << m_final_state << std::endl;
}

nfa NondeterministicFiniteAutomaton::Concatenate(nfa b, nfa a)
{
    nfa result;
    result.m_init_state = a.m_init_state;
    result.m_final_state = b.m_final_state;
    result.copyTransitions(a);
    int mergeState = a.m_final_state;
    for (const auto& transition : b.m_transitions)
    {
        const auto& rule = transition.first;
        const auto& destinations = transition.second;
        if (rule.first == b.m_init_state)
        {
            for (auto state : destinations)
            {
                result.addTransition(mergeState, rule.second, state);
            }
        }
        else
        {
            for (auto state : destinations)
            {
                result.addTransition(rule.first, rule.second, state);
            }
        }
    }
    
    return result;
}

nfa NondeterministicFiniteAutomaton::Alternate(nfa b, nfa a)
{
    nfa result;
    result.m_init_state = m_stateCounter;
    m_stateCounter++;
    result.m_final_state= m_stateCounter;
    m_stateCounter++;
    result.copyTransitions(a);
    result.copyTransitions(b);
    result.addTransition(result.m_init_state, 'L', a.m_init_state);
    result.addTransition(a.m_final_state, 'L', result.m_final_state);
    result.addTransition(result.m_init_state, 'L', b.m_init_state);
    result.addTransition(b.m_final_state, 'L', result.m_final_state);
    return result;
}

nfa NondeterministicFiniteAutomaton::KleeneStar(nfa a)
{
    nfa result;
    result.m_init_state = m_stateCounter;
    m_stateCounter++;
    result.m_final_state = m_stateCounter;
    m_stateCounter++;
    result.copyTransitions(a);
    result.addTransition(result.m_init_state, 'L', a.m_init_state);
    result.addTransition(a.m_final_state, 'L', result.m_final_state);
    result.addTransition(result.m_init_state, 'L', result.m_final_state);
    result.addTransition(a.m_final_state, 'L', a.m_init_state);
    return result;
}


