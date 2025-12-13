#include <iostream>
#include <climits>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


struct Edge {
    int u, v, weight;
    string u_name, v_name;

    Edge() : u(-1), v(-1), weight(0) {}
    Edge(int _u, int _v, int _w, string _un, string _vn)
        : u(_u), v(_v), weight(_w), u_name(_un), v_name(_vn) {
    }

    bool operator<(const Edge& other) { return weight < other.weight; }
    bool operator>(const Edge& other) { return weight > other.weight; }
    bool operator<=(const Edge& other) { return weight <= other.weight; }
    bool operator>=(const Edge& other) { return weight >= other.weight; }
    bool operator==(const Edge& other) { return weight == other.weight; }
    bool operator!=(const Edge& other) { return weight != other.weight; }
};

template <typename T>
class StackNode {
public:
    T value;
    StackNode<T>* next = nullptr;
public:
    StackNode(T value) {
        this->value = value;
    }
};

template <typename T>
class Stack {
public:
    StackNode<T>* top;

public:
    Stack<T>() {
        top = nullptr;
    }
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    void push(T value) {
        StackNode<T>* temp = new StackNode<T>(value);
        temp->next = top;
        top = temp;
    }

    void pop() {
        if (!isEmpty()) {
            StackNode<T>* temp = top;
            top = top->next;
            delete temp;
        }
        else {
            cout << "Стэк и так пуст";
            return;
        }
    }

    bool isEmpty() {
        return (top == nullptr);
    }

    T get() {
        if (!isEmpty()) {
            return top->value;
        }
        else {
            cout << "Стэк пуст";
            return T();
        }
    }

    int size() {
        StackNode<T>* cur = top;
        int count(0);
        while (cur) {
            cur = cur->next;
            count++;
        }
        return count;
    }

};

template <typename T>
class DynamicArray {
public:
    T* data;
    int size;
    int capacity;

public:
    DynamicArray(int capacity) {
        this->capacity = capacity;
        data = new T[capacity];
        this->size = 0;
    }
    ~DynamicArray() {
        delete[] data;
    }
    void resize(int newSize) {
        T* temp = new T[newSize];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        for (int i = size; i < newSize; i++) {
            temp[i] = T();
        }
        this->size = newSize;
        this->capacity = newSize;
        delete[] data;
        this->data = temp;
    }

    void reverse() {
        T* temp = new T[capacity * 2];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        this->capacity *= 2;
        this->data = temp;
    }

