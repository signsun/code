/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
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
    vector<vector<int>> combinationSum2(vector<int> &candidates, int target)
    {
        vector<int> t;
        vector<vector<int>> res;
        sort(candidates.begin(), candidates.end());
        backtrack(candidates, target, 0, t, res);
        return res;
    }
    void backtrack(vector<int> &nums, int target, int level, vector<int> &t, vector<vector<int>> &res)
    {
        if (target == 0)
        {
            res.push_back(t);
            return;
        }

        for (int i = level; i < nums.size(); i++)
        {
            if (target - nums[i] < 0)
                break;
            if (i > level && nums[i] == nums[i - 1])
                continue;
            t.push_back(nums[i]);
            backtrack(nums, target - nums[i], i + 1, t, res);
            t.pop_back();
        }
    }
};
// @lc code=end
