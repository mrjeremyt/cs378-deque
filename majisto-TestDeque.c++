// ----------------------------
// projects/deque/TestDeque.c++
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------------

// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

/*
Google Test Libraries:
    % ls -al /usr/include/gtest/
    ...
    gtest.h
    ...

    % locate libgtest.a
    /usr/lib/libgtest.a

    % locate libpthread.a
    /usr/lib/x86_64-linux-gnu/libpthread.a
    /usr/lib32/libpthread.a

    % locate libgtest_main.a
    /usr/lib/libgtest_main.a

To compile the test:
    % g++-4.7 -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall Deque.h TestDeque.c++ -o TestDeque -lgtest -lgtest_main -lpthread

To run the test:
    % valgrind TestInteger

To obtain coverage of the test:
    % gcov-4.7 -b TestDeque.c++
*/

// --------
// includes
// --------

#include <algorithm> // equal
#include <cstring>   // strcmp
#include <deque>     // deque
#include <sstream>   // ostringstream
#include <stdexcept> // invalid_argument
#include <string>    // ==
#include <iostream>

#include "gtest/gtest.h"

#include "Deque.h"
using namespace std;

#define NAMES       typedef typename TestFixture::deque_type      deque_type; \
                        typedef typename TestFixture::allocator_type  allocator_type; \
                        typedef typename TestFixture::value_type      value_type; \
                        typedef typename TestFixture::size_type       size_type; \
                        typedef typename TestFixture::difference_type difference_type; \
                        typedef typename TestFixture::pointer         pointer; \
                        typedef typename TestFixture::const_pointer   const_pointer; \
                        typedef typename TestFixture::reference       reference; \
                        typedef typename TestFixture::const_reference const_reference;

// ---------
// TestDeque
// ---------

template <typename D>
struct TestDeque : testing::Test 
{
    // --------
    // typedefs
    // --------

    typedef          D                  deque_type;
    typedef typename D::allocator_type  allocator_type;
    typedef typename D::value_type      value_type;
    typedef typename D::size_type       size_type;
    typedef typename D::difference_type difference_type;
    typedef typename D::pointer         pointer;
    typedef typename D::const_pointer   const_pointer;
    typedef typename D::reference       reference;
    typedef typename D::const_reference const_reference;
};

typedef testing::Types<
            std::deque<int>,
            std::deque<double>,
            my_deque<int>,
            my_deque<double> >
        my_types;

TYPED_TEST_CASE(TestDeque, my_types);

TYPED_TEST(TestDeque, Empty) 
{
    NAMES
    deque_type x;
    const bool b = x.empty();
    ASSERT_TRUE(b);
}

TYPED_TEST(TestDeque, Size) 
{
    NAMES
    deque_type x;
    const size_type s = x.size();
    ASSERT_EQ(0, s);
}

TYPED_TEST(TestDeque, default_constructor_1) 
{
    NAMES
    deque_type x;
    const size_type s = x.size();
    ASSERT_EQ(0, s);
}

TYPED_TEST (TestDeque, default_constructor_2)
{
    NAMES
    deque_type x;
    assert(x.size() == 0);
}

TYPED_TEST (TestDeque, fill_constructor_1)
{
    NAMES
    deque_type x(5);
    assert(x.size() == 5);
}

TYPED_TEST (TestDeque, copy_constructor_1)
{
    NAMES
    deque_type x(5);
    deque_type y = x;
    assert(y.size() == 5);
}


TYPED_TEST(TestDeque, push_back_1) 
{
    NAMES
    deque_type x;
    x.push_back(2);
    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, push_back_2) 
{
    NAMES
    deque_type x;
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);
    x.push_back(6);
    x.push_back(7);
    x.push_back(8);
    x.push_back(9);
    x.push_back(10);
    x.push_back(11);
    x.push_back(12);
    ASSERT_EQ(x[0], 2);
    ASSERT_EQ(x[1], 3);
    ASSERT_EQ(x[2], 4);
    ASSERT_EQ(x[3], 5);
    ASSERT_EQ(x[4], 6);
    ASSERT_EQ(x[5], 7);
    ASSERT_EQ(x[6], 8);
}


TYPED_TEST(TestDeque, push_back_3) 
{
    NAMES
    deque_type x;
    x.push_back(0);
    x.push_back(1);
    ASSERT_EQ(x[0], 0);
    ASSERT_EQ(x[1], 1);
}

