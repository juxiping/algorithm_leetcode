#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <stdlib.h>
using namespace std;

#include <algorithm>
#include <string.h>

class Solution {
public:
    vector<int> data_stream;
    void Insert(int num)
    {
        data_stream.push_back(num);
    }

    double GetMedian()
    {
        //对vector进行排序
        sort(data_stream.begin(),data_stream.end());
        int len = data_stream.size();
        int mid = len/2;
        return len%2==0 ? (data_stream[mid]+data_stream[mid-1])/2 : data_stream[mid];

    }

};

//构建最大堆
priority_queue<int,vector<int>> max_heap;
//构建最小堆
priority_queue<int,vector<int>,greater<>> min_heap;
//数据流中的中位数
//采用一个最小堆和一个最大堆
void Insert(int num)
{

    if(max_heap.empty())
        max_heap.push(num);
    else if(min_heap.empty()){
        if(num>=max_heap.top()){
            min_heap.push(num);
        }else{
            int temp = max_heap.top();
            max_heap.pop();
            max_heap.push(num);
            min_heap.push(temp);
        }
    }else{
    //新插入的数字分别于最大堆和最小堆的堆顶元素进行比较，确定插入位置
    if(num<=min_heap.top()){
        if(max_heap.size()-min_heap.size()==-1||max_heap.size()==min_heap.size()){
            max_heap.push(num);
        }else if(max_heap.size()-min_heap.size()==1){
            max_heap.push(num);
            int temp = max_heap.top();
            max_heap.pop();
            min_heap.push(temp);
        }
    } else {
        if(max_heap.size()-min_heap.size()==1||max_heap.size()==min_heap.size()){
            min_heap.push(num);
        }else if(min_heap.size()-max_heap.size()==1){
            min_heap.push(num);
            int temp = min_heap.top();
            min_heap.pop();
            max_heap.push(temp);
        }
    }
    }
}

double GetMedian()
{
    int len = min_heap.size() + max_heap.size();
    bool even = len%2==0;
    if(even)
        return (min_heap.top()+max_heap.top())/2.0;
    else{
        if(min_heap.size()>max_heap.size()){
            return min_heap.top();
        }else
            return max_heap.top();

    }
}

/**
 * 不用加减乘除做加法
 * 1.使用与操作计算进位值
 * 2.使用异或操作计算相加值
 * 3.若进位值为0，则直接返回异或结果
 * @param num1
 * @param num2
 * @return
 */
int Add(int num1, int num2)
{
        int carry = (num1 & num2)<<1;
        int additive = num1 ^ num2,temp;
        while(carry){
            temp = additive;
            additive = carry ^ additive;
            carry = (carry & temp) << 1;
        }
        return additive;
}


struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};

/**
 * 按之字形打印
 * @param pRoot
 * @return
 */
vector<vector<int> > Print(TreeNode* pRoot) {
    vector<vector<int>> res;
    if(pRoot==nullptr)
        return res;
    stack<TreeNode*> zhi_s;
    queue<TreeNode*> zhi_q;
    int num = 1,num_next_level=0;    //每层数目
    int order = 1; //从左往右还是从右往左
    zhi_q.push(pRoot);
    vector<int> level = vector<int>();
    while(!zhi_q.empty()){
        TreeNode* temp = zhi_q.front();
        zhi_q.pop();
        if(temp->left) {
            num_next_level++;
            zhi_q.push(temp->left);
            if(order)
                zhi_s.push(temp->left);
        }
        if(temp->right) {
            num_next_level++;
            zhi_q.push(temp->right);
            if(order)
                zhi_s.push(temp->right);
        }
        if(order)
            level.push_back(temp->val);
        num--;
        if(num==0){
            if(order) {
                res.push_back(level);
            }
            else if(order==0){
                while(!zhi_s.empty()){
                    level.push_back(zhi_s.top()->val);
                    zhi_s.pop();
                }
                res.push_back(level);
            }
            order = order==1 ? 0 : 1;
            level = vector<int>();
            num = num_next_level;
            num_next_level = 0;
        }

    }
    return res;

}

void Serialize_recursive(TreeNode *root, string& str) {
    if(root == nullptr)
        str += "$,";
    else {
        str += to_string(root->val);
        str += ",";
        Serialize_recursive(root->left, str);
        Serialize_recursive(root->right, str);
    }
}


/**
 * 序列化和反序列化二叉树
 * @param root
 * @return
 */
char* Serialize(TreeNode *root) {
    string res;
    //根据前序遍历对二叉树进行序列化
    if(root== nullptr){
        return nullptr;
    }
    Serialize_recursive(root,res);
    int len_s = res.length();
    char *ret = new char[len_s];
    res.copy(ret,len_s-1);
    ret[len_s-1] = '\0';
    return ret;
}

