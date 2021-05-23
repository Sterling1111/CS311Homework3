#ifndef IDAUTOMATON_HPP_
#define IDAUTOMATON_HPP_

#include "Automaton.hpp"

class IDAutomaton : public Automaton {

	bool accept;
	unsigned numRead;
	int read(const std::string&) override;
	void S_0(const std::string&);
	void S_1(const std::string&);

public:
	explicit IDAutomaton(TokenType);
	~IDAutomaton() override = default;
};

IDAutomaton::IDAutomaton(TokenType type) : Automaton(type){
	numRead = 0;
	accept = false;
}

int IDAutomaton::read(const std::string& input) {
	 accept = false;
	 numRead = 0;
	 S_0(input);
	 if(accept)
		 return numRead;
	 else
		 return 0;
}

void IDAutomaton::S_0(const std::string& input) {
	if(input.size() > numRead) {
		if(isalpha(input[numRead])) {
			numRead++;
			accept = true;
			S_1(input);
		}
	}
}
void IDAutomaton::S_1(const std::string& input) {
	if(input.size() > numRead) {
		if(isalpha(input[numRead])) {
			numRead++;
			S_1(input);
		}
	}
}

#endif /* IDAUTOMATON_HPP_ */
