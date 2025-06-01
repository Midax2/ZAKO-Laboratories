#pragma once
#include <string>
#include <algorithm>
#include <cctype>

inline bool is_trim_whitespace(unsigned char c) {
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !is_trim_whitespace(ch);
		}));
}

inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !is_trim_whitespace(ch);
		}).base(), s.end());
}

inline void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

#pragma region Token Utils
inline bool isRegisterName(const std::string& name)
{
	if (name.size() < 2)
		return false;

	if (name[0] != 'r' && name[0] != 'R')
		return false;

	for (int i = 1; i < name.size(); i++)
		if (!isdigit(name[i]))
			return false;

	return true;
}

inline bool isHexNumber(const std::string& number)
{
	if (number.size() < 3)
		return false;

	int base = 0;

	if (number[base] == '-')
		base++;

	if (number[base] != '0' || number[base + 1] != 'x')
		return false;

	for (int i = base + 2; i < number.size(); i++)
	{
		if (!isxdigit(number[i]))
			return false;
	}

	return true;
}

inline bool isIndirectAddressing(const std::string& token)
{
	if (token.size() < 7)
		return false;

	auto indexBegin = token.find_first_of('(');
	if (!isHexNumber(token.substr(0, indexBegin)))
		return false;

	auto indexEnd = token.find_first_of(')');
	if (indexEnd == std::string::npos)
		return false;

	if (!isRegisterName(token.substr(indexBegin + 1, indexEnd - indexBegin - 1)))
		return false;

	return true;
}

inline uint32_t getOffsetFromIndirectAddressing(const std::string& token)
{
	auto indexBegin = token.find_first_of('(');
	if (indexBegin == std::string::npos)
		return 0;
	return std::stoul(token.substr(2, indexBegin), nullptr, 16);
}

inline std::string getIndexRegisterFromIndirectAddressing(const std::string& token)
{
	auto indexBegin = token.find_first_of('(');
	if (indexBegin == std::string::npos)
		return "";
	auto indexEnd = token.find_first_of(')');
	if (indexEnd == std::string::npos)
		return "";

	return token.substr(indexBegin + 1, indexEnd - indexBegin - 1);
}
#pragma endregion