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

void DeterministicFiniteAutomaton::setStates(std::set<int> states)
{
	m_states = states;
}

void DeterministicFiniteAutomaton::setAlphabet(std::set<char> alphabet)
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

void DeterministicFiniteAutomaton::setFinalStates(std::set<int> final_states)
{
	m_final_states = final_states;
}

std::set<int>& DeterministicFiniteAutomaton::getStates()
{
	return m_states;
}

std::set<char>& DeterministicFiniteAutomaton::getAlphabet()
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

std::set<int>& DeterministicFiniteAutomaton::getFinalStates()
{
	return m_final_states;
}

struct SetHash {
	template <typename T>
	std::size_t operator()(const std::set<T>& s) const {
		std::size_t hash_value = 0;
		for (const T& elem : s) {
			hash_value ^= std::hash<T>()(elem) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
		}
		return hash_value;
	}
};

std::set<int> lambda_closure(nfa AFN, const std::set<int>& states) {
	std::set<int> closure = states;
	std::queue<int> state_queue;
	for (int state : states) {
		state_queue.push(state);
	}

	while (!state_queue.empty()) {
		int current_state = state_queue.front();
		state_queue.pop();
		auto transitions = AFN.getTransitions();
		auto it = transitions.find({ current_state, 'L' });
		if (it != transitions.end()) {
			for (int next_state : it->second) {
				if (closure.find(next_state) == closure.end()) {
					closure.insert(next_state);
					state_queue.push(next_state);
				}
			}
		}
	}

	return closure;
};

DeterministicFiniteAutomaton DeterministicFiniteAutomaton::AFNtoAFD(nfa AFN)
{
	DeterministicFiniteAutomaton result;

	std::set<int> dfa_states;
	std::unordered_map<std::pair<int, char>, int, PairHash> dfa_transitions;
	std::set<int> dfa_final_states;
	std::set<char> alphabet = AFN.getAlphabet();

	alphabet.erase('L');

	std::queue<std::set<int>> state_queue;
	std::unordered_map<std::set<int>, int, SetHash> state_mapping;
	int state_counter = 0;

	std::set<int> nfa_init_state = { AFN.getInitState() };
	std::set<int> dfa_init_state = lambda_closure(AFN, nfa_init_state);

	state_mapping[dfa_init_state] = state_counter++;
	state_queue.push(dfa_init_state);
	dfa_states.insert(state_mapping[dfa_init_state]);
	result.setInitialState(state_mapping[dfa_init_state]);

	while (!state_queue.empty()) {
		std::set<int> current_set = state_queue.front();
		state_queue.pop();
		int current_dfa_state = state_mapping[current_set];

		for (char symbol : alphabet) {
			std::set<int> next_states;

			for (int state : current_set) {
				auto transitions = AFN.getTransitions();
				auto it = transitions.find({ state, symbol });
				if (it != transitions.end()) {
					for (int next_state : it->second) {
						next_states.insert(next_state);
					}
				}
			}

			std::set<int> next_closure = lambda_closure(AFN, next_states);

			if (!next_closure.empty()) {
				if (state_mapping.find(next_closure) == state_mapping.end()) {
					state_mapping[next_closure] = state_counter++;
					state_queue.push(next_closure);
					dfa_states.insert(state_mapping[next_closure]);
				}
				dfa_transitions[{current_dfa_state, symbol}] = state_mapping[next_closure];
			}
		}
	}

	for (const auto& [nfa_set, dfa_state] : state_mapping) {
		for (int nfa_state : nfa_set) {
			if (nfa_state == AFN.getFinalState()) {
				dfa_final_states.insert(dfa_state);
				break;
			}
		}
	}

	result.setStates(dfa_states);
	result.setAlphabet(alphabet);
	result.setTransitions(dfa_transitions);
	result.setFinalStates(dfa_final_states);

	return result;
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
	int current_state = m_init_state;
	for (char symbol : word) {
		auto it = m_transitions.find({ current_state, symbol });
		if (it == m_transitions.end()) {
			return false;
		}
		current_state = it->second;
	}

	return m_final_states.find(current_state) != m_final_states.end();
}
