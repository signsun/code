/*
 * @lc app=leetcode.cn id=78 lang=cpp
 *
 * [78] 子集
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
    vector<vector<int>> subsets(vector<int> &nums)
    {
        backtrack(nums, 0);
        return res;
    }
    void backtrack(vector<int> nums, int level)
    {

        res.push_back(t);

        for (int i = level; i < nums.size(); i++)
        {
            t.push_back(nums[i]);
            backtrack(nums, i + 1);
            t.pop_back();
        }
    }
};
// @lc code=end
