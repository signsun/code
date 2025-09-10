/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

// @lc code=start
class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        unordered_map<int, int> ump;
        for (int i = 0; i < nums.size(); i++)
        {
            auto it = ump.find(target - nums[i]);
            if (it != ump.end())
            {
                return {it->second, i};
            }
            ump[nums[i]] = i;
        }
        return {};
    }
};
// @lc code=end
