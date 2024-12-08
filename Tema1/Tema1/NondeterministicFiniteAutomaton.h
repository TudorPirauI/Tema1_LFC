#pragma once

#include<set>
#include<set>
#include<tuple>
#include<unordered_map>
#include<map>
#include<string>
#include<stack>
#include<iostream>

class NondeterministicFiniteAutomaton
{
	typedef NondeterministicFiniteAutomaton nfa;
private:
	struct PairHash {
		template <typename T1, typename T2>
		std::size_t operator()(const std::pair<T1, T2>& p) const {
			std::size_t h1 = std::hash<T1>()(p.first);
			std::size_t h2 = std::hash<T2>()(p.second);
			return h1 ^ (h2 << 1);
		}
	};

public:
	NondeterministicFiniteAutomaton() = default;
	~NondeterministicFiniteAutomaton() = default;

	std::set<int>& getStates();
	std::set<char>& getAlphabet();
	std::unordered_map<std::pair<int, char>, std::vector<int>, PairHash>& getTransitions();
	int getFinalState();
	int getInitState() const;

	void setStates(std::set<int> states);
	void setAlphabet(std::set<char> alphabet);
	void setTransitions(std::unordered_map<std::pair<int, char>, std::vector<int>, PairHash> transitions);
	void setFinalState(int finalState);
	void setInitState(int initState);

	void addTransition(int initalState, char symbol, int finalState);
	void copyTransitions(const nfa& a);
	nfa returnAFNfromPolishForm(std::vector<char> polishForm);
	void PrintAutomation(std::ostream& os);

	nfa Concatenate(nfa b, nfa a);
	nfa Alternate(nfa b, nfa a);
	nfa KleeneStar(nfa a);

	static int m_stateCounter;
private:
	std::set<int> m_states; //Q
	std::set<char> m_alphabet; //Σ
	std::unordered_map<std::pair<int, char>, std::vector<int>, PairHash> m_transitions; //δ
	int m_init_state; //q_0
	int m_final_state; //F
};