#pragma once


namespace my {

template <class T, class Allocator>
class vector;

template <class T, class Allocator>
bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);

template <class T, class Allocator>
bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);

template <class T, class Allocator>
bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);

template <class T, class Allocator>
bool operator> (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);

template <class T, class Allocator>
bool operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);

template <class T, class Allocator>
bool operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs);


template <class T, class Allocator = std::allocator<T>>
class vector {
public:

    typedef T                                                        value_type;
    typedef Allocator                                                allocator_type;
    typedef std::size_t                                              size_type;
    typedef std::ptrdiff_t                                           difference_type;
    typedef value_type&                                              reference;
    typedef const value_type&                                        const_reference;
    typedef typename std::allocator_traits<Allocator>::pointer       pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
    typedef pointer                                                  iterator;
    typedef const_pointer                                            const_iterator;
    typedef std::reverse_iterator<iterator>                          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                    const_reverse_iterator;


    explicit vector(const Allocator& alloc = Allocator()) noexcept;
    vector(size_type count, const T& value, const Allocator& alloc = Allocator());
    explicit vector(size_type count, const Allocator& alloc = Allocator());

    template <class InputIt, class = typename std::iterator_traits<InputIt>::iterator_category>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());

    //не понял как нормально перегрузить
    vector(const vector& other);
    vector(const vector&, const Allocator& alloc);

    vector(vector&& other) noexcept; //почему можно не указывать аргументы шаблона (ну и например имена переменных) в определении (<T, Allocator>) //+докопаться до реализации
    vector(vector&& other, const Allocator& alloc);

    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());

    ~vector();


    vector&              operator=(const vector& other);
    vector<T,Allocator>& operator=(vector<T,Allocator>&& other) noexcept(std::allocator_traits<Allocator>
                                                                         ::propagate_on_container_move_assignment::value
                                                                         || std::allocator_traits<Allocator>
                                                                         ::is_always_equal::value);
    vector&              operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value);

    template <class InputIt, class = typename std::iterator_traits<InputIt>::iterator_category>
    void assign(InputIt first, InputIt last);

    void assign(std::initializer_list<T>);

    allocator_type get_allocator() const;


    reference       at(size_type pos);
    const_reference at(size_type pos) const;
    reference       operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;

    pointer       data() noexcept;
    const_pointer data() const noexcept;


    iterator                begin() noexcept;
    const_iterator          begin() const noexcept;
    const_iterator          cbegin() noexcept;

    iterator                end()   noexcept;
    const_iterator          end()   const noexcept;
    const_iterator          cend() noexcept;

    reverse_iterator        rbegin() noexcept;
    const_reverse_iterator  rbegin() const noexcept;
    const_reverse_iterator  crbegin() const noexcept;

    reverse_iterator        rend() noexcept;
    const_reverse_iterator  rend() const noexcept;
    const_reverse_iterator  crend() const noexcept;


    bool      empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void      reserve(size_type new_cap);
    size_type capacity() const noexcept;
    void      shrink_to_fit();


    void clear() noexcept;


    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);

    template <class InputIt, class = typename std::iterator_traits<InputIt>::iterator_category>
    iterator insert (const_iterator pos, InputIt first, InputIt last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);


    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);


    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void push_back(T&& value);


    template <class... Args>
    reference emplace_back(Args&& ...args);


    void pop_back();


    void resize(size_type count);
    void resize(size_type count, const value_type& value);


    void swap(vector& other) noexcept(std::allocator_traits<Allocator>::
                                          propagate_on_container_swap::value
                                          || std::allocator_traits<Allocator>::
                                          is_always_equal::value);

    friend bool operator== <> (const vector& lhs, const vector& rhs);
    friend bool operator!= <> (const vector& lhs, const vector& rhs);

    friend bool operator<  <> (const vector& lhs, const vector& rhs);
    friend bool operator<= <> (const vector& lhs, const vector& rhs);

    friend bool operator>  <> (const vector& lhs, const vector& rhs);
    friend bool operator>= <> (const vector& lhs, const vector& rhs);


private:
    static constexpr double CAPACITY_INCREASE_FACTOR = 2;

    void allocate_more_if_needed(size_type count = 1);
    void shift_right_from_pos(const_iterator pos, size_type count = 1);
    void shift_left_to_pos(const_iterator pos, size_type count = 1);

    Allocator allocator_;
    pointer data_;
    size_type size_;
    size_type capacity_;

};

template<class T, class Allocator>
vector<T, Allocator>::vector(const Allocator& alloc) noexcept
        : allocator_(alloc),
          data_(nullptr),
          size_(0),
          capacity_(0) {}

template<class T, class Allocator>
vector<T, Allocator>::vector(size_type count, const T& value, const Allocator& alloc)
        : allocator_(alloc),
          data_(allocator_.allocate(count)),
          size_(count),
          capacity_(count)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(value));
    }
}


