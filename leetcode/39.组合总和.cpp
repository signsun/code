/*
 * @lc app=leetcode.cn id=39 lang=cpp
 *
 * [39] 组合总和
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
    vector<vector<int>> res;
    vector<int> t;
    int sum = 0;
    vector<vector<int>> combinationSum(vector<int> &candidates, int target)
    {
        backtrack(candidates, target, 0);
        return res;
    }
    void backtrack(vector<int> &nums, int target, int idx)
    {
        if (sum == target)
        {
            res.push_back(t);
            return;
        }
        if (sum > target)
            return;
        for (int i = 0; i < nums.size(); i++)
        {
            if (i < idx)
                continue;
            sum += nums[i];
            t.push_back(nums[i]);
            backtrack(nums, target, i);
            sum -= nums[i];
            t.pop_back();
        }
    }
};
// @lc code=end
