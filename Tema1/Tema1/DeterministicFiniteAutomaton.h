#pragma once

#include<unordered_set>
#include<tuple>
#include<unordered_map>
#include<string>
#include<iostream>

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

	std::unordered_set<int> m_states; //Q
	std::unordered_set<char> m_alphabet; //Σ
	std::unordered_map<std::pair<int, char>, int,PairHash> m_transitions; //δ
	int m_init_state; //q_0
	std::unordered_set<int> m_final_states; //F

public:
	DeterministicFiniteAutomaton();

	void setStates(std::unordered_set<int> states);
	void setAlphabet(std::unordered_set<char> alphabet);
	void setTransitions(std::unordered_map<std::pair<int, char>, int, PairHash> transitions);
	void setInitialState(int init_state);
	void setFinalStates(std::unordered_set<int> final_states);

	std::unordered_set<int>& getStates();
	std::unordered_set<char>& getAlphabet();
	std::unordered_map<std::pair<int, char>, int, PairHash>& getTransitions();
	int& getInitState();
	std::unordered_set<int>& getFinalStates();

	bool VerifyAutomation();
	void PrintAutomation();
	bool CheckWord(std::string word);
	~DeterministicFiniteAutomaton() = default;
};

