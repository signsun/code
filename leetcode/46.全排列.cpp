/*
 * @lc app=leetcode.cn id=46 lang=cpp
 *
 * [46] 全排列
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
    vector<vector<int>> permute(vector<int> &nums)
    {
        vector<bool> visit(nums.size(), false);
        traceback(nums, 0, visit);
        return res;
    }

    void traceback(vector<int> &nums, int level, vector<bool> &visit)
    {
        if (level == nums.size())
        {
            res.push_back(t);
            return;
        }
        for (int i = 0; i < nums.size(); i++)
        {
            if (visit[i] == true)
                continue;
            visit[i] = true;
            t.push_back(nums[i]);
            traceback(nums, level + 1, visit);
            visit[i] = false;
            t.pop_back();
        }
    }
};
// @lc code=end
