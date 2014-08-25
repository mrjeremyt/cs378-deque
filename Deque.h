// ----------------------
// projects/deque/Deque.h
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------

#ifndef Deque_h
#define Deque_h

// --------
// includes
// --------

#include <algorithm> // copy, equal, lexicographical_compare, max, swap
#include <cassert>   // assert
#include <iterator>  // iterator, bidirectional_iterator_tag
#include <memory>    // allocator
#include <stdexcept> // out_of_range
#include <utility>   // !=, <=, >, >=
#include <iostream>  

// -----
// using
// -----

using std::rel_ops::operator!=;
using std::rel_ops::operator<=;
using std::rel_ops::operator>;
using std::rel_ops::operator>=;

#define size_of_inner_array 10


// -------
// destroy
// -------

template <typename A, typename BI>
BI destroy (A& a, BI b, BI e) {
    while (b != e) {
        --e;
        a.destroy(&*e);
    }
    return b;
}

// ------------------
// uninitialized_copy
// ------------------

template <typename A, typename II, typename BI>
BI uninitialized_copy (A& a, II b, II e, BI x) {
    BI p = x;
    try {
        while (b != e) {
            a.construct(&*x, *b);
            ++b;
            ++x;
        }
    }
    catch (...) {
        destroy(a, p, x);
        throw;
    }
    return x;
}

// ------------------
// uninitialized_fill
// ------------------

template <typename A, typename BI, typename U>
BI uninitialized_fill (A& a, BI b, BI e, const U& v) {
    BI p = b;
    try {
        while (b != e) {
            a.construct(&*b, v);
            ++b;
        }
    }catch (...) {
        destroy(a, p, b);
        throw;
    }
    return e;
}

// -------
// my_deque
// -------

template < typename T, typename A = std::allocator<T> >
class my_deque {
    public:
        // --------
        // typedefs
        // --------

        typedef A                                        allocator_type;
        typedef typename allocator_type::value_type      value_type;

        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::difference_type difference_type;

        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;

        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;

        typedef typename std::allocator_traits<A>::template rebind_alloc<T*> map_allocator_type;
        typedef typename map_allocator_type::value_type      map_value_type;

        typedef typename map_allocator_type::size_type       map_size_type;
        typedef typename map_allocator_type::difference_type map_difference_type;

        typedef typename map_allocator_type::pointer         map_pointer;
        typedef typename map_allocator_type::const_pointer   map_const_pointer;

        typedef typename map_allocator_type::reference       map_reference;
        typedef typename map_allocator_type::const_reference map_const_reference;

    public:
        // -----------
        // operator ==
        // -----------