TYPED_TEST(TestDeque, push_back_4) 
{
    NAMES
    deque_type x(10,2);
    x.push_back(1);
    ASSERT_EQ(x[10], 1);
}

TYPED_TEST(TestDeque, push_back_5)
{
    NAMES
    deque_type x;
    x.push_back(3);
    x.push_back(4);
    ASSERT_EQ(x[1], 4);
    x.push_back(5);
    ASSERT_EQ(x[2], 5);
}

TYPED_TEST(TestDeque, push_front_1)
{
    NAMES
    deque_type x;
    x.push_front(5);
    ASSERT_EQ(x[0], 5);
}

TYPED_TEST(TestDeque, push_front_2)
{
    NAMES
    deque_type x;
    x.push_front(5);
    x.push_front(6);
    ASSERT_EQ(x[0], 6);
}

TYPED_TEST(TestDeque, push_front_3)
{
    NAMES
    deque_type x;
    x.push_front(5);
    x.push_front(6);
    ASSERT_EQ(x[0], 6);
    x.push_front(7);
    ASSERT_EQ(x[0], 7);
}

TYPED_TEST(TestDeque, push_front_4)
{
    NAMES
    deque_type x;
    x.push_front(5);
    x.push_front(6);
    ASSERT_EQ(x[0], 6);
    x.push_front(7);
    ASSERT_EQ(x[0], 7);
    x.push_back(8);
    ASSERT_EQ(x[0], 7);
}

TYPED_TEST(TestDeque, push_front_5)
{
    NAMES
    deque_type x;
    x.push_front(5);
    x.push_front(6);
    ASSERT_EQ(x[0], 6);
    x.pop_front();
    ASSERT_EQ(x[0], 5);
}

TYPED_TEST(TestDeque, push_front_6)
{
    NAMES
    deque_type x;
    x.push_front(5);
    ASSERT_EQ(x[0], 5);
    assert(x.size() == 1);
}

TYPED_TEST(TestDeque, pop_front_1)
{
    NAMES
    deque_type x;
    x.push_back(5);
    ASSERT_EQ(x[0], 5);
    x.pop_front();
    assert(x.size() == 0);
}

TYPED_TEST(TestDeque, pop_front_2)
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.pop_front();
    assert(x.size() == 1);
}

TYPED_TEST(TestDeque, pop_front_3)
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.pop_front();
    x.pop_front();
    assert(x.size() == 0);
}

TYPED_TEST(TestDeque, pop_front_4)
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.pop_front();
    x.push_back(7);
    assert(x.size() == 2);
}

TYPED_TEST(TestDeque, pop_front_5)
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_front(8);
    x.pop_front();
    assert(x.size() == 2);
}

TYPED_TEST(TestDeque, pop_front_6)
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    assert(x.size() == 2);
    x.pop_front();
    assert(x.size() == 1);
}

TYPED_TEST(TestDeque, pop_back_1) 
{
    NAMES
    deque_type x;
    x.push_back(7);
    x.push_back(6);
    x.push_back(5);
    x.pop_back();
    assert (x.size() == 2);
}

TYPED_TEST(TestDeque, pop_back_2) 
{
    NAMES
    deque_type x;
    x.push_back(7);
    x.pop_back();
    assert (x.size() == 0);
}

TYPED_TEST(TestDeque, pop_back_3) 
{
    NAMES
    deque_type x;
    x.push_back(7);
    x.push_back(6);
    x.pop_back();
    assert (x.size() == 1);
}

TYPED_TEST(TestDeque, pop_back_4) 
{
    NAMES
    deque_type x;
    x.push_back(7);
    x.push_back(6);
    x.push_back(5);
    x.pop_back();
    x.pop_back();
    assert (x.size() == 1);
}

TYPED_TEST(TestDeque, pop_back_5) 
{
    NAMES
    deque_type x;
    x.push_back(7);
    x.push_back(6);
    x.push_back(5);
    x.pop_back();
    x.pop_back();
    x.pop_back();
    assert (x.size() == 0);
}

TYPED_TEST(TestDeque, pop_back_6) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.pop_back();
    x.push_back(6);
    assert (x.size() == 1);
}

