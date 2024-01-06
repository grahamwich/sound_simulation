// Copyright 2022 Graham Stelzer
// unit tests for CircularBuffer

#include <iostream>
#include "CircularBuffer.hpp"
#include "StringSound.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
using boost::test_tools::output_test_stream;

// test isFull and isEmpty and size
BOOST_AUTO_TEST_CASE(test_isFull_isEmpty) {
    CircularBuffer<int> cb0(5);
    BOOST_CHECK(cb0.isEmpty());
    BOOST_CHECK(!cb0.isFull());
    cb0.enqueue(1);
    cb0.enqueue(2);
    cb0.enqueue(3);
    BOOST_CHECK_EQUAL(cb0.size(), 3);
    cb0.enqueue(4);
    cb0.enqueue(5);
    BOOST_CHECK(!cb0.isEmpty());
    BOOST_CHECK(cb0.isFull());
}

// attempt to take item from empty buffer
BOOST_AUTO_TEST_CASE(CB_DQ_item_throw) {
    CircularBuffer<int> cb1(5);
    BOOST_REQUIRE_THROW(cb1.dequeue(), std::out_of_range);
}

// attempt to add item to full buffer
BOOST_AUTO_TEST_CASE(CB_ENQ_item_throw) {
    CircularBuffer<int> cb2(1);
    cb2.enqueue(1);
    BOOST_REQUIRE_THROW(cb2.enqueue(2), std::runtime_error);
}

// attempt to construct with bad args
BOOST_AUTO_TEST_CASE(CB_invalid_arg_throw) {
    BOOST_REQUIRE_THROW(CircularBuffer<int> cb3(-1), std::bad_array_new_length);
    BOOST_REQUIRE_THROW(CircularBuffer<int> cb4(0), std::invalid_argument);
}

// test circular functionality
BOOST_AUTO_TEST_CASE(CB_circular_ENQ) {
    CircularBuffer<int> cb5(5);
    for (int i = 1; i <= 5; i++) {
        cb5.enqueue(i);
    }
    cb5.print();
    BOOST_REQUIRE_EQUAL(cb5.peek(), 1);
    for (int i = 1; i <= 5; i++) {
        cb5.dequeue();
    }
    cb5.enqueue(5);
    cb5.print();
    BOOST_REQUIRE_EQUAL(cb5.peek(), 5);
}

// test copy constructor
BOOST_AUTO_TEST_CASE(CB_copy_constructor) {
    CircularBuffer<int> cb6(5);
    for (int i = 1; i <= 5; i++) {
        cb6.enqueue(i);
    }
    CircularBuffer<int> cb7(cb6);
    cb7.print();
    BOOST_REQUIRE_EQUAL(cb7.peek(), 1);
}

// test copy assignment
BOOST_AUTO_TEST_CASE(CB_copy_assignment) {
    CircularBuffer<int> cb8(5);
    for (int i = 1; i <= 5; i++) {
        cb8.enqueue(i);
    }
    CircularBuffer<int> cb9(5);
    cb9 = cb8;
    cb9.print();
    BOOST_REQUIRE_EQUAL(cb9.peek(), 1);
}

// test stringsound constructor
BOOST_AUTO_TEST_CASE(SS_constructor) {
    StringSound ss1(440.0);
    BOOST_CHECK_THROW(ss1.sample(), std::out_of_range);
    BOOST_REQUIRE_EQUAL(ss1.time(), 0);
}

// test stringsound pluck
// also tests stringsound sample
BOOST_AUTO_TEST_CASE(SS_pluck) {
    StringSound ss2(440.0);
    ss2.pluck();
    BOOST_CHECK_NO_THROW(ss2.sample());
    sf::Int16 sample1 = ss2.sample();
    ss2.tick();
    sf::Int16 sample2 = ss2.sample();
    BOOST_CHECK(sample1 != sample2);
}

// test stringsound tick
BOOST_AUTO_TEST_CASE(SS_tick) {
    StringSound ss3(440.0);
    ss3.pluck();
    ss3.tick();
    ss3.tick();
    BOOST_REQUIRE_EQUAL(ss3.time(), 2);
}

// test stringsound time
BOOST_AUTO_TEST_CASE(SS_time) {
    StringSound ss4(440.0);
    ss4.pluck();
    ss4.tick();
    ss4.tick();
    BOOST_REQUIRE_EQUAL(ss4.time(), 2);
}
