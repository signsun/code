/*
 * @lc app=leetcode.cn id=18 lang=cpp
 *
 * [18] 四数之和
 */

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

/**
 * @brief 递归太慢了
 *
 **/

// @lc code=start
class Solution
{
public:
    vector<vector<int>> fourSum(vector<int> &nums, int target)
    {
        if (nums.size() < 4)
            return {};
        vector<vector<int>> res;
        vector<int> t;
        sort(nums.begin(), nums.end());
        backtrack(nums, target, 0, 0, res, t);
        return res;
    }
    void backtrack(vector<int> &nums, int target, int idx, int start, vector<vector<int>> &res, vector<int> &t)
    {
        if (idx == 4 && target == 0)
        {
            res.push_back(t);
            return;
        }
        if (idx == 4)
        {
            return;
        }
        for (int i = start; i < nums.size(); i++)
        {
            if (i > start && nums[i] == nums[i - 1])
                continue;
            if (nums[i] > 0 && target < INT32_MIN + nums[i])
                continue;
            if (nums[i] < 0 && target > INT32_MAX + nums[i])
                continue;
            t.push_back(nums[i]);
            backtrack(nums, target - nums[i], idx + 1, i + 1, res, t);
            t.pop_back();
        }
    }
};
// @lc code=end
