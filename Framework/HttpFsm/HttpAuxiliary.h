#pragma once

#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

std::vector<std::string> SplitString(const std::string& str, std::string delimeter)
{
	std::vector<std::string> tokens;
	std::string s;
	boost::split(tokens, str, boost::is_any_of(delimeter), boost::token_compress_on);
	return tokens;
}

