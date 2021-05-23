#ifndef INTAUTOMATON_HPP_
#define INTAUTOMATON_HPP_

#include "Automaton.hpp"

//an IntAutomaton is an Automaton

class IntAutomaton : public Automaton {

private:
	bool accept;
	unsigned numRead;
	int read(const std::string&) override;
	void S_0(const std::string&);
	void S_1(const std::string&);

public:
	explicit IntAutomaton(TokenType);
	~IntAutomaton() override = default;
};

IntAutomaton::IntAutomaton(TokenType type) : Automaton(type) {
	numRead = 0;
	accept = false;
}

int IntAutomaton::read(const std::string& input) {
	 accept = false;
	 numRead = 0;
	 S_0(input);
	 if(accept)
		 return numRead;
	 else
		 return 0;
}

void IntAutomaton::S_0(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			accept = true;
			S_1(input);
		}
	}
}
void IntAutomaton::S_1(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			S_1(input);
		}
	}
}

#endif /* INTAUTOMATON_HPP_ */
