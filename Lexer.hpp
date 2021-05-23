#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <utility>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <fstream>
#include "RealAutomaton.hpp"
#include "IntAutomaton.hpp"
#include "IDAutomaton.hpp"
#include "MatcherAutomaton.hpp"

class Lexer {

private:
	std::vector<Automaton*> machines;
	std::vector<Token*> tokens;
	std::unordered_map<TokenType, std::unordered_map<std::string, unsigned>> tokenTable;
	std::string sourceFile;
	std::string sourceString;
	void initializeSourceString();
	void initializeMachines();
	void clear();

public:
	Lexer();
	explicit Lexer(std::string);
	~Lexer();
	Token* getToken(unsigned index);
	void setSourceFile(std::string);
	std::string toString();
	unsigned numTokens();
	void runWrapped(std::string);
	void run();
	std::string generateReport();
};

Lexer::Lexer() {
	initializeMachines();
	sourceFile = "";
}

Lexer::Lexer(std::string sourceFile) {
	initializeMachines();
	this->sourceFile = std::move(sourceFile);
	initializeSourceString();
}

//create an FSA for each "class" of token. Polymorphism at work
void Lexer::initializeMachines() {
	machines.push_back(new MatcherAutomaton("if",	 TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("then",	 TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("else", 	 TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("begin",  TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("end", 	 TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("return", TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("int", 	 TokenType::KEYWORD));
	machines.push_back(new MatcherAutomaton("double", TokenType::KEYWORD));

	machines.push_back(new MatcherAutomaton("(", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton(")", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("{", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("}", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("[", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("]", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("+", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("-", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("=", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton(",", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton(";", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton(">", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("<", TokenType::SPECIAL));
	machines.push_back(new MatcherAutomaton("*", TokenType::SPECIAL));

	machines.push_back(new IDAutomaton(TokenType::IDENTIFIER));

	machines.push_back(new IntAutomaton(TokenType::INTEGER));

	machines.push_back(new RealAutomaton(TokenType::REAL));
}

void Lexer::initializeSourceString() {
	std::ifstream inputFile(sourceFile);
	if(inputFile) {
		std::stringstream sourceStream;
		sourceStream << inputFile.rdbuf();
		sourceString = sourceStream.str();

		std::for_each(sourceString.begin(), sourceString.end(), [](char & c){
			if (std::isalpha(c))
				c = std::tolower(c);
		});
	}
	else sourceString = "";
}

Token* Lexer::getToken(unsigned index) {
	return tokens[index];
}

void Lexer::setSourceFile(std::string sourceFile) {
	this->sourceFile = std::move(sourceFile);
	initializeSourceString();
}

unsigned Lexer::numTokens() {
	return tokens.size();
}

std::string Lexer::toString() {
	std::ostringstream out;

	for(auto & token : tokens)
		out << token->toString() << std::endl;

	return out.str();
}

void Lexer::clear() {
	for(auto elem : machines)
		delete elem;

	for(auto elem : tokens)
		delete elem;
}

void Lexer::run() {
	runWrapped(sourceString);
}

void Lexer::runWrapped(std::string input) {
	Automaton* maxMachine = nullptr;
	Token* newToken = nullptr;
	unsigned maxRead, inputRead;

	//while we still have input to read run through all of the FSA and select the
	//machine that accepts the largest input
	while(!input.empty()) {
		maxMachine = machines.front();
		newToken = nullptr;
		maxRead = 0;

		for(auto elem : machines) {
			inputRead = elem->read(input);
			if(inputRead > maxRead) {
				maxRead = inputRead;
				maxMachine = elem;
			}
		}
		if(maxRead) {
			newToken = maxMachine->createToken(input.substr(0, maxRead));
			tokens.push_back(newToken);
			//this creates new entry if the key does not already exist. If it does then the
			//value in the inner unsorted_map is incremented. The value represents
			//the number of occurrences of a particular token.
			tokenTable[newToken->getType()][newToken->getValue()]++;
			input.erase(0, maxRead);
		}
		//this means that we read nothing so we will be in an infinite loop
		//we need to remove a single character. It may be a token that we
		//don't recognize(besides whitespace) so just eliminate the first char.
		else
			input.erase(0, 1);
		//we dont recognize white spaces so remove all of them from the front
		auto iter1 = find_if(input.begin(), input.end(),
						[](char c) {
							return !std::isspace(c);
						});
		input.erase(input.begin(), iter1);
	}
}

std::string Lexer::generateReport() {
	std::ostringstream out;

	for(int typeIndex = TokenType::KEYWORD; typeIndex != TokenType::LAST; typeIndex++) {
		auto type = static_cast<TokenType>(typeIndex);

		//this sums the number of token occurrences in a particular inner unordered_map
		auto classCount = std::accumulate(tokenTable[type].begin(), tokenTable[type].end(), 0,
				[](unsigned s, const std::pair<std::string, unsigned>& p) {return s + p.second; });

		out << "=================================================" << std::endl;
		out << "TOKEN CLASS: " << std::left << std::setw(16) << Token::tokenToString(type);
		out << "TOTAL TOKENS: " << std::right << std::setw(4) << classCount << std::endl;
		out << "=================================================" << std::endl;
		out << std::right << std::setw(8) << "TOKEN" << std::setw(17) << "COUNT" << std::endl;
		out << "-----------" << std::setw(17) << "-----------" << std::endl;

		//iterate through a particular inner unordered_map and print the token and number of occurrences
		for(auto & it : tokenTable[type])
			out << "  " << std::left << std::setw(17) << it.first << std::right << std::setw(4) << it.second << std::endl;

		out << std::endl;
	}

	//remove the last  2 endl from the stream
	return out.str().erase(out.str().size() - 2, 2);
}

Lexer::~Lexer() {
	this->clear();
}

#endif /* LEXER_HPP_ */
