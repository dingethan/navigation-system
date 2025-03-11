#include <iostream>
#include<fstream>
#include<vector>
#include<random>
#include<cmath>
#include <algorithm> // 添加此行
using namespace std;
struct Point {
    double x;
    double y;
};

struct Edge {
    int u;   //起点索引
    int v;   //终点索引
    double w;   //边权（欧式距离）

    Edge(int _u, int _v,double _w):u(_u),v(_v),w(_w){}

    //用于排序的比较运算符
    bool operator<(const Edge& other)const {
        return w < other.w;
    }
};

//初始化种子 固定种子值
mt19937 gen(12345);

// 假设地图范围为1000x1000的二维平面
const double map_min = 0.0;
const double map_max = 1000.0;

//创建均匀浮点数分布（覆盖地图范围） ?如何正态分布（模拟城市中心）生成非均匀分布点和泊松圆盘采样（避免间距过近）
uniform_real_distribution<double> dis(map_min, map_max);

//生成N个随机点
vector<Point> generate_points(int N) {

    vector<Point> points;
    for (int i = 0; i < N; i++) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.push_back(p);
    }
    return points;
}

/**
* 生成边：
为每个点找到k个最近邻，生成边，并存储所有可能的边。
使用Kruskal或Prim算法生成MST，确保图连通。
合并MST的边和邻近边，形成最终的边集合。
检查并移除可能导致交叉的边，或者使用Delaunay三角剖分来避免交叉。
*/

//网格索引类（加速邻近查询）
class GridIndex {
private:
    double cell_size;
    int rows, cols;
    vector<vector<vector<int>>> grid; //三维vector：[row][col][point_indices]
    const vector<Point>& points;

public:
    GridIndex(double _cell_size, double map_size, const vector<Point>& _points) :cell_size(_cell_size),points(_points) {
        rows = cols = static_cast<int>(ceil(map_size / cell_size));
        grid.resize(rows, vector<vector<int>>(cols));
    }


//添加点到网格
void add_point(const Point& p, int idx) {
    int col = static_cast<int>(p.x / cell_size);
    int row = static_cast<int>(p.y / cell_size);
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        grid[row][col].push_back(idx);
    }
}

//查询邻近网格中的点
vector<int> query_neighbors(const Point& p, int k) {
    int col = static_cast<int>(p.x / cell_size);
    int row = static_cast<int>(p.y / cell_size);
    vector<int> candidates;

   //搜索周围3*3的网格区域
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < rows && c >= 0 && c < cols) {
                candidates.insert(candidates.end(), grid[r][c].begin(), grid[r][c].end());
            }
        }
    }

    //按距离排序取前k个
    sort(candidates.begin(), candidates.end(), [&](int a, int b) {
        double dx1 = p.x - points[a].x;
        double dy1 = p.y - points[a].y;
        double dx2 = p.x - points[b].x;
        double dy2 = p.y - points[b].y;
        return (dx1 * dx1 + dy1 * dy1) < (dx2 * dx2 + dy2 * dy2);
        });


    if (candidates.size() > static_cast<size_t>(k)) candidates.resize(k);
    return candidates;
}

};













int main()
{
    const int N = 10000;
    auto points = generate_points(N);
    
    //验证输出前五个点
    for (int i = 0; i < 5; i++) {
        cout << "Point" << i + 1 << ":("
            << points[i].x << "," << points[i].y <<")" << endl;
    }

    //保存数据
    ofstream out("map_data.txt");
    for (auto& p : points) {
        out << p.x << " " << p.y << endl;
    }
    cout << "Map data saved to map_data.txt" << endl;

    return 0;
}