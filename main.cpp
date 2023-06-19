#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <pthread.h>

using namespace std;

struct Node {
    int data_;
    Node *next_;
    mutex mutex_;
    Node():next_(nullptr){};
    Node(int data):data_(data), next_(nullptr){};
};

void Add(int data, Node *head) {
    // TODO 加锁的插入操作
    // 空指针不能加锁 记得异常处理
    // 分三种情况判断：已经存在、已经位于队尾、正常插入（位于队中）
    if(head==nullptr){
        printf("head is null\n");
        return;
    }
    head->mutex_.lock();
    Node *prev=head;
    Node *cur=head->next_;
    while (cur!=nullptr)
    {
        cur->mutex_.lock();
        if (data<cur->data_)
        {
            printf("Add %d success\n",data);
            Node *tmp=new Node(data);
            // tmp->data_=data;
            prev->next_=tmp;
            tmp->next_=cur;
            cur->mutex_.unlock();
            prev->mutex_.unlock();
            return;
        }
        else if(data==cur->data_){
            printf("insert a exist data\n");
            cur->mutex_.unlock();
            prev->mutex_.unlock();
            return;
        }else{
            prev->mutex_.unlock();
            prev=cur;
            cur=cur->next_;
        }
    }
    printf("Add %d to the end\n",data);
    Node *tmp=new Node(data);
    // tmp->data_=data;
    prev->next_=tmp;
    prev->mutex_.unlock();
}

void Add_nolock(int data, Node *head) {
    // TODO 不加锁的插入操作
    if(head==nullptr){
        printf("lock a null pointer!\n");
        return;
    }
    // head->mutex_.lock();
    Node *prev=head;
    Node *cur=head->next_;
    while (cur!=nullptr)
    {
        // cur->mutex_.lock();
        if (data<cur->data_)
        {
            Node *tmp=new Node(data);
            // tmp->data_=data;
            prev->next_=tmp;
            tmp->next_=cur;
            // cur->mutex_.unlock();
            // prev->mutex_.unlock();
            return;
        }
        else if(data==cur->data_){
            printf("insert a exist data %d\n",data);
            // cur->mutex_.unlock();
            // prev->mutex_.unlock();
            return;
        }else{
            // prev->mutex_.unlock();
            prev=cur;
            cur=cur->next_;
        }
    }
    printf("Add %d to the end\n",data);
    Node *tmp=new Node(data);
    // tmp->data_=data;
    prev->next_=tmp;
    // prev->mutex_.unlock();
}

void Contain(int data, Node *head) {
    // TODO 查找列表中是否含有该数据
    if(head==nullptr){
        printf("head is null, Empty list");
        return;
    }
    Node *prev=head;
    Node *cur=head->next_;
    while(cur!=nullptr){
        if(cur->data_==data){
            printf("%d in list\n",data);
            return;
        }else if(cur->data_<data){
            prev=cur;
            cur=cur->next_;
        }else{
            printf("%d not in list\n",data);
            return;
        }
    }
    printf("%d not in list\n",data);
    return;
}

void Remove(int data, Node *head) {
    // TODO 移除列表中特定数据
    if(head==nullptr){
        printf("Empty List, nothing to remove.\n");
        return;
    }
    head->mutex_.lock();
    Node *prev=head;
    Node *cur=head->next_;
    while(cur!=nullptr&&cur->data_<data){
        cur->mutex_.lock();
        prev->mutex_.unlock();
        prev=cur;
        cur=cur->next_;
    }
    if(cur!=nullptr&&cur->data_==data){
        cur->mutex_.lock();
        prev->next_=cur->next_;
        delete cur;
        prev->mutex_.unlock();
        printf("Remove %d success\n",data);
    }else{
        prev->mutex_.unlock();
        printf("Remove %d failed, not such data\n",data);
    }
    return;
}

void Print(Node *head) {
    // TODO  遍历打印
    // 只需要锁头节点
    if(head==nullptr){
        printf("Empty list, nothing to print\n");
    }
    head->mutex_.lock();
    for(Node*cur=head->next_;cur!=nullptr;cur=cur->next_){
        printf("%d ",cur->data_);
    }
    head->mutex_.unlock();
    printf("\n");
    return;
}

int main() {
    Node *head = new Node();
    // Test Case 1
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add_nolock, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);
    // Test Case 2
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);

//    vector<thread> remthreads;
//    for (int i = 0; i < 100; i++)
//        remthreads.emplace_back(Remove, i, head);
//    for (auto &thread2 : remthreads)
//        thread2.join();
//    Print(head);

    // Test case 3
    vector<thread> addthreads;
    for (int i = 0; i < 100; i = i + 2)
        addthreads.emplace_back(Add, i, head);
    for (auto &thread1 : addthreads)
        thread1.join();
    Print(head);
    vector<thread> addremthreads;
    for (int i = 0; i < 100; i = i + 2) {
        addremthreads.emplace_back(Add, i+1, head);
        addremthreads.emplace_back(Remove, i, head);
    }
    for (auto &thread3 : addremthreads)
        thread3.join();
    Print(head);
    // Test case 4
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);
//    vector<thread> conremthreads;
//    for (int i = 0; i < 97; i = i + 2) {
//        conremthreads.emplace_back(Contain, i+3, head);
//        conremthreads.emplace_back(Remove, i, head);
//    }
//    for (auto &thread4 : conremthreads)
//        thread4.join();
//    Print(head);
    delete head;
    return 0;
}