TYPED_TEST(TestDeque, front_1) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    ASSERT_EQ(x.front(), 5);
}

TYPED_TEST(TestDeque, front_2) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_front(2);
    ASSERT_EQ(x.front(), 2);
}

TYPED_TEST(TestDeque, front_3) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    ASSERT_EQ(x.front(), 5);
}

TYPED_TEST(TestDeque, front_4) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.pop_back();
    ASSERT_EQ(x.front(), 5);
}

TYPED_TEST(TestDeque, front_5) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_back(7);
    ASSERT_EQ(x.front(), 5);
}

TYPED_TEST(TestDeque, back_1) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_back(7);
    ASSERT_EQ(x.front(), 5);
}

TYPED_TEST(TestDeque, back_2) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_front(7);
    ASSERT_EQ(x.back(), 6);
}

TYPED_TEST(TestDeque, back_3) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    ASSERT_EQ(x.back(), 5);
}

TYPED_TEST(TestDeque, back_4) 
{
    NAMES
    deque_type x;
    x.push_front(5);
    x.push_front(6);
    x.push_front(5);
    ASSERT_EQ(x.back(), 5);
    x.pop_back();
    ASSERT_EQ(x.back(), 6);
}

TYPED_TEST(TestDeque, back_5) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.pop_back();
    ASSERT_EQ(x.back(), 5);
}

TYPED_TEST(TestDeque, back_6) 
{
    NAMES
    deque_type x;
    x.push_back(5);
    x.push_back(6);
    x.push_back(7);
    x.pop_back();
    ASSERT_EQ(x.back(), 6);
}

TYPED_TEST(TestDeque, at_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    
    ASSERT_EQ(x.at(0), 3);
    ASSERT_EQ(x.at(1), 2);
}

TYPED_TEST(TestDeque, at_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    
    ASSERT_EQ(x.at(0), 2);
    ASSERT_EQ(x.at(1), 1);
}

TYPED_TEST(TestDeque, at_3) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    
    ASSERT_EQ(x.at(0), 1);
    ASSERT_EQ(x.at(1), 2);
}

TYPED_TEST(TestDeque, at_4) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();
    
    ASSERT_EQ(x.at(0), 2);
    ASSERT_EQ(x.at(1), 1);
}

TYPED_TEST(TestDeque, at_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();
    x.pop_front();
    
    ASSERT_EQ(x.at(0), 1);
}

TYPED_TEST(TestDeque, begin_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();
    x.pop_front();
    
    ASSERT_EQ(*x.begin(), 1);
}

TYPED_TEST(TestDeque, begin_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    
    ASSERT_EQ(*x.begin(), 3);
}

TYPED_TEST(TestDeque, begin_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    
    ASSERT_EQ(x.at(0), 2);
}

TYPED_TEST(TestDeque, begin_4) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_back(2);
    
    ASSERT_EQ(x.at(0), 1);
}

TYPED_TEST(TestDeque, begin_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_back(3);
    x.pop_front();

    ASSERT_EQ(x.at(0), 1);
}

TYPED_TEST(TestDeque, end_1) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);

    ASSERT_EQ(*(x.end() - 1), 3);
}

TYPED_TEST(TestDeque, end_2) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(2);

    ASSERT_EQ(*(x.end() - 1), 2);
}

TYPED_TEST(TestDeque, end_3) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(2);

    ASSERT_EQ(*(x.end() - 1), 1);
}

TYPED_TEST(TestDeque, end_4) 
{
    NAMES
    deque_type x(10, 7);
    x.push_back(1);
    x.push_front(2);
    x.pop_back();

    ASSERT_EQ(*(x.end() - 1), 7);
}

TYPED_TEST(TestDeque, end_5) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.pop_back();
    x.pop_back();

    ASSERT_EQ(*(x.end() - 1), 1);
}

TYPED_TEST(TestDeque, end_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(*(x.end() - 1), 1);
}

TYPED_TEST(TestDeque, brack_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, brack_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.pop_front();

    ASSERT_EQ(x[0], 1);
}

TYPED_TEST(TestDeque, brack_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);

    ASSERT_EQ(x[0], 3);
}

TYPED_TEST(TestDeque, brack_4) 
{
    NAMES
    deque_type x;
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, brack_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.push_back(2);
    x.pop_front();

    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, brack_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, size_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x.size(), 2);
}