TreeNode* Deserialize(char *str) {
    //使用一个栈来存储
    stack<TreeNode*> temp;
    if(str== nullptr)
        return nullptr;
    //字符串分割
    char *token = strtok(str,",");
    TreeNode *root = new TreeNode(stoi(token)), *node = root;
    token = strtok(nullptr, "," );
    temp.push(node);
    while(token){
        if(*token!='$'){
            TreeNode* tem = new TreeNode(stoi(token));
            if(node) {
                node->left = tem;
                temp.push(tem);
                node = tem;
            }else {
                node = temp.top();
                node->right = tem;
                temp.pop();
                temp.push(tem);
                node = tem;
            }
        }else{
            if(node) {
                node->left = nullptr;
                //temp.push(node);
            }else {
                node = temp.top();
                node->right = nullptr;
                temp.pop();
            }
            node = nullptr;
        }
        token = strtok(nullptr, "," );
    }
    return root;
}




/**
 * 机器人的运动范围
 * 回溯法
 * @param threshold
 * @param rows
 * @param cols
 * @return
 */

//判断坐标的数位之和是否有超过阈值
bool judge_threshold(int x, int y, int threshold) {
    int count = 0;
    while(x){
        count += x % 10;
        x = x / 10;
    }
    while(y){
        count += y % 10;
        y = y / 10;
    }

    return count > threshold;

}

void run_threshold(int threshold, int s_x, int s_y, vector<int> &flag, int n, int m) {
    //若不满足条件则将结果记录
    if(judge_threshold(s_x,s_y,threshold)){
        return;
    }
    //起点的index
    int s_index = s_x * m + s_y;
    flag[s_index] = 0; //走过之后置位0
    // 往上走
    s_x--;
    s_index = s_x*m+s_y;
    if(s_x>=0 && flag[s_index]){
        run_threshold(threshold,s_x,s_y,flag,n,m);
    }
    // 往左走
    s_x++;
    s_y--;
    s_index = s_x*m+s_y;
    if(s_y>=0&&flag[s_index]){
        run_threshold(threshold,s_x,s_y,flag,n,m);
    }
    // 往右走
    s_y++;
    s_y++;
    s_index = s_x*m+s_y;
    if(s_y<m&&flag[s_index]){
        run_threshold(threshold,s_x,s_y,flag,n,m);
    }
    // 往下走
    s_y--;
    s_x++;
    s_index = s_x*m+s_y;
    if(s_x<n&&flag[s_index]){
        run_threshold(threshold,s_x,s_y,flag,n,m);
    }

}

int movingCount(int threshold, int rows, int cols)
{
    //定义一个一维数组，用于标记哪些格子是已经走过的；
    vector<int> flag(rows*cols,1);
    run_threshold(threshold,0,0,flag,rows,cols);
    int count = 0;
    for (int i = 0; i < rows*cols; ++i) {
        count+=!flag[i];
    }
    return count;
}



//腾讯笔试题

bool run(vector<int> flag, int s_x, int s_y, int t_x, int t_y, int n, int m) {

    //起终点的index
    int s_index = (s_x-1)*m+s_y-1, t_index=(t_x-1)*m+t_y-1;
    flag[s_index] = flag[s_index]-1;
    //循环结束条件
    if(s_index==t_index) {
        if(flag[t_index]==0)
            return true;
        else if(flag[t_index]==1){
            return run(flag,s_x,s_y,t_x,t_y,n,m);
        }
    }
    // 往上走
    s_x--;
    s_index = (s_x-1)*m+s_y-1;
    bool res = false;
    int flag_c=1;
    if(s_x>=1 && flag[s_index]){
        res = run(flag,s_x,s_y,t_x,t_y,n,m);
    }
    // 往左走
    s_x++;
    s_y--;
    s_index = (s_x-1)*m+s_y-1;
    if(!res&&s_y>=1&&flag[s_index]){
        res = run(flag,s_x,s_y,t_x,t_y,n,m);
    }
    // 往右走
    if(flag_c) {
        s_y++;
        flag_c = 1;
    }
    s_y++;
    s_index = (s_x-1)*m+s_y-1;
    if(!res&&s_y<=m&&flag[s_index]){
        res = run(flag,s_x,s_y,t_x,t_y,n,m);
        flag_c = 1;
    }
    // 往下走
    if(flag_c) {
        s_y--;
    }
    s_x++;
    s_index = (s_x-1)*m+s_y-1;
    if(!res&&s_x<=n&&flag[s_index]){
        res = run(flag,s_x,s_y,t_x,t_y,n,m);
    }
    return res;
}

