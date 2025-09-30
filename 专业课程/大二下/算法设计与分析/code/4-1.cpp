#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// 定义活动结构体
struct Activity {
    int start;
    int end;
};

// 比较函数，用于按活动结束时间排序
bool compareEndTime(const Activity& a, const Activity& b) {
    return a.end < b.end;
}

// 贪心算法实现
int greedyActivitySelector(const std::vector<Activity>& activities) {
    if (activities.empty()) {
        return 0;
    }
    // 按结束时间排序活动
    std::vector<Activity> sortedActivities = activities;
    std::sort(sortedActivities.begin(), sortedActivities.end(), compareEndTime);

    int numActivities = 1;  // 至少有一个活动
    int lastEndTime = sortedActivities[0].end;
    for (size_t i = 1; i < sortedActivities.size(); ++i) {
        if (sortedActivities[i].start >= lastEndTime) {
            numActivities++;
            lastEndTime = sortedActivities[i].end;
        }
    }
    return numActivities;
}