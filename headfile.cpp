#include<bits/stdc++.h>
using namespace std;
// 加速函数开始
inline int read(){
    int  x=0,f=1;
    char ch=getchar();
    while(ch<'0'||ch>'9'){
        if(ch=='-')
            f=-1;
        ch=getchar();
    }
    while(ch>='0' && ch<='9')
        x=x*10+ch-'0',ch=getchar();
    return x*f;
}
void write(int x){
    if(x<0)
        putchar('-'),x=-x;
    if(x>9)
        write(x/10);
    putchar(x%10+'0');
    return;
}
int add(int num1, int num2){
    if(num2 == 0)
        return num1;
    int  sum = num1 ^ num2;
    int  carry = (num1 & num2) << 1;
    return add(sum, carry);
}
int substract(int num1, int num2){
	//num1:减数
	//num2:被减数
    int  subtractor = add(~num2, 1);
    int  result = add(num1, subtractor);
    return result ;
}
int multiply(int a, int b) {
    //将乘数和被乘数都取绝对值　
    int multiplicand = a < 0 ? add(~a, 1) : a;
    int multiplier = b < 0 ? add(~b , 1) : b;
    //计算绝对值的乘积　　
    int product = 0;
    while(multiplier > 0) {
        if((multiplier & 0x1) > 0) {// 每次考察乘数的最后一位　　　
            product = add(product, multiplicand);
        }
        multiplicand = multiplicand << 1;// 每运算一次，被乘数要左移一位　　　　
        multiplier = multiplier >> 1;// 每运算一次，乘数要右移一位（可对照上图理解）　　
    }
    //计算乘积的符号　　
    if((a ^ b) < 0) {
        product = add(~product, 1);
    }
    return product;
}
int divide_v2(int a,int b) {   
    // 先取被除数和除数的绝对值    
    int dividend = a > 0 ? a : add(~a, 1);    
    int divisor = b > 0 ? a : add(~b, 1);    
    int quotient = 0;// 商    
    int remainder = 0;// 余数    
    for(int i = 31; i >= 0; i--) {
        //比较dividend是否大于divisor的(1<<i)次方，不要将dividend与(divisor<<i)比较，而是用(dividend>>i)与divisor比较，
        //效果一样，但是可以避免因(divisor<<i)操作可能导致的溢出，如果溢出则会可能dividend本身小于divisor，但是溢出导致dividend大于divisor       
        if((dividend >> i) >= divisor) {            
            quotient = add(quotient, 1 << i);            
            dividend = substract(dividend, divisor << i);        
        }    
    }    
    // 确定商的符号    
    if((a ^ b) < 0){
        // 如果除数和被除数异号，则商为负数        
        quotient = add(~quotient, 1);    
    }    
    // 确定余数符号    
    remainder = b > 0 ? dividend : add(~dividend, 1);    
    return quotient;// 返回商
}
//加速函数结束
//BST（二分查找树）开始
typedef int ElementType;
typedef struct TNode *Position;
typedef Position Binary_tree;
struct TNode{
    ElementType data;
    Binary_tree left;
    Binary_tree right;
};
Position Find(ElementType x,Binary_tree BST){
    if(!BST)return NULL;
    if(x>BST->data)return Find(x,BST->right);
    else if(x<BST->data)return Find(x,BST->left);
    else return BST;
}
Position IterFind(ElementType x, Binary_tree BST){
    while(BST){
        if(x>BST->data)BST=BST->right;
        else if(x<BST->data)BST=BST->left;
        else return BST;
    }
    return NULL;
}
Position FindMin(Binary_tree BST){
    if(BST){
        while(BST->left)BST=BST->left;
    }
    return BST;
}
Position FindMax(Binary_tree BST){
    if(BST){
        while(BST->right)BST=BST->right;
    }
    return BST;
}
Binary_tree Insert(Binary_tree BST,ElementType x){
    if(!BST){
        BST=(Binary_tree)malloc(sizeof(struct TNode));
        BST->data=x;
        BST->left=BST->right=NULL;
    }
    else{
        if(x<BST->data)BST->left=Insert(BST->left,x);
        else if(x>BST->data)BST->right=Insert(BST->right,x);
    }
    return BST;
}
Binary_tree Delete(Binary_tree BST,ElementType x){
    Position temp;
    if(!BST)printf("Didn't find it");
    else{
        if(x<BST->data)BST->left=Delete(BST->left,x);
        else if(x>BST->data)BST->right=Delete(BST->right,x);
        else{
            if(BST->left&&BST->right){
                temp=FindMin(BST->right);
                BST->data=temp->data;
                BST->right=Delete(BST->right,BST->data);
            }
            else{
                temp=BST;
                if(!BST->left)BST=BST->right;
                else BST=BST->left;
                free(temp);
            }
        }
    }
    return BST;
}
//二分查找树结束
//treap开始
const int INF=1e8;
struct Node{
    int left,right;
    int key,value;
    int cnt,size;
}tree[1010];
int root,idx;
void pushup(int x){
    tree[x].size=tree[tree[x].left].size+tree[tree[x].right].size+tree[x].cnt;
}
int get_node(int key){
    tree[++idx].key=key;
    tree[idx].value=rand();
    tree[idx].cnt=tree[idx].size=1;
    return idx;
}
void zig(int &p){
    int q=tree[p].left;
    tree[p].left=tree[q].right,tree[q].right=p,p=q;
    pushup(tree[p].right),pushup(p);
}
void zag(int &p){
    int q=tree[p].right;
    tree[p].right=tree[q].left,tree[q].left=p,p=q;
    pushup(tree[p].left),pushup(p);
}
void build(){
    get_node(-INF),get_node(INF);
    root=1,tree[1].right=2;
    pushup(root);
    if(tree[1].value<tree[2].value)zag(root);
}
void insert(int &p,int key){
    if(!p)p=get_node(key);
    else if(tree[p].key==key)tree[p].cnt++;
    else if(tree[p].key>key){
        insert(tree[p].left,key);
        if(tree[tree[p].left].value>tree[p].value)zig(p);
    }
    else{
        insert(tree[p].right,key);
        if(tree[tree[p].right].value>tree[p].value)zag(p);
    }
    pushup(p);
}
void remove(int &p,int key){
    if(!p)return;
    if(tree[p].key==key){
        if(tree[p].cnt>1)tree[p].cnt--;
        else if(tree[p].left||tree[p].right){
            if(!tree[p].right||tree[tree[p].left].value>tree[tree[p].right].value){
                zig(p);
                remove(tree[p].right,key);
            }
            else{
                zag(p);
                remove(tree[p].left,key);
            }
        }
        else p=0;
    }
    else if(tree[p].key>key)remove(tree[p].left,key);
    else remove(tree[p].right,key);
    pushup(p);
}
int get_rand_by_key(int p,int key){
    if(!p)return INF;
    if(tree[p].key==key)return tree[tree[p].left].size+1;
    if(tree[p].key>key)return get_rand_by_key(tree[p].left,key);
    return tree[tree[p].left].size+tree[p].cnt+get_rand_by_key(tree[p].right,key);
}
int get_key_by_rank(int p, int rank){
    if (!p) return INF;
    if (tree[tree[p].left].size >= rank) return get_key_by_rank(tree[p].left, rank);
    if (tree[tree[p].left].size + tree[p].cnt >= rank) return tree[p].key;
    return get_key_by_rank(tree[p].right, rank - tree[tree[p].left].size - tree[p].cnt);
}

int get_prev(int p, int key){
    if (!p) return -INF;
    if (tree[p].key >= key) return get_prev(tree[p].left, key);
    return max(tree[p].key, get_prev(tree[p].right, key));
}

int get_next(int p, int key){
    if (!p) return INF;
    if (tree[p].key <= key) return get_next(tree[p].right, key);
    return min(tree[p].key, get_next(tree[p].left, key));
}
//treap结束
int main(){
    return 0;
}