/*
 * @lc app=leetcode.cn id=47 lang=cpp
 *
 * [47] 全排列 II
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
    vector<vector<int>> permuteUnique(vector<int> &nums)
    {
        vector<bool> visit(nums.size(), false);
        sort(nums.begin(), nums.end());
        traceback(nums, visit, 0);
        return res;
    }

    void traceback(vector<int> &nums, vector<bool> &visit, int level)
    {
        if (level == nums.size())
        {
            res.push_back(t);
            return;
        }
        for (int i = 0; i < nums.size(); i++)
        {
            if (visit[i])
                continue;
            if (i > 0 && nums[i] == nums[i - 1] && !visit[i - 1])
                continue;
            t.push_back(nums[i]);
            visit[i] = true;
            traceback(nums, visit, level + 1);
            t.pop_back();
            visit[i] = false;
        }
    }
};
// @lc code=end