    void reverse(int capacity) {
        T* temp = new T[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        delete[] data;
        this->capacity = capacity;
        this->data = temp;
    }

    void remove(int index) {
        while (index >= size || index < 0) {
            std::cout << "Индекс либо больше размера массива, либо меньше нуля, попробуйте снова: ";
            std::cin >> index;
        }
        for (int i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    void insert(int index, T value) {
        while (index > size || index < 0) {
            std::cout << "Индекс либо больше размера массива, либо меньше нуля, попробуйте снова: ";
            std::cin >> index;
        }
        if (size == capacity) {
            reverse();
        }
        for (int i = size; i > index; i--) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        size++;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс либо больше размера массива, либо меньше нуля");
        }
        return data[index];
    }
};



int binarySearch(DynamicArray<Edge>& arr, Edge target, int left, int right) {
    int mid = (left + right) / 2;

    if (target == arr[mid]) {
        return mid;
    }
    if (target > arr[mid]) {
        return binarySearch(arr, target, mid + 1, right);
    }
    else {
        return binarySearch(arr, target, left, mid - 1);
    }
}


void insertionSort(DynamicArray<Edge>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        Edge value = arr[i];
        for (int j = i - 1; j >= left && arr[j] > value; j--) {
            arr[j + 1] = arr[j];
            arr[j] = value;
        }
    }
}

int calculateMinRun(int lenArr) {
    int reminder = 0;
    while (lenArr >= 64) {
        if (lenArr & 1) {
            reminder++;
        }
        lenArr >>= 1;
    }
    return lenArr + reminder;
}

void reverseRun(DynamicArray<Edge>& arr, int left, int right) {
    while (left < right) {
        Edge tmp = arr[left];
        arr[left] = arr[right];
        arr[right] = tmp;
        left++;
        right--;
    }
}

int gallopingModernizated(DynamicArray<Edge>& arr, int start, int end, Edge value) {
    int limit = min(start + 7, end);
    int i = start;

    while (i < limit && arr[i] < value) {
        i++;
    }

    if (i < limit) {
        return i;
    }

    int step = 1;
    int posAfterFirstCheck = start + 7;

    while (posAfterFirstCheck < end && arr[posAfterFirstCheck] < value) {
        posAfterFirstCheck = start + 7 + (1 << step);
        step++;
    }

    int leftBound = start + 7 + (1 << (step - 1));
    int rightBound = min(posAfterFirstCheck, end);

    leftBound = min(leftBound, end);
    rightBound = min(rightBound, end);

    return binarySearch(arr, value, leftBound, rightBound);
}

void merge(DynamicArray<Edge>& arr, int left, int mid, int right) {
    int i = 0, j = 0;
    int len1 = mid - left + 1;
    int len2 = right - mid;

    Edge* a = new Edge[len1 + 1];
    Edge* b = new Edge[len2 + 1];

    a[len1] = Edge(-1, -1, INT_MAX, "", "");
    b[len2] = Edge(-1, -1, INT_MAX, "", "");

    for (int i = 0; i < len1; i++) {
        a[i] = arr[left + i];
    }
    for (int i = mid + 1; i < right + 1; i++) {
        b[j++] = arr[i];
    }

    i = 0;
    j = 0;

    for (int k = left; k <= right; k++) {
        if (a[i] <= b[j]) {
            arr[k] = a[i++];
        }
        else {
            if (len2 - j > 7) {
                int gallopingJ = gallopingModernizated(arr, mid + 1 + j, right, a[i]);
                if (gallopingJ > mid + 1 + j) {
                    while (mid + 1 + j < gallopingJ && k <= right) {
                        arr[k++] = b[j++];
                    }
                    k--;
                }
            }
            arr[k] = b[j++];
        }
    }

    delete[] a;
    delete[] b;
}

void mergeRuns(DynamicArray<Edge>& arr, Stack <pair<int, int>>& stack) {
    while (stack.size() > 1) {
        pair<int, int> X, Y, Z;
        Z = stack.get();
        stack.pop();
        Y = stack.get();
        stack.pop();
        bool hasX = false;
        if (!stack.isEmpty()) {
            X = stack.get();
            stack.pop();
            hasX = true;
        }
        bool merged = false;
        if (hasX && X.second <= Y.second + Z.second) {
            merge(arr, X.first, X.first + X.second - 1, Y.first + Y.second - 1);
            stack.push({ X.first, X.second + Y.second });
            stack.push(Z);
            merged = true;
        }
        else if (Y.second <= Z.second) {
            merge(arr, Y.first, Y.first + Y.second - 1, Z.first + Z.second - 1);
            stack.push({ Y.first, Y.second + Z.second });
            if (hasX) stack.push(X);
            merged = true;
        }
        if (!merged) {
            if (hasX) stack.push(X);
            stack.push(Y);
            stack.push(Z);
            break;
        }
    }
    while (stack.size() > 1) {
        pair<int, int> Z = stack.get(); stack.pop();
        pair<int, int> Y = stack.get(); stack.pop();
        merge(arr, Y.first, Y.first + Y.second - 1, Z.first + Z.second - 1);
        stack.push({ Y.first, Y.second + Z.second });
    }
}


void timSort(DynamicArray<Edge>& arr, int lenArr) {
    int minRun = calculateMinRun(lenArr);
    Stack<pair<int, int>> stack;
    int counter;
    int startRun;
    for (int i = 0; i < lenArr; i++) {
        counter = 1;
        startRun = i;
        if (lenArr == i + 1) {
            stack.push({ i,counter });
            mergeRuns(arr, stack);
            break;
        }
        startRun = i;
        if (arr[i] <= arr[i + 1]) {
            while (i + 1 < lenArr && arr[i] <= arr[i + 1]) {
                counter++;
                i++;
            }
        }
        else {
            while (i + 1 < lenArr && arr[i] > arr[i + 1]) {
                counter++;
                i++;
            }
            reverseRun(arr, startRun, startRun + counter - 1);
        }

        if (counter < minRun) {
            int minimal = min(lenArr - startRun, minRun);
            insertionSort(arr, startRun, startRun + minimal - 1);
            counter = minimal;
            i = startRun + counter - 1;
        }
        stack.push({ startRun, counter });
        mergeRuns(arr, stack);
    }

    mergeRuns(arr, stack);
}
class SDS {
public:
    DynamicArray<int> parents;

    SDS(int countEl) : parents(countEl) {
        parents.resize(countEl);
        for (int i = 0; i < countEl; i++) {
            parents[i] = i;
        }
    }