        /**
         * uses the deque iterator and size methods to determine equality component-wise
         */
        friend bool operator == (const my_deque& lhs, const my_deque& rhs) {
            return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        // ----------
        // operator <
        // ----------

        /**
         * uses the deque iterators to component-wise determine less than
         */
        friend bool operator < (const my_deque& lhs, const my_deque& rhs) {
            return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

    private:
        // ----
        // data
        // ----

        allocator_type _a; //size that the user sees

        map_allocator_type _m; //allocator for the maping array

        // <your data>

        map_pointer d_begin; 
        map_pointer d_end;
        map_pointer map_end;
        map_pointer map_begin;


        int size_of_map;

        size_type their_size; // total num elements, size that the user sees
        size_type inner_array_size; // num elements of each inner array
        size_type size_of_front; // num elements in the size_of_front front inner array
        size_type size_of_back; // num elements in the size_of_back back inner array

        pointer _b; // pointer pointing at the first element
        pointer _e; // pointer pointing at the last element



    private:
        // -----
        // valid
        // -----

        bool valid () const {
            return (!map_begin && !d_begin && !d_end && !map_end) || ((map_begin <= d_begin) && (d_begin <= d_end) && (d_end <= map_end));}

    public:
        // --------
        // iterator
        // --------

        class iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename my_deque::value_type      value_type;
                typedef typename my_deque::difference_type difference_type;
                typedef typename my_deque::pointer         pointer;
                typedef typename my_deque::reference       reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * calls the equals method of the deques 
                 */
                friend bool operator == (const iterator& lhs, const iterator& rhs) {
                    return (lhs._deq == rhs._deq) && (lhs._index == rhs._index);}

                /**
                 * not equals, calls deque's equals and then returns the complement of that 
                 */
                friend bool operator != (const iterator& lhs, const iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * plus operator, calls the += of the deque
                 */
                friend iterator operator + (iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * difference operator, calls the -= operator od deque
                 */
                friend iterator operator - (iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                // <your data>

                my_deque* _deq;
                size_type _index;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return _index >= 0;
                }

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * @param d the inputed deque
                 * @param i the index of the inputed deque
                 * the constructor for the non-const iterator 
                 */
                iterator (my_deque* d, size_type i) :
                    _deq (d),
                    _index (i)
                    {assert(valid());
                }

                // Default copy, destructor, and copy assignment.
                // iterator (const iterator&);
                // ~iterator ();
                // iterator& operator = (const iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * dereference operator
                 * @return returnsa pointer to the given deque at said index
                 */
                reference operator * () const {
                    return (*_deq)[_index];
                }

                // -----------
                // operator ->
                // -----------

                /**
                 * reference operator
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * pre-increment operator
                 */
                iterator& operator ++ () {
                    _index++;
                    assert(valid());
                    return *this;
                }

                /**
                 * post-increment operator
                 */
                iterator operator ++ (int) {
                    iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;
                }

                // -----------
                // operator --
                // -----------

                /**
                 * pre-decrement operator
                 */
                iterator& operator -- () {
                    _index--;
                    assert(valid());
                    return *this;
                }

                /**
                 * post-decrement operator
                 */
                iterator operator -- (int) {
                    iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;
                }

                // -----------
                // operator +=
                // -----------

                /**
                 * += operator
                 */
                iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;
                }

                // -----------
                // operator -=
                // -----------

                /**
                 * -= operator
                 */
                iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;
                }
            };

    public:
        // --------------
        // const_iterator
        // --------------

        class const_iterator {
            public:
                // --------
                // typedefs
                // --------

                typedef std::bidirectional_iterator_tag   iterator_category;
                typedef typename my_deque::value_type      value_type;
                typedef typename my_deque::difference_type difference_type;
                typedef typename my_deque::const_pointer   pointer;
                typedef typename my_deque::const_reference reference;

            public:
                // -----------
                // operator ==
                // -----------

                /**
                 * const iterator equals operator
                 */
                friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) {
                    return (lhs._deq == rhs._deq) && (lhs._index == rhs._index);}

                /**
                 * const iterator not equals operator
                 */
                friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) {
                    return !(lhs == rhs);}

                // ----------
                // operator +
                // ----------

                /**
                 * const iterator plus operator
                 */
                friend const_iterator operator + (const_iterator lhs, difference_type rhs) {
                    return lhs += rhs;}

                // ----------
                // operator -
                // ----------

                /**
                 * const iterator minus operator
                 */
                friend const_iterator operator - (const_iterator lhs, difference_type rhs) {
                    return lhs -= rhs;}

            private:
                // ----
                // data
                // ----

                // <your data>

                const my_deque* _deq;
                size_type _index;

            private:
                // -----
                // valid
                // -----

                bool valid () const {
                    return _index >= 0;}

            public:
                // -----------
                // constructor
                // -----------

                /**
                 * @param d the inputed deque
                 * @param i the index of the inputed deque
                 * const iterator constructor
                 */
                const_iterator (const my_deque* d, size_type i) :
                    _deq (d),
                    _index (i)
                    {assert(valid());}

                // Default copy, destructor, and copy assignment.
                // const_iterator (const const_iterator&);
                // ~const_iterator ();
                // const_iterator& operator = (const const_iterator&);

                // ----------
                // operator *
                // ----------

                /**
                 * const iterator dereference operator
                 */
                reference operator * () const {
                    return (*_deq)[_index];}

                // -----------
                // operator ->
                // -----------

                /**
                 * const iterator reference operator
                 */
                pointer operator -> () const {
                    return &**this;}

                // -----------
                // operator ++
                // -----------

