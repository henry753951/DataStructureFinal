template <typename T>
class Vector {
   public:
    Vector() : capacity_(DEFAULT_CAPACITY), size_(0) {
        data_ = new T[capacity_];
    }

    Vector(std::initializer_list<T> initList) : Vector() {
        for (const T& element : initList) {
            push_back(element);
        }
    }

    ~Vector() {
        delete[] data_;
    }

    void push_back(const T& element) {
        if (size_ == capacity_) {
            expandCapacity();
        }
        data_[size_++] = element;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    T& at(int index) {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Invalid index");
        }
        return data_[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("Invalid index");
        }
        return data_[index];
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }
    class Iterator {
       public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(pointer ptr) : ptr_(ptr) {}

        reference operator*() const {
            return *ptr_;
        }

        pointer operator->() const {
            return ptr_;
        }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            --ptr_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

       private:
        pointer ptr_;
    };

    Iterator begin() {
        return Iterator(data_);
    }

    Iterator end() {
        return Iterator(data_ + size_);
    }

   private:
    static const int DEFAULT_CAPACITY = 10;
    static const int GROWTH_FACTOR = 2;

    T* data_;
    int capacity_;
    int size_;

    void expandCapacity() {
        int newCapacity = capacity_ * GROWTH_FACTOR;
        T* newData = new T[newCapacity];
        for (int i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }
};