template<class T, class Allocator>
vector<T, Allocator>::vector(size_type count, const Allocator& alloc)
        : allocator_(alloc),
          data_(allocator_.allocate(count)),
          size_(count),
          capacity_(count)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i);
    }
}

template<class T, class Allocator>
template<class InputIt, class>
vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator& alloc)
        : allocator_(alloc),
          data_(allocator_.allocate(std::distance(first, last))),
          size_(static_cast<size_type>(std::distance(first, last))),
          capacity_(size_)
{
    for (size_type i = 0; first != last; i++, first++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *first);
    }
}

template<class T, class Allocator>
vector<T, Allocator>::vector(const vector<T, Allocator>& other)
        : allocator_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())),
          data_(allocator_.allocate(other.capacity_)),
          size_(other.size_),
          capacity_(other.capacity_)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(other.data_[i]));
    }
}

template<class T, class Allocator>
vector<T, Allocator>::vector(const vector<T, Allocator>& other, const Allocator& alloc) //ask why we need custom alloc
        : allocator_(alloc),
          data_(allocator_.allocate(other.capacity_)),
          size_(other.size_),
          capacity_(other.capacity_)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(other.data_[i]));
    }
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector<T, Allocator>&& other) noexcept  //allocator_traits::propagate_on_container_move_assignment <- what is this???
        : allocator_(std::move(other.get_allocator())),
          data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_)
{
    other.allocator_ = Allocator();
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<class T, class Allocator>
vector<T, Allocator>::vector(vector<T, Allocator>&& other, const Allocator& alloc)
        : allocator_(alloc),
          data_(allocator_.allocate(other.capacity_)),
          size_(other.size_),
          capacity_(other.capacity_)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::move(other.data_[i]));
    }
}

template<class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> init, const Allocator& alloc)
        : allocator_(alloc),
          data_(allocator_.allocate(init.size())),
          size_(init.size()),
          capacity_(size_)
{
    auto l_iterator = init.begin();
    auto l_end = init.end();
    for (size_type i = 0; l_iterator != l_end; i++, l_iterator++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *(l_iterator));
    }
}

