#include "min_heap.hpp"
#include <algorithm>

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index]->getFCost() < heap[parent]->getFCost() ||
           (heap[index]->getFCost() == heap[parent]->getFCost() &&
            heap[index]->hCost < heap[parent]->hCost)) {
            std::swap(heap[index], heap[parent]);
            index = parent;
        } else break;
    }
}

void MinHeap::heapifyDown(int index) {
    int size = (int)heap.size();
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size &&
           (heap[left]->getFCost() < heap[smallest]->getFCost() ||
           (heap[left]->getFCost() == heap[smallest]->getFCost() &&
            heap[left]->hCost < heap[smallest]->hCost))) {
            smallest = left;
        }

        if (right < size &&
           (heap[right]->getFCost() < heap[smallest]->getFCost() ||
           (heap[right]->getFCost() == heap[smallest]->getFCost() &&
            heap[right]->hCost < heap[smallest]->hCost))) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        } else break;
    }
}

void MinHeap::push(Node* node) {
    heap.push_back(node);
    heapifyUp((int)heap.size() - 1);
}

Node* MinHeap::pop() {
    if (heap.empty()) return nullptr;

    Node* top = heap.front();
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) heapifyDown(0);

    return top;
}

bool MinHeap::empty() const {
    return heap.empty();
}

void MinHeap::clear() {
    heap.clear();
}