TYPED_TEST(TestDeque, size_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.pop_front();

    ASSERT_EQ(x.size(), 1);
}

TYPED_TEST(TestDeque, size_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);

    ASSERT_EQ(x.size(), 3);
}

TYPED_TEST(TestDeque, size_4) 
{
    NAMES
    deque_type x;
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x.size(), 1);
}

TYPED_TEST(TestDeque, size_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.push_back(2);
    x.pop_front();

    ASSERT_EQ(x.size(), 3);
}

TYPED_TEST(TestDeque, size_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x.size(), 2);
}

TYPED_TEST(TestDeque, resize_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    x.resize(15);
    ASSERT_EQ(x.size(), 15);
}

TYPED_TEST(TestDeque, resize_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.pop_front();

    x.resize(10);
    ASSERT_EQ(x.size(), 10);
}

TYPED_TEST(TestDeque, resize_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);

    x.resize(100);
    ASSERT_EQ(x.size(), 100);
}

TYPED_TEST(TestDeque, resize_4) 
{
    NAMES
    deque_type x;

    x.resize(10,5);
    ASSERT_EQ(x.size(), 10);
    ASSERT_EQ(x[0], 5);
}

TYPED_TEST(TestDeque, resize_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.push_back(2);
    x.pop_front();

    x.resize(15,5);
    ASSERT_EQ(x.size(), 15);
    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, resize_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    ASSERT_EQ(x.size(), 2);
}

TYPED_TEST(TestDeque, insert_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.insert(x.begin(), 5);

    ASSERT_EQ(x[0], 5);
}

TYPED_TEST(TestDeque, insert_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.insert(x.end(), 5);

    
    ASSERT_EQ(*(x.end() - 1), 5);
}

TYPED_TEST(TestDeque, insert_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);

    ASSERT_EQ(*x.insert(x.begin(), 5), 5);
    ASSERT_EQ(x[0], 5);
    ASSERT_EQ(x[1], 3);
}

TYPED_TEST(TestDeque, insert_4) 
{
    NAMES
    deque_type x;
    x.insert(x.begin(), 5);
    ASSERT_EQ(x[0], 5);
}

TYPED_TEST(TestDeque, insert_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.push_back(2);
    x.pop_front();

    x.insert(x.begin(), 10);
    ASSERT_EQ(x[0], 10);
    ASSERT_EQ(x[1], 2);
}

TYPED_TEST(TestDeque, insert_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.pop_front();

    x.insert(x.end(), 10);
    ASSERT_EQ(*(x.end()-1), 10);
}

TYPED_TEST(TestDeque, erase_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.erase(x.begin());

    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, erase_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.insert(x.end(), 5);
    x.erase(x.end());
    
    ASSERT_EQ(*(x.end() - 1), 1);
}

TYPED_TEST(TestDeque, erase_3) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);

    x.insert(x.begin(), 5);
    x.erase(x.begin());

    ASSERT_EQ(x[0], 3);
}

TYPED_TEST(TestDeque, erase_4) 
{
    NAMES
    deque_type x;
    x.insert(x.begin(), 5);
    x.erase(x.begin());
    assert(x.empty());
}

TYPED_TEST(TestDeque, erase_5) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.push_back(2);
    x.pop_front();

    x.insert(x.begin(), 10);
    x.erase(x.begin());
    ASSERT_EQ(x[0], 2);
}

TYPED_TEST(TestDeque, erase_6) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.push_front(3);
    x.erase(x.begin());
    x.erase(x.begin());
    ASSERT_EQ(*x.begin(), 1);
}

TYPED_TEST(TestDeque, empty_1) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.erase(x.begin());

    ASSERT_EQ(x.size(), 0);
    assert(x.empty());
}

TYPED_TEST(TestDeque, empty_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.erase(x.end());
    x.erase(x.end());
    
    assert(x.empty());
}

TYPED_TEST(TestDeque, empty_3) 
{
    NAMES
    deque_type x;

    assert(x.empty());
}

TYPED_TEST(TestDeque, empty_4) 
{
    NAMES
    deque_type x;
    x.insert(x.begin(), 5);
    x.erase(x.begin());
    assert(x.empty());
}

TYPED_TEST(TestDeque, empty_5) 
{
    NAMES
    deque_type x(10,2);
    assert(!(x.empty()));
}

