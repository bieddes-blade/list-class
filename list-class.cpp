#include <iostream>
#include <utility>

template <typename T>
class Node;

template <typename T>
using NodePointer = Node<T>*;

template <typename T>
class Node {
 public:
    Node() : val(T()), next(nullptr) {}
    Node(const T& val, NodePointer<T> next) : val(val), next(next) {}
    T val;
    NodePointer<T> next;
};

template <typename T>
class List {
 public:
    List(): _first(nullptr), _last(nullptr) {}

    List(const List&) = delete;

    List(List&& other) {
        _first = other._first;
        _last = other._last;
        other._clear();
    }

    size_t size() const {
        size_t size = 0;
        auto pointer = _first;
        while (pointer != nullptr) {
            pointer = pointer->next;
            ++size;
        }
        return size;
    }

    NodePointer<T> begin() const {
        return _first;
    }

    NodePointer<T> end() const {
        return nullptr;
    }

    ~List() {
        clear();
    }

    List& operator=(List&& other) {
        clear();
        _first = other._first;
        _last = other._last;
        other._clear();
        return *this;
    }

    void push_back(const T& val) {
        _push_back(new Node<T>(val, nullptr));
    }

    void clear() {
        while (_first != nullptr) {
            auto next = _first->next;
            delete _first;
            _first = next;
        }
        _last = nullptr;
    }

private:
    List(NodePointer<T> first, NodePointer<T> last) : _first(first), _last(last) {}

    void _clear() {
        _first = nullptr;
        _last = nullptr;
    }

    void _push_back(const NodePointer<T>& back) {
        if (!_last) {
            _first = _last = back;
        } else {
            _last->next = back;
            _last = _last->next;
        }
    }

    void _cend() {
        _last->next = nullptr;
    }

    template <typename D>
    friend List<D> merge(List<D>&& a, List<D>&& b);

    template <typename D>
    friend std::pair<List<D>, List<D>> split(List<D>&& self, size_t size_left);

    NodePointer<T> _first, _last;
};

template <typename T>
std::pair<List<T>, List<T>> split(List<T>&& self, size_t size_left) {
    auto left = self._first;
    auto middle = self._first;

    for (size_t i = 1; i < size_left - 1; ++i) {
        middle = middle->next;
    }

    auto right = self._last;

    auto list_left = List<T>(left, middle);
    auto list_right = List<T>(middle->next, right);

    list_left._cend();
    self._clear();

    return {std::move(list_left), std::move(list_right)};
}

template <typename T>
List<T> merge(List<T>&& left, List<T>&& right) {
    List<T> res;
    auto pointer_right = right.begin();
    for (auto pointer_left = left.begin(); pointer_left != left.end();) {
        if (pointer_right == right.end() || pointer_left->val < pointer_right->val) {
            res._push_back(pointer_left);
            pointer_left = pointer_left->next;
        } else {
            res._push_back(pointer_right);
            pointer_right = pointer_right->next;
        }
    }

    while (pointer_right != right.end()) {
        res._push_back(pointer_right);
        pointer_right = pointer_right->next;
    }

    res._cend();
    left._clear();
    right._clear();

    return res;
}

template <typename T>
void merge_sort(List<T>& my_list) {
    if (my_list.size() <= 1) {
        return;
    }

    auto div_list = split(std::move(my_list), my_list.size() / 2);

    merge_sort(div_list.first);
    merge_sort(div_list.second);

    my_list = merge(std::move(div_list.first), std::move(div_list.second));
}

int main() {
    int n;
    std::cin >> n;

    int tmp;
    List<int> my_list;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> tmp;
        my_list.push_back(tmp);
    }

    merge_sort(my_list);

    auto i = my_list.begin();
    while (i != my_list.end()) {
        std::cout << i->val << " ";
        i = i->next;
    }
    std::cout << "\n";
}