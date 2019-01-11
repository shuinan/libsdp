#pragma once

#include <string>
#include <vector>

using namespace std;

namespace stringUtil
{
	inline char tolower(char c)
	{
		if (c >= 'A' && c <= 'Z')//是大写字母，执行转换。
			c += 'a' - 'A';//转成大写。
		return c;
	}
	inline std::string tolower(std::string str)
	{
		for (unsigned i = 0; i < str.length(); i++)
			str[i] = tolower(str[i]);
		return str;
	}
	inline char toupper(char c)
	{
		if (c >= 'a' && c <= 'z')
			c += 'A' - 'a';
		return c;
	}
	inline std::string toupper(std::string str)
	{
		for (unsigned i = 0; i < str.length(); i++)
			str[i] = toupper(str[i]);
		return str;
	}
	
	template<class T>
	string digitArrayToString(const vector<T>& a, const string& delim) {
		string out;
		for (int i = 0; i < a.size(); ++i) {
			if (i == a.size() - 1)
				out += std::to_string(a[i]) + delim;
			else 
				out += std::to_string(a[i]);
		}
		return out;
		//return trim(strings.Join(strings.Split(fmt.Sprint(a), " "), delim), "[]")
	}

	inline bool contains(const vector<string>& arr, const string& item) {
		return std::find(arr.begin(), arr.end(), item) != arr.end();
	}
		
	std::vector<std::string> split(const std::string &strSource, const string &delim);

	std::string concat(const std::vector<std::string> &strv, const std::string &c = " ");

	inline static std::string trim(const std::string &str, const string& c = " ")
	{
		std::string::size_type pos = str.find_first_not_of(c);
		if (pos == std::string::npos)
		{
			return str;
		}
		std::string::size_type pos2 = str.find_last_not_of(c);
		if (pos2 != std::string::npos)
		{
			return str.substr(pos, pos2 - pos + 1);
		}
		return str.substr(pos);
	}

	inline static string& trimSelf(string& text, const string& delim)
	{
		if (!text.empty())
		{
			text.erase(0, text.find_first_not_of(delim));
			text.erase(text.find_last_not_of(delim) + 1);
		}
		return text;
	}

	inline static string join(const vector<string>& ss, const string& delim) {
		string ret;
		for (int i = 0; i < ss.size(); ++i) {
			if (i != ss.size() - 1) {
				ret += ss[i] + delim;
			}
			else {
				ret += ss[i];
			}
		}
		return ret;
	}
	
	std::string& replace_all(std::string& str, const std::string& old_value, const std::string& new_value);

	uint64_t toUINT64(const std::string &strNumber);

	int32_t toINT32(const std::string &strNumber);

	const std::string copyBytes(const std::vector<char>& bytes, std::string& str, unsigned	maxLen = 0);
	const std::string copyBytes(const std::vector<char>& bytes, unsigned maxLen = 0);

	const void toBytes(const std::string& str, std::vector<char>& bytes);


	std::string getRawString(std::string const& s);
	// sample: toHexString(pDataBuffer, dataLen, " ")
	std::string toHexString(const char* buf, int len, const std::string& tok);

}	// end of stringUtil