TYPED_TEST(TestDeque, clear_1) 
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.clear();
    ASSERT_EQ(x.size(), 0);
}

TYPED_TEST(TestDeque, clear_2) 
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(2);
    x.clear();
    
    assert(x.empty());
}

TYPED_TEST(TestDeque, clear_3) 
{
    NAMES
    deque_type x;
    x.clear();

    assert(x.empty());
}

TYPED_TEST(TestDeque, clear_4) 
{
    NAMES
    deque_type x;
    x.insert(x.begin(), 5);
    x.clear();
    assert(x.empty());
}

TYPED_TEST(TestDeque, clear_5) 
{
    NAMES
    deque_type x(10,2);
    x.clear();
    assert((x.empty()));
}

TYPED_TEST(TestDeque, equal_1) 
{
    NAMES
    deque_type x(10,2);
    deque_type y(10,2);
    ASSERT_EQ(x,y);
}

TYPED_TEST(TestDeque, equal_2) 
{
    NAMES
    deque_type x(10,2);
    deque_type y (x);
    ASSERT_EQ(x,y);
}

TYPED_TEST(TestDeque, equal_3) 
{
    NAMES
    deque_type x(2,2);
    deque_type y;
    y.push_back(2);
    y.push_back(2);
    ASSERT_EQ(x,y);
}

TYPED_TEST(TestDeque, equal_4) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(5);
    y.push_back(5);
    ASSERT_EQ(x,y);
}

TYPED_TEST(TestDeque, equal_5) 
{
    NAMES
    deque_type x(10,2);
    deque_type y(10,2);
    x.clear();
    y.clear();
    ASSERT_EQ(x,y);
}

TYPED_TEST(TestDeque, less_1) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    y.push_back(5);
    ASSERT_EQ(x<y, true);
}

TYPED_TEST(TestDeque, less_2) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(6);
    y.push_back(5);
    ASSERT_EQ(x<y, false);
}

TYPED_TEST(TestDeque, less_3) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    x.push_back(6);
    y.push_back(5);
    ASSERT_EQ(x<y, true);
}

TYPED_TEST(TestDeque, less_4) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    y.push_back(5);
    y.push_back(7);
    ASSERT_EQ(x<y, true);
}

TYPED_TEST(TestDeque, swap_1) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    y.push_back(5);
    y.push_back(7);
    x.swap(y);
    ASSERT_EQ(x.size(), 2);
    ASSERT_EQ(y.size(), 1);
}

TYPED_TEST(TestDeque, swap_2) 
{
    NAMES
    deque_type x;
    deque_type y;
    y.push_back(4);
    x.push_back(5);
    x.push_back(7);
    x.swap(y);
    ASSERT_EQ(y.size(), 2);
    ASSERT_EQ(x.size(), 1);
}

TYPED_TEST(TestDeque, swap_3) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    y.push_back(5);
    y.push_back(7);
    x.swap(y);
    ASSERT_EQ(x[0], 5);
    ASSERT_EQ(y[0], 4);
}

TYPED_TEST(TestDeque, swap_4) 
{
    NAMES
    deque_type x;
    deque_type y;
    x.push_back(4);
    y.push_back(5);
    y.push_back(7);
    x.swap(y);
    ASSERT_EQ(x[1], 7);
    ASSERT_EQ(y[0], 4);
}

TYPED_TEST(TestDeque, star_iterator_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, star_iterator_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(20);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, star_iterator_3)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 3);
}

TYPED_TEST(TestDeque, star_iterator_4)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(30);
    x.push_front(49);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 49);
}

TYPED_TEST(TestDeque, iterator_equals_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    typename deque_type::iterator y = x.begin();
    typename deque_type::iterator z = x.begin();
    assert(y==z);
}

TYPED_TEST(TestDeque, iterator_equals_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    typename deque_type::iterator y = x.end() - 1;
    ASSERT_EQ(*y, 1);
    typename deque_type::iterator z = x.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y==z);
}

TYPED_TEST(TestDeque, iterator_not_equals_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    x.push_front(4);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::iterator z = x.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_not_equals_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(4);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::iterator z = x.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_not_equals_3)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(4);
    x.push_back(5);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::iterator z = x.end() - 1;
    ASSERT_EQ(*z, 5);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_plus_equals_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    y = y + 1;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_plus_equals_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    y = y + 2;
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, iterator_plus_equals_3)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    try
        {y = y + 3;}
    catch(...)
        {assert(true);}
}