template<class T, class Allocator>
vector<T, Allocator>::~vector()
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type pos)
{
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type pos) const {
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept
{
    return capacity_;
}

template<class T, class Allocator>
bool vector<T, Allocator>::empty() const noexcept
{
    return size_ == 0;
}

template<class T, class Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const
{
    return allocator_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept
{
    return size_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept
{
    return std::allocator_traits<Allocator>::max_size(allocator_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos)
{
    if (pos < 0 || pos >= size()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const
{
    if (pos < 0 || pos >= size()) {
        throw std::out_of_range("Index out of range");
    }
    return data_[pos];
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front()
{
    return data_[0];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const
{
    return data_[0];
}

template<class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back()
{
    return data_[size_ - 1];
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
{
    return data_[size_ - 1];
}

template<class T, class Allocator>
typename vector<T, Allocator>::pointer vector<T, Allocator>::data() noexcept
{
    return data_;
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_pointer vector<T, Allocator>::data() const noexcept
{
    return data_;
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(const T& value)
{
    allocate_more_if_needed();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<const T&>(value));
    size_++;
}

template<class T, class Allocator>
void vector<T, Allocator>::push_back(T&& value)
{
    allocate_more_if_needed();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<const T&&>(value));
    size_++;
}

template<class T, class Allocator>
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type count)
{
    if (count == size_) {
        return;
    }
    if (count < size_) {
        erase(begin() + count, end());
        return;
    }
    allocate_more_if_needed(count - size_);
    for (size_type i = size_; i < count; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i);
    }
    size_ = count;
}

template<class T, class Allocator>
void vector<T, Allocator>::resize(typename vector<T, Allocator>::size_type count, const value_type& value)
{
    if (count == size_) {
        return;
    }
    if (count < size_) {
        erase(begin() + count, end());
        return;
    }
    allocate_more_if_needed(count - size_);
    for (size_type i = size_; i < count; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const value_type&>(value));
    }
    size_ = count;
}

template<class T, class Allocator>
void vector<T, Allocator>::reserve(vector<T, Allocator>::size_type new_cap)
{
    if (new_cap <= capacity_) {
        return;
    }
    if (new_cap > max_size()) {
        throw std::length_error("Maximum container size exceeded");
    }
    pointer new_data = allocator_.allocate(new_cap);
    for (size_type i = 0; i < size_; i++) {
        try {
            std::allocator_traits<Allocator>::construct(allocator_, new_data + i, std::forward<T>(data_[i]));
        } catch ( ... ) {
            for (size_type j = 0; j < i; j++){
                std::allocator_traits<Allocator>::destroy(allocator_, new_data + j);
            }
            allocator_.deallocate(new_data, new_cap);
            throw;
        }
    }
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);
    data_= new_data;
    capacity_ = new_cap;
}

template<class T, class Allocator>
void vector<T, Allocator>::pop_back()
{
    if (size_ > 0) {
        size_--;
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + size_);
    }
}

template<class T, class Allocator>
template<class... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator pos, Args&&... args)
{
    if (pos - cbegin() > size_ || pos - cbegin() < 0) {
        throw std::out_of_range("Iterator out of range");
    }
    auto dist = std::distance(cbegin(), pos);
    allocate_more_if_needed();
    shift_right_from_pos(begin() + dist);
    std::allocator_traits<Allocator>::construct(allocator_, &(*(begin() + dist)), std::forward<Args&&>(args)...);
    size_++;
    return begin() + dist;
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, const T& value)
{
    return emplace(pos, std::forward<const T&>(value));
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, T&& value)
{
    return emplace(pos, std::forward<const T&&>(value));
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, size_type count, const T& value)
{
    if (pos - cbegin() > size_ || pos - cbegin() < 0) {
        throw std::out_of_range("Iterator out of range");
    }
    auto dist = std::distance(cbegin(), pos);
    allocate_more_if_needed(count);
    shift_right_from_pos(pos, count);
    for (iterator i = begin() + dist; i != begin() + dist + count; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, &*i, std::forward<const T&>(value));
    }
    size_ += count;
    return begin() + dist;
}

template<class T, class Allocator>
template<class InputIt, class>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last)
{
    if (pos - cbegin() > size_ || pos - cbegin() < 0) {
        throw std::out_of_range("Iterator out of range");
    }
    auto dist = std::distance(cbegin(), pos);
    size_type input_size = static_cast<size_type>(std::distance(first, last));
    allocate_more_if_needed(input_size);
    shift_right_from_pos(pos, input_size);
    for (iterator i = begin() + dist; first != last; i++, first++) {
        std::allocator_traits<Allocator>::construct(allocator_, &*i, *first);
    }
    size_ += input_size;
    return begin() + dist;
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
    return insert(pos, ilist.begin(), ilist.end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator pos)
{
    if (pos - cbegin() > size_ || pos - cbegin() < 0) {
        throw std::out_of_range("Iterator out of range");
    }
    auto dist = std::distance(cbegin(), pos);
    std::allocator_traits<Allocator>::destroy(allocator_, &*pos);
    shift_left_to_pos(pos);
    size_--;
    return begin() + dist;
}


template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first, const_iterator last)
{
    if (last - cbegin() > size_ || first - cbegin() < 0) {
        throw std::out_of_range("Iterator out of range");
    }
    auto dist = std::distance(cbegin(), first);
    size_type count = static_cast<size_type>(std::distance(first, last));
    for (auto i = first; i != last; i++){
        std::allocator_traits<Allocator>::destroy(allocator_, &*i);
    }
    shift_left_to_pos(first, count);
    size_ -= count;
    return begin() + dist;
}

template<class T, class Allocator>
void vector<T, Allocator>::swap(vector<T, Allocator>& other) noexcept(std::allocator_traits<Allocator>::
                                                                              propagate_on_container_swap::value
                                                                      || std::allocator_traits<Allocator>::
                                                                              is_always_equal::value)
{
    std::swap(allocator_, other.allocator_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}

template<class T, class Allocator>
void vector<T, Allocator>::allocate_more_if_needed(size_type count)
{
    for(size_type i = 0; i < count; i++) {
        if (size_ + i == max_size()) {
            throw std::length_error("Maximum container size exceeded");
        }
        if (capacity_ >= size_ + 1 + i) {
            continue;
        }
        size_type new_cap;
        new_cap = static_cast<size_type>(round(capacity_ * CAPACITY_INCREASE_FACTOR));
        if (new_cap == 0) {
            new_cap++;
        }
        if (new_cap <= capacity_ || new_cap > max_size()) {
            new_cap = max_size();
        }
        reserve(new_cap);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit()
{
    if (capacity_ == size_) {
        return;
    }
    pointer new_data = allocator_.allocate(size_);
    for (size_type i = 0; i < size_; i++) {
        try {
            std::allocator_traits<Allocator>::construct(allocator_, new_data + i, std::forward<const T&>(data_[i]));
        } catch ( ... ) {
            for (size_type j = 0; j < i; j++){
                std::allocator_traits<Allocator>::destroy(allocator_, new_data + j);
            }
            allocator_.deallocate(new_data, size_);
            throw;
        }
    }
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);
    data_= new_data;
    capacity_ = size_;
}

template<class T, class Allocator>
template<class... Args>
typename vector<T,Allocator>::reference vector<T, Allocator>::emplace_back(Args&& ...args)
{
    allocate_more_if_needed();
    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<Args&&>(args)...);
    size_++;
    return data_[size_ - 1];
}

template<class T, class Allocator>
void vector<T, Allocator>::clear() noexcept
{
    resize(0);
}

template<class T, class Allocator>
typename vector<T,Allocator>::iterator vector<T, Allocator>::begin() noexcept
{
    return static_cast<iterator>(data_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept
{
    return static_cast<const_iterator>(data_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept
{
    return static_cast<iterator>(data_ + size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept
{
    return static_cast<const_iterator>(data_ + size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() noexcept
{
    return static_cast<const_iterator>(data_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() noexcept
{
    return static_cast<const_iterator>(data_ + size_);
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}


template<class T, class Allocator>
void vector<T, Allocator>::assign(size_type count, const T& value)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);

    data_ = allocator_.allocate(count);
    size_ = count;
    capacity_ = count;
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(value));
    }
}

template<class T, class Allocator>
template<class InputIt, class>
void vector<T, Allocator>::assign(InputIt first, InputIt last)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);

    size_ = static_cast<size_type>(std::distance(first, last));
    capacity_ = size();
    data_ = allocator_.allocate(capacity_);
    for (size_type i = 0; first != last; i++, first++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *first);
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> init_list)
{
    assign(init_list.begin(), init_list.end());
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator>& other)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);

    allocator_ = other.allocator_;
    data_ = allocator_.allocate(other.capacity_);
    capacity_ = other.capacity_;
    size_ = other.size_;

    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::forward<const T&>(other.data_[i]));
    }
    return *this;
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector<T, Allocator>&& other) noexcept(std::allocator_traits<Allocator>
                                                                                             ::propagate_on_container_move_assignment::value
                                                                                             || std::allocator_traits<Allocator>
                                                                                             ::is_always_equal::value)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);

    capacity_ = other.capacity_;
    size_ = other.size_;
    if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value || other.allocator_ == allocator_) {
        allocator_ = std::move(other.allocator_);
        data_ = other.data_;
    } else {
        allocator_ = other.allocator_;
        data_ = allocator_.allocate(capacity_);
        for (size_type i = 0; i < size_; i++) {
            data_[i] = std::move(other.data_[i]);
        }
    }
    other.allocator_ = Allocator();
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

