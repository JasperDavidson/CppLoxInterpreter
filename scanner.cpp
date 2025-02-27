#include "scanner.h"
#include "run.h"

Token::Token(TokenType type, std::string lexeme, int line) : type(type), lexeme(lexeme), line(line) {};

TokenType Token::get_type() const { return type; }
std::string Token::get_lexeme() const { return lexeme; }
int Token::get_line() const { return line; }

Scanner::Scanner(const std::string& source) : source(source) {};

bool Scanner::is_at_end() const {
	return current >= source.length();
}

char Scanner::advance() {
	return source.at(current++);
}

// Allows us to check for the next character - kind of an advance() preview
bool Scanner::match(char c) {
	if (is_at_end()) return false;
	if (source.at(current) != c) return false;

	current++;
	return true;
}

// Lookahead function for the source string
char Scanner::peek() const {
	if (is_at_end()) return '\0';
	return source.at(current);
}

void Scanner::add_token(TokenType type) {
	// Set the lexeme equal to a substring of the source based on current and start
	std::string lexeme = source.substr(start, current - start);
	token_list.push_back(Token(type, lexeme, line));
}

void Scanner::scan_token() {
	char c = advance();
	
	switch(c) {
		case '(': add_token(TokenType::LEFT_PAREN); break;
		case ')': add_token(TokenType::RIGHT_PAREN); break;
		case '{': add_token(TokenType::LEFT_BRACE); break;
		case '}': add_token(TokenType::RIGHT_BRACE); break;
		case ',': add_token(TokenType::COMMA); break;
		case '.': add_token(TokenType::DOT); break;
		case '-': add_token(TokenType::MINUS); break;
		case '+': add_token(TokenType::PLUS); break;
		case ';': add_token(TokenType::SEMICOLON);
		case '*': add_token(TokenType::STAR); break;

		// Check for two-character lexemes
		case '!':
			add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
			break;
		case '=':
			add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
			break;
		case '>':
			add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
			break;
		case '<':
			add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
			break;
		case '/':
			if (match('/')) {
				// Comments go until the end of the line
				while (peek() != '\n' && !is_at_end()) advance();
			} else {
				add_token(TokenType::SLASH);
			}

			break;

		case ' ':
		case '\r':
		case '\t':
			break;

		case '\n': line++; break;

		default:
			report_error(line, "Invalid syntax error");
			break;
	}
}

std::vector<Token> Scanner::scan_tokens() {
	while (!is_at_end()) {
		start = current;
		scan_token();
	}

	token_list.push_back(Token(TokenType::EOFF, "", line));
	return token_list;
}
