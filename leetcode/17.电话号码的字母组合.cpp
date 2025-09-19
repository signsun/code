/*
 * @lc app=leetcode.cn id=17 lang=cpp
 *
 * [17] 电话号码的字母组合
 */

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

// @lc code=start
class Solution
{
public:
    map<char, string> mp = {
        {'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}};
    vector<string> res;
    string str = "";
    vector<string> letterCombinations(string digits)
    {
        if (digits.empty())
            return {};
        backtrace(digits, 0);

        return res;
    }
    void backtrace(string digits, int level)
    {
        if (level == digits.size())
        {
            res.push_back(str);
            return;
        }

        for (auto c : mp[digits[level]])
        {
            str.push_back(c);
            backtrace(digits, level + 1);
            str.pop_back();
        }
    }
};
// @lc code=end
