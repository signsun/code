/*
 * @lc app=leetcode.cn id=216 lang=cpp
 *
 * [216] 组合总和 III
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
    vector<vector<int>> combinationSum3(int k, int n)
    {
        vector<vector<int>> res;
        vector<int> t;
        backtrack(res, t, k, n, 0, 1);
        return res;
    }
    void backtrack(vector<vector<int>> &res, vector<int> &t, int k, int n, int idx, int start)
    {
        if (n == 0 && idx == k)
        {
            res.push_back(t);
            return;
        }
        if (idx == k)
            return;

        for (int i = start; i < 10; i++)
        {
            if (n - i < 0)
                break;
            t.push_back(i);
            backtrack(res, t, k, n - i, idx + 1, i + 1);
            t.pop_back();
        }
    }
};
// @lc code=end