                /**
                 * const iterator pre-increment operator
                 */
                const_iterator& operator ++ () {
                    _index++;
                    assert(valid());
                    return *this;}

                /**
                 * const iterator pre-increment operator
                 */
                const_iterator operator ++ (int) {
                    const_iterator x = *this;
                    ++(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator --
                // -----------

                /**
                 * <const iterator pre-decrement operator
                 */
                const_iterator& operator -- () {
                    _index--;
                    assert(valid());
                    return *this;}

                /**
                 * const iterator post-decrement operator
                 */
                const_iterator operator -- (int) {
                    const_iterator x = *this;
                    --(*this);
                    assert(valid());
                    return x;}

                // -----------
                // operator +=
                // -----------

                /**
                 * const iterator plus equals operator
                 */
                const_iterator& operator += (difference_type d) {
                    _index += d;
                    assert(valid());
                    return *this;}

                // -----------
                // operator -=
                // -----------

                /**
                 *  const iterator minus equals operator
                 */
                const_iterator& operator -= (difference_type d) {
                    _index -= d;
                    assert(valid());
                    return *this;}};

    public:
        // ------------
        // constructors
        // ------------

        /**
         * decault constructor for deque.
         * @param a allocator constructor for T's
         * @param m related allocator constructor for T*'s
         */
        explicit my_deque (const allocator_type& a = allocator_type(), const map_allocator_type& m = map_allocator_type()) : _a (a), _m(m){
            inner_array_size = size_of_inner_array;
            their_size = size_of_back = 0;
            size_of_front = size_of_inner_array;
            d_begin = map_begin = _m.allocate(size_of_map = 1);
            d_end = map_end = d_begin + 1;
            _b = _e = _a.allocate(size_of_inner_array);

            uninitialized_fill(_m, d_begin, d_end, _b);
            assert(valid());
        }
        
        /**
         * main constructor
         * @param s size of the new deque
         * @param v value type of the new deque
         * @param a allocator constructor for T's
         * @param mrelated allocator constructor for T*'s
         */
        explicit my_deque (size_type s, const_reference v = value_type(), const allocator_type& a = allocator_type(),  const map_allocator_type& m = map_allocator_type()) : _a(a), _m(m){
            inner_array_size = their_size = size_of_front = size_of_back = s;

            d_begin = map_begin = _m.allocate(size_of_map = 1);
            d_end = map_end = d_begin + 1;
            _b = _a.allocate(s);
            _e = _b + s;

            uninitialized_fill(_a, _b, _e, v);
            uninitialized_fill(_m, d_begin, d_end, _b);
            assert(valid());
        }

        /**
         * copy constructor
         * @param that the deque to copy construct
         */
        my_deque (const my_deque& that) : _a(that._a), _m(that._m){
            inner_array_size = their_size = size_of_front = size_of_back = that.size();

            d_begin = map_begin = _m.allocate(size_of_map = 1);
            d_end = map_end = d_begin + 1;
            _b = _a.allocate(inner_array_size);
            _e = _b + inner_array_size;

            uninitialized_fill(_a, _b, _e, value_type());
            uninitialized_fill(_m, d_begin, d_end, _b);
            *this = that;
            assert(valid());
        }

        // ----------
        // destructor
        // ----------

        /**
         * deque destructor 
         */
        ~my_deque () {
            for(size_type i = 0; i < size(); i++) _a.destroy(&(*this)[i]);
            map_pointer dest = d_begin;
            while(dest != d_end) _a.deallocate(*dest++, inner_array_size);
            destroy(_m, d_begin, d_end);
            _m.deallocate(map_begin, size_of_map);
            assert(valid());
        }
        

        // ----------
        // operator =
        // ----------

        /**
         * asignment operator for deque
         */
        my_deque& operator = (const my_deque& that) {
            if(this == &that) return *this;
            resize(that.size());
            for(size_type i = 0; i < size(); i++) (*this)[i] = that[i];
            assert(valid());
            return *this;
        }

        // -----------
        // operator []
        // -----------

        /**
         * index operator
         * @param index takes an index
         * @return returns a reference to the opject in the deque at position index
         */
        reference operator [] (size_type index) {
            // size_type range = inner_array_size - size_of_front;
            // size_type actual = index + range;
            size_type actual = index +  (inner_array_size - size_of_front);
            size_type map_index = actual / inner_array_size;
            size_type inner_array_index = actual % inner_array_size;

            return *( *(d_begin + map_index) + inner_array_index);
        }

        /**
         * a const version of the prior stated index operator
         */
        const_reference operator [] (size_type index) const {
            return const_cast<my_deque*>(this)->operator[](index);}

        // --
        // at
        // --

        /**
         * at function, which is like index, but will be nice and tell you if you go out of bounds
         */
        reference at (size_type index) throw (std::out_of_range) {
            if (index >= size()) throw std::out_of_range("deque");
            return (*this)[index];
        }

        /**
         * const version of the at operator
         */
        const_reference at (size_type index) const {
            return const_cast<my_deque*>(this)->at(index);}

        // ----
        // back
        // ----

        /**
         * @return returns a reference to the last element in the deque
         */
        reference back () {
            assert(!empty());
            return (*this)[size()-1];
        }

        /**
         * a const version of the prior back method
         */
        const_reference back () const {
            return const_cast<my_deque*>(this)->back();}

        // -----
        // begin
        // -----

        /**
         * returns an iterator to the first element in the deque
         */
        iterator begin () { return iterator(this, 0); }

        /**
         * a const version of the prior begin method
         */
        const_iterator begin () const { return const_iterator(this, 0); }

        // -----
        // clear
        // -----

        /**
         * clear function erases every element in the deque
         */
        void clear () { resize(0); assert(valid()); }

        // -----
        // empty
        // -----

        /**
         * #return returns a boolean for if the deque is empty or not
         */
        bool empty () const { return !size(); }

        // ---
        // end
        // ---

        /**
         * @ return returns an iterator to the last element in the deque
         */
        iterator end () { return iterator( this, size() ); }

        /**
         * <a const version of the prior end method
         */
        const_iterator end () const { return const_iterator( this, size() ); }

        // -----
        // erase
        // -----

        /**
         * @param it iterator index to remove from the deque
         * @return returns an iterator
         */
        iterator erase (iterator it) {
            assert(!empty());

            if(it == end()){ pop_back(); return it; }
            else if(it == begin()){ pop_front(); return it; }

            iterator t = it;
            iterator c = it + 1;
            while( c != end() ){ *t = *c; t++; c++; }
            pop_back();
            
            assert(valid());
            return it;
        }

        // -----
        // front
        // -----

        /**
         * @return returns a reference to the first element in the deque
         */
        reference front () {
            assert( !empty() );
            return *_b;
        }

        /**
         * a const version of the prior front method
         */
        const_reference front () const { return const_cast<my_deque*>(this)->front(); }

        // ------
        // insert
        // ------

        /**
         * @param it iterator for where to insert
         * @param v value to insert
         */
        iterator insert (iterator it, const_reference v) {
            if(it == end()){ push_back(v); return end() - 1; }
            else if(it == begin()){ push_front(v);  return begin(); }
            push_back(value_type());

            iterator t = end() - 1;
            iterator c = end() - 2;
            while(t != it){ *t = *c; t--; c--; }
            *t = v;
            assert(valid());
            return it;
        }

        // ---
        // pop
        // ---

        /**
         * pop_back method removes the last element in the deque
         */
        void pop_back () {
            assert(!empty());
            if(size_of_back == 0){
                size_of_back = inner_array_size - 1;
                _e = *(d_end - 2) + inner_array_size;
                _e = destroy(_a, _e - 1, _e);
            }else{
                _e = destroy(_a, _e - 1, _e);
                size_of_back--;
            }
            their_size--;
            assert(valid());
        }

        /**
         * the pop_front method removes the first element in the deque
         */
        void pop_front () {
            assert(!empty());
            if(size_of_front == 1){
                size_of_front = inner_array_size;
                destroy(_a, _b, _b + 1);
                _b = *(d_end - 1);
            }else{
                size_of_front--;
                destroy(_a, _b, _b + 1); _b++;
            }
            their_size--;
            assert(valid());
        }

        // ----
        // push
        // ----

        /**
         * push_back is a method for adding an element to the back of the deque
         */
        void push_back (const_reference v) {
            if(size_of_back < inner_array_size){
                _e = uninitialized_fill(_a, _e, _e + 1, v); size_of_back++;
            }else{
                if(d_end != map_end){
                    _e = _a.allocate(inner_array_size);
                    uninitialized_fill(_m, d_end, d_end + 1, _e);
                    size_of_back = 0; d_end++;  push_back(v);
                    return;
                }else{
                    map_pointer t_m = _m.allocate(size_of_map * 3);
                    size_type t = d_begin - map_begin;

                    uninitialized_copy(_m, d_begin, d_end, t_m + size_of_map + t);
                    destroy(_m, map_begin, map_end);
                    _m.deallocate(map_begin, size_of_map);

                    map_begin = t_m;
                    d_end = map_begin + (size_of_map * 2);
                    d_begin = map_begin + size_of_map + t;
                    size_of_map *= 3;
                    map_end = map_begin + size_of_map;

                    _e = _a.allocate(inner_array_size);
                    uninitialized_fill(_m, d_end, d_end + 1, _e);
                    size_of_back = 0; d_end++; push_back(v);
                    return;
                }
            }
            ++their_size;
            assert(valid());
        }

        /**
         * push_front is a method for adding an element to the front of the deque
         */
        void push_front (const_reference v) {
            if(size_of_front < inner_array_size){ _b--;
                uninitialized_fill(_a, _b, _b + 1, v); size_of_front++;
            }else{
                if(d_begin != map_begin){
                    _b = _a.allocate(inner_array_size); d_begin--;
                    uninitialized_fill(_m, d_begin, d_begin + 1, _b);
                    _b += inner_array_size; size_of_front = 0; push_front(v);
                    return;
                }else{
                    map_pointer t_m = _m.allocate(size_of_map * 3);
                    map_pointer temp_e = uninitialized_copy(_m, d_begin, d_end, t_m + size_of_map);

                    destroy(_m, map_begin, map_end);
                    _m.deallocate(map_begin, size_of_map);

                    d_end = temp_e;
                    map_begin = t_m;
                    d_begin = map_begin + size_of_map;
                    size_of_map *= 3;
                    map_end = map_begin + size_of_map;

                    _b = _a.allocate(inner_array_size); d_begin--;
                    uninitialized_fill(_m, d_begin, d_begin + 1, _b);
                    _b += inner_array_size; size_of_front = 0; push_front(v);
                    return;
                }
            }

            ++their_size;
            assert(valid());
        }

        // ------
        // resize
        // ------

        /**
         * @param s makes the deque to be size s, either removing elements to make it smaller or adding ones to make it larger
         * @param v optional value to push_back if the resize is larger than current size
         */
        void resize (size_type s, const_reference v = value_type()) {
            if(s == size()) return;
            else if( s < size() ){
                size_type d = size() - s;
                while(d > 0) { pop_back(); d--; }
            }else{
                size_type d = s - size();
                while(d > 0) { push_back(v); d--; }
            }
            assert(valid());
        }

        // ----
        // size
        // ----

        /**
         * @return returns the size of the deque
         */
        size_type size () const {
            return their_size;}

        // ----
        // swap
        // ----

        /**
         * @param that input of a deque
         * under the covers it swaps the elements of the two deques
         * if the allocators aren't equal, then copies the deque
         */
        void swap (my_deque& that) {
            if (_a == that._a) {
                std::swap(map_begin, that.map_begin);
                std::swap(map_end, that.map_end);
                std::swap(d_begin, that.d_begin);
                std::swap(d_end, that.d_end);
                std::swap(_b, that._b);
                std::swap(_e, that._e);

                std::swap(their_size, that.their_size);
                std::swap(size_of_map, that.size_of_map);
                std::swap(inner_array_size, that.inner_array_size);
                std::swap(size_of_front, that.size_of_front);
                std::swap(size_of_back, that.size_of_back);
            }else {
                my_deque x(*this);
                *this = that;
                that  = x;
            }
            assert(valid());
        }
    };

#endif // Deque_h
