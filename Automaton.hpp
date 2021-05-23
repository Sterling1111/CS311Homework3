#ifndef AUTOMATON_HPP_
#define AUTOMATON_HPP_

#include <utility>

#include "Token.hpp"

class Automaton {

protected:
	TokenType type;

public:
	explicit Automaton(TokenType type) {this->type = type;}
	//pure virtual function must be implemented in all derived classes or they will be abstract
	virtual int read(const std::string&) = 0;
	//virtual destructor because we allocate derived classes from
	//this class pointers in class Lexer.
	virtual ~Automaton() = default;
	Token* createToken(std::string);
};

Token* Automaton::createToken(std::string input) {
	return new Token(type, std::move(input));
}


#endif /* AUTOMATON_HPP_ */
