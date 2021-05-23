#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <string>
#include <sstream>
#include <utility>

//Not a character or digit because a character is an identifier and a digit is an integer
//Also last is not a Token class. It is simply there because in class Lexer, we iterate through
//all of the Token Classes. If we stopped at SPECIAL then adding another Token Class at any point
//would require us to change another file and class. Nobody could remember to do this. By having Last
//we can add another Token class with no other modifications.
enum TokenType { KEYWORD = 0, IDENTIFIER = 1, INTEGER = 2, REAL = 3, SPECIAL = 4, LAST };

class Token {

private:
	std::string value;
	TokenType type;
public:
	Token(TokenType, std::string);
	TokenType getType();
	std::string getValue();
	std::string toString();
	static std::string tokenToString(TokenType);
	~Token() = default;
};

Token::Token(TokenType type, std::string input) {
	value = std::move(input);
	this->type = type;
}

TokenType Token::getType() {
	return type;
}

std::string Token::getValue() {
	return value;
}

//static function so we can convert from TokenType to string in Lexer class
//without an instance of Token.
std::string Token::tokenToString(TokenType type) {
	switch(type) {
	case TokenType::KEYWORD:
		return "KEYWORD";
	case TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case TokenType::INTEGER:
		return "INTEGER";
	case TokenType::REAL:
		return "REAL";
	case TokenType::SPECIAL:
		return "SPECIAL";
	default:
		return "";
	}
}

std::string Token::toString() {
	std::ostringstream out;
	out << "(" << tokenToString(type) << ", \'" << value << "\')";
	return out.str();
}

#endif /* TOKEN_HPP_ */
