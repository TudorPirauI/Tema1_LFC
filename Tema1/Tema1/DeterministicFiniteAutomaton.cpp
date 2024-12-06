#include "DeterministicFiniteAutomaton.h"

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
{
	for (int i = 0; i < 6; i++)
	{
		m_states.insert(i);
	}
	m_alphabet.insert('a');
	m_alphabet.insert('b');
	m_transitions.insert({ {0, 'a'}, 1 });
	m_transitions.insert({ {1, 'a'}, 2 });
	m_transitions.insert({ {2, 'b'}, 3 });
	m_transitions.insert({ {3, 'a'}, 4 });
	m_transitions.insert({ {4, 'b'}, 5 });
	m_init_state = 0;
	m_final_states.insert(5);
}

void DeterministicFiniteAutomaton::setStates(std::unordered_set<int> states)
{
	m_states = states;
}

void DeterministicFiniteAutomaton::setAlphabet(std::unordered_set<char> alphabet)
{
	m_alphabet = alphabet;
}

void DeterministicFiniteAutomaton::setTransitions(std::unordered_map<std::pair<int, char>, int, PairHash> transitions)
{
	m_transitions = transitions;
}

void DeterministicFiniteAutomaton::setInitialState(int init_state)
{
	m_init_state = init_state;
}

void DeterministicFiniteAutomaton::setFinalStates(std::unordered_set<int> final_states)
{
	m_final_states = final_states;
}

std::unordered_set<int>& DeterministicFiniteAutomaton::getStates()
{
	return m_states;
}

std::unordered_set<char>& DeterministicFiniteAutomaton::getAlphabet()
{
	return m_alphabet;
}

std::unordered_map<std::pair<int, char>, int, DeterministicFiniteAutomaton::PairHash>& DeterministicFiniteAutomaton::getTransitions()
{
	return m_transitions;
}

int& DeterministicFiniteAutomaton::getInitState()
{
	return m_init_state;
}

std::unordered_set<int>& DeterministicFiniteAutomaton::getFinalStates()
{
	return m_final_states;
}

bool DeterministicFiniteAutomaton::VerifyAutomation()
{
	if (m_states.size() == 0)
	{
		std::cout << std::endl << "States set is empty!" << std::endl;
		return false;
	}

	if (m_alphabet.size() == 0)
	{
		std::cout << std::endl << "Alphabet set is empty!" << std::endl;
		return false;
	}

	if (m_states.find(m_init_state) == m_states.end())
	{
		std::cout << std::endl << "Initial state does not exist in the states set!" << std::endl;
		return false;
	}

	bool check = 0;
	for (auto symbol : m_alphabet)
	{
		if (m_states.find(symbol) != m_states.end())
		{
			std::cout << std::endl << "State set and alphabet set have an element in common!" << std::endl;
			return false;
		}
		auto it = m_transitions.find({ m_init_state, symbol });
		if (it != m_transitions.end())
			check = 1;
	}
	if (check == 0)
	{
		std::cout << std::endl << "There is no transition that starts with the initial state!" << std::endl;
		return false;
	}

	for (const auto& transition : m_transitions) {
		int from_state = transition.first.first;
		char symbol = transition.first.second;
		int to_state = transition.second;

		if (m_states.find(from_state) == m_states.end())
		{
			std::cout << std::endl << "There is no state with that symbol: " <<from_state<<"!"<< std::endl;
			return false;
		}

		if (m_alphabet.find(symbol) == m_alphabet.end())
		{
			std::cout << std::endl << "This symbol does not exist: " << symbol << "!" << std::endl;
			return false;
		}

		if (m_states.find(to_state) == m_states.end())
		{
			std::cout << std::endl << "There is no state with that symbol: " << to_state << "!" << std::endl;
			return false;
		}
	}

	for (int final_state : m_final_states) {
		if (m_states.find(final_state) == m_states.end())
		{
			std::cout << std::endl << "Final state does not exist in the states set!" << std::endl;
			return false;
		}
	}

	return true;
}

void DeterministicFiniteAutomaton::PrintAutomation()
{
	std::cout << "\nStates:\n";
	for (int state : m_states)
		std::cout << state << std::endl;

	std::cout << "\nAlphabet:\n";
	for (char symbol : m_alphabet)
		std::cout << symbol << std::endl;

	std::cout << "\nTransitions:\n";
	for (const auto& transition : m_transitions)
		std::cout << transition.first.first << " --" << transition.first.second << "--> " << transition.second << std::endl;

	std::cout << "\nInitial state:\n" << m_init_state << std::endl;

	std::cout << "\nFinal states:\n";
	for (int final_state : m_final_states)
		std::cout << final_state << std::endl;
}

bool DeterministicFiniteAutomaton::CheckWord(std::string word)
{

	return false;
}
