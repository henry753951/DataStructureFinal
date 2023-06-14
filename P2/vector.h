#include <iostream>
#include <string>

template <typename T>
class Vector {
   private:
    T* arr;
    size_t capacity;
    size_t size;

   public:
    // 建構式
    Vector() : arr(nullptr), capacity(0), size(0) {}
    Vector(std::initializer_list<T> initList) : arr(nullptr), capacity(0), size(0) {
        reserve(initList.size());
        for (const T& item : initList) {
            emplace_back(item);
        }
    }
    // 解構式
    ~Vector() {
        delete[] arr;
    }

    // 迭代器
    typedef T* iterator;
    typedef const T* const_iterator;

    iterator begin() { return arr; }
    iterator end() { return arr + size; }
    const_iterator begin() const { return arr; }
    const_iterator end() const { return arr + size; }
    const_iterator cbegin() const { return arr; }
    const_iterator cend() const { return arr + size; }

    // emplace_back 函式
    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (size == capacity) {
            // 擴展陣列容量
            size_t newCapacity = capacity == 0 ? 1 : capacity * 2;
            T* newArr = new T[newCapacity];
            if (arr) {
                for (size_t i = 0; i < size; ++i) {
                    newArr[i] = arr[i];
                }
                delete[] arr;
            }
            arr = newArr;
            capacity = newCapacity;
        }
        arr[size++] = T(std::forward<Args>(args)...);
    }

    // 取得元素數量
    size_t getSize() const {
        return size;
    }

    // to array
    T* toArray() {
        T* newArr = new T[size];
        for (size_t i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }
        return newArr;
    }

    // 預留容量
    void reserve(size_t newCapacity) {
        if (newCapacity > capacity) {
            T* newArr = new T[newCapacity];
            if (arr) {
                for (size_t i = 0; i < size; ++i) {
                    newArr[i] = arr[i];
                }
                delete[] arr;
            }
            arr = newArr;
            capacity = newCapacity;
        }
    }
    // 清空陣列
    void clear() {
        delete[] arr;
        arr = nullptr;
        capacity = 0;
        size = 0;
    }

    // 存取元素
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    // 刪除最後一個元素
    void pop_back() {
        if (size > 0) {
            --size;
        }
    }

    // 刪除指定位置的元素
    iterator erase(iterator position) {
        if (position >= begin() && position < end()) {
            for (iterator it = position + 1; it != end(); ++it) {
                *(it - 1) = *it;
            }
            --size;
        }
        return position;
    }

    // 插入元素到指定位置
    iterator insert(iterator position, const T& value) {
        size_t index = position - begin();
        emplace_back(value);
        for (iterator it = end() - 1; it != begin() + index; --it) {
            *it = *(it - 1);
        }
        *(begin() + index) = value;
        return begin() + index;
    }

    // 檢查陣列是否為空
    bool empty() const {
        return size == 0;
    }

    // 取得第一個元素
    T& front() {
        if (size > 0) {
            return arr[0];
        }
        throw std::out_of_range("Empty array");
    }

    const T& front() const {
        if (size > 0) {
            return arr[0];
        }
        throw std::out_of_range("Empty array");
    }

    // 取得最後一個元素
    T& back() {
        if (size > 0) {
            return arr[size - 1];
        }
        throw std::out_of_range("Empty array");
    }

    const T& back() const {
        if (size > 0) {
            return arr[size - 1];
        }
        throw std::out_of_range("Empty array");
    }
};