TYPED_TEST(TestDeque, iterator_minus_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() -1;
    y = y - 1;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_minus_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() - 1;
    y = y - 2;
    ASSERT_EQ(*y, 5);
}

TYPED_TEST(TestDeque, iterator_postincrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    y++;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_postincrement_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    y++;
    y++;
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, iterator_preincrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    ++y;
    ++y;
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, iterator_predecrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() - 1;
    --y;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_predecrement_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() - 1;
    --y;
    --y;
    ASSERT_EQ(*y, 5);
}

TYPED_TEST(TestDeque, iterator_postdecrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() - 1;
    y--;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_postdecrement_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.end() - 1;
    y--;
    y--;
    ASSERT_EQ(*y, 5);
}

TYPED_TEST(TestDeque, iterator_plus_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*(y+1),4);
}

TYPED_TEST(TestDeque, iterator_plus_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*(y+2),1);
}

TYPED_TEST(TestDeque, iterator_plus_3)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    x.push_front(6);
    typename deque_type::iterator y = x.begin();
    ASSERT_EQ(*(y+3),1);
}

TYPED_TEST(TestDeque, const_iterator_equals_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*(y+1),4);
}

TYPED_TEST(TestDeque, const_iterator_equals_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*(y+2),1);
}

TYPED_TEST(TestDeque, const_iterator_equals_3)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    x.push_front(6);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*(y+3),1);
}

TYPED_TEST(TestDeque, star_const_iterator_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, star_const_iterator_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_back(20);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, star_const_iterator_3)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 3);
}

TYPED_TEST(TestDeque, star_const_iterator_4)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(30);
    x.push_front(49);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 49);
}

TYPED_TEST(TestDeque, iterator_const_equals_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    typename deque_type::const_iterator z = a.begin();
    assert(y==z);
}

TYPED_TEST(TestDeque, iterator_const_equals_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.end() - 1;
    ASSERT_EQ(*y, 1);
    typename deque_type::const_iterator z = a.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y==z);
}

TYPED_TEST(TestDeque, iterator_const_not_equals_1)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(3);
    x.push_front(4);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::const_iterator z = a.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_const_not_equals_2)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(4);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::const_iterator z = a.end() - 1;
    ASSERT_EQ(*z, 1);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_const_not_equals_3)
{
    NAMES
    deque_type x;
    x.push_back(1);
    x.push_front(4);
    x.push_back(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ASSERT_EQ(*y, 4);
    typename deque_type::const_iterator z = a.end() - 1;
    ASSERT_EQ(*z, 5);
    assert(y!=z);
}

TYPED_TEST(TestDeque, iterator_const_plus_equals_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    y = y + 1;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_const_plus_equals_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    y = y + 2;
    ASSERT_EQ(*y, 1);
}

TYPED_TEST(TestDeque, iterator_const_plus_equals_3)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    try
        {y = y + 3;}
    catch(...)
        {assert(true);}
}

TYPED_TEST(TestDeque, iterator_const_minus_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.end() -1;
    y = y - 1;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_const_minus_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    auto y = a.end() - 1;
    y = y - 2;
    ASSERT_EQ(*y, 5);
}

TYPED_TEST(TestDeque, iterator_const_minus_equals_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    auto y = a.end() - 1;
    y -= 2;
    ASSERT_EQ(*y, 5);
}

TYPED_TEST(TestDeque, iterator_const_minus_equals_2)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    auto y = a.end() - 1;
    ASSERT_EQ(*(y -2), 5);
}

TYPED_TEST(TestDeque, iterator_const_postincrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    y++;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_const_preincrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.begin();
    ++y;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_const_predecrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.end() - 1;
    --y;
    ASSERT_EQ(*y, 4);
}

TYPED_TEST(TestDeque, iterator_const_postdecrement_1)
{
    NAMES
    deque_type x;
    x.push_front(1);
    x.push_front(4);
    x.push_front(5);
    const deque_type a(x);
    typename deque_type::const_iterator y = a.end() - 1;
    y--;
    ASSERT_EQ(*y, 4);
}
