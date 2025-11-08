#pragma once
#include <vector>
#include "node.hpp"

class MinHeap {
private:
    std::vector<Node*> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    void push(Node* node);
    Node* pop();
    bool empty() const;
    void clear();
};