template<class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(std::initializer_list<T> ilist)
{
    for (size_type i = 0; i < size_; i++) {
        std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
    }
    allocator_.deallocate(data_, capacity_);

    data_ = allocator_.allocate(ilist.size());
    size_ = ilist.size();
    capacity_ = size_;
    auto l_iterator = ilist.begin();
    auto l_end = ilist.end();
    for (size_type i = 0; l_iterator != l_end; i++, l_iterator++) {
        std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *(l_iterator));
    }
    return *this;
}

template<class T, class Allocator>
void vector<T, Allocator>::shift_right_from_pos(const_iterator pos, size_type count)
{
    if (count == 0) {
        return;
    }
    for (const_iterator i = end(); i != pos; i--) {
        std::allocator_traits<Allocator>::construct(allocator_, &*(i + count - 1), std::move(*(i - 1)));
        std::allocator_traits<Allocator>::destroy(allocator_, &*(i - 1));
    }
}

template<class T, class Allocator>
void vector<T, Allocator>::shift_left_to_pos(const_iterator pos, size_type count)
{
    if (count == 0 || pos + count >= end()) {
        return;
    }
    for (const_iterator i = pos; i != end() - count; i++) {
        std::allocator_traits<Allocator>::construct(allocator_, &*i, std::move(*(i + count)));
        std::allocator_traits<Allocator>::destroy(allocator_, &*(i + count));
    }
}


template<class T, class Allocator>
bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    if (lhs.size_ != rhs.size_) {
        return false;
    }
    auto iterator_l = lhs.begin();
    auto iterator_r = rhs.begin();

    while (iterator_l != lhs.end()){
        if (*iterator_l != *iterator_r) {
            return false;
        }
        iterator_l++;
        iterator_r++;
    }
    return true;
}

template<class T, class Allocator>
bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    auto count = std::min(lhs.size(), rhs.size());

    for (typename vector<T, Allocator>::size_type i = 0; i < count; i++) {
        if (lhs[i] < rhs[i]) {
            return true;
        } else if (lhs[i] > rhs[i]) {
            return false;
        }
    }
    return lhs.size() < rhs.size();
}

template<class T, class Allocator>
bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    return !(lhs == rhs);
}

template<class T, class Allocator>
bool operator>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    return rhs < lhs;
}

template<class T, class Allocator>
bool operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    return lhs < rhs || lhs == rhs;
}

template<class T, class Allocator>
bool operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
{
    return rhs <= lhs;
}

} // namespace my
