#ifndef REALAUTOMATON_HPP_
#define REALAUTOMATON_HPP_

#include "Automaton.hpp"

//a RealAutomaton is an Automaton

class RealAutomaton : public Automaton {

private:
	bool accept;
	unsigned numRead;
	int read(const std::string&) override;
	void S_0(const std::string&);
	void S_1(const std::string&);
	void S_2(const std::string&);
	void S_3(const std::string&);

public:
	explicit RealAutomaton(TokenType);
	~RealAutomaton() override = default;
};

RealAutomaton::RealAutomaton(TokenType type) : Automaton(type) {
	numRead = 0;
	accept = false;
}

int RealAutomaton::read(const std::string& input) {
	 accept = false;
	 numRead = 0;
	 S_0(input);
	 if(accept)
		 return numRead;
	 else
		 return 0;
}

void RealAutomaton::S_0(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			S_1(input);
		}
	}
}

void RealAutomaton::S_1(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			S_1(input);
		}
		else if(input[numRead] == '.') {
			numRead++;
			S_2(input);
		}
	}
}

void RealAutomaton::S_2(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			accept = true;
			S_3(input);
		}
	}
}

void RealAutomaton::S_3(const std::string& input) {
	if(input.size() > numRead) {
		if(isdigit(input[numRead])) {
			numRead++;
			S_3(input);
		}
	}
}

#endif /* REALAUTOMATON_HPP_ */
