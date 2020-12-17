#include "halfmanTree.h"

static uint64_t cod[4] = {0};
static int shift = 0;

/*
    arg为halfDirectory 的指针
*/
void *HalfmanTree::codingBefore(void *thisNode, void *arg)
{
    /*
        该函数执行完后会进入左子树，所以先将LEFT填入cod
    */
    uint64_t bitmask = LEFT << shift % 64;
    cod[shift / 64] |= bitmask;
    shift++;
    // cout<< "now shift is: "<<shift<<endl;
    // cout<<"cod: "<<cod[0]<<endl;
    return NULL;
}

void *HalfmanTree::codingMiddle(void *thisNode, void *thisRet, void *leftRet, void *arg)
{
    /*
        执行此处时代表该节点左子树已经遍历完毕
        如果左子树返回值不为NULL，则代表该节点不为叶子节点,接下来将进入右子树，所以将最后一位cod改为RIGHT即可
        如果左子树返回值为NULL，则代表该节点需要被记录，将其写入halfDictionary
            此时已经在cod中加入了向左走的一位，所以要shift-1
        该函数返回值不需要，返回NULL即可。
    */
    if (leftRet != NULL)
    {
        int bitmask = ~(1 << ((shift - 1) % 64));
        cod[shift / 64] &= bitmask;
        // cout<<"cod: "<<cod[0]<<endl;
    }
    else
    {
        TreeNode *tn = (TreeNode *)thisNode;
        uint8_t thisKey = tn->getKey();
        map<uint8_t, HalfmanValue*>& phd = *(map<uint8_t, HalfmanValue*>*)arg;
        phd[thisKey] = new HalfmanValue((uint8_t *)cod, shift - 1);
        // cout<<"key "<<(int)thisKey<<" is setting"<<endl;
        // phd[thisKey]->print();
    }
    return NULL;
}

void *HalfmanTree::codingAfter(void *thisNode, void *thisRet, void *leftRet, void *middleRet, void *rightRet, void *arg)
{
    /*
        执行至此处已经遍历左右子树，即将返回父节点
        该函数只需要将偏移量修正即可
        同时该函数应当返回非空指针，以代表其父节点不为叶子节点
    */
    shift--;
    
    // cout<< "now shift is: "<<shift<<endl;
    return thisNode;
}

struct cmpTreeNode
{
    bool operator()(TreeNode *a, TreeNode *b)
    {
        return *a < *b;
    }
};

void HalfmanTree::init()
{
    priority_queue<TreeNode *, vector<TreeNode *>, cmpTreeNode> pq;
    /*
        将TreeNode放入小顶堆中，进行哈夫曼树构造
    */

    for (auto key : cntMap)
    {
        TreeNode *newNode = new TreeNode(key.first, key.second);
        pq.push(newNode);
    }

    /*
        每次取出最小的两个节点（树），拼成一个树，将根节点放回堆中
    */
    while (pq.size() != 1)
    {
        // cout<<"test"<<endl;
        TreeNode *least = pq.top();
        pq.pop();
        TreeNode *least2 = pq.top();
        pq.pop();
        TreeNode *pnew = *least + *least2;
        pq.push(pnew);
    }
    root = pq.top();

    dfs(root, codingBefore, codingMiddle, codingAfter, &halfDirectory);
}

void *HalfmanTree::dfs(TreeNode *root, HandleFunctionBefore before, HandleFunctionMiddle middle, HandleFunctionAfter after, void *arg)
{
    if (!root)
        return NULL;
    // cout<<"print this child"<<endl;
    void *thisRet = before(root, arg);

    // cout<<"goto print left child"<<endl;
    void *leftRet = dfs(root->getChild(LEFT), before, middle, after, arg);

    void *middleRet = middle(root, thisRet, leftRet, arg);

    // cout<<"goto print right child"<<endl;
    void *rightRet = dfs(root->getChild(RIGHT), before, middle, after, arg);

    return after(root, thisRet, leftRet, middleRet, rightRet, arg);
}

int HalfmanTree::totalSize(){
    int cnt=0;
    for(auto item : halfDirectory){
        uint8_t key = item.first;
        cnt+=cntMap[key]*halfDirectory[key]->bitLength;
    }
    return cnt;
}

HalfmanValue* HalfmanTree::translate(uint8_t key){
    return halfDirectory[key];
}