//华为笔试：报文转义
int as(){
    int len;
    cin >> len;
    vector<int> bw;
    int temp;
    len--;
    vector<int> res;
    int count=0, flag = 1;
    while(len--) {
        flag = 0;
        scanf("%X",&temp);
        if(temp==10) {
            res.push_back(18);
            res.push_back(52);
            count += 2;
        }else if(temp==11){
            res.push_back(171);
            res.push_back(205);
            count += 2;
        } else {
            res.push_back(temp);
            count += 1;
        }
    }
    if(flag)
        printf("%X",count+2);
    else
        printf("%X",count+1);
    cout << " ";
    int j;
    for (j = 0; j < res.size() - 1 ; ++j) {
        printf("%X",res[j]);
        cout << " ";
    }
    printf("%X",res[j]);

    return 0;
}

//华为笔试题，质数组合问题
int zuhe(int low, int high) {
    int s_count = 0, g_count = 0;
    int count = 0;
    for (int i = low; i < high; ++i) {
        //判断是否为质数
        int flag = 1;
        for (int j = 2; j <= i/2; ++j) {
            if(i%j==0){
                flag = 0;
                count++;
                break;
            }
        }
        if(flag){
            g_count += i%10;
            s_count += (i/10)%10;
        }
    }
    return g_count < s_count ? g_count : s_count;
}

//构建乘积数组
vector<int> multiply(const vector<int>& A) {
    vector<int> l_to_r,r_to_l(A.size(),0),res;
    int temp = 1;
    for (int i = 0; i < A.size(); ++i) {
        temp *= A[i];
        l_to_r.push_back(temp);
    }
    temp = 1;
    for (int j = A.size()-1; j >=0 ; --j) {
        temp *= A[j];
        r_to_l[j] = temp;
    }
    res.push_back(r_to_l[1]);
    for (int k = 1; k < A.size()-1; ++k) {
        res.push_back(r_to_l[k+1]*l_to_r[k-1]);
    }
    res.push_back(l_to_r[A.size()-2]);
    return res;
}

//表示数值的字符串
//符号位只能出现在开头，或者是e的右边
//e不能出现在开头
//e两边要有数字，且右边应该为整数
//不能出现其它字母
bool isNumeric(char* string)
{
    int i = 0;
    int flag = 0;//表示是否出现e
    int flag_dot = 0;
    bool res = true;
    char temp;
    while(*string!='\0'){
        temp = *string;
        if(temp>='0'&&temp<='9') {
            i++;
            string++;
            continue;
        }
        else if(temp=='+' || temp == '-'){
            if(i!=0&&flag==0){
                res= false;
                return res;
            }
        }else if(temp=='e'||temp=='E'){
            if(flag==0 && i!=0){
                flag=1;
            }else{
                res = false;
                return res;
            }
        }else if(temp=='.'){
            if(flag_dot==0 && i!=0 && flag==0){
                flag_dot = 1;
            }else{
                res = false;
                return res;
            }
        }else {
            res = false;
            return res;
        }
        i++;
        string++;
    }
    if(temp=='e'|| temp=='E' || temp=='.')
        return false;
    return res;
}


//将两个排序结果进行合并
void merge(vector<int>&data, int l, int mid, int r, int& res) {
    //分配一个辅助空间用于保存原有的数据
    vector<int> temp;
    for (int i = l; i <=r ; ++i) {
        temp.push_back(data[i]);
    }
    //将两个排序好的数组进行合并
    int fir = l,fin = l,offset=l,sec=mid;
    while (sec<=r&&fir<mid){
        if(temp[sec-offset]<temp[fir-offset]){
            data[fin++] = temp[sec-offset];
            res+=mid-fir;
            sec++;
        }else{
            data[fin++] = temp[fir-offset];
            fir++;
        }
    }
    while(fir<mid){
        data[fin++] = temp[fir-offset];
        fir++;
    }
    while(sec<=r){
        data[fin++] = temp[sec-offset];
        sec++;
    }

}

//归并排序data[l...r]
void merge_sort(vector<int>& data, int l, int r, int& res) {
    if(l<r) {
        int mid = (r + l) / 2;
        //左边排序结果
        if(l!=mid)
            merge_sort(data, l, mid, res);
        //右边排序结果
        if(r!=mid+1)
            merge_sort(data, mid+1,r, res);
        //将两个排序结果进行合并
        merge(data,l,mid+1,r, res);
    }
}

//数组中的逆序对
int InversePairs(vector<int> data) {
    if(data.empty())
        return 0;
    int len = data.size();
    int res = 0;
    //归并排序
    merge_sort(data,0,len-1,res);
    return res;
}


int main() {
    vector<int> test = {9,8,7,6,5,4,3,2};
    cout << InversePairs(test) << endl;
    return 0;
}