/*
 * @lc app=leetcode.cn id=90 lang=cpp
 *
 * [90] 子集 II
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
    vector<int> t;
    vector<vector<int>> res;
    vector<vector<int>> subsetsWithDup(vector<int> &nums)
    {
        sort(nums.begin(), nums.end());
        backtrack(nums, 0);
        return res;
    }
    void backtrack(vector<int> nums, int f)
    {
        res.push_back(t);
        for (int i = f; i < nums.size(); i++)
        {
            if (i > f && nums[i] == nums[i - 1])
                continue;
            t.push_back(nums[i]);
            backtrack(nums, i + 1);
            t.pop_back();
        }
    }
};
// @lc code=end
