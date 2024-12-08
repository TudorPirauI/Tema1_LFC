#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<cstring>
#include<format>

#include "DeterministicFiniteAutomaton.h"
#include "NondeterministicFiniteAutomaton.h"

void readRegex(std::string file_name, std::string& regex)
{
    std::ifstream f(file_name);
    f >> regex;
    f.close();
}

void formatRegex(std::string& regex)
{
    std::string regex_aux;
    regex_aux += regex[0];
    for (int i = 1; i < regex.size(); i++)
    {
        if (strchr("()|.*", regex[i]) == 0 && strchr("()|.*", regex[i - 1]) == 0)
        {
            regex_aux += '.';
        }
        else if (strchr("()|.*", regex[i - 1]) == 0 && regex[i] == '(') {
            regex_aux += '.';
        }
        else if ((regex[i - 1] == '*' || regex[i - 1] == ')') && strchr("()|.*", regex[i]) == 0) {
            regex_aux += '.';
        }
        regex_aux += regex[i];
    }
    regex = regex_aux;
}

bool verifyParenthesis(const std::string& regex)
{
    std::stack<char> parenthesis;

    for (int i = 0; regex[i]; ++i)
    {
        if (regex[i] == '(')
            parenthesis.emplace(regex[i]);
        else if (regex[i] == ')')
        {
            if (parenthesis.empty())
                return false;

            parenthesis.pop();
        }
    }

    if (parenthesis.empty())
        return true;
    return false;
}

bool verifyOperators(const std::string& regex)
{
    if (regex[0] == '|' || regex[regex.size() - 1] == '|' || regex[0] == '*')
    {
        return false;
    }

    for (int i = 1; i < regex.size() - 1; ++i)
    {
        if (regex[i] == '|' && (strchr("()|*", regex[i - 1]) || strchr("()|*", regex[i + 1])))
            return false;
        else if (regex[i] == '*' && (regex[i - 1] == '(' || regex[i - 1] == '*'))
            return false;
    }

    return true;
}

bool isValidRegex(const std::string& regex)
{
    return verifyParenthesis(regex) && verifyOperators(regex);
}

int priority(char c)
{

    switch (c)
    {
    case '(':
        return 0;

    case ')':
        return 0;

    case '|':
        return 1;

    case '.':
        return 2;

    case '*':
        return 3;
    default:
        return 0;
    }
}

std::vector<char> regexToPolishForm(std::string pattern)
{
    std::vector<char> polish;
    std::stack<char> op_stack;

    for (int i = 0; pattern[i]; ++i)
    {
        if (isalnum(pattern[i]))
        {
            polish.push_back(pattern[i]);
        }
        else
        {
            if (pattern[i] == '(')
                op_stack.push(pattern[i]);
            else
            {
                if (pattern[i] == ')')
                {
                    while (!op_stack.empty() && op_stack.top() != '(')
                    {
                        polish.push_back(op_stack.top());
                        op_stack.pop();
                    }
                    if (!op_stack.empty())
                        op_stack.pop(); //scoate paranteza
                }
                else
                {
                    //daca expression[i] e operator
                    while (!op_stack.empty() && priority(op_stack.top()) >= priority(pattern[i]))
                    {
                        polish.push_back(op_stack.top());
                        op_stack.pop();
                    }
                    op_stack.push(pattern[i]);
                }
            }
        }
    }

    while (!op_stack.empty())
    {
        polish.push_back(op_stack.top());
        op_stack.pop();
    }

    return polish;
}

int main()
{
    std::string regex;
    readRegex("Input.txt",regex);

    if (isValidRegex(regex) == false)
    {
        std::cout << "REGEX is NOT valid!\n";
    }
    else
    {

        std::string formattedRegex = regex;
        formatRegex(formattedRegex);
        std::vector<char> polishForm = regexToPolishForm(formattedRegex);
        NondeterministicFiniteAutomaton NFA = NFA.returnAFNfromPolishForm(polishForm);
        DeterministicFiniteAutomaton DFA = DFA.AFNtoAFD(NFA);

        bool exitState = false;

        while (!exitState)
        {
            int state;

            std::cout << "----------------------------------\n";
            std::cout << "Choose next step:\n";
            std::cout << "0.EXIT;\n";
            std::cout << "1.Print REGEX;\n";
            std::cout << "2.Print DFA;\n";
            std::cout << "3.Print NFA;\n";
            std::cout << "4.Check Word.\n";
            std::cout << "----------------------------------\n";

            std::cin >> state;

            if (state == 0) // EXIT
            {
                exitState = true;
            }
            else if (state == 1) // Print REGEX
            {
                std::cout << std::format("Regex read from file: {}\n", regex);
            }
            else if (state == 2) // Print DFA
            {
                if (DFA.VerifyAutomation() == true)
                {
                    std::ofstream fout("OutputDFA.txt");
                    DFA.PrintAutomation(std::cout);
                    DFA.PrintAutomation(fout);
                    fout.close();
                }
                else
                    std::cout << std::endl << "DFA is NOT valid!" << std::endl;
            }
            else if (state == 3) // Print NFA
            {
                if (DFA.VerifyAutomation() == true)
                {
                    std::ofstream fout("OutputNFA.txt");
                    NFA.PrintAutomation(std::cout);
                    NFA.PrintAutomation(fout);
                    fout.close();
                }
                else
                    std::cout << std::endl << "DFA is NOT valid!" << std::endl;
            }
            else if (state == 4) // Check WORD
            {
                std::string word;
                std::cout << "Enter word for validation: ";
                std::cin >> word;

                if (DFA.VerifyAutomation() == true)
                {
                    if (DFA.CheckWord(word) == true)
                    {
                        std::cout << std::format("{} is a valid word!\n", word);
                    }
                    else
                    {
                        std::cout << std::format("{} is NOT a valid word!\n", word);
                    }
                }
                else
                    std::cout << std::endl << "DFA is NOT valid!" << std::endl;
            }
        }
    }
	return 0;
}