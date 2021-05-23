#ifndef MATCHERAUTOMATON_HPP_
#define MATCHERAUTOMATON_HPP_

#include "Automaton.hpp"

#include <utility>

//a Matcher Automaton is an Automaton

class MatcherAutomaton : public Automaton {

private:
	std::string toMatch;

public:
	int read(const std::string&) override;
	MatcherAutomaton(std::string, TokenType);
	~MatcherAutomaton() override= default;
};

MatcherAutomaton::MatcherAutomaton(std::string input, TokenType type) : Automaton(type) {
	this->toMatch = std::move(input);
}

//If we have an exact match then return the size. It could be this one
//unless another automaton matches with more.
int MatcherAutomaton::read(const std::string& input) {
	for(unsigned i = 0; i < toMatch.size(); i++) {
		if(input[i] != toMatch[i])
			return 0;
	}
	return toMatch.size();
}
#endif /* MATCHERAUTOMATON_HPP_ */
