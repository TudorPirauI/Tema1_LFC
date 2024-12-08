#pragma once

#include<set>
#include<tuple>
#include<unordered_map>
#include<string>
#include<iostream>
#include<queue>

#include "NondeterministicFiniteAutomaton.h"

typedef NondeterministicFiniteAutomaton nfa;

class DeterministicFiniteAutomaton
{

private:
	struct PairHash {
		template <typename T1, typename T2>
		std::size_t operator()(const std::pair<T1, T2>& p) const {
			std::size_t h1 = std::hash<T1>()(p.first);
			std::size_t h2 = std::hash<T2>()(p.second);
			return h1 ^ (h2 << 1);
		}
	};

	std::set<int> m_states; //Q
	std::set<char> m_alphabet; //Σ
	std::unordered_map<std::pair<int, char>, int,PairHash> m_transitions; //δ
	int m_init_state; //q_0
	std::set<int> m_final_states; //F

public:
	DeterministicFiniteAutomaton();

	void setStates(std::set<int> states);
	void setAlphabet(std::set<char> alphabet);
	void setTransitions(std::unordered_map<std::pair<int, char>, int, PairHash> transitions);
	void setInitialState(int init_state);
	void setFinalStates(std::set<int> final_states);

	std::set<int>& getStates();
	std::set<char>& getAlphabet();
	std::unordered_map<std::pair<int, char>, int, PairHash>& getTransitions();
	int& getInitState();
	std::set<int>& getFinalStates();

	DeterministicFiniteAutomaton AFNtoAFD(nfa regex);
	bool VerifyAutomation();
	void PrintAutomation(std::ostream& os);
	bool CheckWord(std::string word);
	~DeterministicFiniteAutomaton() = default;
};

