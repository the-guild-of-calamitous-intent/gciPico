/**************************************\
 * The MIT License (MIT)
 * Copyright (c) 2023 Kevin Walchko
 * see LICENSE for full details
\**************************************/
#pragma once

#include <cstdint>
#include <cstdio>

/**
 This is a FIFO, but with a fixed size that will over write old
 data. It is designed to be simple and fast with low overhead.
*/
template <uint16_t BUFFER_SIZE> class Fifo {
  // public:
  uint8_t buffer[BUFFER_SIZE];
  uint16_t head{0}; // read / pop
  uint16_t tail{0}; // write / push
  uint16_t numElem{0};

public:
  Fifo() {}

  inline const uint16_t size() const volatile { return numElem; }
  inline bool isFull() volatile { return numElem >= BUFFER_SIZE; }
  inline bool isEmpty() volatile { return numElem == 0; }
  inline uint16_t nextPos(const size_t pos) volatile {
    return (pos + 1) % BUFFER_SIZE;
  }

  void clear() volatile {
    head = 0;
    tail = 0;
    numElem = 0;
  }

  void push(const uint8_t b) volatile {
    if (isFull()) {
      head = nextPos(head); // drop oldest
      numElem = numElem - 1;
    }
    buffer[tail] = b;
    tail         = nextPos(tail);
    numElem = numElem + 1;
  }

  uint8_t pop() volatile {
    if (isEmpty()) return 0;
    uint8_t ret = buffer[head];
    head        = nextPos(head);
    numElem = numElem - 1;
    return ret;
  }
};