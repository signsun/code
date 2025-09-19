/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <stack>

using namespace std;

// @lc code=start
class Solution
{
public:
    vector<string> res;
    vector<string> generateParenthesis(int n)
    {
        backtrace(0, 0, n, "");
        return res;
    }
    void backtrace(int open, int close, int n, string str)
    {
        if (str.size() == n * 2)
        {
            res.push_back(str);
            return;
        }
        if (open < n)
        {
            backtrace(open + 1, close, n, str + '(');
        }
        if (close < open)
        {
            backtrace(open, close + 1, n, str + ')');
        }
    }
};
// @lc code=end