    int find(int index) {
        if (index < 0 || index >= parents.size) {
            throw std::out_of_range("Индекс в find() вне диапазона");
        }
        if (parents[index] == index) {
            return index;
        }
        else {
            parents[index] = find(parents[index]);
            return parents[index];
        }
    }

    void unionSets(int firstEl, int secondEl) {
        int root1 = find(firstEl);
        int root2 = find(secondEl);
        if (root1 != root2) {
            parents[root1] = root2;
        }
    }
};

void DFS(int current, DynamicArray<Edge>& edges, bool visited[], string vertexNames[], int vertexCount) {
    visited[current] = true;
    cout << vertexNames[current] << " ";

    for (int i = 0; i < edges.size; i++) {
        Edge edge = edges[i];
        int neighbor = -1;
        if (edge.u == current) {
            neighbor = edge.v;
        }
        else if (edge.v == current) {
            neighbor = edge.u;
        }
        if (neighbor != -1 && !visited[neighbor]) {
            DFS(neighbor, edges, visited, vertexNames, vertexCount);
        }
    }
}

void BFS(int start, DynamicArray<Edge>& edges, int vertexCount, string vertexNames[]) {
    bool* visited = new bool[vertexCount];
    for (int i = 0; i < vertexCount; i++)
        visited[i] = false;

    int* queue = new int[vertexCount];
    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = true;

    while (front < rear) {
        int current = queue[front++];
        cout << vertexNames[current] << " ";

        for (int i = 0; i < edges.size; i++) {
            int neighbor = -1;
            if (edges[i].u == current) neighbor = edges[i].v;
            else if (edges[i].v == current) neighbor = edges[i].u;

            if (neighbor != -1 && !visited[neighbor]) {
                queue[rear++] = neighbor;
                visited[neighbor] = true;
            }
        }
    }

    delete[] visited;
    delete[] queue;
}

bool edgeExists(DynamicArray<Edge>& edges, int u, int v) {
    for (int i = 0; i < edges.size; i++) {
        if ((edges[i].u == u && edges[i].v == v) || (edges[i].u == v && edges[i].v == u)) {
            return true;
        }
    }
    return false;
}

void kruskal(DynamicArray<Edge>& edges, int vertexCount) {

    timSort(edges, edges.size);

    SDS sds(vertexCount);

    DynamicArray<Edge> mst(vertexCount - 1);

    for (int i = 0; i < edges.size; i++) {
        int u = edges[i].u;
        int v = edges[i].v;

        if (u < 0 || u >= vertexCount || v < 0 || v >= vertexCount) {
            cout << "Пропущено некорректное ребро: "
                << u << " - " << v << " вес: " << edges[i].weight << endl;
            continue;
        }

        if (sds.find(u) != sds.find(v)) {
            mst.insert(mst.size, edges[i]);
            sds.unionSets(u, v);
        }

        if (mst.size == vertexCount - 1) break;
    }


    cout << "Минимальное остовное дерево:" << endl;
    for (int i = 0; i < mst.size; i++) {
        cout << mst[i].u_name << " - " << mst[i].v_name
            << " : " << mst[i].weight << endl;
    }
}

int main() {
    ifstream file("C:\\Users\\tolop\\Desktop\\Graph.txt");
    if (!file.is_open()) {
        cout << "Такого файла не найдено" << endl;
        return 1;
    }
    string line;
    string vertexNames[20];
    int vertexCount = 0;
    if (getline(file, line)) {
        stringstream ss(line);
        string name;
        while (ss >> name) {
            vertexNames[vertexCount++] = name;
        }
    }

    DynamicArray<Edge> edges(10);

    for (int i = 0; i < vertexCount; i++) {
        if (!getline(file, line)) {
            break;
        }
        stringstream ss(line);

        int weight;
        for (int j = 0; j < vertexCount; j++) {
            ss >> weight;
            if (weight != 0 && !edgeExists(edges, i, j)) {
                edges.insert(edges.size, Edge(i, j, weight, vertexNames[i], vertexNames[j]));
            }
        }
    }
    file.close();


    if (vertexCount > 0) {
        bool* visited = new bool[vertexCount];
        for (int i = 0; i < vertexCount; i++) {
            visited[i] = false;
        }
        cout << "DFS: ";
        DFS(0, edges, visited, vertexNames, vertexCount);
        cout << "BFS: ";
        BFS(0, edges, vertexCount, vertexNames);

        kruskal(edges, vertexCount);
    }



}
